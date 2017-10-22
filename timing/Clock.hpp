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
//  Group   : Timing
//
//  TODO    : Anthony Islas
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __TIMING_CLOCK_H__
#define __TIMING_CLOCK_H__

namespace components
{

namespace timing
{
  
class Clock
{
public:
  Clock();
  ~Clock();
  
  void setFrequency( double fp64Freq );
  void setTimeMilli( double fp64Delay );

  void registerResource( std::mutex& m_Resource, unsigned int order )

private:

  void update( );

  double fp64Delay_;
  double fp64Freq_;

  std::vector< std::mutex& > vMutexResources;


};

} // namespace timing

} // namespace components

#endif // __TIMING_CLOCK_H__