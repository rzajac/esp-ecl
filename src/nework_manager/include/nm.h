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
struct _tcp;

// Type alias.
typedef struct _tcp tcp;

/**
 * The fatal error callback prototype.
 *
 * Receiving this callback means:
 * - the connection is disconnected with error.
 * - the disc_cb will not be called.
 *
 * If there is no err_sdk available the 0 will be passed.
 */
typedef void (*nm_err_cb)(tcp* conn, sint8 err, sint16 err_sdk);

// The receive data callback prototype.
typedef void (*nm_rcv_cb)(tcp* conn, uint8_t *data, size_t len);

// The send data callback prototype.
typedef void (*nm_cb)(tcp *conn);


/** NOOP error function */
void ICACHE_FLASH_ATTR
nm_err_noop(tcp *conn, sint8 err, sint16 err_sdk);

/** NOOP receive function */
void ICACHE_FLASH_ATTR
nm_rcv_noop(tcp *conn, uint8_t *data, size_t len);

/** NOOP callback function */
void ICACHE_FLASH_ATTR
nm_cb_noop(tcp *conn);

#endif // NM_H
