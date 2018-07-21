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

#ifndef NM_WIFI_H
#define NM_WIFI_H

#include <user_interface.h>

#include "nm.h"


// Represents WiFi connection configuration.
typedef struct {
    // Maximum WiFi connection retries (0 - no limit).
    uint8_t recon_max;

    // Current WiFi connection retries.
    uint8_t recon_cnt;

    // Static IP, network mask and gateway.
    uint32_t ip, netmask, gw;
} nm_wifi;


/**
 * Connect to WiFi.
 *
 * Configures and schedules WiFi connection.
 *
 * @param wifi     The WiFi configuration.
 * @param name     The access point name.
 * @param pass     The access point password.
 * @param fatal_cb The fatal error callback.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_wifi_start(nm_wifi *wifi, char *name, char *pass, nm_err_cb fatal_cb);
//TODO: change name to connect.

//TODO: change name to AP
//TODO: we will use wifi when creating access points.

/**
 * Stop network manager.
 *
 * Stopping network manager:
 *
 * - aborts all managed connections which are connected,
 * - disconnects from WiFi,
 * - turns off WiFi and sets opmode to NULL_MODE,
 * - releases all the allocated memory.
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
nm_wifi_stop();
// TODO: change name to disconnect.

#endif // NM_WIFI_H
