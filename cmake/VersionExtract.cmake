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
##  File    : VersionExtract.cmake
##  Author  : Anthony Islas
##  Purpose : A version extractor
##  Group   : Cmake
##
##
##  License : None
##
####################################################################################

####################################################################################
#
# \brief extracts the version from a file
#
# \return Will place the respective version parts in the provided variables
#
####################################################################################
function( version_extract FILE VERSION_MAJOR VERSION_MINOR VERSION_PATCH )

  set( REGEX_EXPR "([0-9a-z]+)\\.([0-9a-z]+)\\.([0-9a-z]+).*")

  file( STRINGS ${FILE} VERSION_STR LIMIT_COUNT 1 REGEX ${REGEX_EXPR} )

  string( REPLACE "." ";" VERSION_STR ${VERSION_STR} )

  #
  # Get it out
  #
  list( GET VERSION_STR 0 VERSION_MAJOR_NUM )
  list( GET VERSION_STR 1 VERSION_MINOR_NUM )
  list( GET VERSION_STR 2 VERSION_PATCH_NUM )

  set( ${VERSION_MAJOR} ${VERSION_MAJOR_NUM} PARENT_SCOPE )
  set( ${VERSION_MINOR} ${VERSION_MINOR_NUM} PARENT_SCOPE )
  set( ${VERSION_PATCH} ${VERSION_PATCH_NUM} PARENT_SCOPE )

endfunction()

####################################################################################
#
# \brief extracts the version from a file into a string
#
# \return Will place the version into a string 
#
####################################################################################
function( version_extract_str FILE VERSION_STR )

  version_extract( ${FILE} VERSION_MAJOR VERSION_MINOR VERSION_PATCH )
  
  set( ${VERSION_STR} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" PARENT_SCOPE )

endfunction()