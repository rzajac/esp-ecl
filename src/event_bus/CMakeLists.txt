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


project(event_bus C)

set(SOURCE_FILES event_bus.c)
set(HEADER_FILES include/event_bus.h)
set(PRIVATE_HEADER_FILES event_bus_internal.h)

add_library(${PROJECT_NAME} STATIC
    ${SOURCE_FILES} ${HEADER_FILES} ${PRIVATE_HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${timers_INCLUDE_DIRS}
    ${utils_INCLUDE_DIRS}
    ${ESP_USER_CONFIG_DIR})

target_link_libraries(${PROJECT_NAME} timers utils)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    FRAMEWORK ON
    PUBLIC_HEADER "${HEADER_FILES}"
    PRIVATE_HEADER "${PRIVATE_HEADER_FILES}")

if (DEFINED ENV{EB_DEBUG_ON})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEB_DEBUG_ON")
endif ()

esp_gen_lib(${PROJECT_NAME})
