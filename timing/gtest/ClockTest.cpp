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
///
///
///  \File    : ClockTest.cpp
///  \Author  : Anthony Islas
///  \Purpose : Clock gtest
///  \Group   : Gtest
///
///  \todo    : None
///
///  \License : GNU GPL v3
///
////////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "timing/Clock.hpp"
#include "string/Logging.hpp"

namespace gtest
{


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
// Basic clock functionality with no events
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

TEST( TEST_CASE, CreateClock )
{

  components::timing::Clock clk;

}

TEST( TEST_CASE, StartClock )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

}


TEST( TEST_CASE, RestartClock )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  ASSERT_FALSE( clk.start() );

  ASSERT_TRUE( clk.active() );

}


TEST( TEST_CASE, StopRunningClock )
{

  components::timing::Clock clk;

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}


TEST( TEST_CASE, StopInvalidClock )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_FALSE( clk.stop() );

  ASSERT_FALSE( clk.active () );
  ASSERT_FALSE( clk.stopped() );

}


TEST( TEST_CASE, PauseClock )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  ASSERT_TRUE( clk.pause() );
  
  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

}


TEST( TEST_CASE, PauseInvalidClock )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_FALSE( clk.pause() ); 
  
  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  ASSERT_FALSE( clk.active() );
  ASSERT_FALSE ( clk.paused() );

}




TEST( TEST_CASE, StopPausedClock )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  ASSERT_TRUE( clk.pause() );
  
  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );
  
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}


TEST( TEST_CASE, EventRateUpdateNoEvents )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  // Now update rates
  clk.changeEventRate( "noTag", 500 );

  // Pause
  ASSERT_TRUE( clk.pause() );
  
  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );
  
  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}


TEST( TEST_CASE, EventRateUpdateNoEventsBadMilli )
{

  components::timing::Clock clk;

  ASSERT_FALSE( clk.active() );

  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  // Now update rates
  clk.changeEventRate( "noTag", 0 );

  // Pause
  ASSERT_TRUE( clk.pause() );
  
  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );
  
  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
// Core functionality with events
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void eventCallback( int *x )
{
  if ( x ) ( *x )++;
}

TEST( TEST_CASE, AddSingleEvent5Hz )
{

  components::timing::Clock clk;
  components::timing::Event e;
  int x( 0 );

  ASSERT_FALSE( clk.active() );

  // Create event
  e.setCallback( eventCallback, &x );

  // Make sure it works
  e.callback();

  ASSERT_EQ( 1, x );

  // Event, rate, order
  clk.registerEvent( std::make_pair( std::ref( e ), 200u ) );

  std::cout << "Expected frequency : " << clk.expectedFrequency() << std::endl;

  // Start clock
  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  sleep( 2 );

  // Pause
  ASSERT_TRUE( clk.pause() );

  // Let it finish
  sleep( 2 );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  // Get measured frequency
  std::cout << "Measured frequency : " << clk.measuredFrequency() << std::endl;

  // We can be off by about 0.05 Hz for > 100 Hz frequencies
  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.05 );


  std::cout << "Difference between actual and expected at 5 Hz for 2 sec: "
            << clk.expectedFrequency() - clk.measuredFrequency() << " Hz" 
            << std::endl;

  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}


TEST( TEST_CASE, AddSingleEventMaxClock )
{

  components::timing::Clock clk;
  components::timing::Event e;
  int x( 0 );

  ASSERT_FALSE( clk.active() );

  // Create event
  e.setCallback( eventCallback, &x );

  // Make sure it works
  e.callback();

  ASSERT_EQ( 1, x );

  // Event, rate, order
  clk.registerEvent( std::make_pair( std::ref( e ), 10u ) ); // This seems to be the sweet spot

  std::cout << "Expected frequency : " << clk.expectedFrequency() << std::endl;

  // Start clock
  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  sleep( 2 );

  // Pause
  ASSERT_TRUE( clk.pause() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  // Get measured frequency
  std::cout << "Measured frequency : " << clk.measuredFrequency() << std::endl;

  // We can be off by about ~5 Hz at max clock frequency 
  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 5.5 );

  std::cout << "Difference between actual and expected at max Hz for 2 sec: "
            << clk.expectedFrequency() - clk.measuredFrequency() << " Hz" 
            << std::endl;

  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );
  
}

TEST( TEST_CASE, AddSingleEventOutputTrueMax )
{

  LOG_WARNING( "THIS TEST ALWAYS PASSES, JUST REPORTS ABSOLUTE MAX" );

  components::timing::Clock clk;
  components::timing::Event e;
  int x( 0 );

  ASSERT_FALSE( clk.active() );

  // Create event
  e.setCallback( eventCallback, &x );

  // Make sure it works
  e.callback();

  ASSERT_EQ( 1, x );

  // Event, rate, order
  clk.registerEvent( std::make_pair( std::ref( e ), 1u ) ); 

  std::cout << "Expected frequency : " << clk.expectedFrequency() << std::endl;

  // Start clock
  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  sleep( 10 );

  // Pause
  ASSERT_TRUE( clk.pause() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  // Get measured frequency
  std::cout << "Measured frequency : " << clk.measuredFrequency() << std::endl;

  std::cout << "Difference between actual and expected at max Hz for 10 sec: "
            << clk.expectedFrequency() - clk.measuredFrequency() << " Hz" 
            << std::endl;

  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}

// Test cases I still need - 
// More than one event
// sub events under one event
// events with different frequencies
// subevents under events with different periods
// multiple events assigning order
// multiple events, rearrange order 
// multiple events with different periods, assigned order
// multiple events with different periods, rearrange order 

// TEST( TEST_CASE, )


} // namespace gtest