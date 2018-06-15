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


project(c_json C)

set(SOURCE_FILES
    cJSON.c
    cJSON_Utils.c)

set(HEADER_FILES
    include/cJSON.h
    include/cJSON_Utils.h)

set(PRIVATE_HEADER_FILES
    include/cJSON_Esp.h)

add_library(c_json STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${PRIVATE_HEADER_FILES})

target_include_directories(c_json PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ESP_USER_CONFIG_DIR})

esp_gen_lib(c_json)
