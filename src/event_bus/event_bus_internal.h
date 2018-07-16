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


#ifndef EB_INTERNAL_H
#define EB_INTERNAL_H

#include "event_bus.h"

#if defined(EB_DEBUG_ON) || defined(DEBUG_ON)
    #define EB_DEBUG(format, ...) os_printf("EB  DBG: " format, ## __VA_ARGS__ )
#else
    #define EB_DEBUG(format, ...) do {} while(0)
#endif

#define EB_ERROR(format, ...) os_printf("EB  ERR: " format, ## __VA_ARGS__ )

// The number of milliseconds to use when arming the event callback timer.
#define EB_TIMER_MS 10

// Macro returning eb_event pointer given node payload. 
#define get_event(node) ((eb_event *) (node)->payload)

#endif // EB_INTERNAL_H
