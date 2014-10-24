# project
project(tnsutil)

# requires cmake version 2.8 or greater
cmake_minimum_required(VERSION 2.8)

set(TNSUTIL_SOURCE_DIR ${CMAKE_SOURCE_DIR})
 
#
set(CMAKE_MODULE_PATH ${TNSUTIL_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

set(CMAKE_MODULE_PATH ${TNSUTIL_SOURCE_DIR}/cmake/modules
    ${CMAKE_MODULE_PATH})

# version
set(TNSUTIL_VERSION "")
file(STRINGS "${CMAKE_SOURCE_DIR}/VERSION"
    _TNSUTIL_VERSION REGEX "version:")
string(REGEX REPLACE "version: " "" TNSUTIL_VERSION ${_TNSUTIL_VERSION})

#
include(Utilities)

#
colormsg(HIBLUE "Configuring TNS Utilities version: ${TNSUTIL_VERSION}")

#
add_subdirectory(src)

