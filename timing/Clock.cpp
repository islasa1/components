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
{
  done = true;

  if ( t_Timer_.joinable() )
  {
    t_Timer_.join();
  }

} // Clock::~Clock

//**********************************************************************************
//
//  Clock::synchronize
//
//  \brief Synchronize all clocks in the vector
//  
//  \param vClocks
//   
//  Synchronize the Sync mutexes and conditional variables between all clocks
//
//  \return bool
//
//**********************************************************************************
bool Clock::synchronize( std::vector< Clock& > vClocks )
{
  if ( vClocks.size( ) < 2 ) // 1 or 0 clocks, nothing to sync 
  {
    return true; // Technically synced
  }
  //
  // Grab the first Clock's info
  // 
  std::shared_ptr< std::mutex >                m_sp  = vClocks[ 0 ].m_spPauseSync_;
  std::shared_ptr< std::conditional_variable > cv_sp = vClocks[ 0 ].cv_spPauseSync_;

  if ( m_sp.use_count( ) == 0 && cv_sp.use_count( ) == 0 )
  {
    m_sp = std::make_shared< std::mutex >( );
  }
  else if ( m_sp.use_count( ) != cv_sp.use_count( ) )
  {
    // Somthing is off
    //\todo This should probably be a throw
    return false;
  }
  // else we already have a sync spot

  for (
        std::vector< Clock& >::iterator it = vClocks.begin(); 
        it != vClocks.end(); 
        it++
        )
  {
    //
    // Synchronize the clocks, note you may only be synched with one pause group
    //

  }


  
} // Clock::synchronize


//**********************************************************************************
//
//  Clock::desynchronize
//
//  \brief Desynchronize all clocks in the vector
// 
//  \param vClocks
//  
//  Desynchronizes all the Sync mutexes of the clock if they are linked
//
//  \return bool
//
//**********************************************************************************
bool Clock::desynchronize( std::vector< Clocks& > vClocks )
{
  


} // Clock::desynchronize



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
bool Clock::start( )
{
  if ( !t_Timer_.joinable() )
  {

    t_Timer_ = std::thread( this->update );

  }
  else
  {
    //
    // We were paused
    //
    m_Pause_.unlock();
  }

  return true;

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
  
  if ( t_Timer_.joinable( ) )
  {
    // Stop loop
    done = true;

    try 
    {
      t_Timer_.join( );
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

  return true;

} // Clock::stop

//**********************************************************************************
//
//  Clock::pause
//
//  \brief Pause the clock
// 
//  Pauses the clock temporarily, must be restarted with start
//
//  \return none
//
//**********************************************************************************
void Clock::pause( )
{
  try
  {
    m_Pause_.lock( );
  } 
  catch( ... )
  {
    //
    //
    //
    return false;
  }

  return true;

} // Clock::pause



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

  while ( !done_ )
  {



    std::this_thread::sleep_for( std::chrono::milliseconds( ulDelay_ ) );



} // Clock::update




} // namespace timing

} // namespace components