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

// Represents managed TCP connection.
struct nm_tcp_ {
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

    // Ready callback.
    // Called when WiFi is connected and IP is available.
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
 * This call does only preliminary configuration and
 * does not not connect to server it just
 * configures the client.
 *
 * Most likely you want to call nm_set_callbacks
 * right after this call.
 *
 * More configuration can be done by calling:
 * - nm_set_keepalive
 * - nm_set_callbacks
 * - nm_set_reconnect
 *
 * @param conn
 * @param host
 * @param port
 * @param ssl
 * @return
 */
sint8 ICACHE_FLASH_ATTR
nm_client(nm_tcp *conn, char *host, int port, bool ssl);

#endif // NM_TCP_H
