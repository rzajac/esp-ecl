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

    // Set to true if wifi was connected at some point.
    uint8_t status;
} wifi;

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
 * @param err_cb
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
wifi_start(wifi *wifi, char *name, char *pass, nm_err_cb err_cb);

/**
 * Stop network manager.
 *
 * Stopping network manager:
 *
 * - aborts all connected managed network connections,
 * - disconnects from WiFi
 * - turns off WiFi and sets opmode to NULL_MODE.
 * - releases all the allocated memory,
 *
 * @return Error code.
 */
sint8 ICACHE_FLASH_ATTR
wifi_stop();

#endif // NM_WIFI_H
