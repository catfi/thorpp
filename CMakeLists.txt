#
# Zillians
# Copyright (C) 2011-2012 Zillians.com, Inc.
# For more information see http://www.zillians.com
#

cmake_minimum_required(VERSION 2.6.3)

project(giggle)

find_package(Boost REQUIRED program_options system filesystem)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin CACHE PATH "default executable path" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -fpermissive -fPIC -std=c++0x")
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

