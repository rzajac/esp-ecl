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


#include "nm_wifi.h"

static nm_wifi *g_wifi; // TODO: release it at some point.

sint8 ICACHE_FLASH_ATTR
nm_wifi_start(nm_wifi *wifi, char *name, char *pass)
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

    if (wifi->err_cb == NULL) {
        NM_ERROR("nm_wifi.err_cb must be set");
        return ESP_E_ARG;
    }

    // Switch to station mode and configure static IP if necessary.
    if (wifi_set_opmode_current(STATION_MODE) != true) {
        NM_ERROR("opmode change");
        return ESP_E_SYS;
    }

    // Use DHCP or static IP after connection to WiFi.
    if (wifi->ip == 0 || wifi->netmask == 0 || wifi->gw == 0) {
        if (wifi_station_dhcpc_start() != true) {
            NM_ERROR("DHCP start");
            return ESP_E_SYS;
        }

    } else {
        if (wifi_station_dhcpc_stop() != true) {
            NM_ERROR("DHCP stop");
            return ESP_E_SYS;
        }

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
    esp_eb_err err_eb;
    esp_eb_handle_wifi_events();
    err_eb = esp_eb_attach_wifi_events(nm_wifi_event_cb, EV_GROUP);
    if (err_eb != ESP_OK) {
        NM_ERROR("attach wifi events %d", err_eb);
        return ESP_E_MEM;
    }

    // Create global WiFi struct.
    g_wifi = os_zalloc(sizeof(nm_wifi));
    if (g_wifi == NULL)
        return ESP_E_MEM;

    // Configure wifi structure.
    g_wifi->recon_max = wifi->recon_max;
    g_wifi->recon_cnt = wifi->recon_cnt;
    g_wifi->err_cb = wifi->err_cb;
    g_wifi->ip = wifi->ip;
    g_wifi->netmask = wifi->netmask;
    g_wifi->gw = wifi->gw;

    if (wifi_station_connect() != true) {
        esp_eb_remove_cb(nm_wifi_event_cb);
        NM_ERROR("wifi connect");
        return ESP_E_SYS;
    }

    return ESP_OK;
}

void ICACHE_FLASH_ATTR
nm_wifi_event_cb(uint16_t ev_code, void *arg)
{
    // System_Event_t *ev = arg;

    switch (ev_code) {
        case EVENT_STAMODE_CONNECTED:
            break;

        case EVENT_STAMODE_DISCONNECTED:
            break;

        case EVENT_STAMODE_AUTHMODE_CHANGE:
            break;

        case EVENT_STAMODE_GOT_IP:
            nm_tcp_conn_all();
            break;

        case EVENT_STAMODE_DHCP_TIMEOUT:
            break;

        case EVENT_OPMODE_CHANGED:
            break;

        default:
            break;
    }
}