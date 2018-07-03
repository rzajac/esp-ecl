#!/usr/bin/env bash

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

# ESP8266 library installation script.
#
# Installer expects that:
# - The ESPROOT environment variable set.
# - The esp-open-sdk is installed and compiled at ESPROOT/esp-open-sdk.

# The full GitHub URL to the library.
export LIB_FULL_NAME="rzajac/esp-ecl"

# No modifications below this comment unless you know what you're doing.

# Check / set ESPROOT.
if [ "x${ESPROOT}" == "x" ]; then ESPROOT=$HOME/esproot; fi

# Install.
sh ${ESPROOT}/bin/lib-install.sh

exit $?
