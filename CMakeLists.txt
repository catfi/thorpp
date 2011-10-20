#
# Zillians
# Copyright (C) 2011-2012 Zillians.com, Inc.
# For more information see http://www.zillians.com
#

cmake_minimum_required(VERSION 2.6.3)

project(giggle)

find_package(Boost REQUIRED program_options system filesystem)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -fpermissive -fPIC -std=c++0x")

add_subdirectory(src)

