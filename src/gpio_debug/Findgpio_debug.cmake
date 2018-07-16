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

# Try to find gpio_debug.
#
# Once done this will define:
#
#   gpio_debug_FOUND        - System found the library.
#   gpio_debug_INCLUDE_DIR  - The library include directory.
#   gpio_debug_INCLUDE_DIRS - If library has dependencies this will be set
#                             to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   gpio_debug_LIBRARY      - The path to the library.
#   gpio_debug_LIBRARIES    - The dependencies to link to use the library.
#                             It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(gpio_debug_INCLUDE_DIR gpio_debug.h)
find_library(gpio_debug_LIBRARY NAMES gpio_debug)

find_package(esp_gpio REQUIRED)
find_package(utils REQUIRED)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(gpio_debug
    DEFAULT_MSG
    gpio_debug_LIBRARY
    gpio_debug_INCLUDE_DIR
    esp_gpio_LIBRARIES
    esp_gpio_INCLUDE_DIRS
    utils_LIBRARIES
    utils_INCLUDE_DIRS)

set(gpio_debug_INCLUDE_DIRS
    ${gpio_debug_INCLUDE_DIR}
    ${esp_gpio_INCLUDE_DIRS}
    ${utils_INCLUDE_DIRS})

set(gpio_debug_LIBRARIES
    ${gpio_debug_LIBRARY}
    ${esp_gpio_LIBRARIES}
    ${utils_LIBRARIES})
