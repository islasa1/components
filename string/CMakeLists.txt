######################################################################################
##                                                                                  ##
##     _____    ____ _       ____ _        __ _      __ _  __ _  ______ _   ___ _   ##
##    / /| |]  |  __ \\     / ___ \\      / \ \\    |   \\/   |]|  _____|] / ___|]  ##
##   / //| |]  | |] \ \\   | |]  \_|]    / //\ \\   | |\ / /| |]| |]___ _ ( ((_ _   ##
##  / //_| |]_ | |]  ) ))  | |]  __ _   / _____ \\  | |]\_/ | |]|  _____|] \___ \\  ##
## |_____   _|]| |]_/ //   | |]__/  |] / //    \ \\ | |]    | |]| |]___ _   ___) )) ##
##       |_|]  |_____//     \_____/|]]/_//      \_\\|_|]    |_|]|_______|] |____//  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
##
##
##  File    : CMakeLists.txt
##  Author  : Anthony Islas
##  Purpose : String module files
##  Group   : String
##
##
##  License : GNU GPL v3
##
####################################################################################

#
# We are creating a target for this directory
#
set        (   TARGET ${PROJECT_NAME}String )
add_library( ${TARGET} SHARED ""     )


#
# Includes
#
target_include_directories( ${TARGET} 
                            PUBLIC
                              ${CMAKE_CURRENT_SOURCE_DIR}
                            )

#
# Sources
#
target_sources( ${TARGET}
                PRIVATE 
                  ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/ParserElem.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/strutils.cpp 
                  )



####################################################################################
#
# UNIT TESTS
#
####################################################################################
if ( UNIT_TEST )

  if ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/gtest )

    message( STATUS "No tests to build for ${TARGET}" )

    return()

  else()

  add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/gtest )  
    
  create_gtest( 
                TARGET 
                  ${TARGET}Tests

                INSTALL
                  ${RUNTIME_INSTALL}

                SOURCES
                  ${TEST_SOURCES}

                INSTALL_RPATH
                  ${LIBRARY_INSTALL}

                LIBS
                  ${TARGET}

                INCLUDES
                  ${CMAKE_CURRENT_SOURCE_DIR}

                RESOURCES
                  ${RESOURCE_FILES}

                PRINT_INC_DIR FALSE
                )

  endif()

endif()




####################################################################################
#
# Install rules
#
####################################################################################
install( TARGETS ${TARGET} 
         RUNTIME DESTINATION ${RUNTIME_INSTALL}
         LIBRARY DESTINATION ${LIBRARY_INSTALL}
         )