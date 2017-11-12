# Copyright 2017 Rafal Zajac <rzajac@gmail.com>.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.


project(esp_gpio_debug C)

add_library(esp_gpio_debug STATIC
    esp_gpio_debug.c
    include/esp_gpio_debug.h
    ../esp_gpio/include/esp_gpio.h
    ../esp_util/include/esp_util.h)

target_include_directories(esp_gpio_debug PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR}
    ../esp_gpio/include
    ../esp_util/include)

esp_gen_lib(esp_gpio_debug ${ESP_CMAKE_FIND_DIR})
