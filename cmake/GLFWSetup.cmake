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
##  File    : GoogleTestSetup.cmake
##  Author  : Anthony Islas
##  Purpose : A set of useful functions to setup GLFW
##  Group   : cmake
##
##  License : GNU GPL v3
##
####################################################################################


if ( GLFWSETUP_CMAKE )

  return()

endif()

set( GLFWSETUP_CMAKE 1 )


####################################################################################
#
# GTest Vars
#
####################################################################################
if ( THIRDPARTY )

  set ( GLFW_ROOT     
        ${THIRDPARTY}/glfw3
      )

else()

  message( FATAL_ERROR "No thirdparty provided" )

endif()




####################################################################################
#
# Setup
#
####################################################################################
if ( GLFW_GRAPHICS )
  
  # Include if we haven't already
  include( ExternalProject )

  if ( NOT BUILD_THIRDPARTY )

    set( DISABLE_BUILD    BUILD_COMMAND    ${CMAKE_COMMAND} -E echo_append )
    set( DISABLE_DOWNLOAD DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E echo_append )

  endif() 

  ExternalProject_Add( 
                      # Target name
                      glfw
                      # Turn off certain features
                      UPDATE_COMMAND      ""
                      ${DISABLE_DOWNLOAD}
                      ${DISABLE_BUILD}
                      # Dowload step
                      DOWNLOAD_DIR        ${GLFW_ROOT}/
                      GIT_REPOSITORY      https://github.com/glfw/glfw
                      GIT_TAG master
                      # Configure step
                      SOURCE_DIR          ${GLFW_ROOT}
                      BINARY_DIR          ${THIRDPARTY}/_build/
                      # Install step - kinda dont like it being GLFW but ehh
                      # INSTALL_COMMAND     bash -c "mkdir ${GLFW_ROOT}/include/glfw3 -p"
                      # COMMAND             bash -c "cp ${GLFW_ROOT}/include/* ${GLFW_ROOT}/include/glfw3"
                      CMAKE_ARGS          -DBUILD_SHARED_LIBS:BOOL=ON
                                          -DGLFW_BUILD_EXAMPLES:BOOL=ON
                                          -DCMAKE_INSTALL_PREFIX:STRING=${GLFW_ROOT}
                      )

endif()


#
# Now
# 
# Not sure how it's supposed to "find" glfw
# find_package( PkgConfig REQUIRED )

# pkg_search_module( GLFW REQUIRED glfw3 )
find_library( GLFW_LIBS glfw ${GLFW_ROOT}/lib )
