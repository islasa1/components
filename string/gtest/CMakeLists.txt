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

set ( LOCAL_TEST_SOURCES 
      ${CMAKE_CURRENT_SOURCE_DIR}/ParserTest.cpp
      )


set ( LOCAL_TEST_INCLUDES 
      ${CMAKE_CURRENT_SOURCE_DIR}/.. 
      )

####################################################################################
#
# Pass up
#
####################################################################################
set ( TEST_SOURCES  ${TEST_SOURCES}  ${LOCAL_TEST_SOURCES}  PARENT_SCOPE )
set ( TEST_INCLUDES ${TEST_INCLUDES} ${LOCAL_TEST_INCLUDES} PARENT_SCOPE )


message ( "Configured " ${TARGET_NAME} )
message ( "Local Tests: " ${LOCAL_TEST_SOURCES} )