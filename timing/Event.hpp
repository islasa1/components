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

#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

namespace components
{

namespace timing
{
  
class Event
{
public:
  Event();

  ~Event();

  template< typename F, typename ... Args >
  Event& setCallback( F&& f, Args ... args );
  void   callback   (                      );


  void enable () { enabled_ = true;  }
  void disable() { enabled_ = false; }
  bool enabled() { return enabled_;  }

  bool   addSubevent   ( Event *e, std::string tag );
  bool   removeSubevent( std::string tag );
  Event* getSubevent   ( std::string tag );

  static
  void 
  print( const Event& e, std::string indent = "" );

private:

  std::function< void () > callback_;
  bool                     enabled_;
  std::string              tag_;

  // subevents
  std::vector< Event * >                      subevents_;     ///< pointer to map events
  std::unordered_map< std::string, Event * >  tagToSubevent_; ///< actual subevents

};


///**********************************************************************************
///
///  \function Event::setCallback
///
///  \brief    Sets the event callback using any arguments provided to call it when
///            ( *this )->callback() is used. The callback will be treated as a 
///            void (*)( ) function
///
///  \return   Event& - reference to ( *this ) for inline use elsewhere
///
///**********************************************************************************
template< typename F, typename ... Args >
Event& 
Event::setCallback(
                   F&&      f,
                   Args ... args
                   )
{

  callback_ = std::bind( f, args ... );

  return ( *this );

} // Event::setCallback


} // namespace timing

} // namespace components

#endif // __TIMING_EVENT_H__