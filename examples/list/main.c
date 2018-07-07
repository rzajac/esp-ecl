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


#include <user_interface.h>
#include <osapi.h>
#include <mem.h>
#include <esp_list.h>
#include "esp_sdo.h"

esp_dll_node *head;

void ICACHE_FLASH_ATTR
sys_init_done(void)
{
    esp_dll_node *tmp;
    esp_dll_node *next;

    os_printf("heap size %d\n", system_get_free_heap_size());
    os_printf("node size %d\n", sizeof(esp_dll_node));

    next = head = esp_dll_new((void *) 1);

    // Create 100 nodes.
    for (int i = 2; i <= 20; i++) {
        next = esp_dll_append(next, esp_dll_new((void *) i));
    }

    // Display 100 nodes.
    next = head;
    while (next != NULL) {
        os_printf("node %p %d\n", next, (int) next->payload);
        next = next->next;
    }

    // Find specific node by payload.
    tmp = esp_dll_find(head, (void *) 13);
    os_printf("found node #13 %p %d\n", tmp, (int) tmp->payload);

    // Remove node #13
    esp_dll_remove(tmp);
    os_free(tmp);

    // Display 19 nodes.
    next = head;
    esp_dll_node *tail = head;
    while (next != NULL) {
        tail = next;
        os_printf("node %p %d\n", next, (int) next->payload);
        next = next->next;
    }

    // Display 19 nodes backwards.
    next = tail;
    while (next != NULL) {
        os_printf("node %p %d\n", next, (int) next->payload);
        next = next->prev;
    }

    // Release memory for nodes.
    next = head;
    while (next != NULL) {
        tmp = next->next;
        os_free(next);
        next = tmp;
    }

    os_printf("heap size %d\n", system_get_free_heap_size());
}

void ICACHE_FLASH_ATTR
user_init()
{
    // No need for wifi for this example.
    wifi_station_disconnect();
    wifi_set_opmode_current(NULL_MODE);

    stdout_init(BIT_RATE_74880);
    system_init_done_cb(sys_init_done);
}
