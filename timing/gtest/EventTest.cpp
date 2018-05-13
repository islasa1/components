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

#include "timing/Event.hpp"

namespace gtest
{

class TEST_CASE : public ::testing::Test
{
public:
  TEST_CASE() 
  : counter_( 0 )
  {}

  ~TEST_CASE() 
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

  void callback_x(  
                  unsigned int *arr, 
                  unsigned int idx0,
                  unsigned int idx1 
                  )
  {

    arr[ idx1 ] = arr[ idx0 ]; 

  }


  virtual void SetUp( )
  {

    ASSERT_EQ( 0, counter_ );
    ASSERT_TRUE( e_.enabled() );

  }  

  void singleCallback()
  {

    unsigned int previousCounter = counter_;

    e_.setCallback( &TEST_CASE::callback_1, this );

    ASSERT_EQ( previousCounter, counter_ );

    e_.callback( );

    ASSERT_NE( previousCounter, counter_ );

    ASSERT_EQ( previousCounter + 1, counter_ );

  }

  ::testing::AssertionResult
  doubleCallback( )
  {

    unsigned int previousCounter = counter_;

    e_.setCallback( &TEST_CASE::callback_2, this, 2 );

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

TEST_F( TEST_CASE, EmptyEvent )
{

  unsigned int previousCounter = counter_;

  ASSERT_EQ( previousCounter, counter_ );

  e_.callback( );

  ASSERT_EQ( previousCounter, counter_ );

}


TEST_F( TEST_CASE, SingleCallback )
{

  singleCallback();

}


TEST_F( TEST_CASE, ArgumentCallback )
{

  doubleCallback();

}


TEST_F( TEST_CASE, ReassignCallback )
{

  singleCallback();

  doubleCallback();

  singleCallback();

}




TEST_F( TEST_CASE, DisableEvent )
{

  doubleCallback();

  e_.disable();

  ASSERT_FALSE( e_.enabled() );

  doubleCallback();

}


TEST_F( TEST_CASE, ReenableEvent )
{

  doubleCallback();

  e_.disable();
  
  ASSERT_FALSE( e_.enabled() );

  doubleCallback();

  e_.enable();

  ASSERT_TRUE( e_.enabled() );

  doubleCallback();

}




TEST_F( TEST_CASE, AddSubevent )
{

  singleCallback();

  doubleCallback();

  components::timing::Event *sub( new components::timing::Event() );
  sub->setCallback( &TEST_CASE::callback_1, this );

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


TEST_F( TEST_CASE, AddSubeventNamed )
{

  singleCallback();

  doubleCallback();

  components::timing::Event *sub( new components::timing::Event( "AddSubeventNamed" ) );
  sub->setCallback( &TEST_CASE::callback_1, this );

  unsigned int previousCounter = counter_;

  ASSERT_EQ( previousCounter, counter_ );

  sub->callback();

  ASSERT_EQ( previousCounter + 1, counter_ );




  ASSERT_TRUE( e_.addSubevent( sub, "single" ) );

  ASSERT_EQ( sub->tag(), std::string( "AddSubeventNamed" ) );

  previousCounter = counter_;

  ASSERT_EQ( previousCounter, counter_ );

  e_.callback( );

  ASSERT_NE( previousCounter, counter_ );


  // Calls single and double
  ASSERT_EQ( previousCounter + 3, counter_ );

  delete sub;

}



TEST_F( TEST_CASE, RemoveSubevent )
{

  singleCallback();

  components::timing::Event *sub( new components::timing::Event() );
  sub->setCallback( &TEST_CASE::callback_1, this );

  unsigned int previousCounter = counter_;


  ASSERT_TRUE( e_.addSubevent( sub, "single" ) );

  e_.callback( );

  ASSERT_NE( previousCounter, counter_ );


  // Calls single twice
  ASSERT_EQ( previousCounter + 2, counter_ );


  ASSERT_TRUE( e_.removeSubevent( "single" ) );

  singleCallback();


  ASSERT_FALSE( e_.removeSubevent( "single" ) );


  delete sub;

}


TEST_F( TEST_CASE, GetSubevent )
{

  singleCallback();
  std::vector< components::timing::Event > events( 4 );

  events[ 0 ].setCallback( &TEST_CASE::callback_3, this        );
  events[ 1 ].setCallback( &TEST_CASE::callback_4, this, false );
  events[ 2 ].setCallback( &TEST_CASE::callback_1, this        );
  events[ 3 ].setCallback( &TEST_CASE::callback_4, this, false );

  e_.addSubevent( &events[ 0 ], "first"  );
  e_.addSubevent( &events[ 1 ], "second" );

  ASSERT_NE( nullptr, e_.getSubevent( "first"   ) );
  ASSERT_NE( nullptr, e_.getSubevent( "second"  ) );

  ASSERT_EQ( &events[ 0 ], e_.getSubevent( "first"   ) );
  ASSERT_EQ( &events[ 1 ], e_.getSubevent( "second"  ) );

  ASSERT_EQ( nullptr, e_.getSubevent( "third"  ) );
  ASSERT_EQ( nullptr, e_.getSubevent( "fourth" ) );


  e_.getSubevent( "first"  )->addSubevent( &events[ 2 ], "third"  );
  e_.getSubevent( "second" )->addSubevent( &events[ 3 ], "fourth" );

  components::timing::Event::print( e_ );

}




TEST_F( TEST_CASE, MultiEvents )
{

  std::vector< components::timing::Event > events( 4 );

  events[ 0 ].setCallback( &TEST_CASE::callback_3, this        );
  events[ 1 ].setCallback( &TEST_CASE::callback_4, this, false );

  e_.addSubevent( &events[ 0 ], "first"  );
  e_.addSubevent( &events[ 1 ], "second" );

  unsigned int previousCounter = counter_;
  ASSERT_EQ( previousCounter, counter_ );

  e_.callback();

  ASSERT_EQ( previousCounter + 3, counter_ );



  events[ 1 ].setCallback( &TEST_CASE::callback_4, this, true );

  previousCounter = counter_;

  e_.callback();

  ASSERT_EQ( previousCounter + 7, counter_ );


  // Add subevents to sub-events
  events[ 2 ].setCallback( &TEST_CASE::callback_1, this        );
  events[ 3 ].setCallback( &TEST_CASE::callback_4, this, false );

  events[ 0 ].addSubevent( &events[ 2 ], "third"  );
  events[ 2 ].addSubevent( &events[ 3 ], "fourth" );

  previousCounter = counter_;

  e_.callback();

  ASSERT_EQ( previousCounter + 8, counter_ );

  events[ 3 ].setCallback( &TEST_CASE::callback_4, this, true );
  previousCounter = counter_;
  
  e_.callback();

  ASSERT_EQ( previousCounter + 12, counter_ );

}


TEST_F( TEST_CASE, PrintEvents )
{

  singleCallback();
  std::vector< components::timing::Event > events( 4 );

  events[ 0 ].setCallback( &TEST_CASE::callback_3, this        );
  events[ 1 ].setCallback( &TEST_CASE::callback_4, this, false );
  events[ 2 ].setCallback( &TEST_CASE::callback_1, this        );
  events[ 3 ].setCallback( &TEST_CASE::callback_4, this, false );

  e_.addSubevent( &events[ 0 ], "first"  );
  e_.addSubevent( &events[ 1 ], "second" );
  events[ 0 ].addSubevent( &events[ 2 ], "third"  );
  events[ 2 ].addSubevent( &events[ 3 ], "fourth" );

  components::timing::Event::print( e_ );

}


TEST_F( TEST_CASE, OrderedEvents )
{

  unsigned int testArray[ 5 ];

  memset( testArray, 0, sizeof( testArray[ 0 ] ) * 5 );

  testArray[ 0 ] = 7;

  ASSERT_EQ( 7, testArray[ 0 ] );
  ASSERT_EQ( 0, testArray[ 1 ] );
  ASSERT_EQ( 0, testArray[ 2 ] );
  ASSERT_EQ( 0, testArray[ 3 ] );
  ASSERT_EQ( 0, testArray[ 4 ] );

  std::vector< components::timing::Event > events( 5 );

  events[ 0 ].setCallback( &TEST_CASE::callback_x, this, testArray, 0, 1 );
  events[ 1 ].setCallback( &TEST_CASE::callback_x, this, testArray, 1, 2 );
  events[ 2 ].setCallback( &TEST_CASE::callback_x, this, testArray, 2, 3 );
  events[ 3 ].setCallback( &TEST_CASE::callback_x, this, testArray, 3, 4 );
  events[ 4 ].setCallback( &TEST_CASE::callback_x, this, testArray, 4, 0 );


  for(  
      unsigned int i = 0; 
      i < events.size();
      i++
      )
  {

    std::stringstream ss;

    ss << i;

    e_.addSubevent( &events[ i ], ss.str() );

  }

  e_.callback();

  ASSERT_EQ( 7, testArray[ 0 ] );
  ASSERT_EQ( 7, testArray[ 1 ] );
  ASSERT_EQ( 7, testArray[ 2 ] );
  ASSERT_EQ( 7, testArray[ 3 ] );
  ASSERT_EQ( 7, testArray[ 4 ] );

}

} // namespace gtest