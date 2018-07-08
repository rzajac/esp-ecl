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


#ifndef NM_INTERNAL_H
#define NM_INTERNAL_H

#include <esp.h>

#include "nm_wifi.h"
#include "nm_tcp.h"
#include "esp_eb.h"

#if defined(NM_DEBUG_ON) || defined(DEBUG_ON)
    #define NM_DEBUG(format, ...) os_printf("NM DBG: " format "\n", ## __VA_ARGS__ )
#else
    #define NM_DEBUG(format, ...) do {} while(0)
#endif

#define NM_ERROR(format, ...) os_printf("NM ERR: " format "\n", ## __VA_ARGS__ )

#define EV_GROUP 1

// Takes esp_dll_node* and returns its payload as nm_tcp*.
#define get_conn(node) ((node) == NULL ? NULL : ((nm_tcp *) (node)->payload))

// Takes struct espconn* and finds list node which has it as a payload.
// Returns NULL or nm_tcp*.
#define find_conn(esp) ((esp) == NULL ? NULL : get_conn(esp_dll_find(head, (esp))))

#endif //NM_INTERNAL_H
