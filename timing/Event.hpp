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
//  File    : Event.hpp
//  Author  : Anthony Islas
//  Purpose : Generic Event class to wrap std::bind and std::function usage to 
//            create callbacks for any type signature
//  Group   : Timing
//
//  TODO    : Anthony Islas
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TIMING_EVENT_H__
#define __TIMING_EVENT_H__

#include <fucntional>
#include <template>

namespace components
{

namespace timing
{
  
class Event
{
public:
  Event()
  {}

  ~Event()
  {}

  template< typename F, typename ... Args >
  void setCallback( F&& f, args ... )
  {
    callback_ = std::bind( f, args ... );
  }

  void callback() { callback_(); }

private:

  std::function< void () > callback_;
  
};


} // namespace timing

} // namespace components

#endif // __TIMING_EVENT_H__