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


#include "event_bus.h"
#include "event_bus_internal.h"

// Structure defining event.
typedef struct {
    // The group, 0 - no group (1-128 reserved).
    uint8_t group;

    // Event code (0-1023 reserved for internal use).
    uint16_t ev_code;

    // Event callback function.
    eb_cb cb;

    // Last time callback was called.
    uint32_t ctime_us;

    // Throttle callback calls (0 - no throttle).
    // The minimum number of microseconds to wait between callback executions.
    uint32_t throttle_us;

    // Passed to callback.
    void *payload;
} eb_event;

// The linked list head node.
static lst_node *head;

/**
 * Create new event structure.
 *
 * @param ev_code     The event code (0-1023 reserved for internal use).
 * @param cb          The callback.
 * @param throttle_us Throttle callback calls (0 - no throttle).
 * @param group       The group, 0 - no group (1-128 reserved).
 */
static eb_event *ICACHE_FLASH_ATTR
event_new(uint16_t ev_code, eb_cb cb, uint32_t throttle_us, uint8_t group)
{
    eb_event *new = os_zalloc(sizeof(eb_event));
    if (new == NULL)
        return NULL;

    new->group = group;
    new->ev_code = ev_code;
    new->cb = cb;
    new->throttle_us = throttle_us;

    return new;
}

/**
 * Create new event list node.
 *
 * @param ev_code     The event code (0-1023 reserved for internal use).
 * @param cb          The callback.
 * @param throttle_us Throttle callback calls (0 - no throttle).
 * @param group       The group, 0 - no group (1-128 reserved).
 */
static lst_node *ICACHE_FLASH_ATTR
new_node(uint16_t ev_code, eb_cb cb, uint32_t throttle_us, uint8_t group)
{
    eb_event *event = event_new(ev_code, cb, throttle_us, group);
    if (event == NULL)
        return NULL;

    lst_node *node = lst_new(event);
    if (node == NULL) {
        os_free(event);
        return NULL;
    }

    return node;
}

/**
 * Find node on the list.
 *
 * @param ev_code The event code (0-1023 reserved for internal use).
 * @param cb      The event callback.
 *
 * @return The found node or NULL
 */
static lst_node *ICACHE_FLASH_ATTR
find_node(uint16_t ev_code, eb_cb cb)
{
    lst_node *curr = head;
    while (curr != NULL) {
        if (get_event(curr)->ev_code == ev_code && get_event(curr)->cb == cb)
            break;
        curr = curr->next;
    }
    return curr;
}

/**
 * Remove node from linked list.
 *
 * @param node The node to remove.
 *
 * @return The next node.
 */
static lst_node *ICACHE_FLASH_ATTR
remove_node(lst_node *node)
{
    lst_node *next = node->next;

    if (node == head)
        head = node->next;

    os_free(get_event(node));
    lst_remove(node);

    return next;
}

