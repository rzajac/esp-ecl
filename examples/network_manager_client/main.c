/**
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


#include <user_interface.h>
#include <osapi.h>

#include "stdout.h"
#include "network_manager.h"
#include "callbacks.h"

void ICACHE_FLASH_ATTR
use_dhcp(void)
{
    os_printf("USR: system initialized\n");

    nm_wifi wifi;
    memset(&wifi, 0, sizeof(nm_wifi));
    wifi.recon_max = 10;

    wifi.ip = ipaddr_addr("192.168.2.101");
    wifi.netmask = ipaddr_addr("255.255.255.0");
    wifi.gw = ipaddr_addr("192.168.2.100");

    sint8 err = nm_wifi_start(&wifi, "TestHive", "xqfiricw2g", fatal_cb);
    if (err != ESP_OK) {
        os_printf("USR: nm_wifi_start error %d!\n", err);
        return;
    }

    // The client connection can be scheduled right away.
    nm_tcp *conn = os_zalloc(sizeof(nm_tcp));
    if (conn == NULL) {
        os_printf("USR: OOM!\n");
        return;
    }

    err = nm_client(conn, "192.168.1.149", 3333, false);
    if (err != ESP_OK) {
        os_printf("USR: nm_client error %d\n", err);
        // TODO: release memory.
        nm_stop(); // TODO: releasing already freed memory!
        return;
    }

    nm_set_callbacks(conn, ready_cb, disc_cb, sent_cb, recv_cb, err_cb);

    err = nm_client_connect(conn);
    if (err != ESP_OK) {
        os_printf("USR: nm_client_connect error %d\n", err);
        // TODO: release memory.
        nm_stop(); // TODO: releasing already freed memory!
        return;
    }
}

void ICACHE_FLASH_ATTR user_init()
{
    stdout_init(BIT_RATE_74880);
    system_init_done_cb(use_dhcp);
}
