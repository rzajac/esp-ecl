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
#include "nm_internal.h"

/////////////////////////////////////////////////////////////////////////////
// Declarations.
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Code.
/////////////////////////////////////////////////////////////////////////////

/** NOOP error function */
void ICACHE_FLASH_ATTR
nm_err_noop(nm_tcp *conn, sint8 err, sint16 err_sdk)
{}

/** NOOP receive function */
void ICACHE_FLASH_ATTR
nm_rcv_noop(nm_tcp *conn, uint8_t *data, size_t len)
{}


/** NOOP callback function */
void ICACHE_FLASH_ATTR
nm_cb_noop(nm_tcp *conn)
{}

// Global fatal error callback. Must return as soon as possible.
nm_err_cb nm_g_fatal_err = nm_err_noop;





sint8 ICACHE_FLASH_ATTR
nm_disconnect(nm_tcp *conn)
{
    sint8 err = espconn_disconnect(conn->esp);
    if (err != ESPCONN_OK) {
        NM_ERROR("espconn_disconnect error %d [%p]", err, conn);
    }
    return err;
}

void ICACHE_FLASH_ATTR
nm_abort(nm_tcp *conn)
{
    NM_DEBUG("aborting [%p]", conn);

    // Prevent callbacks after call to espconn_abort.
    espconn_regist_reconcb(conn->esp, NULL);
    espconn_regist_disconcb(conn->esp, NULL);

    // Abort and release memory.
    espconn_abort(conn->esp);
    nm_tcp_release_espconn(conn);
    nm_tcp_remove_conn(conn);
}

void ICACHE_FLASH_ATTR
nm_set_keepalive(nm_tcp *conn, int idle, int intvl, int cnt)
{
    conn->ka_idle = idle;
    conn->ka_itvl = intvl;
    conn->ka_cnt = cnt;
}

void ICACHE_FLASH_ATTR
nm_set_callbacks(nm_tcp *conn,
                 nm_cb ready_cb,
                 nm_cb disc_cb,
                 nm_cb sent_cb,
                 nm_rcv_cb rcv_cb,
                 nm_err_cb err_cb)
{
    conn->ready_cb = ready_cb;
    conn->disc_cb = disc_cb;
    conn->sent_cb = sent_cb;
    conn->rcv_cb = rcv_cb;
    conn->err_cb = err_cb;

    NM_DEBUG("set callbacks for [%p]", conn);
}

sint8 ICACHE_FLASH_ATTR
nm_client_connect(nm_tcp *conn)
{
    sint8 err = nm_tcp_add_conn(conn);
    if (err != ESPCONN_OK) {
        return err;
    }

    if (wifi_station_get_connect_status() == STATION_GOT_IP) {
        return nm_tcp_connect(conn);
    }

    NM_DEBUG("connection scheduled [%p] ", conn);
    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
nm_client_release(nm_tcp *conn)
{
    NM_DEBUG("nm_client_release [%p] ", conn);

    if (conn->esp->state != ESPCONN_CLOSE) {
        nm_abort(conn);
        return ESP_OK;
    }

    // Prevent callbacks.
    espconn_regist_reconcb(conn->esp, NULL);
    espconn_regist_disconcb(conn->esp, NULL);
    espconn_regist_recvcb(conn->esp, NULL);
    espconn_regist_sentcb(conn->esp, NULL);
    nm_tcp_release_espconn(conn);
    os_free(conn);

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
nm_send(nm_tcp *conn, uint8_t *data, size_t len)
{
    sint8 err = espconn_send(conn->esp, data, (uint16) len);
    if (err != ESPCONN_OK) {
        NM_ERROR("nm_send error %d [%p]", err, conn);
        conn->err_cb(conn, ESP_E_NET, err);
    }
    return err;
}
