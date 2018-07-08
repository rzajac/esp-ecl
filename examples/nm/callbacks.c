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

#include "callbacks.h"

void ICACHE_FLASH_ATTR
ready_cb(nm_tcp *conn)
{
    os_printf("USER: ready_cb called\n");
}

void ICACHE_FLASH_ATTR
disc_cb(nm_tcp *conn)
{
    os_printf("USER: disc_cb called\n");
}

void ICACHE_FLASH_ATTR
sent_cb(nm_tcp *conn)
{
    os_printf("USER: sent_cb called\n");
}

void ICACHE_FLASH_ATTR
recv_cb(nm_tcp *conn, uint8_t *data, size_t len)
{
    os_printf("USER: recv_cb called\n");
}

void ICACHE_FLASH_ATTR
err_cb(nm_tcp *conn, nm_err err, sint8 err2)
{
    os_printf("USER: err_cb called %d %d\n", err, err2);
}
