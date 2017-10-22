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
##  TODO    : None
##
##  License : None
##
####################################################################################

####################################################################################
#
# Automate naming of target
#
####################################################################################
get_filename_component ( TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE )
string ( REPLACE " " "_" TARGET_NAME ${TARGET_NAME}Test)
string ( REGEX MATCH "[0-9a-zA-Z]+Test" TARGET_NAME ${TARGET_NAME} )

message ( "Target Name: " ${TARGET_NAME} )

####################################################################################
#
# Test Sources & Resources
#
####################################################################################
set ( LOCAL_TEST_SOURCES 
      ${CMAKE_CURRENT_SOURCE_DIR}/ParserTest.cpp
    )

set ( TEST_SOURCES
      ${TEST_SOURCES}
      ${LOCAL_TEST_SOURCES}
      PARENT_SCOPE
    )

set ( LOCAL_TEST_INCLUDES 
      ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    )
set ( TEST_INCLUDES
      ${TEST_INCLUDES}
      ${LOCAL_TEST_INCLUDES}
      PARENT_SCOPE
     )
set ( RESOURCE_FILES 
      ${CMAKE_CURRENT_SOURCE_DIR}/../../sprites/template.gsf
      ${CMAKE_CURRENT_SOURCE_DIR}/../../sprites/fake_template.gsf
    )

####################################################################################
#
# Code to test source
#
####################################################################################
get_test_sources ( "${LOCAL_TEST_INCLUDES}" HEADER_FILES SOURCE_FILES)

set ( SOURCES 
      ${HEADER_FILES}
      ${SOURCE_FILES}
      ${LOCAL_TEST_SOURCES}
    )

####################################################################################
#
# Local tests executable
#
####################################################################################
add_executable ( ${TARGET_NAME}  ${SOURCES} )



####################################################################################
#
# Resources necessary to run these tests
#
####################################################################################
setup_resources (${TARGET_NAME} "${RESOURCE_FILES}" )

####################################################################################
#
# Add GTest includes
#
####################################################################################
target_include_directories ( ${TARGET_NAME} PUBLIC 
                             ${LOCAL_TEST_INCLUDES}
                             ${GTEST_ROOT}/include 
                           )
print_include_dirs ( ${TARGET_NAME} )

####################################################################################
#
# Set default main and gtest libraries
#
####################################################################################
set ( LIBS 
      ${GTEST_LIB}
      ${GTEST_LIB_MAIN}
      ${CMAKE_THREAD_LIBS_INIT}
    )


####################################################################################
#
# Local tests executable
#
####################################################################################
target_link_libraries ( ${TARGET_NAME} ${LIBS} )


message ( "Configured " ${TARGET_NAME} )
message ( "Local Tests: " ${LOCAL_TEST_SOURCES} )