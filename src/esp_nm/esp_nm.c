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


#include <espconn.h>
#include "include/esp_nm.h"
#include "esp_nm_internal.h"
#include "esp_nm_list.h"

/////////////////////////////////////////////////////////////////////////////
// Declarations.
/////////////////////////////////////////////////////////////////////////////

static void ICACHE_FLASH_ATTR wifi_event_cb(const char *event, void *arg);

static void ICACHE_FLASH_ATTR wifi_events_detach();

static void ICACHE_FLASH_ATTR release_espconn(esp_nm_conn *conn);

static void ICACHE_FLASH_ATTR connect_cb(void *arg);

static void ICACHE_FLASH_ATTR error_cb(void *arg, sint8 errCode);

static void ICACHE_FLASH_ATTR disc_cb(void *arg);

static void ICACHE_FLASH_ATTR sent_cb(void *arg);

/////////////////////////////////////////////////////////////////////////////
// Code.
/////////////////////////////////////////////////////////////////////////////

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_init()
{
  if (esp_nm_list_init() != ESP_NM_OK) return ESP_NM_INITIALIZED;

  // Receive WiFi events.
  esp_eb_handle_wifi_events();

  esp_eb_err err;

  err = esp_eb_attach(ESP_EB_EVENT_STAMODE_CONNECTED, wifi_event_cb);
  if (err != ESP_EB_ATTACH_OK) return ESP_NM_ERR_MEM;

  err = esp_eb_attach(ESP_EB_EVENT_STAMODE_DISCONNECTED, wifi_event_cb);
  if (err != ESP_EB_ATTACH_OK) {
    wifi_events_detach();
    return ESP_NM_ERR_MEM;
  }

  err = esp_eb_attach(ESP_EB_EVENT_STAMODE_GOT_IP, wifi_event_cb);
  if (err != ESP_EB_ATTACH_OK) {
    wifi_events_detach();
    return ESP_NM_ERR_MEM;
  }

  err = esp_eb_attach(ESP_EB_EVENT_STAMODE_DHCP_TIMEOUT, wifi_event_cb);
  if (err != ESP_EB_ATTACH_OK) {
    wifi_events_detach();
    return ESP_NM_ERR_MEM;
  }

  err = esp_eb_attach(ESP_EB_EVENT_OPMODE_CHANGED, wifi_event_cb);
  if (err != ESP_EB_ATTACH_OK) {
    wifi_events_detach();
    return ESP_NM_ERR_MEM;
  }

  return ESP_NM_OK;
}

esp_nm_err ICACHE_FLASH_ATTR
esp_nm_new(esp_nm_conn *conn, char *host, int port, bool ssl)
{
  conn->esp = os_zalloc(sizeof(struct espconn));
  if (conn->esp == NULL) return ESP_NM_ERR_MEM;

  conn->esp->proto.tcp = os_zalloc(sizeof(esp_tcp));
  if (conn->esp->proto.tcp == NULL) {
    release_espconn(conn);
    return ESP_NM_ERR_MEM;
  }

  conn->ssl = ssl;

  // Configure TCP/IP connection.
  conn->esp->type = ESPCONN_TCP;
  uint32_t ip = ipaddr_addr(host);
  os_memcpy(conn->esp->proto.tcp->remote_ip, &ip, 4);
  conn->esp->proto.tcp->local_port = espconn_port();
  conn->esp->proto.tcp->remote_port = port;

  // Register callbacks for successful connection or error.
  espconn_regist_connectcb(conn->esp, connect_cb);
  espconn_regist_reconcb(conn->esp, error_cb);

  return ESP_NM_OK;
}

void ICACHE_FLASH_ATTR
esp_nm_set_keepalive(esp_nm_conn *conn, int idle, int intvl, int cnt)
{
  conn->ka_idle = idle;
  conn->ka_intvl = intvl;
  conn->ka_cnt = cnt;
}

void ICACHE_FLASH_ATTR
esp_nm_set_callbacks(esp_nm_conn *conn,
                     esp_nm_err_cb *err_cb,
                     espconn_recv_callback *recv_cb)
{
  conn->err_cb = err_cb;
  conn->recv_cb = recv_cb;
}

static void ICACHE_FLASH_ATTR
wifi_event_cb(const char *event, void *arg)
{
  UNUSED(event);

  System_Event_t *ev = arg;

  switch (ev->event) {
    case EVENT_STAMODE_CONNECTED:
      break;

    case EVENT_STAMODE_DISCONNECTED:
      break;

    case EVENT_STAMODE_GOT_IP:
      break;

    case EVENT_STAMODE_DHCP_TIMEOUT:
      break;

    case EVENT_OPMODE_CHANGED:
      break;

    default:
      ESP_NM_ERROR("unexpected wifi event: %d\n", ev->event);
  }
}

static void ICACHE_FLASH_ATTR
wifi_events_detach()
{
  esp_eb_detach(ESP_EB_EVENT_STAMODE_CONNECTED, wifi_event_cb);
  esp_eb_detach(ESP_EB_EVENT_STAMODE_DISCONNECTED, wifi_event_cb);
  esp_eb_detach(ESP_EB_EVENT_STAMODE_GOT_IP, wifi_event_cb);
  esp_eb_detach(ESP_EB_EVENT_STAMODE_DHCP_TIMEOUT, wifi_event_cb);
  esp_eb_detach(ESP_EB_EVENT_OPMODE_CHANGED, wifi_event_cb);
}

/**
 * Release espconn memory.
 *
 * @param conn The managed connection.
 *
 * @return void
 */
static void ICACHE_FLASH_ATTR
release_espconn(esp_nm_conn *conn)
{
  if (conn->esp == NULL) return;
  if (conn->esp->proto.tcp != NULL) os_free(conn->esp->proto.tcp);

  ESP_NM_DEBUG("release_espconn\n");
  os_free(conn->esp);
  conn->esp = NULL;
}

static void ICACHE_FLASH_ATTR
connect_cb(void *arg)
{
  struct espconn *esp = (struct espconn *) arg;
  esp_nm_conn *conn = esp_nm_list_find(esp);

  if (conn->ka_idle && conn->ka_intvl && conn->ka_cnt) {
    espconn_set_opt(conn, ESPCONN_KEEPALIVE);
    espconn_set_keepalive(conn, ESPCONN_KEEPIDLE, &conn->ka_idle);
    espconn_set_keepalive(conn, ESPCONN_KEEPINTVL, &conn->ka_intvl);
    espconn_set_keepalive(conn, ESPCONN_KEEPCNT, &conn->ka_cnt);
  }

  espconn_regist_disconcb(conn, disc_cb);
  espconn_regist_recvcb(conn, conn->recv_cb);
  espconn_regist_sentcb(conn, sent_cb);
}

static void ICACHE_FLASH_ATTR
error_cb(void *arg, sint8 errCode)
{
  struct espconn *esp = (struct espconn *) arg;
  esp_nm_conn *conn = esp_nm_list_find(esp);
}

static void ICACHE_FLASH_ATTR
disc_cb(void *arg)
{
  struct espconn *esp = (struct espconn *) arg;
  esp_nm_conn *conn = esp_nm_list_find(esp);
}

static void ICACHE_FLASH_ATTR
sent_cb(void *arg)
{
  struct espconn *esp = (struct espconn *) arg;
  esp_nm_conn *conn = esp_nm_list_find(esp);
}
