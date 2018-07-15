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
    #define NM_DEBUG(format, ...) os_printf("NM  DBG: " format "\n", ## __VA_ARGS__ )
    #define NM_DEBUG_CONN(header, conn) ({ os_printf("NM  TCP: %s [%p]\n", (header), (conn)); \
                                           os_printf("         state     %d\n", (conn)->esp->state); \
                                           os_printf("         link_cnt  %d\n", (conn)->esp->link_cnt); \
                                           os_printf("         reverse   %p\n", (conn)->esp->reverse); \
                                        })
#else
    #define NM_DEBUG(format, ...) do {} while(0)
    #define NM_DEBUG_CONN(format, conn) do {} while(0)
#endif

#define NM_ERROR(format, ...) os_printf("NM  ERR: " format "\n", ## __VA_ARGS__ )

#define EV_GROUP 1

// Takes esp_dll_node* and returns its payload as nm_tcp*.
#define get_conn(node) ((node) == NULL ? NULL : ((nm_tcp *) (node)->payload))

// Some helper macros for getting espconn statuses.
#define is_conn_ready(conn) (((conn) == NULL || (conn)->esp == NULL) ? false : (conn)->esp->state == ESPCONN_NONE)
#define is_conn_busy(conn) (((conn) == NULL || (conn)->esp == NULL) ? true : (conn)->esp->state == ESPCONN_WAIT || (conn)->esp->state == ESPCONN_WRITE || (conn)->esp->state == ESPCONN_READ)
#define is_conn_closed(conn) (((conn) == NULL || (conn)->esp == NULL) ? true : (conn)->esp->state == ESPCONN_CLOSE)
#define is_conn_connected(conn) (((conn) == NULL || (conn)->esp == NULL) ? true : (conn)->esp->state == ESPCONN_CONNECT)

#endif //NM_INTERNAL_H
