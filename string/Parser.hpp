//////////////////////////////////////////////////////////////////////////////////////
//                                                                                  //
//     _____    ____ _       ____ _        __ _      __ _  __ _  ______ _   ___ _   //
//    / /| |]  |  __ \\     / ___ \\      / \ \\    |   \\/   |]|  _____|] / ___|]  //
//   / //| |]  | |] \ \\   | |]  \_|]    / //\ \\   | |\ / /| |]| |]___ _ ( ((_ _   //
//  / //_| |]_ | |]  ) ))  | |]  __ _   / _____ \\  | |]\_/ | |]|  _____|] \___ \\  //
// |_____   _|]| |]_/ //   | |]__/  |] / //    \ \\ | |]    | |]| |]___ _   ___) )) //
//       |_|]  |_____//     \_____/|]]/_//      \_\\|_|]    |_|]|_______|] |____//  //
//                                                                                  //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////
//
//
//  File    : Parser.hpp
//  Author  : Anthony Islas
//  Purpose : A C-like generic parser
//  Group   : Components
//
//  TODO    : None
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __COMPONENTS_PARSER_H__
#define __COMPONENTS_PARSER_H__

#include <vector>
#include <string>

#include "ParserElem.hpp"

namespace components
{

namespace string
{

class ParserElem;

//
// C-Like parser
//
class Parser
{
  
public:
  //
  // Constructor
  //
  Parser( const char commentChar    = '#',
          const char escapeChar     = '\\',
          const char scopeStartChar = '{',
          const char scopeStopChar  = '}' );

  virtual ~Parser( );

  ParserElem parseFile ( std::string path );

private:
  //
  // Comment character that tells us to ignore the rest of the line
  //
  const char commentChar_;

  //
  // Escape character that lets us use all these characters regularly
  //
  const char escapeChar_;

  //
  // Scope character to tell us the start of an element (usually {)
  //
  const char scopeStartChar_;

  //
  // Scopre character to tell us the end of an element (usually })
  //
  const char scopeStopChar_;

  //
  // Internal recursive function to scope elements
  //
  bool parseLines( std::vector< std::string >::iterator &itStart, 
                   std::vector< std::string >::iterator  itEnd,
                   unsigned int                         &lineIdx,
                   ParserElem                           &elem, 
                   bool                                  isChild );


};



} // namespace string

} // namespace components

#endif
