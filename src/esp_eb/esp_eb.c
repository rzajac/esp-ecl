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


#include <mem.h>
#include <user_interface.h>

#include "include/esp_eb.h"
#include "esp_eb_internal.h"

// Structure defining event.
typedef struct {
    // The group, 0 - no group (1-128 reserved).
    uint8_t group;

    // Event code (0-1023 reserved for internal use).
    uint16_t ev_code;

    // Event callback function.
    esp_eb_cb cb;

    // Last time callback was called.
    uint32_t ctime_us;

    // Throttle callback calls (0 - no throttle).
    // The minimum number of microseconds to wait between callback executions.
    uint32_t throttle_us;

    // Passed to callback.
    void *payload;
} eb_event;

// The linked list head node.
static esp_dll_node *head;

/**
 * Create new event structure.
 *
 * @param ev_code     The event code (0-1023 reserved for internal use).
 * @param cb          The callback.
 * @param throttle_us Throttle callback calls (0 - no throttle).
 * @param group       The group, 0 - no group (1-128 reserved).
 */
static eb_event *ICACHE_FLASH_ATTR
event_new(uint16_t ev_code, esp_eb_cb cb, uint32_t throttle_us, uint8_t group)
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
static esp_dll_node *ICACHE_FLASH_ATTR
new_node(uint16_t ev_code, esp_eb_cb cb, uint32_t throttle_us, uint8_t group)
{
    eb_event *event = event_new(ev_code, cb, throttle_us, group);
    if (event == NULL)
        return NULL;

    esp_dll_node *node = esp_dll_new(event);
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
static esp_dll_node *ICACHE_FLASH_ATTR
find_node(uint16_t ev_code, esp_eb_cb cb)
{
    esp_dll_node *curr = head;
    while (curr != NULL) {
        if (GET_EVENT(curr)->ev_code == ev_code && GET_EVENT(curr)->cb == cb)
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
static esp_dll_node *ICACHE_FLASH_ATTR
remove_node(esp_dll_node *node)
{
    esp_dll_node *next = node->next;
    esp_dll_remove(node);

    if (node == head)
        head = node->next;

    os_free(GET_EVENT(node));
    os_free(node);

    return next;
}

esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach_throttled(uint16_t ev_code,
                        esp_eb_cb cb,
                        uint32_t throttle_us,
                        uint8_t group)
{
    // Empty list.
    if (head == NULL) {
        head = new_node(ev_code, cb, throttle_us, group);
        if (head == NULL)
            return ESP_EB_E_MEM;
        ESP_EB_DEBUG("added head %d %p %d %d\n", ev_code, cb, throttle_us,
                     group);

        return ESP_EB_OK;
    }

    esp_dll_node *node = find_node(ev_code, cb);

    // If node already exists we return.
    if (node != NULL) {
        if (GET_EVENT(node)->group != group)
            return ESP_EB_E_EXISTED_GROUP;
        return ESP_EB_OK;
    }

    node = new_node(ev_code, cb, throttle_us, group);
    if (node == NULL)
        return ESP_EB_E_MEM;

    esp_dll_append(head, node);
    ESP_EB_DEBUG("added %d %p %d %d\n", ev_code, cb, throttle_us, group);

    return ESP_EB_OK;
}

esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach(uint16_t ev_code, esp_eb_cb cb, uint8_t group)
{
    return esp_eb_attach_throttled(ev_code, cb, 0, group);
}

bool ICACHE_FLASH_ATTR
esp_eb_detach(uint16_t ev_code, esp_eb_cb cb)
{
    esp_dll_node *node = find_node(ev_code, cb);

    // Not found.
    if (node == NULL)
        return true;

    remove_node(node);

    ESP_EB_DEBUG("detached node %d %p\n", ev_code, cb);
    return true;
}

bool ICACHE_FLASH_ATTR
esp_eb_remove_cb(esp_eb_cb cb)
{
    esp_dll_node *curr = head;

    while (curr != NULL) {
        if (GET_EVENT(curr)->cb == cb) {
            curr = remove_node(curr);
            continue;
        }
        curr = curr->next;
    }
    return true;
}

bool ICACHE_FLASH_ATTR
esp_eb_remove_group(uint8_t group)
{
    esp_dll_node *curr = head;

    while (curr != NULL) {
        if (GET_EVENT(curr)->group == group) {
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
    esp_tim_timer *timer = arg;
    eb_event *event = timer->payload;

    // Event no longer exists.
    esp_dll_node *node = find_node(event->ev_code, event->cb);
    if (node == NULL) {
        os_free(event);
        return;
    }

    uint32_t now = system_get_time();
    uint32_t diff = now - GET_EVENT(node)->ctime_us;

    if (event->throttle_us == 0) {
        GET_EVENT(node)->ctime_us = now;
        event->cb(event->ev_code, event->payload);
    } else if (event->throttle_us > 0 && diff >= event->throttle_us) {
        GET_EVENT(node)->ctime_us = now;
        event->cb(event->ev_code, event->payload);
    }

    os_free(event);
    esp_tim_stop(timer);
}

static esp_tim_timer *ICACHE_FLASH_ATTR
timer_start(const esp_dll_node *node, void *payload, uint32_t delay)
{
    // We are scheduling callback, by the time it is called
    // it is possible node will no longer exist.
    eb_event *event = event_new(GET_EVENT(node)->ev_code,
                                GET_EVENT(node)->cb,
                                GET_EVENT(node)->throttle_us,
                                GET_EVENT(node)->group);
    if (!event)
        return false;

    event->payload = payload;

    ESP_EB_DEBUG("scheduling ev#%d in %d ms\n",
                 GET_EVENT(node)->ev_code,
                 delay);

    return esp_tim_start_delay(timer_cb, event, delay);
}

void ICACHE_FLASH_ATTR
esp_eb_trigger_delayed(uint16_t ev_code, uint32_t delay, void *arg)
{
    esp_dll_node *curr = head;

    while (curr) {
        if (GET_EVENT(curr)->ev_code == ev_code) {
            if (timer_start(curr, arg, delay) == NULL) {
                ESP_EB_ERROR("error scheduling timer for %d\n", ev_code);
            }
        }
        curr = curr->next;
    }
}

void ICACHE_FLASH_ATTR
esp_eb_trigger(uint16_t ev_code, void *arg)
{
    esp_eb_trigger_delayed(ev_code, ESP_EB_TIMER_MS, arg);
}

void ICACHE_FLASH_ATTR
esp_eb_print_list()
{
    os_printf("list state:\n");
    esp_dll_node *node = head;
    while (node != NULL) {
        os_printf("    %d %p\n", GET_EVENT(node)->ev_code, GET_EVENT(node)->cb);
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
    switch (e->event) {
        case EVENT_STAMODE_CONNECTED:
            ESP_EB_DEBUG("WIFI: EVENT_STAMODE_CONNECTED\n");
            ESP_EB_DEBUG("      ssid    %s\n",
                         e->event_info.connected.ssid);
            ESP_EB_DEBUG("      bssid   "
                             MACSTR
                             "\n", MAC2STR(e->event_info.connected.bssid));
            ESP_EB_DEBUG("      channel %d\n",
                         e->event_info.connected.channel);
            esp_eb_trigger(EVENT_STAMODE_CONNECTED, (void *) e);
            break;

        case EVENT_STAMODE_DISCONNECTED:
            ESP_EB_DEBUG("WIFI: EVENT_STAMODE_DISCONNECTED reason\n");
            ESP_EB_DEBUG("      ssid   %s\n",
                         e->event_info.disconnected.ssid);
            ESP_EB_DEBUG("      bssid  "
                             MACSTR
                             "\n",
                         MAC2STR(e->event_info.disconnected.bssid));
            ESP_EB_DEBUG("      reason %d\n",
                         e->event_info.disconnected.reason);
            esp_eb_trigger(EVENT_STAMODE_DISCONNECTED, (void *) e);
            break;

        case EVENT_STAMODE_AUTHMODE_CHANGE:
            // The mode is one of the AUTH_* values of AUTH_MODE defined in user_interface.h
            os_printf("WIFI: EVENT_STAMODE_AUTHMODE_CHANGE %d -> %d\n",
                      e->event_info.auth_change.old_mode,
                      e->event_info.auth_change.new_mode);
            esp_eb_trigger(EVENT_STAMODE_AUTHMODE_CHANGE, (void *) e);
            break;

        case EVENT_STAMODE_GOT_IP:
            ESP_EB_DEBUG("WIFI: EVENT_STAMODE_GOT_IP\n");
            ESP_EB_DEBUG("      ip   "
                             IPSTR
                             "\n", IP2STR(&(e->event_info.got_ip.ip)));
            ESP_EB_DEBUG("      mask "
                             IPSTR
                             "\n", IP2STR(&(e->event_info.got_ip.mask)));
            ESP_EB_DEBUG("      gw   "
                             IPSTR
                             "\n", IP2STR(&(e->event_info.got_ip.gw)));
            esp_eb_trigger(EVENT_STAMODE_GOT_IP, (void *) e);
            break;

        case EVENT_STAMODE_DHCP_TIMEOUT:
            ESP_EB_DEBUG("WIFI: EVENT_STAMODE_DHCP_TIMEOUT\n");
            esp_eb_trigger(EVENT_STAMODE_DHCP_TIMEOUT, (void *) e);
            break;

        case EVENT_SOFTAPMODE_STACONNECTED:
            ESP_EB_DEBUG("WIFI: EVENT_SOFTAPMODE_STACONNECTED\n");
            ESP_EB_DEBUG("      aid %d\n", e->event_info.sta_connected.aid);
            ESP_EB_DEBUG("      mac "
                             MACSTR
                             "\n",
                         MAC2STR(e->event_info.sta_connected.mac));
            esp_eb_trigger(EVENT_SOFTAPMODE_STACONNECTED, (void *) e);
            break;

        case EVENT_SOFTAPMODE_STADISCONNECTED:
            ESP_EB_DEBUG("WIFI: EVENT_SOFTAPMODE_STADISCONNECTED\n");
            ESP_EB_DEBUG("      aid %d\n", e->event_info.sta_connected.aid);
            ESP_EB_DEBUG("      mac "
                             MACSTR
                             "\n",
                         MAC2STR(e->event_info.sta_disconnected.mac));
            esp_eb_trigger(EVENT_SOFTAPMODE_STADISCONNECTED, (void *) e);
            break;

        case EVENT_OPMODE_CHANGED:
            ESP_EB_DEBUG("WIFI: EVENT_OPMODE_CHANGED %d -> %d\n",
                         e->event_info.opmode_changed.old_opmode,
                         e->event_info.opmode_changed.new_opmode);
            esp_eb_trigger(EVENT_OPMODE_CHANGED, (void *) e);
            break;

        case EVENT_SOFTAPMODE_PROBEREQRECVED:
            ESP_EB_DEBUG("WIFI: EVENT_SOFTAPMODE_PROBEREQRECVED\n");
            ESP_EB_DEBUG("      rssi %d\n",
                         e->event_info.ap_probereqrecved.rssi);
            ESP_EB_DEBUG("      mac  "
                             MACSTR
                             "\n",
                         MAC2STR(e->event_info.ap_probereqrecved.mac));
            esp_eb_trigger(EVENT_SOFTAPMODE_PROBEREQRECVED, (void *) e);
            break;

        default:
            ESP_EB_ERROR("unexpected wifi event: %d\n", e->event);
            break;
    }
}

void ICACHE_FLASH_ATTR
esp_eb_handle_wifi_events()
{
    wifi_set_event_handler_cb(wifi_event_cb);
}

esp_eb_err ICACHE_FLASH_ATTR
esp_eb_attach_wifi_events(esp_eb_cb cb, uint8_t group)
{
    esp_eb_err err;

    err = esp_eb_attach(EVENT_STAMODE_CONNECTED, cb, group);
    if (err)
        return err;

    err = esp_eb_attach(EVENT_STAMODE_DISCONNECTED, cb, group);
    if (err)
        goto cleanup;

    err = esp_eb_attach(EVENT_STAMODE_AUTHMODE_CHANGE, cb, group);
    if (err)
        goto cleanup;

    err = esp_eb_attach(EVENT_STAMODE_GOT_IP, cb, group);
    if (err)
        goto cleanup;

    err = esp_eb_attach(EVENT_STAMODE_DHCP_TIMEOUT, cb, group);
    if (err)
        goto cleanup;

    err = esp_eb_attach(EVENT_OPMODE_CHANGED, cb, group);
    if (err)
        goto cleanup;

    return ESP_EB_OK;

cleanup:
    esp_eb_remove_cb(cb);

    return err;
}
