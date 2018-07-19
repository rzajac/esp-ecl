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

#ifndef NM_H
#define NM_H

#include <user_interface.h>
#include <espconn.h>
#include <mem.h>
#include <esp.h>

#include "event_bus.h"
#include "utils.h"

// Default keep alive values.
#define NM_DEFAULT_KA_IDLE 3
#define NM_DEFAULT_KA_ITVL 5
#define NM_DEFAULT_KA_CNT  4

// Declaration.
struct nm_tcp_;

// Type alias.
typedef struct nm_tcp_ nm_tcp;

/**
 * The fatal error callback prototype.
 *
 * Receiving this callback means:
 * - the connection is disconnected with error.
 * - the disc_cb will not be called.
 *
 * If there is no err_sdk available the 0 will be passed.
 */
typedef void (*nm_err_cb)(nm_tcp*, sint8 err, sint16 err_sdk);

// The receive callback prototype.
typedef void (*nm_rcv_cb)(nm_tcp*, uint8_t *data, size_t len);

// The send callback prototype.
typedef void (*nm_cb)(nm_tcp *);







/**
 * Set keep alive configuration for the client.
 *
 * @param conn
 * @param idle
 * @param intvl
 * @param cnt
 */
void ICACHE_FLASH_ATTR
nm_set_keepalive(nm_tcp *conn, int idle, int intvl, int cnt);

/**
 * Set callbacks.
 *
 * Callbacks are triggered based on network events.
 *
 * - ready_cb - Called when client is ready to send / receive.
 *
 * - disc_cb  - Called when client has been disconnected from a server.
 *              After this call sending data will return error.
 *              Sending will be possible again after receiving
 *              ready_cb callback.
 *
 * - sent_cb  - Called when clien successfully sent data.
 *
 * - rcv_cb  - Called when client received the data.
 *
 * @param conn
 * @param ready_cb
 * @param disc_cb
 * @param sent_cb
 * @param rcv_cb
 * @param err_cb
 */
void ICACHE_FLASH_ATTR
nm_set_callbacks(nm_tcp *conn,
                 nm_cb ready_cb,
                 nm_cb disc_cb,
                 nm_cb sent_cb,
                 nm_rcv_cb rcv_cb,
                 nm_err_cb err_cb);

/**
 * Connect client.
 *
 * @param conn The connection to manage.
 *
 * @return Erorr code.
 */
sint8 ICACHE_FLASH_ATTR
nm_client_connect(nm_tcp *conn);

sint8 ICACHE_FLASH_ATTR
nm_client_release(nm_tcp *conn);

sint8 ICACHE_FLASH_ATTR
nm_disconnect(nm_tcp *conn);

void ICACHE_FLASH_ATTR
nm_abort(nm_tcp *conn);

sint8 ICACHE_FLASH_ATTR
nm_send(nm_tcp *conn, uint8_t *data, size_t len);

#endif //NM_H
