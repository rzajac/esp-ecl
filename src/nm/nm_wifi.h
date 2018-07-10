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

#include "internal.h"
#include "nm_tcp.h"

extern nm_err_cb nm_g_fatal_err;

/**
 * Handles all WiFi events.
 *
 * @param ev_code The event code (same as wifi event codes).
 * @param arg     The System_Event_t instance.
 */
void ICACHE_FLASH_ATTR
nm_wifi_event_cb(uint16_t ev_code, void *arg);

#endif //NM_WIFI_H
