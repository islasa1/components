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

void eventCallback0( int *y )
{
  if ( y ) ( *y )++;
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

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  // Get measured frequency
  std::cout << "Measured frequency : " << clk.measuredFrequency() << std::endl;

  // We can be off by about 0.05 Hz for < 100 Hz frequencies
  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.05 );


  std::cout << "Difference between actual and expected for 2 sec: "
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
  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 2.5 );

  std::cout << "Difference between actual and expected for 2 sec: "
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



TEST( TEST_CASE, AddSingleEvent60HzChangeCallback )
{

  components::timing::Clock clk;
  components::timing::Event e;
  int x( 0 );
  int y( 0 );

  ASSERT_FALSE( clk.active() );

  e.setCallback( eventCallback, &x );
  e.callback();

  ASSERT_EQ( 1, x );

  // Event, rate, order
  clk.registerEvent( std::make_pair( std::ref( e ), 16u ) );

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

  // We can be off by about 0.5 Hz for 0 Hz < X < 80 Hz frequencies
  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.5 );


  std::cout << "Difference between actual and expected for 2 sec: "
            << clk.expectedFrequency() - clk.measuredFrequency() << " Hz" 
            << std::endl;

  // Check values
  ASSERT_NEAR( x, 126, 2 );
  int tempX( x ); // hold x value


  e.setCallback( eventCallback0, &y );
  e.callback();

  ASSERT_EQ( y, 1 );

  // Re-do
  ASSERT_TRUE( clk.start() );
  ASSERT_TRUE( clk.active() );

  sleep( 2 );

  // Pause
  ASSERT_TRUE( clk.pause() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.5 );

  ASSERT_NEAR( y, 126, 2 );

  // Make sure X did not change
  ASSERT_EQ( tempX, x );
  int tempY( y );

  // Re-do but change while clock is running
  ASSERT_TRUE( clk.start() );
  ASSERT_TRUE( clk.active() );

  sleep( 1 );
  e.setCallback( eventCallback, &x );
  sleep( 1 );

  // Pause
  ASSERT_TRUE( clk.pause() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.5 );

  // Check values
  ASSERT_NE( y, tempY );
  ASSERT_NE( x, tempX );

  ASSERT_NEAR( y, 188, 3 );
  ASSERT_NEAR( x, 188, 3 );


  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}



// Test cases I still need - 
// subevents under events with different periods
// multiple events assigning order
// multiple events, rearrange order 
// multiple events with different periods, assigned order
// multiple events with different periods, rearrange order 

TEST( TEST_CASE, MultiEventSubEvent )
{

  components::timing::Clock clk;
  components::timing::Event e;
  components::timing::Event sub;
  int x( 0 );
  int y( 0 );

  ASSERT_FALSE( clk.active() );

  e.setCallback( eventCallback, &x );
  e.callback();

  sub.setCallback( eventCallback0, &y );
  sub.callback();

  ASSERT_EQ( 1, x );
  ASSERT_EQ( 1, y );

  // Event, rate, order
  clk.registerEvent( std::make_pair( std::ref( e ), 200u ) );

  e.addSubevent( &sub, "sub" );

  std::cout << "Expected frequency : " << clk.expectedFrequency() << std::endl;

  // Start clock
  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  sleep( 2 );

  // Pause
  ASSERT_TRUE( clk.pause() );
  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );


  // We can be off by about 0.05 Hz for < 100 Hz frequencies
  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.05 );

  // Check values
  ASSERT_EQ( x, y );
  ASSERT_NEAR( x, 10, 1 );
  ASSERT_NEAR( y, 10, 1 );


  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

}



TEST( TEST_CASE, MultiEventSameFreq30Hz )
{

  components::timing::Clock clk;
  components::timing::Event e0( "e0" );
  components::timing::Event e1( "e1" );

  int x( 0  );
  int y( 10 );

  ASSERT_FALSE( clk.active() );

  // Create event
  e0.setCallback( eventCallback, &x );
  e0.callback();
  ASSERT_EQ( 1, x );

  e1.setCallback( eventCallback0, &y );
  e1.callback();
  ASSERT_EQ( 11, y );

  std::cout << "X : " << x << " || Y : " << y << std::endl;

  // Event, rate, order
  clk.registerEvent( components::timing::Clock::PeriodicEvent( e0 , 33u ) ); 
  clk.registerEvent( components::timing::Clock::PeriodicEvent( e1 , 33u ) ); 

  std::cout << "Expected frequency : " << clk.expectedFrequency() << std::endl;

  // Start clock
  ASSERT_TRUE( clk.start() );

  ASSERT_TRUE( clk.active() );

  std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

  // Pause
  ASSERT_TRUE( clk.pause() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  // Get measured frequency
  std::cout << "Measured frequency : " << clk.measuredFrequency() << std::endl;

  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.5 );

  std::cout << "Difference between actual and expected at max Hz for 2 sec: "
            << clk.expectedFrequency() - clk.measuredFrequency() << " Hz" 
            << std::endl;

  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

  // Check values
  ASSERT_EQ( x + 10, y );
  ASSERT_NEAR( x, 61, 2 );
  ASSERT_NEAR( y, 71, 2 );

  std::cout << "X : " << x << " || Y : " << y << " updated +1 @ 30Hz for 2sec" << std::endl;

}



TEST( TEST_CASE, MultiEventDifferentFreq )
{

  components::timing::Clock clk;
  components::timing::Event e0( "e0" );
  components::timing::Event e1( "e1" );

  int x( 0  );
  int y( 0 );

  ASSERT_FALSE( clk.active() );

  // Create event
  e0.setCallback( eventCallback, &x );
  e0.callback();
  ASSERT_EQ( 1, x );

  e1.setCallback( eventCallback0, &y );
  e1.callback();
  ASSERT_EQ( 1, y );

  std::cout << "X : " << x << " || Y : " << y << std::endl;

  // Event, rate, order - one at 40 Hz and one at 20 Hz
  clk.registerEvent( components::timing::Clock::PeriodicEvent( e0 , 25u ) ); 
  clk.registerEvent( components::timing::Clock::PeriodicEvent( e1 , 50u ) ); 

  ASSERT_FLOAT_EQ( clk.expectedFrequency(), 1000.0 / 25.0 );

  // Start clock
  ASSERT_TRUE( clk.start() );
  ASSERT_TRUE( clk.active() );

  std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

  // Pause
  ASSERT_TRUE( clk.pause() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.paused() );

  ASSERT_NEAR( clk.measuredFrequency(), clk.expectedFrequency(), 0.5 );


  // Stop
  ASSERT_TRUE( clk.stop() );

  ASSERT_FALSE( clk.active() );
  ASSERT_TRUE ( clk.stopped() );

  // Check values
  // No matter what, since it is a 2t relation, we can only be off by one
  // is stopped in the middle of the updates
  ASSERT_EQ( ( x - 1 ) / 2, y - 1 );
  ASSERT_NEAR( x, 81, 2 );
  ASSERT_NEAR( y, 41, 2 );

  std::cout << "X : " << x << " || Y : " << y << " updated +1 @ 30Hz for 2sec" << std::endl;

}

} // namespace gtest