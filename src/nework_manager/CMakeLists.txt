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


project(network_manager C)

set(SOURCE_FILES network_manager.c nm_tcp.c nm_wifi.c)
set(HEADER_FILES include/network_manager.h ${ESP_HEADER})
set(PRIVATE_HEADER_FILES nm_internal.h nm_tcp.h nm_wifi.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${PRIVATE_HEADER_FILES})

target_include_directories(${PROJECT_NAME} BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${stdout_SOURCE_DIR}/include
    ${event_bus_SOURCE_DIR}/include
    ${utils_SOURCE_DIR}/include
    ${ESP_USER_CONFIG_DIR})

target_link_libraries(${PROJECT_NAME} stdout event_bus utils lists)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    FRAMEWORK ON
    PUBLIC_HEADER "${HEADER_FILES}"
    PRIVATE_HEADER "${PRIVATE_HEADER_FILES}")

if (DEFINED ENV{NM_DEBUG_ON})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNM_DEBUG_ON")
endif ()

esp_gen_lib(${PROJECT_NAME})
