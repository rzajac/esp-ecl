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

#include "list.h"
#include "utils_internal.h"


lst_node *ICACHE_FLASH_ATTR
lst_new(void *payload)
{
    lst_node *n = os_zalloc(sizeof(lst_node));
    if (n == NULL)
        return NULL;
    n->payload = payload;
    UTILS_DEBUG("created node [%p]", n);
    return n;
}

lst_node *ICACHE_FLASH_ATTR
lst_append(lst_node *n1, lst_node *n2)
{
    n2->next = n1->next;
    n2->prev = n1;

    if (n1->next != NULL)
        n1->next->prev = n2;
    n1->next = n2;
    UTILS_DEBUG("added [%p] after [%p]", n1, n2);

    return n2;
}

lst_node *ICACHE_FLASH_ATTR
lst_prepend(lst_node *n1, lst_node *n2)
{
    n2->prev = n1->prev;
    n2->next = n1;

    if (n1->prev != NULL)
        n1->prev->next = n2;
    n1->prev = n2;
    UTILS_DEBUG("added [%p] before [%p]", n2, n1);

    return n2;
}

lst_node *ICACHE_FLASH_ATTR
lst_remove(lst_node *n)
{
    UTILS_DEBUG("removing [%p]", n);

    if (n->prev != NULL) {
        n->prev->next = n->next;
        n->prev = NULL;
    }

    lst_node *next = NULL;
    if (n->next != NULL) {
        next = n->next;
        n->next->prev = n->prev;
        n->next = NULL;
    }
    os_free(n);

    return next;
}

lst_node *ICACHE_FLASH_ATTR
lst_find(lst_node *start, void *payload)
{
    lst_node *curr = start;
    while (curr != NULL) {
        if (curr->payload == payload)
            return curr;
        curr = curr->next;
    }
    return NULL;
}
