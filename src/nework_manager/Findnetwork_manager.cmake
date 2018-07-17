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

# Try to find utils
#
# Once done this will define:
#
#   network_manager_FOUND        - System found the library.
#   network_manager_INCLUDE_DIR  - The library include directory.
#   network_manager_INCLUDE_DIRS - If library has dependencies this will be set
#                                  to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   network_manager_LIBRARY      - The path to the library.
#   network_manager_LIBRARIES    - The dependencies to link to use the library.
#                                  It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(network_manager_INCLUDE_DIR network_manager.h)
find_library(network_manager_LIBRARY NAMES network_manager)

find_package(event_bus REQUIRED)
find_package(utils REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(network_manager
    DEFAULT_MSG
    network_manager_LIBRARY
    network_manager_INCLUDE_DIR
    event_bus_LIBRARIES
    event_bus_INCLUDE_DIRS
    utils_LIBRARIES
    utils_INCLUDE_DIRS
    lists_LIBRARIES
    lists_INCLUDE_DIRS)

set(network_manager_INCLUDE_DIRS
    ${network_manager_INCLUDE_DIR}
    ${event_bus_INCLUDE_DIRS}
    ${lists_INCLUDE_DIRS}
    ${utils_INCLUDE_DIRS})

set(network_manager_LIBRARIES
    ${network_manager_LIBRARY}
    ${event_bus_LIBRARIES}
    ${lists_LIBRARIES}
    ${utils_LIBRARIES})
