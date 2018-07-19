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

#ifndef ESP_EX_CALLBACKS_H
#define ESP_EX_CALLBACKS_H

#include <user_interface.h>

#include <esp.h>
#include "../../src/nework_manager/include/nm.h"

void ICACHE_FLASH_ATTR
ready_cb(nm_tcp *conn);

void ICACHE_FLASH_ATTR
disc_cb(nm_tcp *conn);

void ICACHE_FLASH_ATTR
sent_cb(nm_tcp *conn);

void ICACHE_FLASH_ATTR
recv_cb(nm_tcp *conn, uint8_t *data, size_t len);

void ICACHE_FLASH_ATTR
err_cb(nm_tcp *conn, sint8 err, sint16 aux);

void ICACHE_FLASH_ATTR
fatal_cb(nm_tcp *conn, sint8 err, sint16 aux);

#endif //ESP_EX_CALLBACKS_H
