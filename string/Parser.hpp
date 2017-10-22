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

//
// Parse Elements
//
typedef struct sParseElementStructure
{
  //
  // Lines that are not part of internal elements
  //
  std::vector< std::string > vElementLines;

  //
  // Nested elements
  //
  std::vector< struct sParseElementStructure > vChildren;

} sParseElement_t; 

//
// C-Like parser
//
class Parser
{
  private:
    //
    // Comment character that tells us to ignore the rest of the line
    //
    const char cCommentChar;

    //
    // Escape character that lets us use all these characters regularly
    //
    const char cEscapeChar;

    //
    // Scope character to tell us the start of an element (usually {)
    //
    const char cScopeStartChar;

    //
    // Scopre character to tell us the end of an element (usually })
    //
    const char cScopeStopChar;

    //
    // Internal recursive function to scope elements
    //
    bool ParseLines( std::vector< std::string >::iterator& itStart, 
                     std::vector< std::string >::iterator itEnd,
                     int& iLineIdx,
                     sParseElement_t& sElem, 
                     bool bIsChild );

  public:
    //
    // Constructor
    //
    Parser( const char cCommentChar    = '#',
            const char cEscapeChar     = '\\',
            const char cScopeStartChar = '{',
            const char cScopeStopChar  = '}' );

    virtual ~Parser( );

    sParseElement_t ParseFile ( std::string ssPath );
};


} // namespace components

#endif
