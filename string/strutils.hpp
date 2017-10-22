////////////////////////////////////////////////////////////////////////////////////
//
//     _____    ____ _       ____ _        __ _      __ _  __ _  ______ _   ___ _
//    / /| |]  |  __ \\     / ___ \\      / \ \\    |   \\/   |]|  _____|] / ___|]
//   / //| |]  | |] \ \\   | |]  \_|]    / //\ \\   | |\ / /| |]| |]___ _ ( ((_ _
//  / //_| |]_ | |]  ) ))  | |]  __ _   / _____ \\  | |]\_/ | |]|  _____|] \___ \\
// |_____   _|]| |]_/ //   | |]__/  |] / //    \ \\ | |]    | |]| |]___ _   ___) ))
//       |_|]  |_____//     \_____/|]]/_//      \_\\|_|]    |_|]|_______|] |____//
// 
//
////////////////////////////////////////////////////////////////////////////////////
//
//
//  File    : strutils.hpp
//  Author  : Anthony Islas
//  Purpose : A set of string utilities not essential or specific to implementation
//  Group   : Components
//
//  TODO    : None
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __STRUTILS_H__
#define __STRUTILS_H__

#include <string>
#include <cstring>
#include <sstream>
#include <vector>

namespace components
{

void split( char cDelim,
            const std::string &s,  
            std::vector< std::string > &vElems, 
            bool bRemoveDelim = true );
void split( char cDelim, std::vector< std::string>& rvElems );

std::vector< std::string > split( char cDelim,
                                  const std::string &s );
std::vector< std::string > split( char pDelims[],
                                  const std::string &s );
bool isWhiteSpace( std::string s );

} // namespace components

#endif
