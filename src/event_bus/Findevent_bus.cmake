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

# Try to find event_bus.
#
# Once done this will define:
#
#   event_bus_FOUND        - System found the library.
#   event_bus_INCLUDE_DIR  - The library include directory.
#   event_bus_INCLUDE_DIRS - If library has dependencies this will be set
#                            to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   event_bus_LIBRARY      - The path to the library.
#   event_bus_LIBRARIES    - The dependencies to link to use the library.
#                            It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(event_bus_INCLUDE_DIR event_bus.h)
find_library(event_bus_LIBRARY NAMES event_bus)

find_package(timers REQUIRED)
find_package(utils REQUIRED)
find_package(lists REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(event_bus
    DEFAULT_MSG
    event_bus_LIBRARY
    event_bus_INCLUDE_DIR
    timers_LIBRARIES
    timers_INCLUDE_DIRS
    utils_LIBRARIES
    utils_INCLUDE_DIRS
    lists_LIBRARIES
    lists_INCLUDE_DIRS)

set(event_bus_INCLUDE_DIRS
    ${event_bus_INCLUDE_DIR}
    ${timers_INCLUDE_DIRS}
    ${utils_INCLUDE_DIRS}
    ${lists_INCLUDE_DIRS})

set(event_bus_LIBRARIES
    ${event_bus_LIBRARY}
    ${timers_LIBRARIES}
    ${utils_LIBRARIES}
    ${lists_LIBRARIES})
