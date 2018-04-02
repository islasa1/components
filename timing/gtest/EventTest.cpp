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
///  \File    : EventTest.cpp
///  \Author  : Anthony Islas
///  \Purpose : Event gtest
///  \Group   : Gtest
///
///  \todo    : None
///
///  \License : GNU GPL v3
///
////////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "Event.hpp"

namespace gtest
{

class EventTest : public ::testing::Test
{
public:
  EventTest() 
  : counter_( 0 )
  {}

  ~EventTest() 
  {}
  

  void callback_1(  ) 
  {

    counter_++;

  }

  void callback_2( unsigned int x )
  {

    counter_ += x;

  }

  void callback_3( )
  {

    counter_ += 3;

  }

  void callback_4( bool add )
  {

    if ( add )
    {

      counter_ += 4;

    }

  }


  virtual void SetUp( )
  {

    ASSERT_EQ( 0, counter_ );
    ASSERT_TRUE( e_.enabled() );

  }  

  void singleCallback()
  {

    unsigned int previousCounter = counter_;

    e_.setCallback( &EventTest::callback_1, this );

    ASSERT_EQ( previousCounter, counter_ );

    e_.callback( );

    ASSERT_NE( previousCounter, counter_ );

    ASSERT_EQ( previousCounter + 1, counter_ );

  }

  ::testing::AssertionResult
  doubleCallback( )
  {

    unsigned int previousCounter = counter_;

    e_.setCallback( &EventTest::callback_2, this, 2 );

    if ( previousCounter != counter_ )
    {

      return ::testing::AssertionFailure();

    }

    e_.callback( );

    if ( ( previousCounter + 2 ) == counter_ )
    {
    
      return ::testing::AssertionSuccess();
        
    }
    else if ( !e_.enabled() )
    {
      return ::testing::AssertionSuccess();
    }
    else
    {

      return ::testing::AssertionFailure();

    }

  }

protected:

  unsigned int              counter_;
  components::timing::Event e_;

};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


TEST_F( EventTest, EmptyEvent )
{

  unsigned int previousCounter = counter_;

  ASSERT_EQ( previousCounter, counter_ );

  e_.callback( );

  ASSERT_EQ( previousCounter, counter_ );

}


TEST_F( EventTest, SingleCallback )
{

  singleCallback();

}


TEST_F( EventTest, ArgumentCallback )
{

  doubleCallback();

}


TEST_F( EventTest, ReassignCallback )
{

  singleCallback();

  doubleCallback();

  singleCallback();

}




TEST_F( EventTest, DisableEvent )
{

  doubleCallback();

  e_.disable();

  ASSERT_FALSE( e_.enabled() );

  doubleCallback();

}


TEST_F( EventTest, ReenableEvent )
{

  doubleCallback();

  e_.disable();
  
  ASSERT_FALSE( e_.enabled() );

  doubleCallback();

  e_.enable();

  ASSERT_TRUE( e_.enabled() );

  doubleCallback();

}




TEST_F( EventTest, AddSubevent )
{

  singleCallback();

  doubleCallback();

  components::timing::Event *sub( new components::timing::Event() );
  sub->setCallback( &EventTest::callback_1, this );

  unsigned int previousCounter = counter_;

  ASSERT_EQ( previousCounter, counter_ );

  sub->callback();

  ASSERT_EQ( previousCounter + 1, counter_ );




  ASSERT_TRUE( e_.addSubevent( sub, "single" ) );

  previousCounter = counter_;

  ASSERT_EQ( previousCounter, counter_ );

  e_.callback( );

  ASSERT_NE( previousCounter, counter_ );


  // Calls single and double
  ASSERT_EQ( previousCounter + 3, counter_ );

  delete sub;

}


TEST_F( EventTest, DISABLED_RemoveSubevent )
{


}


TEST_F( EventTest, DISABLED_GetSubevent )
{


}




TEST_F( EventTest, MultiEvents )
{

  std::vector< components::timing::Event > events( 4 );

  events[ 0 ].setCallback( &EventTest::callback_3, this        );
  events[ 1 ].setCallback( &EventTest::callback_4, this, false );

  e_.addSubevent( &events[ 0 ], "first"  );
  e_.addSubevent( &events[ 1 ], "second" );

  unsigned int previousCounter = counter_;
  ASSERT_EQ( previousCounter, counter_ );

  e_.callback();

  ASSERT_EQ( previousCounter + 3, counter_ );



  events[ 1 ].setCallback( &EventTest::callback_4, this, true );

  previousCounter = counter_;

  e_.callback();

  ASSERT_EQ( previousCounter + 7, counter_ );


  // Add subevents to sub-events
  events[ 2 ].setCallback( &EventTest::callback_1, this        );
  events[ 3 ].setCallback( &EventTest::callback_4, this, false );

  events[ 0 ].addSubevent( &events[ 2 ], "third"  );
  events[ 2 ].addSubevent( &events[ 3 ], "fourth" );

  previousCounter = counter_;

  e_.callback();

  ASSERT_EQ( previousCounter + 8, counter_ );

  events[ 3 ].setCallback( &EventTest::callback_4, this, true );
  previousCounter = counter_;
  
  e_.callback();

  ASSERT_EQ( previousCounter + 12, counter_ );

}


TEST_F( EventTest, PrintEvents )
{

  singleCallback();
  std::vector< components::timing::Event > events( 4 );

  events[ 0 ].setCallback( &EventTest::callback_3, this        );
  events[ 1 ].setCallback( &EventTest::callback_4, this, false );
  events[ 2 ].setCallback( &EventTest::callback_1, this        );
  events[ 3 ].setCallback( &EventTest::callback_4, this, false );

  e_.addSubevent( &events[ 0 ], "first"  );
  e_.addSubevent( &events[ 1 ], "second" );
  events[ 0 ].addSubevent( &events[ 2 ], "third"  );
  events[ 2 ].addSubevent( &events[ 3 ], "fourth" );

  components::timing::Event::print( e_ );

}


TEST_F( EventTest, DISABLED_OrderedEvents )
{



}

} // namespace gtest