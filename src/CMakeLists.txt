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


project(esp_ecl C)

################################## esp_gpio ###################################

add_library(esp_gpio STATIC
    esp_gpio.c
    include/esp_gpio.h)

target_include_directories(esp_gpio PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_gpio ${ESP_CMAKE_FIND_DIR})

################################## esp_gpio_debug #############################

add_library(esp_gpio_debug STATIC
    esp_gpio_debug.c
    include/esp_gpio_debug.h)

target_include_directories(esp_gpio_debug PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_gpio_debug ${ESP_CMAKE_FIND_DIR})

################################## esp_sdo ####################################

add_library(esp_sdo STATIC
    esp_sdo.c
    include/esp_sdo.h)

target_include_directories(esp_sdo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_sdo ${ESP_CMAKE_FIND_DIR})

################################## esp_json ###################################

add_library(esp_json STATIC
    esp_json.c
    include/esp_json.h)

target_include_directories(esp_json PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_json ${ESP_CMAKE_FIND_DIR})

################################## esp_cfg ###################################

add_library(esp_cfg STATIC
    esp_cfg.c
    include/esp_cfg.h)

target_include_directories(esp_cfg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_cfg ${ESP_CMAKE_FIND_DIR})

################################## esp_aes ###################################

add_library(esp_aes STATIC
    esp_aes.c
    include/esp_aes.h)

target_include_directories(esp_aes PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_aes ${ESP_CMAKE_FIND_DIR})
install(FILES "${ESP_LIB_BIN_DIR}/gen_aes.sh" DESTINATION "${CMAKE_INSTALL_BINDIR}")

################################## esp_tim ###################################

add_library(esp_tim STATIC
    esp_tim.c
    esp_util.c
    include/esp_tim.h
    include/esp_util.h)

target_include_directories(esp_tim PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_tim ${ESP_CMAKE_FIND_DIR})

################################## esp_util ###################################

add_library(esp_util STATIC
    esp_util.c
    include/esp_util.h)

target_include_directories(esp_util PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_util ${ESP_CMAKE_FIND_DIR})

################################## esp_eb ###################################

add_library(esp_eb STATIC
    esp_eb.c
    include/esp_eb.h)

target_include_directories(esp_eb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(esp_eb ${ESP_CMAKE_FIND_DIR})

####################### Install all header files #############################

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
