/*
 * Copyright 2017 Rafal Zajac <rzajac@gmail.com>.
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

#include <esp_util.h>
#include <osapi.h>


void esp_util_delay_us(uint32_t us)
{
  uint32_t idx;
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
  uint32_t reg;
  #pragma GCC diagnostic pop

  us *= ESP_DELAY_MULTIPLIER;
  for (idx = 0; idx < us; idx++) reg = *((volatile uint32_t *) (ESP_DELAY_REG_READ));
}

void ICACHE_FLASH_ATTR
esp_util_dump_binary32(uint32_t data)
{
  uint32_t mask;

  for (mask = 0x80000000; mask != 0; mask >>= 1) {
    os_printf("%d", (data & mask) > 0);
    if (
      (mask == 0x10000000) ||
      (mask == 0x1000000) ||
      (mask == 0x100000) ||
      (mask == 0x1000) ||
      (mask == 0x100) ||
      (mask == 0x10))
      os_printf(" ");
    if (mask == 0x10000) os_printf("  ");
  }
  os_printf("\n");
}

void ICACHE_FLASH_ATTR
esp_util_dump_binary16(uint16_t data)
{
  uint16_t mask;

  for (mask = 0x8000; mask != 0; mask >>= 1) {
    os_printf("%d", (data & mask) > 0);
    if ((mask == 0x1000) || (mask == 0x100) || (mask == 0x10)) os_printf(" ");
  }
  os_printf("\n");
}

void ICACHE_FLASH_ATTR
esp_util_dump_binary8(uint8_t data)
{
  uint32_t mask;

  for (mask = 0x80; mask != 0; mask >>= 1) {
    os_printf("%d", (data & mask) > 0);
    if ((mask == 0x10)) os_printf(" ");
  }
  os_printf("\n");
}

int ICACHE_FLASH_ATTR
esp_util_power(int base, int exp)
{
  int result = 1;
  while (exp) {
    result *= base;
    exp--;
  }

  return result;
}

char *ICACHE_FLASH_ATTR
esp_util_ftoa(float num, uint8_t decimals)
{
  static char *buf[16];

  int whole = (int) num;
  int decimal = (int) ((num - whole) * esp_util_power(10, decimals));
  if (decimal < 0) {
    // get rid of sign on decimal portion
    decimal -= 2 * decimal;
  }

  char *pattern[10]; // setup printf pattern for decimal portion
  os_sprintf((char *) pattern, "%%d.%%0%dd", decimals);
  os_sprintf((char *) buf, (const char *) pattern, whole, decimal);

  return (char *) buf;
}

void ICACHE_FLASH_ATTR
esp_util_dump_bytes(const char *label, const uint8_t *buf, uint16 len)
{
  uint8_t i;

  os_printf("%s (%d)\n", label, len);
  for (i = 0; i < len; i++) {
    os_printf("%02x ", buf[i]);
    if ((i + 1) % 8 == 0) os_printf(" ");
    if ((i + 1) % 16 == 0) os_printf("\n");
  }
  os_printf("\n");
  os_printf("\n");
}
