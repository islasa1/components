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

namespace gtest
{
  
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


} // namespace gtest