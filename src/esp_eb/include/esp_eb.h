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


#ifndef ESP_EB_H
#define ESP_EB_H

#include <c_types.h>
#include <osapi.h>

#include "esp_tim.h"
#include "esp_util.h"
#include "esp_list.h"

#ifndef ESP_EB_DEBUG_ON
    #ifdef DEBUG_ON
        #define ESP_EB_DEBUG_ON
    #endif
#endif

// The number of milliseconds to use when arming the event callback timer.
#define ESP_EB_TIMER_MS 10

// The event callback prototype.
typedef void (*esp_eb_cb)(uint16_t ev_code, void *arg);

// Event bus errors.
typedef enum {
    ESP_EB_OK,              // OK.
    ESP_EB_E_MEM,           // Out of memory.
    ESP_EB_E_EXISTED_GROUP, // Attach failed due to group conflict.
} esp_eb_err;

/**
 * Subscribe to event.
 *
 * @param ev_code The event code (0-1023 reserved for internal use).
 * @param cb      The event callback.
 * @param group   The group, 0 - no group (1-128 reserved).
 *
 * @return Error code.
 */
esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach(uint16_t code, esp_eb_cb cb, uint8_t group);

/**
 * Subscribe to event and throttle callbacks.
 *
 * @param ev_code     The event code (0-1023 reserved for internal use).
 * @param cb          The event callback.
 * @param throttle_us Wait at least microseconds between callback executions.
 *                    Turn off throttling by passing 0.
 * @param group       The group, 0 - no group (1-128 reserved).
 *
 * @return Error code.
 */
esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach_throttled(uint16_t ev_code,
                        esp_eb_cb cb,
                        uint32_t throttle_us,
                        uint8_t group);

/**
 * Stop event subscription.
 *
 * @param ev_code The event code (0-1023 reserved for internal use).
 * @param cb      The event callback.
 *
 * @return true - success, false - failure
 */
bool ICACHE_FLASH_ATTR
esp_eb_detach(uint16_t ev_code, esp_eb_cb cb);

/**
 * Remove all event subscriptions with given callback.
 *
 * @param cb The event callback.
 *
 * @return true - success, false - failure
 */
bool ICACHE_FLASH_ATTR
esp_eb_remove_cb(esp_eb_cb cb);

/**
 * Romove all event subscriptions with given group.
 *
 * @param group The group, 0 - no group (1-128 reserved).
 *
 * @return true - success, false - failure
 */
bool ICACHE_FLASH_ATTR
esp_eb_remove_group(uint8_t group);

/**
 * Trigger event and notify all subscribers.
 *
 * @param ev_code The event code to trigger.
 * @param arg     The optional argument to pass to all subscribers.
 */
void ICACHE_FLASH_ATTR
esp_eb_trigger(uint16_t ev_code, void *arg);

/**
 * Trigger event and notify all subscribers after delay.
 *
 * @param ev_code The event code to trigger (0-1023 reserved for internal use).
 * @param delay   The delay in milliseconds.
 * @param arg     The optional argument to pass to all subscribers.
 */
void ICACHE_FLASH_ATTR
esp_eb_trigger_delayed(uint16_t ev_code, uint32_t delay, void *arg);

/**
 * Make esp_eb to trigger events on WiFi events.
 *
 * After calling this function you will be able to
 * attach callbacks to WiFi events defined in user_interface.h
 */
void ICACHE_FLASH_ATTR
esp_eb_handle_wifi_events();

/**
 * Attach all station mode events to given callback.
 *
 * The esp_eb_handle_wifi_events must be called beforehand.
 *
 * The callback should be dedicated function for WiFi event
 * handling because if one of the attach calls returns OOM
 * the function before exiting will remove all events from
 * given callback function.
 *
 * @param cb    The event callback.
 * @param group The group, 0 - no group (1-128 reserved).
 *
 * @return Error code.
 */
esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach_wifi_events(esp_eb_cb cb, uint8_t group);

/**
 * Print elements in the event list.
 *
 * For debugging purposes.
 */
void ICACHE_FLASH_ATTR
esp_eb_print_list();

#endif //ESP_EB_H
