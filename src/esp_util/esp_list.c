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

#include "include/esp_list.h"


esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_new(void *payload)
{
    esp_dll_node *n = os_zalloc(sizeof(esp_dll_node));
    if (n == NULL)
        return NULL;
    n->payload = payload;
    return n;
}

esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_append(esp_dll_node *n1, esp_dll_node *n2)
{
    n2->next = n1->next;
    n2->prev = n1;

    if (n1->next != NULL)
        n1->next->prev = n2;
    n1->next = n2;

    return n2;
}

esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_prepend(esp_dll_node *n1, esp_dll_node *n2)
{
    n2->prev = n1->prev;
    n2->next = n1;

    if (n1->prev != NULL)
        n1->prev->next = n2;
    n1->prev = n2;

    return n2;
}

esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_remove(esp_dll_node *n)
{
    if (n->prev != NULL)
        n->prev->next = n->next;
    if (n->next != NULL)
        n->next->prev = n->prev;

    return n;
}

esp_dll_node *ICACHE_FLASH_ATTR
esp_dll_find(esp_dll_node *start, void *payload)
{
    while (start != NULL) {
        if (start->payload == payload)
            return start;
        start = start->next;
    }
    return NULL;
}
