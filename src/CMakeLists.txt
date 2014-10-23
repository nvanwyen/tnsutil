#
set(TNS_UTIL_DEPENDENCIES)

#
colormsg(HIGREEN "Finding Threads library")
set(DL_FIND_QUIETLY TRUE)
include(FindThreads)

#
if(Threads_FOUND OR CMAKE_USE_WIN32_THREADS_INIT OR CMAKE_THREAD_LIBS_INIT)
    colormsg(HICYAN "Processing Threads library")
    list(APPEND TNS_UTIL_DEPENDENCIES ${CMAKE_THREAD_LIBS_INIT})
    add_definitions(-DHAVE_THREADS=1)

else()
    colormsg(HIRED "Threads library not found!")
    message(FATAL_ERROR "No Thread library found")
endif()

#
if(NOT MSVC)

    set(DL_FIND_QUIETLY TRUE)

    colormsg(HIGREEN "Finding DL library")
    find_package(DL REQUIRED)

    if(DL_FOUND)

        colormsg(HICYAN "Processing DL library")

        list(APPEND TNS_UTIL_DEPENDENCIES ${DL_LIBRARY})
        set_directory_properties(PROPERTIES
            INCLUDE_DIRECTORIES ${DL_INCLUDE_DIR})
        add_definitions(-DHAVE_DL=1)

    else()

        colormsg(HIRED "DL library not found!")
        message(FATAL_ERROR "No DL library found")

    endif()

endif()

#
colormsg(HIGREEN "Finding Boost library")
set(Boost_FIND_QUIETLY TRUE)
include(FindBoost)
find_package(Boost REQUIRED COMPONENTS thread program_options system)

#
if(Boost_FOUND)

    colormsg(HICYAN "Processing Boost library")

    get_property(TNS_UTIL_COMPILE_DEFINITIONS
        DIRECTORY ${CMAKE_SOURCE_DIR}
        PROPERTY COMPILE_DEFINITIONS)
    
    list(APPEND TNS_UTIL_COMPILE_DEFINITIONS "HAVE_BOOST=1")
    add_definitions(-DHAVE_BOOST=1)

    #
    if(Boost_DATE_TIME_FOUND)

        list(APPEND TNS_UTIL_DEPENDENCIES
            ${Boost_DATE_TIME_LIBRARY})

        list(APPEND TNS_UTIL_COMPILE_DEFINITIONS
            "HAVE_BOOST_DATE_TIME=1")
        add_definitions(-DHAVE_BOOST_DATE_TIME=1)

    else()

        add_definitions(-DHAVE_BOOST_DATE_TIME=0)

    endif()

    #
    if(Boost_SYSTEM_FOUND)

        list(APPEND TNS_UTIL_DEPENDENCIES
            ${Boost_SYSTEM_LIBRARY})

        list(APPEND TNS_UTIL_COMPILE_DEFINITIONS
            "HAVE_BOOST_SYSTEM=1")
        add_definitions(-DHAVE_BOOST_SYSTEM=1)

    else()

        add_definitions(-DHAVE_BOOST_SYSTEM=0)

    endif()

    #
    if(Boost_THREAD_FOUND)

        list(APPEND TNS_UTIL_DEPENDENCIES
            ${Boost_THREAD_LIBRARY})

        list(APPEND TNS_UTIL_COMPILE_DEFINITIONS
            "HAVE_BOOST_THREAD=1")
        add_definitions(-DHAVE_BOOST_THREAD=1)

    else()

        add_definitions(-DHAVE_BOOST_THREAD=0)

    endif()

    set_property(DIRECTORY ${CMAKE_SOURCE_DIR}
        PROPERTY COMPILE_DEFINITIONS
        ${TNS_UTIL_COMPILE_DEFINITIONS})
    
    set_directory_properties(PROPERTIES
        INCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS})
    
    set_property(DIRECTORY
        ${CMAKE_SOURCE_DIR}
        PROPERTY INCLUDE_DIRECTORIES
        ${Boost_INCLUDE_DIRS})

    link_directories(${Boost_LIBRARY_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})

else()

    add_definitions(-DHAVE_BOOST=0)
    colormsg(HIRED "Boost library not found!")
    message(FATAL_ERROR "No Boost library found")

endif()

#
colormsg(HIGREEN "Finding Oracle library")
set(ORACLE_FIND_QUIETLY TRUE)
include(FindORACLE)
find_package(ORACLE REQUIRED)

#
if(ORACLE_FOUND)

    colormsg(HICYAN "Processing oracle library")

    get_property(Loadstar_COMPILE_DEFINITIONS
        DIRECTORY ${CMAKE_SOURCE_DIR}
        PROPERTY COMPILE_DEFINITIONS)
    
    list(APPEND Loadstar_COMPILE_DEFINITIONS "HAVE_ORACLE=1")
    add_definitions(-DHAVE_ORACLE=1)
    
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR}
        PROPERTY COMPILE_DEFINITIONS
        ${Loadstar_COMPILE_DEFINITIONS})
    
    set_directory_properties(PROPERTIES
        INCLUDE_DIRECTORIES
        ${ORACLE_INCLUDE_DIR})
    
    set_property(DIRECTORY
        ${CMAKE_SOURCE_DIR}
        PROPERTY INCLUDE_DIRECTORIES
        ${ORACLE_INCLUDE_DIR})

    link_directories(${ORACLE_LIBRARY_DIR})
    include_directories(${ORACLE_INCLUDE_DIR})

else()

    add_definitions(-DHAVE_ORACLE=0)
    colormsg(HIRED "Oracle library not found!")
    message(FATAL_ERROR "No Oracle library found")

endif()

#
colormsg(HIBLUE "Configuring output directories")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#
colormsg(HIBLUE "Configuring TNS2LDIF source files")
file(GLOB TNS2LDIF_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tns2ldif.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tns2ldif.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ver.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ldp.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/exp.h")

#
add_executable(tns2ldif ${TNS2LDIF_SOURCE_FILES})
#target_link_libraries(tns2ldif clntsh)
target_link_libraries(tns2ldif)

#
colormsg(HIBLUE "Configuring LDIF2TNS source files")
file(GLOB LDIF2TNS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ldif2tns.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ldif2tns.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ver.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ldp.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/exp.h")

#
add_executable(ldif2tns ${LDIF2TNS_SOURCE_FILES})
#target_link_libraries(ldif2tns clntsh)
target_link_libraries(ldif2tns)

#
colormsg(HIBLUE "Configuring TNS2LDAP source files")
file(GLOB TNS2LDAP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tns2ldap.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tns2ldap.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ver.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ldp.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/exp.h")

#
add_executable(tns2ldap ${TNS2LDAP_SOURCE_FILES})
#target_link_libraries(tns2ldap clntsh)
target_link_libraries(tns2ldap)

#
colormsg(HIBLUE "Configuring LDAP2TNS source files")
file(GLOB LDAP2TNS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ldap2tns.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/ldap2tns.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ver.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/tns.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/ora.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ldp.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/env.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/exp.h")

#
add_executable(ldap2tns ${LDIF2TNS_SOURCE_FILES})
#target_link_libraries(ldap2tns clntsh)
target_link_libraries(ldap2tns)

#
colormsg(HIBLUE "Configuring target installation destination")
install(TARGETS tns2ldif DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(TARGETS ldif2tns DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(TARGETS tns2ldap DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(TARGETS ldap2tns DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

