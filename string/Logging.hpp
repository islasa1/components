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
///
///
///  \File    : Logging.hpp
///  \Author  : Anthony Islas
///  \Purpose : Generic way of logging
///  \Group   : String
///
///  \todo    : None
///
///  \License : GNU GPL v3
///
////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#if DEBUG

#define LOG_DEBUG( ... ) if ( 1 ) std::cout << "DEBUG: "  << __VA_ARGS__ << std::endl;

#else

#define LOG_DEBUG( ... ) 

#endif


namespace components
{

namespace string
{

#define LOG_WARNING( ... ) std::cout << "WARNING: " << __VA_ARGS__ << std::endl;
#define LOG_ERROR( ... )   std::cerr << "ERROR  : " << __PRETTY_FUNCTION__ << ":" << __LINE__ << " : " << __VA_ARGS__ << std::endl;

} // namespace string

} // namespace components