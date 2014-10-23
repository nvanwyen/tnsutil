# project
project(auditshield)

# requires cmake version 2.8 or greater
cmake_minimum_required(VERSION 2.8)

set(AuditShield_SOURCE_DIR ${CMAKE_SOURCE_DIR})
 
# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which
# stores the
# directories of additional CMake modules (ie. MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${AuditShield_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${AuditShield_SOURCE_DIR}/cmake/modules
    ${CMAKE_MODULE_PATH})

#
include(Utilities)

#
colormsg(HIBLUE "Configuring Audit Shield")

#
add_subdirectory(src)

