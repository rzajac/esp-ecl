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


#include "nm.h"
#include "internal.h"

/////////////////////////////////////////////////////////////////////////////
// Declarations.
/////////////////////////////////////////////////////////////////////////////

static void ICACHE_FLASH_ATTR
connect_all();

/////////////////////////////////////////////////////////////////////////////
// Code.
/////////////////////////////////////////////////////////////////////////////


nm_err ICACHE_FLASH_ATTR
nm_stop()
{
    // Remove WiFi callbacks.
    esp_eb_remove_group(EV_GROUP);
    nm_tcp_abort_all();
    wifi_station_disconnect();
    wifi_set_opmode(NULL_MODE);

    return NM_OK;
}

nm_err ICACHE_FLASH_ATTR
nm_client(nm_tcp *conn, char *host, int port, bool ssl)
{
    conn->esp = os_zalloc(sizeof(struct espconn));
    if (conn->esp == NULL)
        return NM_E_MEM;

    conn->esp->proto.tcp = os_zalloc(sizeof(esp_tcp));
    if (conn->esp->proto.tcp == NULL) {
        nm_tcp_release_espconn(conn);
        return NM_E_MEM;
    }

    conn->ssl = ssl;

    // Configure TCP/IP connection.
    conn->esp->type = ESPCONN_TCP;
    uint32_t ip = ipaddr_addr(host);
    os_memcpy(conn->esp->proto.tcp->remote_ip, &ip, 4);
    conn->esp->proto.tcp->local_port = espconn_port();
    conn->esp->proto.tcp->remote_port = port;

    // Register callbacks for successful connection or error.
    sint8 err = nm_tcp_set_conn_cb(conn);
    if (err != 0)
        return NM_E_TCP_CONFIG;

    return NM_OK;
}

void ICACHE_FLASH_ATTR
nm_abort(nm_tcp *conn)
{
    // Prevent callbacks after call to espconn_abort.
    espconn_regist_reconcb(conn->esp, NULL);
    espconn_regist_disconcb(conn->esp, NULL);

    // Abort and release memory.
    espconn_abort(conn->esp);
    nm_tcp_release_espconn(conn);
}

void ICACHE_FLASH_ATTR
nm_set_keepalive(nm_tcp *conn, int idle, int intvl, int cnt)
{
    // conn->ka_idle = idle;
    // conn->ka_intvl = intvl;
    // conn->ka_cnt = cnt;
}

void ICACHE_FLASH_ATTR
nm_set_callbacks(nm_tcp *conn,
                     nm_cb ready_cb,
                     nm_cb disc_cb,
                     nm_cb sent_cb,
                     nm_recv_cb recv_cb,
                     nm_err_cb err_cb)
{
    conn->ready_cb = ready_cb;
    conn->ready_cb = disc_cb;
    conn->sent_cb = sent_cb;
    conn->recv_cb = recv_cb;
    conn->err_cb = err_cb;
}

void ICACHE_FLASH_ATTR
nm_set_reconnect(nm_tcp *conn, uint8_t recon_max)
{
    // conn->recon_max = recon_max;
    // conn->recon_cnt = 0;
}

sint8 ICACHE_FLASH_ATTR
nm_send(nm_tcp *conn, uint8_t *data, size_t len)
{
    sint8 err = espconn_send(conn->esp, data, (uint16) len);
    NM_ERROR("nm_send error %d\n", err);
    return err;
}

sint8 ICACHE_FLASH_ATTR
nm_client_connect(nm_tcp *conn)
{
    sint8 err = espconn_connect(conn->esp);
    NM_ERROR("nm_client_connect error %d\n", err);
    return err;
}
