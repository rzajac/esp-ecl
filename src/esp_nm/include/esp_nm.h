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

#include <esp_eb.h>
#include <esp_util.h>


#ifdef ESP_NM_DEBUG_ON
  #define ESP_NM_DEBUG(format, ...) os_printf("NM DBG: " format, ## __VA_ARGS__ )
#else
  #define ESP_NM_DEBUG(format, ...) do {} while(0)
#endif

#define ESP_NM_ERROR(format, ...) os_printf("NM ERR: " format, ## __VA_ARGS__ )

// Network manager errors.
typedef enum {
  ESP_NM_OK,            // OK.
  ESP_NME_MEM,          // Out of memory.
  ESP_NME_INITIALIZED,  // Network manager already initialized.
  ESP_NME_MODE,         // Error changing current opmode.
  ESP_NME_DHCP_STOP,    //
  ESP_NME_DHCP_START,   //
  ESP_NME_STATIC_IP,    //
  ESP_NME_POLICY,       //
  ESP_NME_WIFI_CFG,     //
  ESP_NME_WIFI_CONNECT, //
  ESP_NME_KEEPALIVE,    // Error setting keep alive on the connection.
} esp_nm_err;

struct esp_nmc_;

// The fatal error callback prototype.
typedef void (*esp_nm_err_cb)(struct esp_nmc_ *, esp_nm_err);

// The receive callback prototype.
typedef void (*esp_nm_recv_cb)(struct esp_nmc_ *, uint8_t *data, size_t len);

// The send callback prototype.
typedef void (*esp_nm_cb)(struct esp_nmc_ *);


// Represents managed connection.
typedef struct esp_nmc_ {
  struct espconn *esp;    // The connection.
  bool ssl;               // Use SSL for the connection.
  uint8_t recon_max;      // Maximum number of reconnection retries.
  uint8_t recon_cnt;      // Number of reconnection retries.
  int ka_idle;            // Keep alive idle.
  int ka_intvl;           // Keep alive interval.
  int ka_cnt;             // Keep alive count.
  esp_nm_cb ready_cb;     // Ready callback.
  esp_nm_cb send_cb;      // Sent callback.
  esp_nm_recv_cb recv_cb; // Receive callback.
  esp_nm_err_cb err_cb;   // Fatal error callback.
} esp_nm_conn;

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_start(char *wifi_name,
             char *wifi_pass,
             bool reconnect_policy,
             uint32_t static_ip,
             uint32_t static_netmask,
             uint32_t static_gw);

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_client(esp_nm_conn *conn, char *host, int port, bool ssl);

void ICACHE_FLASH_ATTR
esp_nm_set_keepalive(esp_nm_conn *conn, int idle, int intvl, int cnt);

void ICACHE_FLASH_ATTR
esp_nm_set_callbacks(esp_nm_conn *conn,
                     esp_nm_cb ready_cb,
                     esp_nm_cb sent_cb,
                     esp_nm_recv_cb recv_cb,
                     esp_nm_err_cb err_cb);

void ICACHE_FLASH_ATTR
esp_nm_reconnect(esp_nm_conn *conn, uint8_t recon_max);

void ICACHE_FLASH_ATTR
esp_nm_cleanup(esp_nm_conn *conn);

sint8 ICACHE_FLASH_ATTR
esp_nm_send(esp_nm_conn *conn, uint8_t *data, size_t len);

#endif //ESP_NM_H
