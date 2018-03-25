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

####################################################################################
#
# Automate naming of target
#
####################################################################################
message ( "Target Name: " ${GTEST_ARGS_TARGET} )


####################################################################################
#
# executable
#
####################################################################################
add_executable ( ${GTEST_ARGS_TARGET}  ${GTEST_ARGS_SOURCES} )

#
# Depends on thirdparty target ( added by setup module )
#
add_dependencies( ${GTEST_ARGS_TARGET} gtest )


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
setup_resources (${GTEST_ARGS_TARGET} "${GTEST_ARGS_RESOURCES}" )

####################################################################################
#
# Add GTest includes
#
####################################################################################
target_include_directories ( ${GTEST_ARGS_TARGET} 
                              PUBLIC 
                              ${GTEST_ARGS_INCLUDES}
                              ${GTEST_ROOT}/include 
                              )

if ( GTEST_ARGS_PRINT_INC_DIR )

  print_include_dirs ( ${GTEST_ARGS_TARGET} )

endif()

####################################################################################
#
# Set default main and gtest libraries
#
####################################################################################
set ( GTEST_LIBS 
      ${GTEST_LIB}
      ${GTEST_LIB_MAIN}
      ${CMAKE_THREAD_LIBS_INIT}
      ${GTEST_ARGS_LIBS}
      )

target_link_libraries( ${GTEST_ARGS_TARGET} ${GTEST_LIBS} )



####################################################################################
#
# Install rules
#
####################################################################################

#
# RPATH
#
set_target_properties( ${GTEST_ARGS_TARGET}
                       PROPERTIES
                         INSTALL_RPATH ${GTEST_ARGS_INSTALL_RPATH} )

install( TARGETS ${GTEST_ARGS_TARGET} 
         RUNTIME DESTINATION ${GTEST_ARGS_INSTALL}
         )