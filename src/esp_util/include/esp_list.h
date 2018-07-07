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

#ifndef ESP_LIST_H
#define ESP_LIST_H

#include "esp_util.h"


// Double linked list.
typedef struct esp_dll_node_ {
    void *payload;              // The list payload.
    struct esp_dll_node_ *next; // The pointer to the next list node.
    struct esp_dll_node_ *prev; // The pointer to the previous list node.
} esp_dll_node;


/**
 * Return new double linked list.
 *
 * @param payload The payload to attach to the list node.
 *
 * @return Returns NULL on out of memory.
 */
esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_new(void *payload);

/**
 * Add node n2 after node n1.
 *
 * @param n1 The double link list node.
 * @param n2 The double link list node.
 *
 * @return n2
 */
esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_append(esp_dll_node *n1, esp_dll_node *n2);

/**
 * Add node n2 before node n1.
 *
 * @param n1 The double link list node.
 * @param n2 The double link list node.
 *
 * @return n2
 */
esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_prepend(esp_dll_node *n1, esp_dll_node *n2);

/**
 * Remove node from double linked list.
 *
 * @param n The double link list node.
 *
 * @return n
 */
esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_remove(esp_dll_node *n);

/**
 * Find payload on the list.
 *
 * @param start   The starting node.
 * @param payload The payload to find.
 *
 * @return Returns pointer to node with payload or NULL.
 */
esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_find(esp_dll_node *start, void *payload);

#endif //ESP_LIST_H
