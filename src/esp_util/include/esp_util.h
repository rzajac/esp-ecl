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


#ifndef ESP_UTIL_H
#define ESP_UTIL_H

#include <c_types.h>


// The register to read in a loop.
#define ESP_DELAY_REG_READ 0x60000318
// How many loops take 1us.
#define ESP_DELAY_MULTIPLIER 3


/**
 * Duplicate string.
 *
 * User must take care of releasing memory allocated to
 * string duplicate.
 *
 * @param str The string to duplicate.
 *
 * @return Pointer to duplicated string or NULL on error.
 */
char *ICACHE_FLASH_ATTR
esp_util_strdup(const char *str);

/**
 * Noop delay.
 *
 * @param us Number of microseconds.
 */
void esp_util_delay_us(uint32_t us);

/**
 * Dump binary representation of the 8bit value.
 *
 * @param value
 */
void ICACHE_FLASH_ATTR
esp_util_dump_binary8(uint8_t data);

/**
 * Dump binary representation of the 16bit value.
 *
 * @param value
 */
void ICACHE_FLASH_ATTR
esp_util_dump_binary16(uint16_t data);

/**
 * Dump binary representation of the 32bit value.
 *
 * @param value
 */
void ICACHE_FLASH_ATTR
esp_util_dump_binary32(uint32_t value);

/**
 * Dump buffer as hexadecimal values.
 *
 * @param label The label to print.
 * @param buf   The buffer.
 * @param len   The length of the buffer.
 */
void ICACHE_FLASH_ATTR
esp_util_dump_bytes(const char *label, const uint8_t *buf, uint16 len);

/**
 * Rise base to power of exp.
 *
 * From: http://bbs.espressif.com/viewtopic.php?t=246
 *
 * @param base The number.
 * @param exp  The exponent.
 *
 * @return Product.
 */
int ICACHE_FLASH_ATTR
esp_util_power(int base, int exp);

/**
 * Get string representation of float.
 *
 * From: http://bbs.espressif.com/viewtopic.php?t=246
 *
 * Warning: limited to 15 chars & non-reentrant.
 *          e.g., don't use more than once per os_printf call.
 *
 * @param num       The float to convert to string.
 * @param decimals  The number of decimal places.
 *
 * @return The float string representation.
 */
char *ICACHE_FLASH_ATTR
esp_util_ftoa(float num, uint8_t decimals);

#endif //ESP_UTIL_H
