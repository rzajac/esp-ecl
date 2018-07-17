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
#include "stdout.h"
#include "event_bus.h"


#define EVENT_BATTERY_LOW 2000
#define EVENT_BATTERY_FULL 2001
#define EVENT_THROTTLED 2002

#define EVENT_GROUP 200

uint32_t arg1, arg2;


void ICACHE_FLASH_ATTR
my_event1_cb(uint16_t ev_code, void *arg)
{
    os_printf("event1 %d handled with arg: %d\n", ev_code, *(uint32_t *) arg);
}

void ICACHE_FLASH_ATTR
my_event2_cb(uint16_t ev_code, void *arg)
{
    os_printf("event2 %d handled with arg: %d\n", ev_code, *(uint32_t *) arg);
}

void ICACHE_FLASH_ATTR
my_event_throttled_cb(uint16_t ev_code, void *arg)
{
    os_printf("event throttled %d handled with arg: %d\n", ev_code,
              *(uint32_t *) arg);
}

void ICACHE_FLASH_ATTR
start(void)
{
    arg1 = 123;
    arg2 = 321;

    os_printf("system initialized\n");
    os_printf("callback pointers:\n event1: %p\n event2: %p\n", my_event1_cb,
              my_event2_cb);

    // Add two subscribers to batteryLow event.
    eb_attach(EVENT_BATTERY_LOW, my_event1_cb, EVENT_GROUP);
    eb_attach(EVENT_BATTERY_LOW, my_event2_cb, EVENT_GROUP);

    // Adding already existing subscriber will not do anything.
    eb_attach(EVENT_BATTERY_LOW, my_event2_cb, EVENT_GROUP);

    // List should have two entries.
    os_printf("\n");
    eb_print_list();

    // Stop subscribing to batteryLow event.
    eb_detach(EVENT_BATTERY_LOW, my_event1_cb);

    // Un-subscribing not existing is OK.
    eb_detach(EVENT_BATTERY_FULL, my_event1_cb);

    // Add batteryFull subscriber.
    eb_attach(EVENT_BATTERY_FULL, my_event2_cb, EVENT_GROUP);

    // List should have one batteryLow and batteryFull subscriber.
    os_printf("\n");
    eb_print_list();

    // Trigger events.
    eb_trigger(EVENT_BATTERY_LOW, &arg1);
    eb_trigger(EVENT_BATTERY_LOW, &arg1);
    eb_trigger(EVENT_BATTERY_FULL, &arg2);
    eb_trigger_delayed(EVENT_BATTERY_FULL, 5000, &arg2);

    os_printf("\n");
    eb_attach_throttled(EVENT_THROTTLED,
                        my_event_throttled_cb,
                        40000,
                        EVENT_GROUP);

    eb_trigger(EVENT_THROTTLED, &arg1);
    eb_trigger(EVENT_THROTTLED, &arg1);
    eb_trigger(EVENT_THROTTLED, &arg1);
    eb_trigger(EVENT_THROTTLED, &arg1);
    os_delay_us(40000);
    eb_trigger(EVENT_THROTTLED, &arg1);

    eb_print_list();
}

void ICACHE_FLASH_ATTR
user_init()
{
    // No need for wifi for this example.
    wifi_station_disconnect();
    wifi_set_opmode_current(NULL_MODE);

    stdout_init(BIT_RATE_74880);
    system_init_done_cb(start);
}