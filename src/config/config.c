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


#include "include/config.h"


// Maximum config index.
#define CONFIG_MAX_INDEX (CONFIG_NUMBER - 1)


// Type defining user configuration structure.
typedef struct {
    uint32 cfg_size; // The size of the user configuration structure.
    void *cfg;       // The pointer to user config structure.
} user_config;

// Pointers to initialized configuration structures.
static user_config *user_configs[CONFIG_NUMBER];


static sint8 ICACHE_FLASH_ATTR
config_check(uint8_t cfg_num)
{
    if (cfg_num > CONFIG_MAX_INDEX)
        return ESP_E_ARG;
    if (user_configs[cfg_num] == NULL)
        return ESP_E_LOG;

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
config_init(uint8_t cfg_num, void *config, uint32 size)
{
    sint8 err = config_check(cfg_num);

    // We ignore ESP_E_LOG in this case.
    if (!(err == ESP_OK || err == ESP_E_LOG))
        return err;

    user_configs[cfg_num] = os_zalloc(sizeof(user_config));
    if (user_configs[cfg_num] == NULL)
        return ESP_E_MEM;

    user_configs[cfg_num]->cfg = config;
    user_configs[cfg_num]->cfg_size = size;

    return ESP_OK;
}

sint8 ICACHE_FLASH_ATTR
config_read(uint8_t cfg_num)
{
    sint8 err = config_check(cfg_num);
    if (err != ESP_OK)
        return err;

    uint32 sec_adr = (uint32) ((CONFIG_START_SECTOR + cfg_num) *
                               SPI_FLASH_SEC_SIZE);
    SpiFlashOpResult result = spi_flash_read(sec_adr,
                                             (uint32 *) user_configs[cfg_num]->cfg,
                                             user_configs[cfg_num]->cfg_size);

    return result;
}

sint8 ICACHE_FLASH_ATTR
config_write(uint8_t cfg_num)
{
    sint8 err = config_check(cfg_num);
    if (err != ESP_OK)
        return err;

    uint32 sec_adr = (uint32) ((CONFIG_START_SECTOR + cfg_num) *
                               SPI_FLASH_SEC_SIZE);

    ETS_UART_INTR_DISABLE();
    SpiFlashOpResult result = spi_flash_erase_sector(
        (uint16) (CONFIG_START_SECTOR + cfg_num));
    if (result == SPI_FLASH_RESULT_OK) {
        result = spi_flash_write(sec_adr, (uint32 *) user_configs[cfg_num]->cfg,
                                 user_configs[cfg_num]->cfg_size);
    }
    ETS_UART_INTR_ENABLE();

    return result;
}
