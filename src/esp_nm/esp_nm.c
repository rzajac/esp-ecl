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


#include "include/esp_nm.h"
#include "esp_nm_internal.h"

/////////////////////////////////////////////////////////////////////////////
// Declarations.
/////////////////////////////////////////////////////////////////////////////

static void ICACHE_FLASH_ATTR
wifi_event_cb(uint16_t ev_code, void *arg);

static esp_nm_err ICACHE_FLASH_ATTR
release_espconn(esp_nm_conn *conn, esp_nm_err err);

static void ICACHE_FLASH_ATTR
connect_cb(void *arg);

static void ICACHE_FLASH_ATTR
sent_cb(void *arg);

static void ICACHE_FLASH_ATTR
recv_cb(void *arg, char *pdata, unsigned short len);

static void ICACHE_FLASH_ATTR
error_cb(void *arg, sint8 errCode);

static void ICACHE_FLASH_ATTR
disc_cb(void *arg);

/////////////////////////////////////////////////////////////////////////////
// Code.
/////////////////////////////////////////////////////////////////////////////

// Linked list head.
static esp_dll_node *head;

esp_nm_conn *ICACHE_FLASH_ATTR
esp_nm_lst_find(struct espconn *esp)
{
    return get_conn(esp_dll_find(head, esp));
}

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_start(char *wifi_name,
             char *wifi_pass,
             bool reconnect_policy,
             uint32_t static_ip,
             uint32_t static_netmask,
             uint32_t static_gw)
{
    if (wifi_station_get_connect_status() != STATION_IDLE)
        return ESP_NM_E_STATUS;

    // Switch to station mode and configure static IP if necessary.
    bool suc = wifi_set_opmode_current(STATION_MODE);
    if (!suc)
        return ESP_NM_E_MODE;

    if (static_ip == 0 || static_netmask == 0 || static_gw == 0) {
        suc = wifi_station_dhcpc_start();
        if (!suc)
            return ESP_NM_E_DHCP_START;
    } else {
        suc = wifi_station_dhcpc_stop();
        if (!suc)
            return ESP_NM_E_DHCP_STOP;

        struct ip_info info;
        info.ip.addr = static_ip;
        info.netmask.addr = static_netmask;
        info.gw.addr = static_gw;
        suc = wifi_set_ip_info(STATION_IF, &info);
        if (!suc)
            return ESP_NM_E_STATIC_IP;
    }

    // Configure WiFi.
    struct station_config station_config;
    os_memset(&station_config, 0, sizeof(struct station_config));
    strlcpy((char *) station_config.ssid, wifi_name, 32);
    strlcpy((char *) station_config.password, wifi_pass, 64);
    suc = wifi_station_set_config_current(&station_config);
    if (!suc)
        return ESP_NM_E_WIFI_CFG;

    suc = wifi_station_set_reconnect_policy(reconnect_policy);
    if (!suc)
        return ESP_NM_E_POLICY;

    // Receive WiFi events.
    esp_eb_handle_wifi_events();
    esp_eb_err err_eb = esp_eb_attach_wifi_events(wifi_event_cb,
                                                  ESP_NM_EV_GROUP);
    if (err_eb != ESP_EB_OK)
        return ESP_NM_E_MEM;

    suc = wifi_station_connect();
    if (!suc) {
        esp_eb_remove_cb(wifi_event_cb);
        return ESP_NM_E_WIFI_CONNECT;
    }

    return ESP_NM_OK;
}

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_stop()
{
    // Remove WiFi callbacks.
    esp_eb_remove_group(ESP_NM_EV_GROUP);

    // TODO:
    // - abort all managed connections.
    // - disconnect from WiFi
    // - turn off WiFi (opmode)

    return ESP_NM_OK;
}

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_client(esp_nm_conn *conn, char *host, int port, bool ssl)
{
    conn->esp = os_zalloc(sizeof(struct espconn));
    if (conn->esp == NULL)
        return ESP_NM_E_MEM;

    conn->esp->proto.tcp = os_zalloc(sizeof(esp_tcp));
    if (conn->esp->proto.tcp == NULL)
        return release_espconn(conn, ESP_NM_E_MEM);

    conn->ssl = ssl;

    // Configure TCP/IP connection.
    conn->esp->type = ESPCONN_TCP;
    uint32_t ip = ipaddr_addr(host);
    os_memcpy(conn->esp->proto.tcp->remote_ip, &ip, 4);
    conn->esp->proto.tcp->local_port = espconn_port();
    conn->esp->proto.tcp->remote_port = port;

    // Register callbacks for successful connection or error.
    sint8 err = espconn_regist_connectcb(conn->esp, connect_cb);
    if (err != 0)
        ESP_NM_ERROR("regist_connectcb fail %d\n", err);

    err = espconn_regist_reconcb(conn->esp, error_cb);
    if (err != 0)
        ESP_NM_ERROR("regist_reconcb fail %d\n", err);

    return ESP_NM_OK;
}

sint8 ICACHE_FLASH_ATTR
esp_nm_connect(esp_nm_conn *conn)
{
    sint8 err = espconn_connect(conn->esp);
    if (err != 0)
        return err;


}

