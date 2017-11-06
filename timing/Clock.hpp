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
//  File    : Clock.hpp
//  Author  : Anthony Islas
//  Purpose : Generic Clock class creates a single event ( tick ) when registered 
//            resources are ready to update, can be placed in a certain order
//            to control pipeline / dataflow for resources tied to a similar clock
//            Supports up to a 1000Hz clock with one resources to manage ( ordering 
//            resources can cause delays )
//  Group   : Timing
//
//  TODO    : Anthony Islas
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __TIMING_CLOCK_H__
#define __TIMING_CLOCK_H__

#include <vector>
#include <functional>

class std::thread;

namespace components
{

namespace timing
{
  
class Clock
{
public:
  Clock();
  ~Clock();
  
  void start( );
  void stop( );

  void setFrequency( double fp64Freq );
  void setTimeMillis( unsigned int ui32Delay );
  void setTimeNano  ( unsigned int ui32Delay );

  void setPipeline() { bPipelined_ = true;  }
  void setParallel() { bPipelined_ = false; }

  void registerResource( Event& e, unsigned int order = -1 )

private:

  void update( );

  unsigned int ui32Delay_;

  std::thread t_Timer;

  bool bDone_;
  bool bPipelined_;

  std::vector< Event& > vEvents_;


};

} // namespace timing

} // namespace components

#endif // __TIMING_CLOCK_H__