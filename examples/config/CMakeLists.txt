# Copyright 2018 Rafal Zajac <rzajac@gmail.com>.
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


# The example doesn't use cmake find_package() function because we want
# local libraries not the ones installed in $ESPROOT.

project(ex_config C)

set(SOURCE_FILES main.c)
set(HEADER_FILES )

add_executable(${PROJECT_NAME} main.c
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${ESP_USER_CONFIG})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${stdout_SOURCE_DIR}/include
    ${config_SOURCE_DIR}/include
    ${ESP_USER_CONFIG_DIR})

target_link_libraries(${PROJECT_NAME} stdout config)

esp_gen_exec_targets(${PROJECT_NAME})
