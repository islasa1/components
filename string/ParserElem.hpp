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
//  File    : ParserElem.hpp
//  Author  : Anthony Islas
//  Purpose : Storage container for parsed elements
//  Group   : String
//
//  TODO    : None
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __STRING_PARSERELEM_H__
#define __STRING_PARSERELEM_H__

#include <string>
#include <vector>

namespace components
{

namespace string
{
  

class ParserElem 
{

public:
  
  ParserElem();
  ~ParserElem();

  void          print( std::string indent = "" );
  unsigned int  totalElems();

private:
  //
  // Name of current element
  //
  std::string name_;
  std::vector< std::string > elementLines_;

  //
  // Immediate children
  //
  std::vector< ParserElem > children_;

  //
  // The parser should be the only one allowed to fill us up
  //
  friend class Parser;

};


} // namespace string


} // namespace components

 
#endif // __STRING_PARSERELEM_H__
