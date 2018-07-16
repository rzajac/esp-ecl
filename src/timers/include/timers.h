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


#ifndef TIMERS_H
#define TIMERS_H

#include <c_types.h>
#include <os_type.h>


// The structure wrapping timer data.
typedef struct {
    os_timer_func_t *os_timer_cb; // System timer callback.
    os_timer_t *_os_timer;        // System timer. Don't touch it.
    uint32_t delay;               // The timer delay in milliseconds.
    void *payload;                // The payload.
} tim_timer;


/**
 * Start timer.
 *
 * @param cb      The callback.
 * @param payload The timer payload.
 *
 * @return Returns timer structure or NULL when out of memory.
 */
tim_timer *ICACHE_FLASH_ATTR
tim_start(os_timer_func_t *cb, void *payload);

/**
 * Disarm timer.
 *
 * @param timer
 */
void ICACHE_FLASH_ATTR
tim_disarm(tim_timer *timer);

/**
 * Start timer with specific delay.
 *
 * @param cb      The callback.
 * @param payload The timer payload.
 * @param delay   The delay in milliseconds.
 *
 * @return Returns timer structure or NULL when out of memory.
 */
tim_timer *ICACHE_FLASH_ATTR
tim_start_delay(os_timer_func_t *cb, void *payload, uint32_t delay);

/**
 * Stop timer.
 *
 * @param timer
 */
void ICACHE_FLASH_ATTR
tim_stop(tim_timer *timer);

/**
 * Run timer again.
 *
 * @param timer
 */
void ICACHE_FLASH_ATTR
tim_continue(tim_timer *timer);

#endif // TIMERS_H
