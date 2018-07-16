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
#include "nm_internal.h"

extern nm_err_cb nm_g_fatal_err;

#define use_ka(c) ((c)->ka_idle != 0 && (c)->ka_itvl == 0 && (c)->ka_cnt == 0)

/**
 * Release espconn memory.
 *
 * Sets conn->esp to NULL after release memory.
 *
 * @param conn The managed connection.
 */
void ICACHE_FLASH_ATTR
nm_tcp_release_espconn(nm_tcp *conn);

/**
 * Register callbacks for successful connection or error.
 *
 * @param conn The managed connection.
 *
 * @return The 0 - success, ESPCONN_ARG - fail.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_set_conn_cb(nm_tcp *conn);

/**
 * Manage new connection.
 *
 * @param conn The connection to manage.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_add_conn(nm_tcp *conn);

/**
 * Stop managing the connection.
 *
 * @param conn The managed connection.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_remove_conn(nm_tcp *conn);

/**
 * Connect to a server.
 *
 * @param conn The managed connection.
 *
 * @return ESPCONN_* error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_tcp_connect(nm_tcp *conn);

/** Connect all not connected clients. */
void ICACHE_FLASH_ATTR
nm_tcp_conn_all();

/**
 * Abort all managed connections.
 *
 * Abort will not send any
 */
void ICACHE_FLASH_ATTR
nm_tcp_abort_all();

#endif //NM_TCP_H
