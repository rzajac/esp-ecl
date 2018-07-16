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

#include "nm_tcp.h"

/////////////////////////////////////////////////////////////////////////////
// Declarations.
/////////////////////////////////////////////////////////////////////////////

static nm_tcp *ICACHE_FLASH_ATTR
tcp_find_by_esp(struct espconn *esp);

static void ICACHE_FLASH_ATTR
tcp_disc_cb(void *arg);

static void ICACHE_FLASH_ATTR
tcp_receive_cb(void *arg, char *data, unsigned short len);

static void ICACHE_FLASH_ATTR
tcp_error_cb(void *arg, sint8 errCode);

static void ICACHE_FLASH_ATTR
tcp_sent_cb(void *arg);

static void ICACHE_FLASH_ATTR
tcp_connect_cb(void *arg);

/////////////////////////////////////////////////////////////////////////////
// Code.
/////////////////////////////////////////////////////////////////////////////

// Linked list head.
static esp_dll_node *head;

void ICACHE_FLASH_ATTR
nm_tcp_release_espconn(nm_tcp *conn)
{
    if (conn->esp == NULL)
        return;

    if (conn->esp->proto.tcp != NULL)
        os_free(conn->esp->proto.tcp);

    os_free(conn->esp);
    conn->esp = NULL;

    NM_DEBUG("espconn released [%p]", conn);
}

