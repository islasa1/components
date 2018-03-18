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

namespace components
{


namespace string
{


//
// Parse Elements
//
typedef struct ParseElementStructure
{

  ParseElementStructure() 
  : name_( "main" )
  , children_   (        )
  {}

  //
  // Name of current element
  //
  std::string name_;
  std::vector< std::string > elementLines_;

  //
  // Immediate children
  //
  std::vector< ParseElementStructure > children_;

  void print( std::string indent = "" )
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
    for ( std::vector< ParseElementStructure >::iterator childIt = children_.begin(); 
          childIt != children_.end(); 
          childIt++ 
          )
    {
      childIt->print( indent );
    }

  }

} ParseElement_t; 



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

  ParseElement_t ParseFile ( std::string path );

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
  bool ParseLines( std::vector< std::string >::iterator &itStart, 
                   std::vector< std::string >::iterator  itEnd,
                   int                                  &lineIdx,
                   ParseElement_t                       &elem, 
                   bool                                  isChild );


};



} // namespace string

} // namespace components

#endif
