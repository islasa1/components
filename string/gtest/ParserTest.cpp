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
//  File    : ParserTests.cpp
//  Author  : Anthony Islas
//  Purpose : Unit test for generic parser
//  Group   : Components Unit Tests
//
//  TODO    : None
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "Parser.hpp"
#include "config.hpp"

namespace components
{

namespace string
{



TEST( ComponentsTestsParser, StandardParse )
{
  Parser     parser;
  ParserElem elem;
  std::cout << "Opening file \'" << TEST_RESOURCES
            << "template.gsf\' " << std::endl;
  std::string filename( std::string ( TEST_RESOURCES ) + "template.gsf" );
  elem = parser.ParseFile( filename );

  ASSERT_NE( elem.totalElems(), 0 );

  elem.print();
  
}

TEST( ComponentsTestsParser, ModifiedParse )
{
  //
  // Not how you should parse, but tests dynamism of this parser
  //
  Parser     parser( '%', '\\', '[', ']' );
  ParserElem elem;
  std::cout << "Opening file \'"      << TEST_RESOURCES
            << "fake_template.gsf\' " << std::endl;
  std::string filename( std::string ( TEST_RESOURCES ) + "fake_template.gsf" );
  elem = parser.ParseFile( filename );

  ASSERT_NE( elem.totalElems(), 0 );

  elem.print();

}





} // namespace string

} // namespace components