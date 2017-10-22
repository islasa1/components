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
//  File    : strutils.cpp
//  Author  : Anthony Islas
//  Purpose : A set of string utilities not essential or specific to implementation
//  Group   : Components
//
//  TODO    : None
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#include "strutils.hpp"

namespace components
{

//**********************************************************************************
//
//  Core split function, tokenizing string into parts based on a delimeter 
//
//  cDelim is the character to split on
//  s is a reference to the string to split
//  rvElems is a reference to a vector of strings to fill with the results
//  bRemoveDelim is whether the delimiter should be removed in result (i.e. an 
//  empty split)
//
//  This function is the core process by which the split ( ... ) strutils 
//  function family 
//  operates. The results are split based on the next occurence of cDelim and 
//  stored in a provided vector
//
//  return None
//
//**********************************************************************************
void split( char cDelim,
            const std::string &s,  
            std::vector< std::string > &rvElems, 
            bool bRemoveDelim ) 
{
    std::stringstream ss;
    ss.str(s);
    std::string item;

    while ( getline( ss, item, cDelim ) ) 
    {
        rvElems.push_back(item);
    }
  

  // Remove empty elems or leave it in
  if ( bRemoveDelim )
  {
    for(std::vector< std::string >::iterator it = rvElems.begin(); 
        it != rvElems.end(); 
        it++)
    {
      if( (*it)[0] == cDelim)
      {
        // Remove empty entries
        it = rvElems.erase( it );
      }
    }
  }
}


//**********************************************************************************
//
//  Split a string and return a vector of strings
//
//  cDelim is the character to split on
//  s is a reference to the string to split
//
//  Redirects to split( char, const std::string, std::vector, bool )
//
//  return std::vector containing split results
//
//**********************************************************************************
std::vector< std::string > split( char cDelim,
                                  const std::string &s )  
{
    std::vector< std::string > vElems;
    split( cDelim, s, vElems );
    return vElems;
}

//**********************************************************************************
//
//  Split a series of strings based on delimiter
//
//  cDelim is the character to split on
//  rvElems is a reference to a vector of strings to split and hold results
//
//  This function is iterates across a series of strings, splitting each 
//  individually by the provided delimiter. Redirects to split( char, std::string )
//
//  return None
//
//**********************************************************************************
void split( char cDelim, std::vector< std::string>& rvElems )
{
  std::vector< std::string > vNewElems;
  
  //
  // Iterate across all string elements in the vector to split
  //  
  for( std::vector< std::string >::iterator it = rvElems.begin(); 
       it != rvElems.end(); 
       it++ )
  {
    if( ( *it ) != "" && !( *it ).empty())
    {
      //
      // Split sub elements
      //
      std::vector< std::string > vTemp = split( cDelim, ( *it ) );
      //
      // Insert into new vector
      //
      for( int i = 0; i < ( int ) vTemp.size(); i++)
      {
        vNewElems.push_back( vTemp[i] );
      } 
    }
  }
    
  rvElems.assign( vNewElems.begin(), vNewElems.end() );
}


//**********************************************************************************
//
//  Split a series of strings based on a series / array of delimiter
//
//  pDelim is an array of characters to split on
//  s is the string to split 
//
//  This function is iterates across multiple delimiters, splitting a string
//  Redirects to split( char, std::vector )
//
//  return std::vector containing split strings
//
//**********************************************************************************
std::vector< std::string > split( char pDelims[],
                                  const std::string &s ) 
{
  std::vector< std::string > vElems;
  int i = 0;

  //
  // Split on first char
  //
  if( vElems.empty() )
  {
      split(pDelims[0], s, vElems);
  }

  //
  // Now that vElems is populated we can iterate across it and split
  //
  if( strlen( pDelims ) > 1)
  {
      for( i = 1; i < ( int ) strlen( pDelims ); i++)
      {
         split( pDelims[i], vElems);
      }
  }

  return vElems;
}

//**********************************************************************************
//
//  Checks if a string is composed entirely of whitespace
//
//  s is the string to check
//
//  This function iterates through the string to check if it is only whitespace
//
//  return bool true if only whitespace
//
//**********************************************************************************
bool isWhiteSpace( std::string s )
{
  return ( s.find_first_not_of( " \t\n\r" ) == std::string::npos );
}

} // namespace components