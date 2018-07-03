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

#ifndef ESP_NM_H
#define ESP_NM_H

#include <user_interface.h>
#include <espconn.h>
#include <mem.h>

#include "esp_eb.h"


#ifdef ESP_NM_DEBUG_ON
  #define ESP_NM_DEBUG(format, ...) os_printf("NM DBG: " format, ## __VA_ARGS__ )
#else
  #define ESP_NM_DEBUG(format, ...) do {} while(0)
#endif

#define ESP_NM_ERROR(format, ...) os_printf("NM ERR: " format, ## __VA_ARGS__ )

// Network manager errors.
typedef enum {
  ESP_NM_OK,            // OK.
  ESP_NM_ERR_MEM,       // Out of memory.
  ESP_NM_INITIALIZED,   // Network manager already initialized.
  ESP_NM_ERR_KEEPALIVE, // Error setting keep alive on the connection.
} esp_nm_err;

struct esp_nm_conn_;

// The fatal error callback prototype.
typedef void (esp_nm_err_cb)(struct esp_nm_conn_ *, esp_nm_err);

// Represents managed connection.
typedef struct esp_nm_conn_ {
  struct espconn *esp;   // The connection.
  bool ssl;              // Use SSL for the connection.
  int ka_idle;           // Keep alive idle.
  int ka_intvl;          // Keep alive interval.
  int ka_cnt;            // Keep alive count.
  esp_nm_err_cb *err_cb; // Fatal error callback.
  espconn_recv_callback *recv_cb; // Receive callback.
} esp_nm_conn;

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_new(esp_nm_conn *conn, char *host, int port, bool ssl);

void ICACHE_FLASH_ATTR
esp_nm_set_keepalive(esp_nm_conn *conn, int idle, int intvl, int cnt);

void ICACHE_FLASH_ATTR
esp_nm_set_callbacks(esp_nm_conn *conn,
                     esp_nm_err_cb *err_cb,
                     espconn_recv_callback *recv_cb);

#endif //ESP_NM_H
