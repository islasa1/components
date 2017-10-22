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


#include <iostream>
#include <fstream>
#include <string>

#include "strutils.hpp"
#include "Parser.hpp"

namespace components
{

//**********************************************************************************
//
//  Constructor
//
//  Can use default values for parser or define the comment and scoping characters 
//  the parser will use when evaluating the file. 
//
//**********************************************************************************
Parser::Parser( const char cCommentChar,
                const char cEscapeChar,
                const char cScopeStartChar,
                const char cScopeStopChar         ) : 
                cCommentChar    ( cCommentChar    ),
                cEscapeChar     ( cEscapeChar     ),
                cScopeStartChar ( cScopeStartChar ),
                cScopeStopChar  ( cScopeStopChar  )
{ };


//**********************************************************************************
//
// Destructor
//
//**********************************************************************************
Parser::~Parser( ) { };


//**********************************************************************************
//
//  File parser
//
//  ssPath is the path ( relative or absolute ) to the file to parse
// 
//  Files are parsed in a pseudo-C-like manner, allowing scoping and unnested 
//  comments
//  The parser does not go beyond scoping of elements, after that it is up to the 
//  application specific implementation 
//
//  return A set of parsed elements
//
//**********************************************************************************
sParseElement_t Parser::ParseFile ( std::string ssPath )
{
  sParseElement_t            sMainElem;
  std::vector< std::string > vLines;
  std::ifstream              ifFile ( ssPath.c_str() );

  //
  // Read in file lines
  //
  #ifdef DEBUG
    std::cout << "Reading file :" << ssPath << std::endl;
  #endif  

  if ( ifFile.is_open() )
  {
    std::string ssCurLine;

    while ( getline ( ifFile, ssCurLine) )
    {
      vLines.push_back (ssCurLine );
    }

    //
    // close file
    //
    ifFile.close();

    std::vector< std::string >::iterator itStart = vLines.begin();
    std::vector< std::string >::iterator itEnd   = vLines.end();
    int iLineIdx = 0;

    ParseLines( itStart, itEnd, iLineIdx, sMainElem, false);
  }
  else
  {
    std::cerr << "Error at: " << __FILE__ << ":" 
                              << __LINE__ << " unable to open file \"" 
                              << ssPath   << "\"";
  }


  return sMainElem;
}

//**********************************************************************************
//
//  Parse lines
//
//  it is the iterator from which the set of lines parsed left off on
//  sElem is an instance of an element to which the lines will be written to
//  bIsChild lets the parser know if the current iteration is a child or original
//
//  Recursively parse lines, allowing scoping of child elements under original
//
// return successful parse
//
//**********************************************************************************
bool Parser::ParseLines( std::vector< std::string >::iterator& itStart, 
                         std::vector< std::string >::iterator itEnd,
                         int& iLineIdx,
                         sParseElement_t& sElem, 
                         bool bIsChild )
{
  //
  // Used for storing and parsing
  //
  std::string ssLine;

  //
  // Parse lines into elements:
  //  Scan for everything that is not a comment
  // 
  for ( itStart ;
        itStart != itEnd;
        itStart++ )
  {
    //
    // Clear all 
    //
    ssLine.clear();

    //
    // Scan lines, iterate across chars
    //
    if ( (*itStart )[0] == this->cCommentChar ) 
      continue; // skip

    for ( iLineIdx; iLineIdx < itStart->length(); iLineIdx++ )
    {
      if ( ( *itStart )[iLineIdx] == this->cEscapeChar )
      {
        /// TODO: Limit escaping to parser specific chars 
        if ( itStart->length() != (iLineIdx + 1) ) 
        {
          //
          // Escape the next char, treat it as actual
          //
          ssLine += ( *itStart )[ iLineIdx + 1 ];
          //
          // Shift over to next char
          //
          iLineIdx++;  
        }
        else
        {
          //
          // Try escaping eol (end of line)
          //
          std::cerr << "Error at: " << __FILE__ << ":" 
                                    << __LINE__ << "No char to escape" << std::endl;
          return false;
        }
      } // End Escape char ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if ( ( *itStart )[iLineIdx] == this->cCommentChar )
      {
        //
        // Comment, stop evaluating line
        //
        break; 
      } // End Comment Char ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if ( ( *itStart )[iLineIdx] == this->cScopeStartChar )
      {
        //
        // We have a new element
        //
        bool bSuccess;
        sParseElement_t sChildElem;
        iLineIdx++;
        bSuccess = ParseLines( itStart, itEnd, iLineIdx, sChildElem, true );

        if ( bSuccess )
        {
          sElem.vChildren.push_back( sChildElem );
        }
        else
        {
          return false;
        }

      } // End Scope Start Char ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if ( ( *itStart )[iLineIdx] == this->cScopeStopChar )
      {
        //
        // Are we a child or the original?
        //
        if ( bIsChild )
        {
          //
          // Good! end scope
          //
          if ( !ssLine.empty() && !isWhiteSpace ( ssLine ) )
          {
            sElem.vElementLines.push_back( ssLine );
          }
          return true; 
        }
        else
        {
          //
          // Whooaa, unexpected end-scope
          //
          std::cerr << "Error at: " << __FILE__ << ":" 
                                    << __LINE__ << "unexpected end of scope" 
                                    << std::endl;
          return false;
        }
      } // End Scope End Char ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
      else
      {
        // 
        // Normal character
        //
        ssLine += ( *itStart )[iLineIdx];
      }
    } // End line evaluation

    //
    // We reached EOF
    //
    if ( bIsChild && (itStart == itEnd ) )
    {
      //
      // Never ended scope!
      //
      std::cerr << "Error at: " << __FILE__ << ":" 
                                << __LINE__ << "Expected \'}\' before EOF" 
                                << std::endl;
      return false;
    }
    //
    // Add line to element
    //
    if( !ssLine.empty() && !isWhiteSpace ( ssLine ) )
    {
      sElem.vElementLines.push_back( ssLine );
    }

    //
    // Restart line
    //
    iLineIdx = 0;
  } // End lines

  //
  // All is good
  //
  return true;
}


} // namespace graphics