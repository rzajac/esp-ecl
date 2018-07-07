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


#ifndef ESP_NM_INTERNAL_H
#define ESP_NM_INTERNAL_H

#ifndef UNUSED
    #define UNUSED(x) ( (void)(x) )
#endif

#define ESP_NM_EV_GROUP 1

// Takes esp_dll_node* and returns its payload as esp_nm_conn*.
#define get_conn(node) ((node) == NULL ? NULL : ((esp_nm_conn *) (node)->payload))

// Takes struct espconn* and finds list node which has it as a payload.
// Returns NULL or esp_nm_conn*.
#define find_conn(esp) ((esp) == NULL ? NULL : get_conn(esp_dll_find(head, (esp))))

#endif //ESP_NM_INTERNAL_H