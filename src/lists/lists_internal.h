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

#ifndef UTIL_INTERNAL_H
#define UTIL_INTERNAL_H

#include <osapi.h>

#if defined(LISTS_DEBUG_ON) || defined(DEBUG_ON)
    #define LISTS_DEBUG(format, ...) os_printf("LST DBG: " format "\n", ## __VA_ARGS__ )
#else
    #define LISTS_DEBUG(format, ...) do {} while(0)
#endif

// The register to read in a loop.
#define DELAY_REG_READ 0x60000318
// How many loops take 1us.
#define DELAY_MULTIPLIER 3

#endif // UTIL_INTERNAL_H
