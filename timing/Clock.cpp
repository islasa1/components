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
#include <chrono>


namespace components
{
  
namespace timing
{

///**********************************************************************************
///
///  \function Clock::Clock
///
///  \brief    ctor
///
///**********************************************************************************
Clock::Clock( )
  : delay_( 1 )
  , done_ ( false )
{} // Clock::Clock


///**********************************************************************************
///
///  \function Clock::~Clock
///
///  \brief    DTOR
///
///**********************************************************************************
Clock::~Clock( )
{
  done_ = true;

  if ( tTimer_.joinable() )
  {
    tTimer_.join();
  }

} // Clock::~Clock


///**********************************************************************************
///
///  \function Clock::start
///
///  \brief    Starts the clock
/// 
///            Initializes the clock in a dedicated timing thread
///
///  \return   true
///
///**********************************************************************************
bool Clock::start( )
{
  if ( !tTimer_.joinable() )
  {

    tTimer_ = std::thread( &Clock::update, this );

  }
  else
  {
    //
    // We were paused
    //
    mPause_.unlock();
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
  
  if ( tTimer_.joinable( ) )
  {
    // Stop loop
    done_ = true;

    try 
    {
      tTimer_.join( );
    }
    catch( ... )
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

///**********************************************************************************
///
///  \function Clock::pause
///
///  \brief    Pause the clock
/// 
///            Pauses the clock temporarily, must be restarted with start
///
///  \return bool - whether the clock was able to be paused
///
///**********************************************************************************
bool Clock::pause( )
{
  try
  {
    mPause_.lock( );
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



///**********************************************************************************
///
///  \function Clock::registerResource
///
///  \brief    Register a resource to this particular clock
///  
///            Register a particular event to this particular clock, assigning it an 
///            order. Should order conflict, the previous event will be moved back 
///            ( adjusting all others behind as well ).
///
///  \return none
///
///**********************************************************************************
void Clock::registerResource( 
                              std::pair< Event, unsigned int > e, 
                              int order 
                              )
{

  // Attempt to lock the clock and safely add
  mPause_.lock();

  if ( 
      order < 0 ||
      order >= static_cast< int >( events_.size() )
      )
  {

    events_.push_back( e );
  }
  else // Valid order
  {
    
    itEvent_ = events_.begin();
    std::advance( itEvent_, order );

    events_.insert( itEvent_, e );

  }

  mPause_.unlock();

} // Clock::registerResource


///**********************************************************************************
///
///  \function Clock::update
///
///  \brief    Thread running time updates
/// 
///            Threaded update of resources, sleeping for a given time and then 
///            releasing locks
///
///  \return none
///
///**********************************************************************************
void Clock::update( )
{

  while ( !done_ )
  {

    // Time stamp

    mPause_.lock();

    for ( itEvent_ = events_.begin();
          itEvent_ != events_.end();
          itEvent_++ )
    {

      itEvent_->first.callback();

    }
    
    // Do delta time and diff with delay_
    std::this_thread::sleep_for( std::chrono::milliseconds( delay_ ) );

    mPause_.unlock();

  }

} // Clock::update




} // namespace timing

} // namespace components
