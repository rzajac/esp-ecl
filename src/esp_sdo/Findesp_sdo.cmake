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

# Try to find esp_sdo
#
# Once done this will define:
#
#   esp_sdo_FOUND        - System found the library.
#   esp_sdo_INCLUDE_DIR  - The library include directory.
#   esp_sdo_INCLUDE_DIRS - If library has dependencies this will be set
#                          to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   esp_sdo_LIBRARY      - The path to the library.
#   esp_sdo_LIBRARIES    - The dependencies to link to use the library.
#                          It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(esp_sdo_INCLUDE_DIR esp_sdo.h)
find_library(esp_sdo_LIBRARY NAMES esp_sdo)

find_package(esp_gpio REQUIRED)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(esp_sdo
    DEFAULT_MSG
    esp_sdo_LIBRARY
    esp_sdo_INCLUDE_DIR)

set(esp_sdo_INCLUDE_DIRS ${esp_sdo_INCLUDE_DIR} ${esp_gpio_INCLUDE_DIRS})
set(esp_sdo_LIBRARIES ${esp_sdo_LIBRARY} ${esp_gpio_LIBRARIES})
