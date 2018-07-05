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

#ifndef ESP_EB_DEBUG_ON
  #ifdef DEBUG_ON
    #define ESP_EB_DEBUG_ON
  #endif
#endif

// The number of milliseconds to use when arming the event callback timer.
#define ESP_EB_TIMER_MS 10

// The event callback prototype.
typedef void (esp_eb_cb)(uint16_t ev_code, void *arg);

// Event bus errors.
typedef enum {
  ESP_EB_ATTACH_OK,
  ESP_EB_ATTACH_EXISTED,
  ESP_EB_ATTACH_MEM // Out of memory.
} esp_eb_err;

/**
 * Subscribe to event.
 *
 * @param group   The group, 0 - no group (1-128 reserved).
 * @param ev_code The event code (0-1023 reserved for internal use).
 * @param cb      The event callback.
 *
 * @return The result of adding new subscriber.
 */
esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach(uint8_t group, uint16_t code, esp_eb_cb *cb);

/**
 * Subscribe to event and throttle callbacks.
 *
 * @param group       The group, 0 - no group (1-128 reserved).
 * @param ev_code     The event code (0-1023 reserved for internal use).
 * @param cb          The event callback.
 * @param throttle_us Wait at least microseconds between callback executions.
 *                    Turn off throttling by passing 0.
 *
 * @return The result of adding new subscriber.
 */
esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach_throttled(uint8_t group,
                        uint16_t ev_code,
                        esp_eb_cb *cb,
                        uint32_t throttle_us);

/**
 * Stop event subscription.
 *
 * @param ev_code The event code (0-1023 reserved for internal use).
 * @param cb      The event callback.
 *
 * @return true - success, false - failure
 */
bool ICACHE_FLASH_ATTR
esp_eb_detach(uint16_t ev_code, esp_eb_cb *cb);

/**
 * Remove all event subscriptions with given callback.
 *
 * @param cb The event callback.
 *
 * @return true - success, false - failure
 */
bool ICACHE_FLASH_ATTR
esp_eb_remove_cb(esp_eb_cb *cb);

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
 * Print elements in the event list.
 *
 * For debugging purposes.
 */
void ICACHE_FLASH_ATTR
esp_eb_print_list();

#endif //ESP_EB_H
