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
//  File    : Clock.hpp
//  Author  : Anthony Islas
//  Purpose : Generic Clock class creates a single event ( tick ) when registered 
//            resources are ready to update, can be placed in a certain order
//            to control pipeline / dataflow for resources tied to a similar clock
//           
//            Supports up to a 100Hz clock with one resources to manage ( ordering 
//            resources can cause delays ). Clock can go beyond that, but becomes 
//            unreliable and will not reflect expected frequency. Max seen frequency
//            seen is around 2125 Hz, but with current implementation it is somewhere
//            between 650 and 850, scaling more to the lower end
//
//  Group   : Timing
//
//  TODO    : Anthony Islas
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __TIMING_CLOCK_H__
#define __TIMING_CLOCK_H__

#include <functional>
#include <thread>
#include <mutex>
#include <list>

#include "Event.hpp"

namespace components
{

namespace timing
{
  
class Clock
{

public:

  typedef std::pair< Event&, uint > PeriodicEvent;

  Clock();
  ~Clock();

  // optimizing removes the ability to get a measured
  // frequency
  void optimized( bool enable ) { optimized_ = enable; }
  bool optimized(             ) { return optimized_;   }
  
  //
  // Basic Functionality
  //
  bool start( );
  bool stop ( );
  bool pause( );

  //
  // Statistics
  //
  float measuredFrequency() { return frequency_; } // This requires the clock to run
  float expectedFrequency() { return ( 1000.0 / static_cast< double >( delay_ ) );  }

  //  
  // Status
  //
  bool active()  { return tTimer_.joinable( ) && !done_ && !paused_; }
  bool stopped() { return done_;   }
  bool paused () { return paused_; }

  //
  // Event interaction
  //
  void registerEvent( std::pair< Event&, uint > e, int order = -1 );
  void changeEventRate( std::string tag, uint milli );

private:

  void update( );
  void setPeriods( );

  uint         delay_;
  float        frequency_;

  bool         done_;
  bool         paused_;
  bool         optimized_;

  std::thread  tTimer_;
  std::mutex   mPause_;

  // pair of pair of <Event and rate ( milli )> and 
  //         pair of <number of periods, number of elapsed periods>
  std::list< std::pair< 
                        std::pair< Event&, uint >, 
                        std::pair< uint,   uint > 
                        >
              >::iterator itEvent_;

  std::list< std::pair< 
                        std::pair< Event&, uint >, 
                        std::pair< uint,   uint > 
                        >           
              > events_;


};

} // namespace timing

} // namespace components

#endif // __TIMING_CLOCK_H__