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

namespace string
{


//**********************************************************************************
//
//  Constructor
//
//  Can use default values for parser or define the comment and scoping characters 
//  the parser will use when evaluating the file. 
//
//**********************************************************************************
Parser::Parser( const char commentChar,
                const char escapeChar,
                const char scopeStartChar,
                const char scopeStopChar   ) 
: commentChar_    ( commentChar    )
, escapeChar_     ( escapeChar     )
, scopeStartChar_ ( scopeStartChar )
, scopeStopChar_  ( scopeStopChar  )
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
//  path is the path ( relative or absolute ) to the file to parse
// 
//  Files are parsed in a pseudo-C-like manner, allowing scoping and unnested 
//  comments
//  The parser does not go beyond scoping of elements, after that it is up to the 
//  application specific implementation 
//
//  return A set of parsed elements
//
//**********************************************************************************
ParseElement_t Parser::ParseFile ( std::string path )
{
  ParseElement_t             mainElem;
  std::vector< std::string > lines;
  std::ifstream              ifFile ( path.c_str() );

  //
  // Read in file lines
  //
  #ifdef DEBUG
    std::cout << "Reading file :" << path << std::endl;
  #endif  

  if ( ifFile.is_open() )
  {
    std::string curLine;

    while ( getline ( ifFile, curLine) )
    {
      lines.push_back (curLine );
    }

    //
    // close file
    //
    ifFile.close();

    std::vector< std::string >::iterator itStart = lines.begin();
    std::vector< std::string >::iterator itEnd   = lines.end();
    int lineIdx = 0;

    ParseLines( itStart, itEnd, lineIdx, mainElem, false);
  }
  else
  {
    std::cerr << "Error at: " << __FILE__ << ":" 
                              << __LINE__ << " unable to open file \"" 
                              << path   << "\"";
  }


  return mainElem;
}

//**********************************************************************************
//
//  Parse lines
//
//  it is the iterator from which the set of lines parsed left off on
//  elem is an instance of an element to which the lines will be written to
//  bIsChild lets the parser know if the current iteration is a child or original
//
//  Recursively parse lines, allowing scoping of child elements under original
//
// return successful parse
//
//**********************************************************************************
bool Parser::ParseLines( std::vector< std::string >::iterator &itStart, 
                         std::vector< std::string >::iterator  itEnd,
                         int                                  &lineIdx,
                         ParseElement_t                       &elem, 
                         bool                                  isChild )
{
  //
  // Used for storing and parsing
  //
  std::string line;

  //
  // Parse lines into elements:
  //  Scan for everything that is not a comment
  // 
  for ( itStart;
        itStart != itEnd;
        itStart++ )
  {
    //
    // Clear all 
    //
    line.clear();

    //
    // Scan lines, iterate across chars
    //
    if ( (*itStart )[ 0 ] == this->commentChar_ ) 
      continue; // skip

    for ( lineIdx; lineIdx < itStart->length(); lineIdx++ )
    {
      if ( ( *itStart )[ lineIdx ] == this->escapeChar_ )
      {
        /// TODO: Limit escaping to parser specific chars 
        if ( itStart->length() != ( lineIdx + 1 ) ) 
        {
          //
          // Escape the next char, treat it as actual
          //
          line += ( *itStart )[ lineIdx + 1 ];
          //
          // Shift over to next char
          //
          lineIdx++;  
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
      else if ( ( *itStart )[ lineIdx ] == this->commentChar_ )
      {
        //
        // Comment, stop evaluating line
        //
        break; 
      } // End Comment Char ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if ( ( *itStart )[ lineIdx ] == this->scopeStartChar_ )
      {
        //
        // We have a new element
        //
        bool            success;
        ParseElement_t  childElem;
        lineIdx++;
        success = ParseLines( itStart, itEnd, lineIdx, childElem, true );

        if ( success )
        {
          elem.children_.push_back( childElem );
        }
        else
        {
          return false;
        }

      } // End Scope Start Char ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      else if ( ( *itStart )[ lineIdx ] == this->scopeStopChar_ )
      {
        //
        // Are we a child or the original?
        //
        if ( isChild )
        {
          //
          // Good! end scope
          //
          if ( !line.empty() && !isWhiteSpace ( line ) )
          {
            elem.elementLines_.push_back( line );
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
        line += ( *itStart )[ lineIdx ];
      }
    } // End line evaluation

    //
    // We reached EOF
    //
    if ( isChild && ( itStart == itEnd ) )
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
    if( !line.empty() && !isWhiteSpace ( line ) )
    {
      elem.elementLines_.push_back( line );
    }

    //
    // Restart line
    //
    lineIdx = 0;
  } // End lines

  //
  // All is good
  //
  return true;
}


} // namespace string

} // namespace graphics