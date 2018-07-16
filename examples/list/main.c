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

#include "lists.h"
#include "stdout.h"

lst_node *head;

void ICACHE_FLASH_ATTR
use_dhcp(void)
{
    lst_node *tmp;
    lst_node *next;

    os_printf("heap size %d\n", system_get_free_heap_size());
    os_printf("node size %d\n", sizeof(lst_node));

    next = head = lst_new((void *) 1);

    // Create 19 nodes.
    for (int i = 2; i <= 20; i++) {
        next = lst_append(next, lst_new((void *) i));
    }

    // Display 19 nodes.
    next = head;
    while (next != NULL) {
        os_printf("node %p %d\n", next, (int) next->payload);
        next = next->next;
    }

    // Find specific node by payload.
    tmp = lst_find(head, (void *) 13);
    os_printf("found node #13 %p %d\n", tmp, (int) tmp->payload);

    // Remove node #13
    lst_remove(tmp);

    // Display 18 nodes.
    next = head;
    lst_node *tail = head;
    while (next != NULL) {
        tail = next;
        os_printf("node %p %d\n", next, (int) next->payload);
        next = next->next;
    }

    // Display 18 nodes backwards.
    next = tail;
    while (next != NULL) {
        os_printf("node %p %d\n", next, (int) next->payload);
        next = next->prev;
    }

    // Release memory for nodes.
    next = head;
    while (next != NULL) {
        tmp = next->next;
        lst_remove(next);
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
    system_init_done_cb(use_dhcp);
}
