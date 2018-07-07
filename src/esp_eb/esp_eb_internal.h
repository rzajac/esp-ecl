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


#ifndef ESP_EB_INTERNAL_H
#define ESP_EB_INTERNAL_H

#include "esp_eb.h"

#ifdef ESP_EB_DEBUG_ON
  #define ESP_EB_DEBUG(format, ...) os_printf("EB DBG: " format, ## __VA_ARGS__ )
#else
  #define ESP_EB_DEBUG(format, ...) do {} while(0)
#endif

#define ESP_EB_ERROR(format, ...) os_printf("EB ERR: " format, ## __VA_ARGS__ )

#define GET_EVENT(node) ((eb_event *) (node)->payload)

#endif //ESP_EB_INTERNAL_H
