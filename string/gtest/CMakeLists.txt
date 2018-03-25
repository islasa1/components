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
# Pass up
#
####################################################################################
set (   TEST_SOURCES  
      ${TEST_SOURCES}  
      # Put test sources here
      ${CMAKE_CURRENT_SOURCE_DIR}/ParserTest.cpp

        PARENT_SCOPE 
        )

set (   TEST_INCLUDES 
      ${TEST_INCLUDES} 
      # Put any specific includes here
      ${CMAKE_CURRENT_SOURCE_DIR}/.. 

        PARENT_SCOPE )

set ( RESOURCE_FILES
      ${RESOURCE_FILES}
      # Put any specific resource files required by tests here
      ${CMAKE_CURRENT_SOURCE_DIR}/template.gsf
      ${CMAKE_CURRENT_SOURCE_DIR}/fake_template.gsf


      PARENT_SCOPE
      )