sint8 ICACHE_FLASH_ATTR
eb_attach_throttled(uint16_t ev_code,
                    eb_cb cb,
                    uint32_t throttle_us,
                    uint8_t group)
{
    // Empty list.
    if (head == NULL) {
        head = new_node(ev_code, cb, throttle_us, group);
        if (head == NULL)
            return ESP_E_MEM;

        EB_DEBUG("added (h) ev#%d cb:%p th:%d gr:%d",
                 ev_code,
                 cb,
                 throttle_us,
                 group);

        return ESP_OK;
    }

    lst_node *node = find_node(ev_code, cb);

    // If node already exists we return OK unless group is different.
    if (node != NULL) {
        if (get_event(node)->group != group)
            return ESP_E_ARG;
        return ESP_OK;
    }

    node = new_node(ev_code, cb, throttle_us, group);
    if (node == NULL)
        return ESP_E_MEM;

    lst_append(head, node);
    EB_DEBUG("added (n) ev#%d cb:%p th:%d gr:%d",
             ev_code,
             cb,
             throttle_us,
             group);

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
eb_attach(uint16_t ev_code, eb_cb cb, uint8_t group)
{
    return eb_attach_throttled(ev_code, cb, 0, group);
}

bool ICACHE_FLASH_ATTR
eb_detach(uint16_t ev_code, eb_cb cb)
{
    lst_node *node = find_node(ev_code, cb);

    // Not found.
    if (node == NULL)
        return true;

    remove_node(node);

    EB_DEBUG("detached node %d %p", ev_code, cb);
    return true;
}

bool ICACHE_FLASH_ATTR
eb_remove_cb(eb_cb cb)
{
    lst_node *curr = head;

    while (curr != NULL) {
        if (get_event(curr)->cb == cb) {
            curr = remove_node(curr);
            continue;
        }
        curr = curr->next;
    }
    return true;
}

bool ICACHE_FLASH_ATTR
eb_remove_group(uint8_t group)
{
    lst_node *curr = head;

    EB_DEBUG("removing group %d callbacks", group);
    while (curr != NULL) {
        if (get_event(curr)->group == group) {
            curr = remove_node(curr);
            continue;
        }
        curr = curr->next;
    }
    return true;
}

/**
 * Event timer callback.
 *
 * @param arg
 */
static void ICACHE_FLASH_ATTR
timer_cb(void *arg)
{
    tim_timer *timer = arg;
    eb_event *event = timer->payload;

    // Event no longer exists.
    lst_node *node = find_node(event->ev_code, event->cb);
    if (node == NULL) {
        os_free(event);
        return;
    }

    uint32_t now = system_get_time();
    uint32_t diff = now - get_event(node)->ctime_us;

    if (event->throttle_us == 0) {
        get_event(node)->ctime_us = now;
        event->cb(event->ev_code, event->payload);
    } else if (event->throttle_us > 0 && diff >= event->throttle_us) {
        get_event(node)->ctime_us = now;
        event->cb(event->ev_code, event->payload);
    }

    os_free(event);
    tim_stop(timer);
}

static tim_timer *ICACHE_FLASH_ATTR
timer_start(const lst_node *node, void *payload, uint32_t delay)
{
    // We are scheduling callback, by the time it is called
    // it is possible node will no longer exist.
    eb_event *event = event_new(get_event(node)->ev_code,
                                get_event(node)->cb,
                                get_event(node)->throttle_us,
                                get_event(node)->group);
    if (!event)
        return false;

    event->payload = payload;

    EB_DEBUG("scheduling ev#%d in %d ms", get_event(node)->ev_code, delay);

    return tim_start_delay(timer_cb, event, delay);
}

void ICACHE_FLASH_ATTR
eb_trigger_delayed(uint16_t ev_code, uint32_t delay, void *arg)
{
    lst_node *curr = head;

    while (curr) {
        // Check if we have this event code attached.
        if (get_event(curr)->ev_code == ev_code) {
            if (timer_start(curr, arg, delay) == NULL) {
                EB_ERROR("error scheduling timer for %d\n", ev_code);
            }
        }
        curr = curr->next;
    }
}

void ICACHE_FLASH_ATTR
eb_trigger(uint16_t ev_code, void *arg)
{
    eb_trigger_delayed(ev_code, EB_TIMER_MS, arg);
}

void ICACHE_FLASH_ATTR
eb_print_list()
{
    os_printf("list state:\n");
    lst_node *node = head;
    while (node != NULL) {
        os_printf("    %d %p\n", get_event(node)->ev_code, get_event(node)->cb);
        node = node->next;
    }
}

/**
 * The WiFi events handler.
 *
 * @param e The WiFi event.
 */
static void ICACHE_FLASH_ATTR
wifi_event_cb(System_Event_t *e)
{
    Event_Info_u *info = &e->event_info;

    switch (e->event) {
        case EVENT_STAMODE_CONNECTED:
            EB_DEBUG("WIFI: EVENT_STAMODE_CONNECTED");
            EB_DEBUG("    ssid    %s", info->connected.ssid);
            EB_DEBUG("    bssid   " MACSTR, MAC2STR(info->connected.bssid));
            EB_DEBUG("    channel %d", info->connected.channel);
            EB_DEBUG("    status  %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_STAMODE_CONNECTED, (void *) e);
            break;

        case EVENT_STAMODE_DISCONNECTED:
            EB_DEBUG("WIFI: EVENT_STAMODE_DISCONNECTED");
            EB_DEBUG("    ssid   %s", info->disconnected.ssid);
            EB_DEBUG("    bssid  " MACSTR, MAC2STR(info->disconnected.bssid));
            EB_DEBUG("    reason %d", info->disconnected.reason);
            EB_DEBUG("    status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_STAMODE_DISCONNECTED, (void *) e);
            break;

        case EVENT_STAMODE_AUTHMODE_CHANGE:
            // The mode is one of the AUTH_* values of
            // AUTH_MODE defined in user_interface.h
            EB_DEBUG("WIFI: EVENT_STAMODE_AUTHMODE_CHANGE %d -> %d",
                      info->auth_change.old_mode,
                      info->auth_change.new_mode);
            EB_DEBUG("      status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_STAMODE_AUTHMODE_CHANGE, (void *) e);
            break;

        case EVENT_STAMODE_GOT_IP:
            EB_DEBUG("WIFI: EVENT_STAMODE_GOT_IP");
            EB_DEBUG("    ip     %d.%d.%d.%d", IP2STR(&(info->got_ip.ip)));
            EB_DEBUG("    mask   %d.%d.%d.%d", IP2STR(&(info->got_ip.mask)));
            EB_DEBUG("    gw     %d.%d.%d.%d", IP2STR(&(info->got_ip.gw)));
            EB_DEBUG("    status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_STAMODE_GOT_IP, (void *) e);
            break;

        case EVENT_STAMODE_DHCP_TIMEOUT:
            EB_DEBUG("WIFI: EVENT_STAMODE_DHCP_TIMEOUT");
            EB_DEBUG("    status  %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_STAMODE_DHCP_TIMEOUT, (void *) e);
            break;

        case EVENT_SOFTAPMODE_STACONNECTED:
            EB_DEBUG("WIFI: EVENT_SOFTAPMODE_STACONNECTED");
            EB_DEBUG("    aid    %d", info->sta_connected.aid);
            EB_DEBUG("    mac    " MACSTR, MAC2STR(info->sta_connected.mac));
            EB_DEBUG("    status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_SOFTAPMODE_STACONNECTED, (void *) e);
            break;

        case EVENT_SOFTAPMODE_STADISCONNECTED:
            EB_DEBUG("WIFI: EVENT_SOFTAPMODE_STADISCONNECTED");
            EB_DEBUG("    aid    %d", info->sta_connected.aid);
            EB_DEBUG("    mac    " MACSTR, MAC2STR(info->sta_disconnected.mac));
            EB_DEBUG("    status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_SOFTAPMODE_STADISCONNECTED, (void *) e);
            break;

        case EVENT_OPMODE_CHANGED:
            EB_DEBUG("WIFI: EVENT_OPMODE_CHANGED %d -> %d",
                     info->opmode_changed.old_opmode,
                     info->opmode_changed.new_opmode);
            EB_DEBUG("    status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_OPMODE_CHANGED, (void *) e);
            break;

        case EVENT_SOFTAPMODE_PROBEREQRECVED:
            EB_DEBUG("WIFI: EVENT_SOFTAPMODE_PROBEREQRECVED");
            EB_DEBUG("    rssi   %d", info->ap_probereqrecved.rssi);
            EB_DEBUG("    mac    " MACSTR, MAC2STR(info->ap_probereqrecved.mac));
            EB_DEBUG("    status %d", wifi_station_get_connect_status());

            eb_trigger(EVENT_SOFTAPMODE_PROBEREQRECVED, (void *) e);
            break;

        default:
            EB_ERROR("unexpected wifi event: %d", e->event);
            EB_DEBUG("    status %d", wifi_station_get_connect_status());
            break;
    }
}

void ICACHE_FLASH_ATTR
eb_handle_wifi_events()
{
    wifi_set_event_handler_cb(wifi_event_cb);
}

sint8 ICACHE_FLASH_ATTR
eb_attach_wifi_events(eb_cb cb, uint8_t group)
{
    sint8 err;

    err = eb_attach(EVENT_STAMODE_CONNECTED, cb, group);
    if (err)
        return err;

    err = eb_attach(EVENT_STAMODE_DISCONNECTED, cb, group);
    if (err)
        goto cleanup;

    err = eb_attach(EVENT_STAMODE_AUTHMODE_CHANGE, cb, group);
    if (err)
        goto cleanup;

    err = eb_attach(EVENT_STAMODE_GOT_IP, cb, group);
    if (err)
        goto cleanup;

    err = eb_attach(EVENT_STAMODE_DHCP_TIMEOUT, cb, group);
    if (err)
        goto cleanup;

    err = eb_attach(EVENT_OPMODE_CHANGED, cb, group);
    if (err)
        goto cleanup;

    return ESP_OK;

cleanup:
    eb_remove_cb(cb);

    return err;
}
