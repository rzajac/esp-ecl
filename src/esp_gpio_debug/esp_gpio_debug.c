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


#include <osapi.h>
#include "../esp_gpio/include/esp_gpio.h"
#include "../esp_util/include/esp_util.h"
#include "include/esp_gpio_debug.h"


void ICACHE_FLASH_ATTR
esp_gpiod_dump_mux_addr(uint8_t gpio_num)
{
    os_printf("GPIO%d MUX 0x%X: ", gpio_num,
              (0x60000000 + GPIO_MUX_ADR(gpio_num)));
}

void ICACHE_FLASH_ATTR
esp_gpiod_dump_mux_reg(uint8_t gpio_num)
{
    esp_gpiod_dump_mux_addr(gpio_num);
    esp_util_dump_binary32(GPIO_MUX(gpio_num));
}

void ICACHE_FLASH_ATTR
esp_gpiod_dump_reg(uint32_t gpio_num)
{
    os_printf("GPIO%d REG 0x%X: ", gpio_num,
              (0x60000000 + GPIO_CFG_ADR(gpio_num)));
    esp_util_dump_binary32(GPIO_CFG(gpio_num));
}

void ICACHE_FLASH_ATTR
esp_gpiod_dump_en()
{
    os_printf("GPIO EN 0x0x6000030C: ");
    esp_util_dump_binary32(GPIO_EN);
}

void ICACHE_FLASH_ATTR
esp_gpiod_dump_out()
{
    os_printf("GPIO OU 0x0x60000300: ");
    esp_util_dump_binary32(GPIO_OUT);
}

void ICACHE_FLASH_ATTR
esp_gpiod_dump_in()
{
    os_printf("GPIO IN 0x0x60000318: ");
    esp_util_dump_binary32(GPIO_IN);
}
