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
//
//            Clocks may be synchronized with other Clocks' start, stop, and pause functions
//            while also having independent control, with *Unique() functions. Only when 
//            the group is running will individual control work, otherwise group control 
//            has precedence. Unique Pause must be undone by unqiue start
//            The table below demonstrates the functionality of the Clock when synced
//            
//            ___________________________________________  STATES : Upper is Group, Lower is THIS
//            |   |             |STATE Group -> This    |    R = Running
//            |   |             |_______________________|    X = Stopped
//            |   |             | Sync Group            |    P = Paused
//            |   |  CALLS      |  Run  |  Stop | Pause |    t = Depends on THIS state
//            |___|_____________|_______|_______|_______|___ - = No change in state 
//            |   |             |       |       |       | S
//            | T | Start       |   t   |   R   |   R   | Y
//            | H | Stop        |   X   |   -   |   X   | N
//            | I | Pause       |   P   |   X   |   -   | C
//            | S |             |       |       |       |__
//            |   | StartUnique |  Rr   |  Xx   |  Pr   | T
//            |   | StopUnique  |  Rx   |  Xx   |  Px   | H
//            |   | PauseUnique |  Rp   |  Xx   |  Pp   | I
//            |   |             |       |       |       | S
//            |___|_____________|_______|_______|_______|__ 
//            Note : When not synced all functions do the unique functions
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
  
  static bool synchronize ( std::vector< Clock& > vClocks );
  static bool desyncronize( std::vector< Clock& > vClocks );

  //
  // Can be synchronized with other Clocks
  //
  bool start( );
  bool stop( );
  bool pause( );

  //
  // Cannot be synchronized
  //
  bool startUnique( );
  bool stopUnique( );
  bool pauseUnique( );

  void setFrequency( double fp64Freq );
  void setTimeMillis( unsigned int ui32Delay );
  void setTimeNano  ( unsigned int ui32Delay );

  void setPipeline() { bPipelined_ = true;  }
  void setParallel() { bPipelined_ = false; }

  void registerResource( Event& rEvent, unsigned int ui32Order = -1 )

  //
  // Synchronize starts, stops & pauses
  //


private:

  void update( );

  unsigned int ui32Delay_;

  std::thread             t_Timer_;

  std::shared_ptr< std::mutex >              m_spPauseSync_;
  std::shared_ptr< std::condition_variable > cv_spPauseSync_;

  std::shared_ptr< std::mutex >              m_spPauseUnique_;
  std::shared_ptr< std::condition_variable > cv_spPauseUnique_;

  bool bDone_;
  bool bPipelined_;

  std::list< Event& >::iterator itEvent_;
  std::list< Event& > lEvents_;


};

} // namespace timing

} // namespace components

#endif // __TIMING_CLOCK_H__