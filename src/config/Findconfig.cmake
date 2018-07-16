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

# Try to find config.
#
# Once done this will define:
#
#   config_FOUND        - System found the library.
#   config_INCLUDE_DIR  - The library include directory.
#   config_INCLUDE_DIRS - If library has dependencies this will be set
#                         to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   config_LIBRARY      - The path to the library.
#   config_LIBRARIES    - The dependencies to link to use the library.
#                         It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(config_INCLUDE_DIR config.h)
find_library(config_LIBRARY NAMES config)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(config
    DEFAULT_MSG
    config_LIBRARY
    config_INCLUDE_DIR)

set(config_INCLUDE_DIRS ${config_INCLUDE_DIR})
set(config_LIBRARIES ${config_LIBRARY})
