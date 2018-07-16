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


#ifndef UTL_H
#define UTL_H

#include <user_interface.h>
#include <c_types.h>
#include <mem.h>


/**
 * Duplicate string.
 *
 * User must take care of releasing memory allocated to
 * duplicated string.
 *
 * @param str The string to duplicate.
 *
 * @return Pointer to duplicated string or NULL on error.
 */
char *ICACHE_FLASH_ATTR
utl_strdup(const char *str);

/**
 * Noop delay.
 *
 * Runs tight for loop for approximately given number of microseconds.
 *
 * @param us Number of microseconds.
 */
void utl_delay_us(uint32_t us);

/**
 * Dump binary representation of the 8bit value.
 *
 * @param value
 */
void ICACHE_FLASH_ATTR
utl_dump_binary8(uint8_t value);

/**
 * Dump binary representation of the 16bit value.
 *
 * @param value
 */
void ICACHE_FLASH_ATTR
utl_dump_binary16(uint16_t value);

/**
 * Dump binary representation of the 32bit value.
 *
 * @param value
 */
void ICACHE_FLASH_ATTR
utl_dump_binary32(uint32_t value);

/**
 * Dump buffer as hexadecimal values.
 *
 * @param label The label to print.
 * @param buf   The buffer.
 * @param len   The length of the buffer.
 */
void ICACHE_FLASH_ATTR
utl_dump_bytes(const char *label, const uint8_t *buf, uint16 len);

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
utl_power(int base, int exp);

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
utl_ftoa(float num, uint8_t decimals);

#endif //UTL_H
