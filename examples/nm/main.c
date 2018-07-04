/**
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
#include <osapi.h>
#include <mem.h>

#include "esp_sdo.h"
#include "esp_nm.h"

void ICACHE_FLASH_ATTR
sys_init_done(void)
{
  os_printf("USER: system initialized\n");

  esp_nm_start("TestHive", "xqfiricw2g", true, 0, 0, 0);
}

void ICACHE_FLASH_ATTR user_init()
{
  stdout_init(BIT_RATE_74880);
  system_init_done_cb(sys_init_done);
}
