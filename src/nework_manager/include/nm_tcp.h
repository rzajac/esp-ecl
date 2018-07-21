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

#ifndef NM_TCP_H
#define NM_TCP_H

#include <user_interface.h>

#include "nm.h"

// Represents managed TCP connection.
struct _nm_tcp {
    // The underlying connection.
    struct espconn *esp;

    // Use SSL for the connection.
    bool ssl;

    // Keep alive idle.
    int ka_idle;

    // Keep alive interval.
    int ka_itvl;

    // Keep alive count.
    int ka_cnt;

    // Ready callback. Called when WiFi is connected and IP is available.
    nm_cb ready_cb;

    // Disconnect callback.
    nm_cb disc_cb;

    // Sent callback.
    nm_cb sent_cb;

    // Receive callback.
    nm_rcv_cb rcv_cb;

    // Non fatal error callback.
    nm_err_cb err_cb;
};

/**
 * Create and configure new network client.
 *
 * This call does only preliminary
 * configuration and does not not
 * connect to server. It just configures
 * the client.
 *
 * Most likely you want to call
 * nm_set_callbacks right after this call.
 *
 * More configuration can be done by calling:
 * - tcp_set_keepalive
 * - tcp_set_callbacks
 * - tcp_set_reconnect
 *
 * @param conn The connection to configure.
 * @param host The server address.
 * @param port The server port.
 * @param ssl  Use SSL.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_client(nm_tcp *conn, char *host, int port, bool ssl);


/**
 * Set keep alive configuration for the client.
 *
 * @param conn The connection to configure.
 * @param idle The idle time in seconds.
 * @param itvl The interval in seconds.
 * @param cnt  The count.
 */
void ICACHE_FLASH_ATTR
nm_tcp_set_kalive(nm_tcp *conn, int idle, int itvl, int cnt);

/**
 * Set client callbacks.
 *
 * Callbacks are triggered based on network events.
 *
 * - ready_cb - Called when client is ready to send / receive.
 *
 * - disc_cb  - Called when client has been disconnected from a server.
 *              After this call sending data will return error.
 *              Sending will be possible again after receiving
 *              ready_cb callback or after connecting if disconnected
 *              manually.
 *
 * - sent_cb  - Called when client successfully sent data.
 *
 * - rcv_cb   - Called when client received data.
 *
 * @param conn  The connection to configure callbacks for.
 * @param ready The ready callback.
 * @param disc  The disconnect callback.
 * @param sent  The data sent callback.
 * @param rcv   The receive data callback.
 * @param err   The error callback.
 */
void ICACHE_FLASH_ATTR
nm_tcp_set_callbacks(nm_tcp *conn,
                     nm_cb ready,
                     nm_cb disc,
                     nm_cb sent,
                     nm_rcv_cb rcv,
                     nm_err_cb err);

/**
 * Connect client.
 *
 * @param conn The connection to manage.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_connect(nm_tcp *conn);

/**
 * Disconnect TCP connection.
 *
 * @param conn The connection to disconnect.
 *
 * @return The error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_disconnect(nm_tcp *conn);

/**
 * Release memory.
 *
 * @param conn The connection to abort.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_release(nm_tcp *conn);

/**
 * Abort connection.
 *
 * @param conn The managed connection.
 */
void ICACHE_FLASH_ATTR
nm_tcp_abort(nm_tcp *conn);

/**
 * Send data through connection.
 *
 * @param conn The managed connection.
 * @param data The data to send.
 * @param len  The length of the data.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_send(nm_tcp *conn, uint8_t *data, size_t len);

#endif // NM_TCP_H
