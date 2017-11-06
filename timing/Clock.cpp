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
//  File    : Clock.cpp
//  Author  : Anthony Islas
//  Purpose : Clock implementation
//  Group   : Timing
//
//  TODO    : Anthony Islas
//
//  License : None
//
////////////////////////////////////////////////////////////////////////////////////

#include "Clock.hpp"

#include <cmath>
#include <exception>
#include <thread>
#include <chrono>


namespace components
{
  
namespace timing
{

//**********************************************************************************
//
//  Clock::Clock
//
//  \brief CTOR
//
//  \return Clock
//
//**********************************************************************************
Clock::Clock( ) 
  : ui3264Delay_( 1 )
  , bPipelined_ ( true )
  , done_       ( false )
{} // Clock::Clock

//**********************************************************************************
//
//  Clock::~Clock
//
//  \brief DTOR
//
//  \return none
//
//**********************************************************************************
Clock::~Clock( )
{} // Clock::~Clock

//**********************************************************************************
//
//  Clock::start
//
//  \brief Starts the clock
// 
//  Initializes the clock in a dedicated timing thread
//
//  \return none
//
//**********************************************************************************
void Clock::start( )
{
  
  t_Timer = std::thread( this->update );

} // Clock::start


//**********************************************************************************
//
//  Clock::stop
//
//  \brief Stop clock permanently
// 
//  Stop the timing thread and rejoin to the main thread that spawned this clock
//
//  \return bool
//
//**********************************************************************************
bool Clock::stop( )
{
  
  if ( t_Timer.joinable( ) )
  {
    // Stop loop
    done = true;

    try 
    {
      t_Timer.join( );
    }
    catch
    {
      //\todo AI: Add some logging info and try to diagnose
      return false;
    }

  }
  else
  {
    //\todo AI: Add some logging info here
    return false;
  }
} // Clock::stop



//**********************************************************************************
//
//  Clock::setFrequency
//
//  \brief Set Clock Frequency in Hz
// 
//  \param fp64Freq 
// 
//  Adjust internal clock to a delay closest to achieve desired frequency
//
//  \return none
//
//**********************************************************************************
void Clock::setFrequency( double fp64Freq )
{
  
  ui32Delay_ = static_cast< unsigned int >(
                pow( 
                    fp64Freq, 
                    static_cast< double >( -1 ) 
                    ) 
                * 1e9
              );

} // Clock::setFrequency

//**********************************************************************************
//
//  Clock::setTimeMillis
//
//  \brief Set Clock Delay in Milliseconds
// 
//  \param ui32Delay 
// 
//  Adjust internal clock to given delay with millisecond accuracy
//
//  \return none
//
//**********************************************************************************
void Clock::setTimeMillis( unsigned int ui32Delay )
{
  
  ui32Delay_ = ui32Delay * 1000;

} // Clock::setTimeMillis



//**********************************************************************************
//
//  Clock::setTimeNano
//
//  \brief Set Clock Delay in Nanoseconds
// 
//  \param ui32Delay
//
//  \return none
//
//**********************************************************************************
void Clock::setTimeNano( unsigned int ui32Delay )
{
    
  ui32Delay_ = ui32Delay;

} // Clock::setTimeNano




//**********************************************************************************
//
//  Clock::registerResource
//
//  \brief Register a resource to this particular clock
// 
//  \param m_Resource 
//  \param order
//  
//  Register a particular event to this particular clock, assigning it an order. Should 
//  order conflict, the previous event will be moved back ( adjusting all others 
//  behind as well ).
//
//  \return none
//
//**********************************************************************************
void Clock::registerResource( Event& e, unsigned int order )
{
  if ( 
      order < 0 ||
      order >= lEvents.size()
      )
  {

    lEvents.push_back( e );
  }
  else // Valid order
  {
    lEvents.insert( lEvents.begin() + order, e );
  }

} // Clock::registerResource


//**********************************************************************************
//
//  Clock::update
//
//  \brief Thread running time updates
// 
//  \param clock 
// 
//  Threaded update of resources, sleeping for a given time and then releasing locks
//
//  \return none
//
//**********************************************************************************
void Clock::update( Clock& clock )
{
  std::vector< std::mutex& >::iterator it;
  while ( !done )
  {

    // Iterate across all resources
    for (
          it = vMutexResources_.begin( ); 
          it != vMutexResources_.end( );
          it++
          )
    {
      // Loop until we have the lock
      while ( !it->try_lock( ) );

    }

    std::this_thread::sleep_for( std::chrono::milliseconds( ulDelay_ ) );

    // Iterate across all resources
    for (
          it = vMutexResources_.begin( ); 
          it != vMutexResources_.end( );
          it++
          )
    {
      // Release locks
      it->unlock( );
    }

  }

} // Clock::update




} // namespace timing

} // namespace components