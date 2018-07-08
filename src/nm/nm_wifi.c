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

nm_err ICACHE_FLASH_ATTR
nm_wifi_start(nm_wifi *wifi, char *name, char *pass, nm_err_cb err_cb)
{
    if (g_wifi != NULL)
        return NM_E_WIFI_STARTED;

    uint8 status = wifi_station_get_connect_status();
    if (!(status == STATION_IDLE || status == 255)) {
        NM_ERROR("nm_wifi_start station status %d\n", status);
        return NM_E_STATUS;
    }

    // Switch to station mode and configure static IP if necessary.
    bool suc = wifi_set_opmode_current(STATION_MODE);
    if (!suc)
        return NM_E_MODE;

    // Use DHCP or static IP after connection to WiFi.
    if (wifi->ip == 0 || wifi->netmask == 0 || wifi->gw == 0) {
        suc = wifi_station_dhcpc_start();
        if (!suc)
            return NM_E_DHCP_START;
    } else {
        suc = wifi_station_dhcpc_stop();
        if (!suc)
            return NM_E_DHCP_STOP;

        struct ip_info info;
        info.ip.addr = wifi->ip;
        info.netmask.addr = wifi->netmask;
        info.gw.addr = wifi->gw;
        suc = wifi_set_ip_info(STATION_IF, &info);
        if (!suc)
            return NM_E_STATIC_IP;
    }

    // Configure WiFi.
    struct station_config station_config;
    os_memset(&station_config, 0, sizeof(struct station_config));
    strlcpy((char *) station_config.ssid, name, 32);
    strlcpy((char *) station_config.password, pass, 64);
    suc = wifi_station_set_config_current(&station_config);
    if (!suc)
        return NM_E_WIFI_CFG;

    // Set reconnect policy to true and manage
    // reconnects internally based on recon_max and recon_cnt
    suc = wifi_station_set_reconnect_policy(true);
    if (!suc)
        return NM_E_POLICY;

    // Receive WiFi events.
    esp_eb_err err_eb;
    esp_eb_handle_wifi_events();
    err_eb = esp_eb_attach_wifi_events(nm_wifi_event_cb, EV_GROUP);
    if (err_eb != ESP_EB_OK)
        return NM_E_MEM;

    suc = wifi_station_connect();
    if (!suc) {
        esp_eb_remove_cb(nm_wifi_event_cb);
        return NM_E_WIFI_CONNECT;
    }

    g_wifi = os_zalloc(sizeof(nm_wifi));
    if (g_wifi == NULL)
        return NM_E_MEM;

    g_wifi->recon_max = wifi->recon_max;
    g_wifi->recon_cnt = wifi->recon_cnt;
    g_wifi->err_cb = wifi->err_cb;
    g_wifi->ip = wifi->ip;
    g_wifi->netmask = wifi->netmask;
    g_wifi->gw = wifi->gw;

    return NM_OK;
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