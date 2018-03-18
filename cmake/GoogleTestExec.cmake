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
##  File    : GoogleTestExec.cmake
##  Author  : Anthony Islas
##  Purpose : Generic creation of a google test executable
##  Group   : Cmake
##
##  TODO    : None
##
##  License : GNU GPL v3
##
####################################################################################

include( GoogleTestSetup )

####################################################################################
#
# Automate naming of target
#
####################################################################################
auto_test_name( ${TEST_DIR} OPT_DIR GTEST_NAME )
set           ( GTEST_TARGET "${PROJECT_NAME}${GTEST_NAME}Tests" )

message ( "Target Name: " ${GTEST_TARGET} )


####################################################################################
#
# executable
#
####################################################################################
add_executable ( ${GTEST_TARGET}  ${TEST_SOURCES} )

#
# Depends on thirdparty target ( added by setup module )
#
add_dependencies( ${GTEST_TARGET} gtest )


####################################################################################
#
# Define test cases for each
#
####################################################################################



####################################################################################
#
# Resources necessary to run these tests
#
####################################################################################
setup_resources (${GTEST_TARGET} "${${RESOURCE_FILES}}" )

####################################################################################
#
# Add GTest includes
#
####################################################################################
target_include_directories ( ${GTEST_TARGET} 
                              PUBLIC 
                              ${${GTEST_TEST_INCLUDES}}
                              ${GTEST_ROOT}/include 
                              )

print_include_dirs ( ${GTEST_TARGET} )

####################################################################################
#
# Set default main and gtest libraries
#
####################################################################################
set ( GTEST_LIBS 
      ${GTEST_LIB}
      ${GTEST_LIB_MAIN}
      ${CMAKE_THREAD_LIBS_INIT}
      ${PROJECT_LIBS}
      )

target_link_libraries( ${GTEST_TARGET} ${LIBS} )


#
# RPATH
#
set_target_properties( ${GTEST_TARGET}
                       PROPERTIES
                         INSTALL_RPATH ${GTEST_RPATH} )