sint8 ICACHE_FLASH_ATTR
nm_tcp_set_conn_cb(nm_tcp *conn)
{
    sint8 err = espconn_regist_connectcb(conn->esp, tcp_connect_cb);
    if (err != 0) {
        NM_ERROR("nm_tcp_set_conn_cb error %d [%p]", err, conn);
        return err;
    }

    err = espconn_regist_reconcb(conn->esp, tcp_error_cb);
    if (err != 0) {
        NM_ERROR("nm_tcp_set_conn_cb error %d [%p]", err, conn);
        return err;
    }

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
nm_tcp_add_conn(nm_tcp *conn)
{
    // Prevent adding the same connection twice.
    if (tcp_find_by_esp(conn->esp) != NULL) {
        return ESP_E_ARG;
    }

    esp_dll_node *node = esp_dll_new(conn);
    if (node == NULL)
        return ESP_E_MEM;

    if (head == NULL) {
        head = node;
        return ESP_EB_OK;
    }

    esp_dll_append(head, node);
    NM_DEBUG("managing [%p]", conn);

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
nm_tcp_remove_conn(nm_tcp *conn)
{
    NM_DEBUG("stop managing [%p]", conn);
    esp_dll_node *node = esp_dll_find(head, conn);
    if (node == NULL) {
        NM_ERROR("nm_tcp_remove_conn not found [%p]", conn);
        return ESP_E_ARG;
    }

    if (node == head) {
        head = NULL;
    }

    esp_dll_remove(node);
    NM_DEBUG("stop managing - done [%p]", conn);
    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
nm_tcp_connect(nm_tcp *conn)
{
    sint8 err;

    NM_DEBUG("connecting [%p]", conn);
    if (is_conn_ready(conn) || is_conn_closed(conn)) {
        err = espconn_connect(conn->esp);
        if (err != 0) {
            NM_ERROR("nm_tcp_connect error %d [%p]", err, conn);
            nm_tcp_remove_conn(conn);
            conn->err_cb(conn, ESP_E_NET, err);
            return err;
        }
        return ESPCONN_OK;
    }
    NM_ERROR("nm_tcp_connect wrong state %d [%p]", conn->esp->state, conn);

    return ESPCONN_ARG;
}

void ICACHE_FLASH_ATTR
nm_tcp_conn_all()
{
    sint8 err;
    nm_tcp *conn;

    esp_dll_node *curr = head;
    while (curr != NULL) {
        conn = get_conn(curr);
        err = nm_tcp_connect(conn);
        if (err != 0) {
            NM_ERROR("nm_tcp_conn_all error %d [%p]", err, curr);
            conn->err_cb(conn, ESP_E_NET, err);
        }
        curr = curr->next;
    }
}

void ICACHE_FLASH_ATTR
nm_tcp_abort_all()
{
    NM_DEBUG("nm_tcp_abort_all");
    esp_dll_node *curr = head;
    esp_dll_node *next = NULL;
    while (curr != NULL) {
        next = curr->next;
        nm_abort(get_conn(curr));
        curr = next;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Static methods.
/////////////////////////////////////////////////////////////////////////////

static nm_tcp *ICACHE_FLASH_ATTR
tcp_find_by_esp(struct espconn *esp)
{
    nm_tcp *conn;
    esp_dll_node *curr = head;
    while (curr != NULL) {
        conn = get_conn(curr);
        if (conn->esp == esp)
            return conn;
        curr = curr->next;
    }
    return NULL;
}

static void ICACHE_FLASH_ATTR
tcp_disc_cb(void *arg)
{
    struct espconn *esp = (struct espconn *) arg;
    nm_tcp *conn = tcp_find_by_esp(esp);

    if (conn == NULL) {
        NM_ERROR("tcp_disc_cb unknown connection [%p]", conn);
        return;
    }

    NM_DEBUG_CONN("tcp_disc_cb", conn);

    conn->disc_cb(conn);
    nm_tcp_release_espconn(conn);
}

static void ICACHE_FLASH_ATTR
tcp_receive_cb(void *arg, char *data, unsigned short len)
{
    struct espconn *esp = (struct espconn *) arg;
    nm_tcp *conn = tcp_find_by_esp(esp);
    if (conn == NULL) {
        NM_ERROR("tcp_receive_cb unknown connection [%p]", conn);
        return;
    }

    NM_DEBUG_CONN("tcp_receive_cb", conn);
    conn->rcv_cb(conn, (uint8_t *) data, len);
}

static void ICACHE_FLASH_ATTR
tcp_error_cb(void *arg, sint8 errCode)
{
    struct espconn *esp = (struct espconn *) arg;
    nm_tcp *conn = tcp_find_by_esp(esp);
    if (conn == NULL) {
        NM_ERROR("tcp_error_cb unknown connection [%p]", conn);
        return;
    }

    NM_DEBUG_CONN("tcp_error_cb", conn);
    conn->err_cb(conn, ESP_E_NET, errCode);
}

static void ICACHE_FLASH_ATTR
tcp_sent_cb(void *arg)
{
    struct espconn *esp = (struct espconn *) arg;
    nm_tcp *conn = tcp_find_by_esp(esp);
    if (conn == NULL) {
        NM_ERROR("tcp_sent_cb unknown connection [%p]", conn);
        return;
    }

    NM_DEBUG_CONN("tcp_sent_cb", conn);
    conn->sent_cb(conn);
}

static void ICACHE_FLASH_ATTR
tcp_connect_cb(void *arg)
{
    struct espconn *esp = (struct espconn *) arg;
    nm_tcp *conn = tcp_find_by_esp(esp);

    if (conn == NULL) {
        NM_ERROR("tcp_connect_cb unknown connection [%p]", conn);
        return;
    }

    sint8 err = espconn_set_opt(conn->esp, ESPCONN_REUSEADDR);
    if (err != 0) {
        NM_ERROR("espconn_set_opt error %d [%p]", err, conn);
        nm_g_fatal_err(conn, ESP_E_NET, err);
        return;
    }

    // Set default keep alive configuration.
    // Send keep alive packet every 3 seconds. If sending
    // fails ESP8266 will try to send 4 keep alive packets
    // every 5 seconds. If after that time (3s + 4 * 5s = 23s) there
    // is no response err_cb will be called on the connection with
    // ESP_NET and `SPCONN_ABRT` error codes.
    if (!use_ka(conn)) {
        conn->ka_idle = NM_DEFAULT_KA_IDLE;
        conn->ka_itvl = NM_DEFAULT_KA_ITVL;
        conn->ka_cnt = NM_DEFAULT_KA_CNT;
    }

    espconn_set_opt(conn->esp, ESPCONN_KEEPALIVE);
    espconn_set_keepalive(conn->esp, ESPCONN_KEEPIDLE, &conn->ka_idle);
    espconn_set_keepalive(conn->esp, ESPCONN_KEEPINTVL, &conn->ka_itvl);
    espconn_set_keepalive(conn->esp, ESPCONN_KEEPCNT, &conn->ka_cnt);

    espconn_regist_disconcb(conn->esp, tcp_disc_cb);
    espconn_regist_recvcb(conn->esp, tcp_receive_cb);
    espconn_regist_sentcb(conn->esp, tcp_sent_cb);

    NM_DEBUG_CONN("tcp_connect_cb", conn);
    conn->ready_cb(conn);
}
