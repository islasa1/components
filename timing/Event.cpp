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
///  \File    : Event.cpp
///  \Author  : Anthony Islas
///  \Purpose : Event system implementation
///  \Group   : Timing
///
///  \todo    : None
///
///  \License : GNU GPL v3
///
////////////////////////////////////////////////////////////////////////////////////

#include "Event.hpp"

#include <exception>
#include <algorithm>

#include "string/Logging.hpp"


namespace components
{

namespace timing
{

///**********************************************************************************
///
///  \function Event::Event
///
///  \brief    ctor
///
///********************************************************************************** 
Event::Event( )
: callback_ ( nullptr )
, enabled_  (  true   )
, tag_      ( "Event" )
, subevents_(    0    )
, tagToSubevent_( )
{ } // Event::Event


///**********************************************************************************
///
///  \function Event::~Event
///
///  \brief    dtor
///
///  \tod      Fix memory leak
///
///********************************************************************************** 
Event::~Event( )
{ } // Event::~Event


///**********************************************************************************
///
///  \function Event::callback
///
///  \brief    Uses provided callback
///
///  \return   void - callback is treated as void (*)()
///
///**********************************************************************************
void 
Event::callback( )
{
  
  if ( enabled_ ) 
  {

    if ( callback_ )
    {
      callback_( ); 
    }

    for (
          std::vector< Event * >::iterator it = subevents_.begin(); 
          it != subevents_.end(); 
          it++
          )
    {

      ( *it )->callback();

    }  

  }

} // Event::callback


///**********************************************************************************
///
///  \function Event::addSubevent
///
///  \brief    Adds a subevent to this event to be called after the main callback 
///            of this event. Order in which subevents are called is preserved from
///            order of addition ( FIFO ). Events should created via new Event() 
///            to be added to subevents, or be persistent
///
///  \return   bool - if a subevent tag already exists return is false, else true
///
///**********************************************************************************
bool   
Event::addSubevent(
                    Event       *e, 
                    std::string  tag
                  )
{

  if ( e == nullptr )
  {

    return false;

  }
 
  if ( tagToSubevent_.find( tag ) != tagToSubevent_.end() )
  {

    LOG_WARNING( "Tag " << tag << " already exists" ); 

    return false;

  }

  tagToSubevent_[ tag ] = e;
  subevents_.push_back( tagToSubevent_[ tag ] );
  e->tag_ = tag;

  return true;

} // Event::addSubevent


///**********************************************************************************
///
///  \function Event::removeSubevent
///
///  \brief    Recursively searches to remove event with tag once, including subevents'
///            children. This can lead to misleading results if subevents possess
//             string-comparable equivalent names, only first occurence will be removed
///
///  \return   bool - if no subevent with tag exists, return false.  
///
///**********************************************************************************
bool    
Event::removeSubevent(
                      std::string tag
                      )
{
  
  std::unordered_map< std::string, Event * >::iterator itEvent;

  itEvent = tagToSubevent_.find( tag );

  if ( itEvent == tagToSubevent_.end() )
  {

    // try subchildren 
    for ( std::unordered_map< std::string, Event * >::iterator it;
          it != tagToSubevent_.end();
          it++ 
          )
    {

      if ( it->second->removeSubevent( tag ) )
      {

        return true;

      }

    }

    // not found
    return false;

  }
  else
  {

    subevents_.erase( 
                      std::remove( 
                                  subevents_.begin(),
                                  subevents_.end(),
                                  tagToSubevent_[ tag ] 
                                  ),
                      subevents_.end()
                    );

    tagToSubevent_.erase( tag );

    return true;

  }

} // Event::removeSubevent


///**********************************************************************************
///
///  \function Event::getSubevent
///
///  \brief    Get a subevent with a particular tag, searching recursively, 
///            depth-first.
///
///  \return   Event* - pointer to event, allowing manipulation of the event
///
///**********************************************************************************
Event*
Event::getSubevent(
                    std::string tag
                  )
{
 
  if ( tagToSubevent_.find( tag ) == tagToSubevent_.end() )
  {

    LOG_WARNING( "No event exists with tag " << tag );

    return nullptr;

  }

  return tagToSubevent_[ tag ];

} // Event::getSubevent


///**********************************************************************************
///
///  \function Event::print
///
///  \brief    print out event tree
///
///  \return   void - none
///
///**********************************************************************************
void 
Event::print( 
              const Event& e, 
              std::string indent 
            )    
{
 
  std::cout << indent << "-->" << e.tag_ << std::endl;
  indent += "   |";

  for ( 
        std::vector< Event * >::const_iterator it = e.subevents_.begin();
        it != e.subevents_.end();
        it++
      )
  {

    Event::print( **it, indent );

  }

} // Event::print

} // namespace timing

} // namespace components
