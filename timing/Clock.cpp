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
#include <numeric>

#include "string/Logging.hpp"
#include "math/Arithmetic.hpp"

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
  : delay_    ( 100   ) // default to 10 Hz no matter what
  , frequency_( 0.0f  ) // No frequency until we start running
  , done_     ( false )
  , paused_   ( false )
  , optimized_( false ) 
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

  if ( active() || paused_ )
  {
    // Stop loop
    done_ = true;

    // unpause regardless
    mPause_.unlock();

    if ( tTimer_.joinable() )
    {

      tTimer_.join( );

    }

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
  else if ( paused_ )
  {
    //
    // We were paused
    //
    mPause_.unlock();
    paused_ = false;

  }
  else
  {

    LOG_WARNING( "Attempting to restart a running clock, ignored" );
    return false;

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
  
  if ( active() || paused_ )
  {
    // Stop loop
    done_ = true;

    // unpause regardless
    mPause_.unlock();

    try 
    {
      tTimer_.join( );
    }
    catch( ... )
    {
      //\todo AI: Add some logging info and try to diagnose
      LOG_ERROR( "Clock could not stop" );
      return false;
    }

  }
  else
  {
    //\todo AI: Add some logging info here
    LOG_WARNING( "Clock is not joinable" );
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
  if ( active() )
  {

    try
    {
      mPause_.lock( );
      paused_ = true;
    } 
    catch( ... )
    {
      LOG_ERROR( "Clock could not be paused" );
      return false;
    }

  }
  else
  {

    LOG_WARNING( "Attempting to pause an inactive clock, ignored" );
    return false;

  }

  return true;

} // Clock::pause



///**********************************************************************************
///
///  \function Clock::registerEvent
///
///  \brief    Register a resource to this particular clock
///  
///            Register a particular event to this particular clock, assigning it an 
///            order. Should order conflict, the previous event will be moved back 
///            ( adjusting all others behind as well ). Take a pair with an Event 
///            reference so users can adjust an Event without the clock present, as
///            well as a rate ( in milliseconds ) to be able to adjust its rate 
///            dynamically
///
///  \return none
///
///**********************************************************************************
void Clock::registerEvent( 
                          std::pair< Event&, uint > e, 
                          int order 
                          )
{

  if ( e.second == 0 )
  {
    LOG_WARNING( "Cannot have an event with 0 milliseconds period" );
    return;
  }

  // Attempt to lock the clock and safely add
  mPause_.lock();

  std::pair< uint, uint > period( 0, 0 );

  if ( 
      order < 0 ||
      order >= static_cast< int >( events_.size() )
      )
  {
    // The period pair doesnt matter yet
    events_.push_back( std::make_pair( e, period ) );
  }
  else // Valid order
  {
    
    itEvent_ = events_.begin();
    std::advance( itEvent_, order );

    events_.insert( itEvent_, std::make_pair( e, period ) );

  }

  // Recalc
  setPeriods();

  mPause_.unlock();

} // Clock::registerEvent


///**********************************************************************************
///
///  \function Clock::changeEventRate
///
///  \brief    Updates the rate of the Event in the Clock with equivalent tag. Note 
///            that the Events are not stored in a map or guarantee uniqueness, so 
///            the user should ensure that if Events are to be updated with this call
///            they have unique tags. 
///  \note     This is an expensive call as we will a) iterate through a list and b)
///            recalulate the minimum period for all Events to be processed. It is 
///            expected that Events do not change their rates often
///
///  \return   void
///
///**********************************************************************************
void 
Clock::changeEventRate(
                       std::string  tag, 
                       uint milli
                       )
{


  if ( milli == 0 )
  {
    LOG_WARNING( "Cannot have an event with 0 milliseconds period" );
    return;
  }
  else if ( !events_.size() )
  {

    LOG_WARNING( "No events to update" );
    return;

  }

  // Attempt to lock the clock and safely modify
  mPause_.lock();

  for ( itEvent_ = events_.begin();
        itEvent_ != events_.end();
        itEvent_++ )
  {

    if ( itEvent_->first.first.tag() == tag )
    {

      itEvent_->first.second = milli;

    }

  }

  // Recalc
  setPeriods();


  mPause_.unlock();

} // Clock::changeEventRate




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

  std::chrono::high_resolution_clock::time_point before;
  std::chrono::high_resolution_clock::time_point after;
  std::chrono::duration< double > timeSpan;

  while( !done_ )
  {

    // atomic lock
    mPause_.lock();

    // Prevents bad access of callbacks terminated early
    // Without this, we have a potential to segfault when 
    // accessing a callback that has been destroyed
    if ( done_ ) break;

    // time stamp
    before = std::chrono::high_resolution_clock::now();

    for ( itEvent_ = events_.begin();
          itEvent_ != events_.end();
          itEvent_++ )
    {

      // If periods elapsed is greater than or equal to periodic interval
      if ( itEvent_->second.second >= itEvent_->second.first )
      {
        itEvent_->first.first.callback();
        itEvent_->second.second = 1; // We techinically passed one period
        
      }
      else
      {
        itEvent_->second.second++;
      }

    }

    // end time stamp
    after = std::chrono::high_resolution_clock::now();

    timeSpan = std::chrono::duration_cast< std::chrono::milliseconds >( after - before );

    // Unlock BEFORE sleep so others have a chance to interact with clock
    mPause_.unlock();    

    // Do delta time and diff with delay_
    uint deltaDelay = delay_ - static_cast< uint >( timeSpan.count() );
   
    // This optimization would be hardware specific 
    // if ( deltaDelay < 10 )
    // {
    //   deltaDelay = ( ( deltaDelay + 7 ) / 2 );
    // }

    std::this_thread::sleep_for( std::chrono::milliseconds( deltaDelay ) );

    if ( !optimized_ )
    {
      // Calculate a measured clock frequency
      after = std::chrono::high_resolution_clock::now();

      // Get duration in seconds
      timeSpan = std::chrono::duration_cast< std::chrono::duration< double > >( after - before );
      frequency_ = std::pow( timeSpan.count(), -1 );

    }

  }

} // Clock::update


///**********************************************************************************
///
///  \function Clock::setPeriods
///
///  \brief    Brief Description
///
///  \return   void - 
///
///**********************************************************************************
void 
Clock::setPeriods( )
{

  // Recalc with only values in there, not taking into account delay_

  std::vector< uint > rates;
  for ( itEvent_ = events_.begin();
        itEvent_ != events_.end();
        itEvent_++ )
  {

    rates.push_back( itEvent_->first.second );

  }

  delay_ = std::accumulate( 
                             rates.begin(),
                             rates.end(),
                            *rates.begin(),
                             components::math::gcd< uint >
                            );


  for ( itEvent_ = events_.begin();
        itEvent_ != events_.end();
        itEvent_++ )
  {

    // Get periods based on delay_
    itEvent_->second.first = itEvent_->first.second / delay_;

  }  
  
} // Clock::setPeriods


} // namespace timing

} // namespace components
