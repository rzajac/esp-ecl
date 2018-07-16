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

# Try to find timers.
#
# Once done this will define:
#
#   timers_FOUND        - System found the library.
#   timers_INCLUDE_DIR  - The library include directory.
#   timers_INCLUDE_DIRS - If library has dependencies this will be set
#                         to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   timers_LIBRARY      - The path to the library.
#   timers_LIBRARIES    - The dependencies to link to use the library.
#                         It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(timers_INCLUDE_DIR timers.h)
find_library(timers_LIBRARY NAMES timers)

find_package(utils REQUIRED)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(timers
    DEFAULT_MSG
    timers_LIBRARY
    timers_INCLUDE_DIR
    utils_LIBRARIES
    utils_INCLUDE_DIRS)

set(timers_INCLUDE_DIRS
    ${timers_INCLUDE_DIR}
    ${utils_INCLUDE_DIRS})

set(timers_LIBRARIES
    ${timers_LIBRARY}
    ${utils_LIBRARIES})
