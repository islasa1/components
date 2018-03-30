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
//  File    : ParserElem.cpp
//  Author  : Anthony Islas
//  Purpose : Implementation of parsing elements
//  Group   : String
//
//  TODO    : None
//
//  License : GNU GPL v3
//
////////////////////////////////////////////////////////////////////////////////////

#include "ParserElem.hpp"

#include <iostream>

namespace components
{

namespace string
{

  
///**********************************************************************************
///
///  \function ParserElem::ParserElem
///
///  \brief    ctor
///
///**********************************************************************************
ParserElem::ParserElem( )
: name_    ( "main" )
, children_(        )
{} // ParserElem::ParserElem


///**********************************************************************************
///
///  \function ParserElem::~ParserElem
///
///  \brief    dtor
///
///**********************************************************************************
ParserElem::~ParserElem( )
{} // ParserElem::~ParserElem


///**********************************************************************************
///
///  \function ParserElem::print
///
///  \brief    Prints out the elements with indentation
///
///  \return   void - its a printer...
///
///**********************************************************************************
void 
ParserElem::print(
                  std::string indent
                  )
{

  std::cout << indent << "ELEMENT : " << name_ << std::endl;
  indent += "  ";

  for ( std::vector< std::string >::iterator it = elementLines_.begin(); 
        it != elementLines_.end(); 
        it++ 
        )
  {
    
    std::cout << indent << *it << std::endl;

  }

  //
  // Now print all children
  //
  for ( std::vector< ParserElem >::iterator childIt = children_.begin(); 
        childIt != children_.end(); 
        childIt++ 
        )
  {
    childIt->print( indent );
  }

} // ParserElem::print


///**********************************************************************************
///
///  \function ParserElem::totalElems
///
///  \brief    Returns total number of elements, including children
///
///  \return   unsigned int - number of elems
///
///**********************************************************************************
unsigned int 
ParserElem::totalElems( )
{
  
  unsigned int numElems = 0;

  numElems += elementLines_.size();

  for ( 
        std::vector< ParserElem >::iterator it = children_.begin(); 
        it != children_.end(); 
        ++it )
  {
    numElems += it->totalElems();
  }

  return numElems;

} // ParserElem::totalElems


} // namespace string


} // namespace components