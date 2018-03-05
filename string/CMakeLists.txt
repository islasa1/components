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
##  Purpose : String module files
##  Group   : String
##
##
##  License : None
##
####################################################################################


set( STRING_TARGETS "" )

#
# We are creating a target for this directory
#
set        (   TARGET KomorebiString )
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
                  ${CMAKE_CURRENT_SOURCE_DIR}/strutils.cpp 
                  )

#
# Add to targets
#
set( STRING_TARGETS 
    ${STRING_TARGETS} 
    ${TARGET} 
    )


if ( UNIT_TEST )

  if ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/gtest )

    message( STATUS "No tests to build for ${TARGET}" )

    return()

  else()

    add_subdirectory ( ${CMAKE_CURRENT_SOURCE_DIR}/gtest )


    ####################################################################################
    #
    # Automate naming of target
    #
    ####################################################################################
    get_filename_component ( TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE )
    string ( REPLACE " " "_" TARGET_NAME ${TARGET_NAME}Test)
    string ( REGEX MATCH "[0-9a-zA-Z]+Test" TARGET_NAME ${TARGET_NAME} )

    message ( "Target Name: " ${TARGET_NAME} )

    #
    # Setup 
    #
    set ( RESOURCE_FILES 
          ${CMAKE_CURRENT_SOURCE_DIR}/gtest/template.gsf
          ${CMAKE_CURRENT_SOURCE_DIR}/gtest/fake_template.gsf
        )

    ####################################################################################
    #
    # executable
    #
    ####################################################################################
    add_executable ( ${TARGET_NAME}  ${TEST_SOURCES} )

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
    target_include_directories ( ${TARGET_NAME} 
                                  PUBLIC 
                                  ${TEST_INCLUDES}
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
          KomorebiString
          )

    target_link_libraries( ${TARGET_NAME} ${LIBS} )


    #
    # RPATH
    #
    set_target_properties( ${TARGET_NAME}
                           PROPERTIES
                             INSTALL_RPATH ${PROJECT_SOURCE_DIR}/lib )


    #
    # Add to targets
    #
    set( STRING_TARGETS 
        ${STRING_TARGETS} 
        ${TARGET_NAME} 
        )

  endif()

endif()




####################################################################################
#
# Install rules
#
####################################################################################
install( TARGETS ${STRING_TARGETS} 
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
         )