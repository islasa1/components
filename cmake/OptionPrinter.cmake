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
##  File    : OptionPrinter.cmake
##  Author  : Anthony Islas
##  Purpose : Fancy color option printer for CMake
##  Group   : Cmake
##
##  TODO    : None
##
##  License : None
##
####################################################################################

if ( OPTIONPRINTER_CMAKE ) 
  
  return()

endif()

set( OPTIONPRINTER_CMAKE 1 )



if ( NOT WIN32 )

  string( ASCII 27 ESC )

  set( CLR_RESET       "${ESC}[m"     )
  set( CLR_BOLD        "${ESC}[1m"    )
  set( CLR_RED         "${ESC}[31m"   )
  set( CLR_GREEN       "${ESC}[32m"   )
  set( CLR_YELLOW      "${ESC}[33m"   )
  set( CLR_BLUE        "${ESC}[34m"   )
  set( CLR_MAGENTA     "${ESC}[35m"   )
  set( CLR_CYAN        "${ESC}[36m"   )
  set( CLR_WHITE       "${ESC}[37m"   )
  set( CLR_BOLDRED     "${ESC}[1;31m" )
  set( CLR_BOLDGREEN   "${ESC}[1;32m" )
  set( CLR_BOLDYELLOW  "${ESC}[1;33m" )
  set( CLR_BOLDBLUE    "${ESC}[1;34m" )
  set( CLR_BOLDMAGENTA "${ESC}[1;35m" )
  set( CLR_BOLDCYAN    "${ESC}[1;36m" )
  set( CLR_BOLDWHITE   "${ESC}[1;37m" )

endif()





####################################################################################
#
# padder
#
####################################################################################
function( pad_string OUTSTR INSTR NUM FILL JUSTIFICATION )

  string( TOLOWER ${JUSTIFICATION} JUSTIFY )

  if ( NOT( ${JUSTIFY} STREQUAL "left" OR ${JUSTIFY} STREQUAL "right" ) )

    set( JUSTIFY "left" )

  endif()

  string( LENGTH "${INSTR}" STR_LEN ) 

  math( EXPR NUMPADS "${NUM} - ${STR_LEN} - 1" )
  set( PAD_STR ${INSTR} )

  if ( ${NUMPADS} GREATER -1 )

    foreach( IT RANGE ${NUMPADS} )

      if ( ${JUSTIFY} STREQUAL "left" )

        set( PAD_STR "${PAD_STR}${FILL}" )

      elseif( ${JUSTIFY} STREQUAL "right" )

        set( PAD_STR "${FILL}${PAD_STR}" )

      endif()

    endforeach()

  endif()

  set( ${OUTSTR} ${PAD_STR} PARENT_SCOPE )

endfunction()






function( detailed_print KEY KEY_CLR VALUE VALUE_CLR NUM JUSTIFY )
  
  pad_string( KEY   ${KEY}   ${NUM} " " ${JUSTIFY} )

  message( "${${KEY_CLR}}${KEY} : ${${VALUE_CLR}}${VALUE}${CLR_RESET}" )

endfunction()







function ( print_include_dirs target)

  message( "Listing search paths..." )
  get_target_property ( 
                        dirs 
                        ${target} 
                        INCLUDE_DIRECTORIES
                      )
  foreach( dir ${dirs} )
    message( STATUS "${CLR_GREEN}'${dir}'${CLR_RESET}" )
  endforeach()

endfunction ()
