# Copyright (c) 2011 Zillians Inc, Taiwan. All Rights Reserved.
#
# For more information about Zillians, you can take a look at
# 
#    http://www.zillians.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.6.3)

project(giggle)

find_package(Boost REQUIRED program_options system filesystem)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin CACHE PATH "default executable path" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -fpermissive -fPIC -std=c++0x")
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