void ICACHE_FLASH_ATTR
esp_nm_remove(esp_nm_conn *conn)
{
    // Prevent callbacks after call to espconn_abort.
    espconn_regist_reconcb(conn->esp, NULL);
    espconn_regist_disconcb(conn->esp, NULL);

    // Abort and release memory.
    espconn_abort(conn->esp);
    release_espconn(conn, ESP_NM_OK);
}

void ICACHE_FLASH_ATTR
esp_nm_set_keepalive(esp_nm_conn *conn, int idle, int intvl, int cnt)
{
    conn->ka_idle = idle;
    conn->ka_intvl = intvl;
    conn->ka_cnt = cnt;
}

void ICACHE_FLASH_ATTR
esp_nm_set_callbacks(esp_nm_conn *conn,
                     esp_nm_cb ready_cb,
                     esp_nm_cb sent_cb,
                     esp_nm_recv_cb recv_cb,
                     esp_nm_err_cb err_cb)
{
    conn->ready_cb = ready_cb;
    conn->send_cb = sent_cb;
    conn->recv_cb = recv_cb;
    conn->err_cb = err_cb;
}

void ICACHE_FLASH_ATTR
esp_nm_set_reconnect(esp_nm_conn *conn, uint8_t recon_max)
{
    conn->recon_max = recon_max;
    conn->recon_cnt = 0;
}


sint8 ICACHE_FLASH_ATTR
esp_nm_send(esp_nm_conn *conn, uint8_t *data, size_t len)
{
    return espconn_send(conn->esp, data, (uint16) len);
}

static void ICACHE_FLASH_ATTR
wifi_event_cb(uint16_t ev_code, void *arg)
{
    System_Event_t *ev = arg;

    UNUSED(ev);

    switch (ev_code) {
        case EVENT_STAMODE_CONNECTED:
            break;

        case EVENT_STAMODE_DISCONNECTED:
            break;

        case EVENT_STAMODE_AUTHMODE_CHANGE:
            break;

        case EVENT_STAMODE_GOT_IP:
            break;

        case EVENT_STAMODE_DHCP_TIMEOUT:
            break;

        case EVENT_OPMODE_CHANGED:
            break;

        default:
            break;
    }
}

/**
 * Release espconn memory.
 *
 * @param conn The managed connection.
 *
 * @return Passthrough error.
 */
static esp_nm_err ICACHE_FLASH_ATTR
release_espconn(esp_nm_conn *conn, esp_nm_err err)
{
    if (conn->esp == NULL)
        return err;
    if (conn->esp->proto.tcp != NULL)
        os_free(conn->esp->proto.tcp);

    ESP_NM_DEBUG("release_espconn\n");
    os_free(conn->esp);
    conn->esp = NULL;
    return err;
}

static void ICACHE_FLASH_ATTR
connect_cb(void *arg)
{
    struct espconn *esp = (struct espconn *) arg;
    esp_nm_conn *conn = find_conn(esp);

    // TODO: what if conn is NULL?

    if (conn->ka_idle && conn->ka_intvl && conn->ka_cnt) {
        espconn_set_opt(conn->esp, ESPCONN_KEEPALIVE);
        espconn_set_keepalive(conn->esp, ESPCONN_KEEPIDLE, &conn->ka_idle);
        espconn_set_keepalive(conn->esp, ESPCONN_KEEPINTVL, &conn->ka_intvl);
        espconn_set_keepalive(conn->esp, ESPCONN_KEEPCNT, &conn->ka_cnt);
    }

    espconn_regist_disconcb(conn->esp, disc_cb);
    espconn_regist_recvcb(conn->esp, recv_cb);
    espconn_regist_sentcb(conn->esp, sent_cb);
}

static void ICACHE_FLASH_ATTR
recv_cb(void *arg, char *pdata, unsigned short len)
{
    struct espconn *esp = (struct espconn *) arg;
    esp_nm_conn *conn = find_conn(esp);
    if (conn == NULL) {
        ESP_NM_ERROR("recv_cb on not managed connection %p\n", conn);
        return;
    }

    conn->recv_cb(conn, (uint8_t *) pdata, len);
}

static void ICACHE_FLASH_ATTR
error_cb(void *arg, sint8 errCode)
{
    struct espconn *esp = (struct espconn *) arg;
    esp_nm_conn *conn = find_conn(esp);
    if (conn == NULL) {
        ESP_NM_ERROR("error_cb on not managed connection %p\n", conn);
        return;
    }

    UNUSED(errCode);
}

static void ICACHE_FLASH_ATTR
disc_cb(void *arg)
{
    struct espconn *esp = (struct espconn *) arg;
    esp_nm_conn *conn = find_conn(esp);

    if (conn == NULL) {
        ESP_NM_ERROR("disc_cb on not managed connection %p\n", conn);
        return;
    }
}

static void ICACHE_FLASH_ATTR
sent_cb(void *arg)
{
    struct espconn *esp = (struct espconn *) arg;
    esp_nm_conn *conn = find_conn(esp);
    if (conn == NULL) {
        ESP_NM_ERROR("sent_cb on not managed connection %p\n", conn);
        return;
    }
}
