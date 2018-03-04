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

#
# Get our modules
#
list( APPEND 
      CMAKE_MODULE_PATH
      ${CMAKE_CURRENT_SOURCE_DIR}/cmake 
      )

include( GoogleTestSetup )
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

message ( "Creating " ${PROJECT_NAME} )
message ( STATUS "PROJECT VERSION : "  ${PROJECT_VERSION} )

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
      ${CMAKE_BINARY_DIR}/resources/
    )

####################################################################################
#
# Necessary packages
#
####################################################################################
find_package ( Threads )


####################################################################################
#
# Custom functions
#
####################################################################################
function ( print_include_dirs target)

  message( "Listing search paths..." )
  get_target_property ( 
                        dirs 
                        ${target} 
                        INCLUDE_DIRECTORIES
                      )
  foreach( dir ${dirs} )
    message( STATUS "dir = '${dir}'" )
  endforeach()

endfunction ()

# DO NOT USE to get sources from actual build, only tests
function ( get_test_sources dirs header_files source_files )

  foreach ( dir ${dirs} )
    message ( "Acquiring test source files..." )

    file ( GLOB HPP 
           ${dir}/*.hpp
         )
    # Report
    foreach ( header ${HPP} )
      message ( STATUS "Found '${header}'" )
    endforeach ()

    file ( GLOB CXX
           ${dir}/*.cpp
         )
    # Report
    foreach ( source ${CXX} )
      message ( STATUS "Found '${source}'" )
    endforeach ()

    # add
    list ( APPEND 
           HPP_LIST
           ${HPP}
         )
    list ( APPEND
           CXX_LIST
           ${CXX}
         )
  endforeach ()

  # set
  set ( ${source_files}
        ${CXX_LIST}
        PARENT_SCOPE
      )
  set ( ${header_files}
        ${HPP_LIST}
        PARENT_SCOPE
      )

endfunction ()

function ( setup_resources cur_target res_files )

  if ( NOT EXISTS ${TEST_RESOURCES} )
    add_custom_command ( TARGET ${cur_target} 
                         POST_BUILD
                         COMMAND mkdir ${TEST_RESOURCES}
                         COMMAND echo "Built resource directory" 
                       )
  endif ()
  foreach ( res_file ${res_files} )
    string( REGEX MATCH "([a-zA-Z0-9]|_)+\\..*" simple_res ${res_file} )
    message( STATUS "Making resource ${simple_res}}" ) 
    if ( NOT EXISTS ${new_res} )
      add_custom_command ( TARGET ${TARGET_NAME}
                           POST_BUILD
                           COMMAND cp ${res_file} ${TEST_RESOURCES}/
                           COMMAND echo "       Copied ${simple_res} to ${TEST_RESOURCES}"
                         )
    endif ()
  endforeach()
    
endfunction ()


####################################################################################
#
# GTest Vars
#
####################################################################################
set ( GTEST_ROOT     
      ${PROJECT_SOURCE_DIR}/thirdparty/gtest/googletest 
    )
set ( GTEST_LIB      
      ${PROJECT_SOURCE_DIR}/thirdparty/gtest/googletest/libgtest.a 
    )
set ( GTEST_LIB_MAIN 
      ${PROJECT_SOURCE_DIR}/thirdparty/gtest/googletest/libgtest_main.a
    )


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
  if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/gtest )

    #
    # Must be last to allow correct setup of regresiion test
    #
    add_subdirectory ( gtest )
    message ( "Current Tests: " ) 
    foreach (test_file ${TEST_SOURCES} )
      message ( '${test_file}' )
    endforeach ()

    message ( "Resources located in : '${TEST_RESOURCES}'" )

  endif()

endif()


