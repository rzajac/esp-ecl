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

# Try to find stdout.
#
# Once done this will define:
#
#   stdout_FOUND        - System found the library.
#   stdout_INCLUDE_DIR  - The library include directory.
#   stdout_INCLUDE_DIRS - If library has dependencies this will be set
#                         to <lib_name>_INCLUDE_DIR [<dep1_name_INCLUDE_DIRS>, ...].
#   stdout_LIBRARY      - The path to the library.
#   stdout_LIBRARIES    - The dependencies to link to use the library.
#                         It will have a form of <lib_name>_LIBRARY [dep1_name_LIBRARIES, ...].
#


find_path(stdout_INCLUDE_DIR stdout.h)
find_library(stdout_LIBRARY NAMES stdout)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(stdout
    DEFAULT_MSG
    stdout_LIBRARY
    stdout_INCLUDE_DIR)

set(stdout_INCLUDE_DIRS ${stdout_INCLUDE_DIR})
set(stdout_LIBRARIES ${stdout_LIBRARY})
