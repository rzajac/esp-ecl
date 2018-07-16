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


#ifndef CONFIG_H
#define CONFIG_H

#include <c_types.h>
#include <ets_sys.h>
#include <mem.h>
#include <spi_flash.h>

#include <esp.h>

// The start address for configuration sectors.
#ifndef CONFIG_START_SECTOR
    #define CONFIG_START_SECTOR 0xC
#endif

// Defines the number of configuration sectors available.
#ifndef CONFIG_NUMBER
    #define CONFIG_NUMBER 4
#endif

// Espressif SDK missing includes.
void ets_isr_mask(unsigned intr);
void ets_isr_unmask(unsigned intr);

/**
 * Initialize user config.
 *
 * This MUST be called before reading or writing custom configuration.
 *
 * @param cfg_num The config number. See CONFIG_NUMBER.
 * @param config  The pointer to custom config structure.
 * @param size    The size of the custom config structure.
 *
 * @return Error code or ESP_OK on success.
 */
sint8 ICACHE_FLASH_ATTR
config_init(uint8_t cfg_num, void *config, uint32 size);

/**
 * Read custom config from flash memory.
 *
 * @param cfg_num The config number. See CONFIG_NUMBER.
 *
 * @return The status of read operation.
 */
sint8 ICACHE_FLASH_ATTR
config_read(uint8_t cfg_num);

/**
 * Write custom configuration to flash.
 *
 * @param cfg_num The config number. See CONFIG_NUMBER.
 *
 * @return The status of write operation.
 */
sint8 ICACHE_FLASH_ATTR
config_write(uint8_t cfg_num);

#endif // CONFIG_H
