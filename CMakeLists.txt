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

#
include(Utilities)

#
colormsg(HIBLUE "Configuring TNMS Utilities")

#
add_subdirectory(src)

