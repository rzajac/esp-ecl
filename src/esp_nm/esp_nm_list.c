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

#include "esp_nm_list.h"


// List of managed connections.
typedef struct _list {
  esp_nm_conn *conn;
  struct _list *next;
} list;

// Linked list head.
static list *head;


esp_nm_err ICACHE_FLASH_ATTR
esp_nm_list_init()
{
  if (head != NULL) return ESP_NME_INITIALIZED;

  // Initialize linked list.
  head = os_zalloc(sizeof(list));
  if (head == NULL) return ESP_NME_MEM;

  return ESP_NM_OK;
}

esp_nm_conn *ICACHE_FLASH_ATTR
esp_nm_list_find(struct espconn *esp)
{
  if (head == NULL) return NULL;

  list *curr = head;
  while (curr != NULL) {
    if (curr->conn->esp == esp) return curr->conn;
    curr = curr->next;
  }

  return NULL;
}