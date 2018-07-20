/*
 * Copyright 2018 Rafal Zajac <rzajac@gmail.com>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */


#include <user_interface.h>

#include "include/wifi.h"
#include "nm_internal.h"

/////////////////////////////////////////////////////////////////////////////
// Defines.
/////////////////////////////////////////////////////////////////////////////

#define WIFI_WAS_CONECTED 0b00000001

// Macro evaluating to true if static IP was configured.
#define use_static_ip(w) ((w)->ip == 0 || (w)->netmask == 0 || (w)->gw == 0)

/////////////////////////////////////////////////////////////////////////////
// Globals.
/////////////////////////////////////////////////////////////////////////////

// Global fatal callback.
extern nm_err_cb nm_g_fatal_err;

// Global WiFi structure.
static wifi *g_wifi; // TODO: release it at some point.

/////////////////////////////////////////////////////////////////////////////
// Static methods.
/////////////////////////////////////////////////////////////////////////////

static void ICACHE_FLASH_ATTR
wifi_fatal(sint8 err, sint16 err_sdk)
{
    eb_remove_group(EV_GROUP);

    bool suc = wifi_set_opmode(NULL_MODE);
    if (!suc)
        NM_ERROR("wifi_set_opmode error");

    nm_g_fatal_err(NULL, err, err_sdk);
}

/**
 * Handles all WiFi events.
 *
 * @param ev_code The event code (same as wifi event codes).
 * @param arg     The System_Event_t instance.
 */
static void ICACHE_FLASH_ATTR
nm_wifi_event_cb(uint16_t ev_code, void *arg)
{
    System_Event_t *ev = arg;

    switch (ev_code) {
        case EVENT_STAMODE_CONNECTED:
            NM_DEBUG("EVENT_STAMODE_CONNECTED");
            if (use_static_ip(g_wifi) && wifi_station_dhcpc_set_maxtry(3) != true) {
                NM_ERROR("DHCP set max try failed");
                wifi_fatal(ESP_E_NET, 0);
            }
            break;

        case EVENT_STAMODE_DISCONNECTED:
            g_wifi->recon_cnt++;
            NM_DEBUG("EVENT_STAMODE_DISCONNECTED %d", g_wifi->recon_cnt);

            // Check if we reached reconnect max.
            if (g_wifi->recon_cnt == g_wifi->recon_max) {
                wifi_fatal(ESP_E_WIF, ev->event_info.disconnected.reason);
                return;
            }

            tcp_abort_all(); // TODO: is this a good place for this?


            // If we were connected notify user code.
            if (g_wifi->status & WIFI_WAS_CONECTED) {
                nm_g_fatal_err(NULL, ESP_E_WIF, EVENT_STAMODE_DISCONNECTED);
                g_wifi->status &= ~WIFI_WAS_CONECTED;
            }
            break;

        case EVENT_STAMODE_AUTHMODE_CHANGE:
            NM_DEBUG("EVENT_STAMODE_AUTHMODE_CHANGE");
            break;

        case EVENT_STAMODE_GOT_IP:
            NM_DEBUG("EVENT_STAMODE_GOT_IP");
            g_wifi->recon_cnt = 0;
            g_wifi->status |= WIFI_WAS_CONECTED;
            tcp_conn_all();
            nm_g_fatal_err(NULL, ESP_OK, 0);
            break;

        case EVENT_STAMODE_DHCP_TIMEOUT:
            NM_DEBUG("EVENT_STAMODE_DHCP_TIMEOUT");
            break;

        case EVENT_OPMODE_CHANGED:
            NM_DEBUG("EVENT_OPMODE_CHANGED");
            break;

        default:
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// User interface.
/////////////////////////////////////////////////////////////////////////////

sint8 ICACHE_FLASH_ATTR
wifi_start(wifi *wifi, char *name, char *pass, nm_err_cb err_cb)
{
    if (g_wifi != NULL) {
        NM_ERROR("g_wifi already set");
        return ESP_E_LOG;
    }

    uint8 status = wifi_station_get_connect_status();
    if (!(status == STATION_IDLE || status == 255)) {
        NM_ERROR("wifi already started %d", status);
        return ESP_E_LOG;
    }

    if (err_cb == NULL) {
        NM_ERROR("err_cb must be set");
        return ESP_E_ARG;
    }

    // Switch to station mode and configure static IP if necessary.
    if (wifi_set_opmode_current(STATION_MODE) != true) {
        NM_ERROR("opmode change");
        return ESP_E_SYS;
    }

    // Use DHCP or static IP after connection to WiFi.
    if (use_static_ip(wifi)) {
        if (wifi_station_dhcpc_start() != true) {
            NM_ERROR("DHCP start");
            return ESP_E_SYS;
        }
        NM_DEBUG("DHCP started");
    } else {
        if (wifi_station_dhcpc_stop() != true) {
            NM_ERROR("DHCP stop");
            return ESP_E_SYS;
        }

        NM_DEBUG("using static IP");
        struct ip_info info;
        info.ip.addr = wifi->ip;
        info.netmask.addr = wifi->netmask;
        info.gw.addr = wifi->gw;
        if (wifi_set_ip_info(STATION_IF, &info) != true) {
            NM_ERROR("set static IP");
            return ESP_E_SYS;
        }
    }

    // Configure WiFi.
    struct station_config station_config;
    os_memset(&station_config, 0, sizeof(struct station_config));
    strlcpy((char *) station_config.ssid, name, 32);
    strlcpy((char *) station_config.password, pass, 64);
    if (wifi_station_set_config_current(&station_config) != true) {
        NM_ERROR("wifi config set");
        return ESP_E_SYS;
    }

    // Set reconnect policy to true and manage
    // reconnects internally based on recon_max and recon_cnt
    if (wifi_station_set_reconnect_policy(true) != true) {
        NM_ERROR("policy set");
        return ESP_E_SYS;
    }

    // Receive WiFi events.
    sint8 err_eb;
    eb_handle_wifi_events();
    err_eb = eb_attach_wifi_events(nm_wifi_event_cb, EV_GROUP);
    if (err_eb != ESP_OK) {
        NM_ERROR("attach wifi events %d", err_eb);
        return ESP_E_MEM;
    }

    // Create global WiFi struct.
    g_wifi = os_zalloc(sizeof(wifi));
    if (g_wifi == NULL)
        return ESP_E_MEM;

    // Set global error callback.
    nm_g_fatal_err = err_cb;

    // Configure wifi structure.
    g_wifi->recon_max = wifi->recon_max ? wifi->recon_max : (uint8_t) 1;
    g_wifi->recon_cnt = 1;
    g_wifi->ip = wifi->ip;
    g_wifi->netmask = wifi->netmask;
    g_wifi->gw = wifi->gw;

    if (wifi_station_connect() != true) {
        eb_remove_cb(nm_wifi_event_cb);
        NM_ERROR("wifi connect");
        return ESP_E_SYS;
    }

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
wifi_stop()
{
    // Remove WiFi callbacks.
    NM_DEBUG("nm_wifi_stop: removing NM group callbacks");
    eb_remove_group(EV_GROUP);
    tcp_abort_all();
    NM_DEBUG("nm_wifi_stop: station disconnect");
    wifi_station_disconnect();
    NM_DEBUG("nm_wifi_stop: set NULL opmode");
    wifi_set_opmode(NULL_MODE);

    return ESP_OK;
}
