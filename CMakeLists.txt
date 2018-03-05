####################################################################################
##
##     _____    ____ _       ____ _        __ _      __ _  __ _  ______ _   ___ _
##    / /| |]  |  __ \\     / ___ \\      / \ \\    |   \\/   |]|  _____|] / ___|]
##   / //| |]  | |] \ \\   | |]  \_|]    / //\ \\   | |\ / /| |]| |]___ _ ( ((_ _
##  / //_| |]_ | |]  ) ))  | |]  __ _   / _____ \\  | |]\_/ | |]|  _____|] \___ \\
## |_____   _|]| |]_/ //   | |]__/  |] / //    \ \\ | |]    | |]| |]___ _   ___) ))
##       |_|]  |_____//     \_____/|]]/_//      \_\\|_|]    |_|]|_______|] |____//
##       
##
####################################################################################
##
##
##  File    : CMakeLists.txt
##  Author  : Anthony Islas
##  Purpose : Directions for CMake to auto-generate Makefiles
##  Group   : Components
##
##  License : GNU GPL v3
##
####################################################################################

#
# Dependencies
#
cmake_minimum_required ( VERSION 3.5.0 )

set ( CMAKE_BUILD_TYPE debug )


####################################################################################
#
# Options
#
####################################################################################
option( UNIT_TEST   "Enable Google Test Framework" OFF )
option( XX_LIB      "Set to compile XX LIB"        OFF )

set   ( THIRDPARTY 
        "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty" 
        CACHE STRING 
        "Thirdparty directory" )


#
# Get our modules after options
#
list( APPEND 
      CMAKE_MODULE_PATH
      ${CMAKE_CURRENT_SOURCE_DIR}/cmake 
      )

include( GoogleTestSetup )
include( OptionPrinter   )
include( VersionExtract  )

####################################################################################
#
# Project setup
#
####################################################################################
version_extract_str( ${CMAKE_CURRENT_SOURCE_DIR}/versions.txt PROJECT_VERSION )
project ( 
          Components 
          VERSION 
          ${PROJECT_VERSION}
          )

message ( "Configuring " ${PROJECT_NAME} )
detailed_print( "PROJECT VERSION" CLR_MAGENTA ${PROJECT_VERSION} CLR_CYAN 14 LEFT )


include_directories ( ${PROJECT_BINARY_DIR} )


####################################################################################
#
# Regression test variable
#
####################################################################################
set ( TEST_SOURCES
      "" 
    )
set ( TEST_INCLUDES
      ""
    )
set ( TEST_RESOURCES
      ${CMAKE_BINARY_DIR}/gtest_resources/
    )

####################################################################################
#
# Necessary packages
#
####################################################################################
find_package ( Threads )


####################################################################################
#
# Source directories
#
####################################################################################
message ( "Configuring ${PROJECT_NAME}")
add_subdirectory ( resources )
add_subdirectory ( string    )
add_subdirectory ( timing    )





####################################################################################
#
# Configuration stuff
#
####################################################################################
configure_file ( ${PROJECT_SOURCE_DIR}/common/config.hpp.in
                 ${PROJECT_BINARY_DIR}/config.hpp 
                 @ONLY 
                )


####################################################################################
#
# Regression test
#
####################################################################################
if ( UNIT_TEST )

  message ( "Current Tests: " ) 
  foreach (test_file ${TEST_SOURCES} )
    message ( '${test_file}' )
  endforeach ()

  message ( "Resources located in : '${TEST_RESOURCES}'" )

endif()


