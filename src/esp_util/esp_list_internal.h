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

#ifndef ESP_LIST_INTERNAL_H
#define ESP_LIST_INTERNAL_H

#include <osapi.h>

#if defined(DLL_DEBUG_ON) || defined(DEBUG_ON)
    #define DLL_DEBUG(format, ...) os_printf("DLL DBG: " format "\n", ## __VA_ARGS__ )
#else
    #define DLL_DEBUG(format, ...) do {} while(0)
#endif

#endif //ESP_LIST_INTERNAL_H
