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

#include <esp_delay.h>

void esp_delay_us(uint32_t us)
{
  uint32_t idx;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
  uint32_t reg;
#pragma GCC diagnostic pop
  us *= ESP_DELAY_MULTIPLIER;
  for (idx = 0; idx < us; idx++) reg = *((volatile uint32_t *) (ESP_DELAY_REG_READ));
}
