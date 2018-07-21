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
    os_printf("USR: ready_cb [%p]\n", conn);
    nm_tcp_send(conn, (uint8 *) "HELLO", 5);
}

void ICACHE_FLASH_ATTR
disc_cb(nm_tcp *conn)
{
    os_printf("USR: disc_cb [%p]\n", conn);
}

void ICACHE_FLASH_ATTR
sent_cb(nm_tcp *conn)
{
    os_printf("USR: sent_cb [%p]\n", conn);
}

void ICACHE_FLASH_ATTR
recv_cb(nm_tcp *conn, uint8_t *data, size_t len)
{
    UNUSED(data);

    os_printf("USR: rcv_cb len:%d [%p]\n", len, conn);
    nm_tcp_send(conn, (uint8 *) "HELLO", 5);
}

void ICACHE_FLASH_ATTR
err_cb(nm_tcp *conn, sint8 err, sint16 err_sdk)
{
    os_printf("USER: err_cb err:%d err_sdk:%d [%p]\n", err, err_sdk, conn);
    nm_tcp_release(conn);
}

void ICACHE_FLASH_ATTR
fatal_cb(nm_tcp *conn, sint8 err, sint16 err_sdk)
{
    os_printf("USR: fatal_cb err:%d err_sdk:%d [%p]\n", err, err_sdk, conn);

    // Connected to WiFi - schedule client connection.
    // if (conn == NULL && err == ESP_OK && err_sdk == 0) {
    //     conn = os_zalloc(sizeof(nm_tcp));
    //     if (conn == NULL) {
    //         os_printf("USR: OOM!\n");
    //         nm_wifi_stop();
    //     }
    //
    //     err = nm_client(conn, "192.168.1.149", 3333, false);
    //     if (err != ESP_OK) {
    //         os_printf("USR: esp_nm_client error %d\n", err);
    //         nm_wifi_stop();
    //     }
    //
    //     nm_set_callbacks(conn, ready_cb, disc_cb, sent_cb, rcv_cb, err_cb);
    //
    //     err = nm_client_connect(conn);
    //     if (err != ESP_OK) {
    //         os_printf("USR: nm_client_connect error %d\n", err);
    //         nm_wifi_stop();
    //     }
    // }
}
