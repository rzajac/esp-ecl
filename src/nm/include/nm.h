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

#include "esp_eb.h"
#include "esp_util.h"
#include "user_config.h"


struct nm_tcp_;

/**
 * The fatal error callback prototype.
 *
 * Receiving this callback means:
 * - the connection is disconnected with error.
 * - the disc_cb will not be called.
 */
typedef void (*nm_err_cb)(struct nm_tcp_ *, sint8 err, sint8 err_sdk);

// The receive callback prototype.
typedef void (*nm_recv_cb)(struct nm_tcp_ *, uint8_t *data, size_t len);

// The send callback prototype.
typedef void (*nm_cb)(struct nm_tcp_ *);

// Represents WiFi connection configuration.
typedef struct {
    // Maximum WiFi connection retries (0 - no limit).
    uint8_t recon_max;

    // Current WiFi connection retries.
    uint8_t recon_cnt;

    // Fatal error callback. Must return as soon as possible.
    nm_err_cb err_cb; // TODO: check if not NULL.

    // Static IP, network mask and gateway.
    uint32_t ip, netmask, gw;
} nm_wifi;

// Represents managed TCP connection.
typedef struct nm_tcp_ {
    // The underlying connection.
    struct espconn *esp;

    // Use SSL for the connection.
    bool ssl;

    // Ready callback.
    // Called when WiFi is connected and IP is available.
    nm_cb ready_cb; // TODO: check if not NULL.

    // Disconnect callback.
    nm_cb disc_cb; // TODO: check if not NULL.

    // Sent callback.
    nm_cb sent_cb; // TODO: check if not NULL.

    // Receive callback.
    nm_recv_cb recv_cb; // TODO: check if not NULL.
} nm_tcp;


/**
 * Start network manager.
 *
 * Configures and schedules WiFi connection.
 * You may call nm_client as many times as you need
 * right after it and wait for ready_cb for the created
 * clients.
 *
 * @param wifi
 * @param name
 * @param pass
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_wifi_start(nm_wifi *wifi, char *name, char *pass);

/**
 * Stop network manager.
 *
 * Stopping network manager:
 *
 * - releases all the allocated memory,
 * - aborts all connected managed network connections,
 * - disconnects from WiFi
 * - turns off WiFi and sets opmode to NULL_MODE.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_stop();

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
 *              Sending will be possible again after ready_cb callback.
 *
 * - sent_cb  - Called when clien successfully sent data.
 *
 * - recv_cb  - Called when client received the data.
 *
 * @param conn
 * @param ready_cb
 * @param sent_cb
 * @param recv_cb
 * @param err_cb
 */
void ICACHE_FLASH_ATTR
nm_set_callbacks(nm_tcp *conn,
                 nm_cb ready_cb,
                 nm_cb disc_cb,
                 nm_cb sent_cb,
                 nm_recv_cb recv_cb);

void ICACHE_FLASH_ATTR
nm_set_reconnect(nm_tcp *conn, uint8_t recon_max);

void ICACHE_FLASH_ATTR
nm_abort(nm_tcp *conn);

sint8 ICACHE_FLASH_ATTR
nm_send(nm_tcp *conn, uint8_t *data, size_t len);

#endif //NM_H