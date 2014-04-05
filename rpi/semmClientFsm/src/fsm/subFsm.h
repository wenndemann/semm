/*
 * selectColorFsm.h
 *
 *  Created on: 22.03.2014
 *      Author: micha
 */

#ifndef SELECTCOLORFSM_H_
#define SELECTCOLORFSM_H_

#include "../defs.h"

#include "../../../semmVis/pimpl.h"

#include <vector>
#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>

#include <boost/shared_ptr.hpp>

#include "../game/Game.h"

#include "mainFsm/events.h"

//using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

#ifdef FSM_EVENT_COUT
#undef FSM_EVENT_COUT
#endif
#define FSM_EVENT_COUT( event_name ) ( std::cout << "E " << event_name << std::endl )

namespace fsm // Concrete FSM implementation
{

	// events
	struct scmEvEnter { scmEvEnter( ){ FSM_EVENT_COUT( "scmEvEnter" ); } };
	struct scmEvCancel{ scmEvCancel( ){ FSM_EVENT_COUT( "scmEvCancel" ); } };
	struct scmEvColors{ scmEvColors( ){ FSM_EVENT_COUT( "scmEvColors" ); } };
	struct scmEvMeeplesNotOK { scmEvMeeplesNotOK( ){ FSM_EVENT_COUT( "scmEvMeeplesNotOK" ); } };
	struct scmEvMeeplesOK { scmEvMeeplesOK( ){ FSM_EVENT_COUT( "scmEvMeeplesOK" ); } };
	struct scmEvGetReady{ scmEvGetReady( ){ FSM_EVENT_COUT( "scmEvGetReady" ); } };

	// front-end: define the FSM structure
	struct SelectColorFSM_ : public msm::front::state_machine_def<SelectColorFSM_>
    {
		SelectColorFSM_( int32_t color_id,
				         boost::shared_ptr< Game > gamePtr,
				         boost::shared_ptr< pimpl > gui )
		: _gamePtr( gamePtr )
		, _gui( gui )
		, _color_id( color_id )
		, _ready( false )
		{ }

    	template <class Event,class FSM>
		void on_entry(Event const&, FSM& fsm)
		{
			std::cout << "-> SelectColorFSM " << fsm._color_id << std::endl;
		}
		template <class Event,class FSM>
		void on_exit(Event const&, FSM&  fsm)
		{
			std::cout << "<- SelectColorFSM " << fsm._color_id << std::endl;
		}

		// The list of FSM states
		struct ScmSemm : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmSemm " << fsm._color_id << std::endl;
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmSemm " << fsm._color_id << std::endl;
			}
		};
		struct ScmEnter : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmEnter " << fsm._color_id << std::endl;

				Playboard::DisplayPtr display = fsm._gamePtr->playboard()->display( fsm._color_id );
				if ( display )
					display->setPictures( I2C_DBEN_PIC_ENTER );
				else
				{
					std::cout << "   Display " << fsm._color_id <<
						" not available. Map has size " << fsm._gamePtr->playboard()->displays().size() << std::endl;
				}
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmEnter " << fsm._color_id << std::endl;
			}
		};
		struct ScmDisable : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmDisable " << fsm._color_id << std::endl;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_DISABLED );

				while( !fsm._gamePtr->playboard()->addPlayer( static_cast<int32_t>(fsm._color_id) ) )
				{ boost::this_thread::sleep( boost::posix_time::seconds( 1 ) ); }

				// add player to visualization
				Playboard::PlayerMapIt playerIt =
					fsm._gamePtr->playboard()->players( ).find( static_cast<int32_t>( fsm._color_id ) );
				std::cout << "playerIt created!" << std::endl;
				if ( playerIt != fsm._gamePtr->playboard()->players( ).end( ) )
				{
					const MeepleVec& vecMeeples = playerIt->second->meeples( );
					std::cout << "meeplesVec got!" << std::endl;
					std::map< uint16_t, uint8_t > meepleTagFieldIdMap;
					for ( uint32_t i = 0; i < vecMeeples.size( ); i++ )
					{
						meepleTagFieldIdMap[ vecMeeples[ i ]->tag( ) ] =
							static_cast<uint8_t>(vecMeeples[ i ]->fieldId( ) );
					}
					std::cout << "meepleTagFieldIdMap filled!" << std::endl;
					fsm._gui->addMeeples( fsm._color_id, meepleTagFieldIdMap );
					std::cout << "meeples added to Vis!" << std::endl;
				}

				//if ( !fsm._gamePtr->playboard()->addPlayer( fsm._color_id ) )
				//	fsm._gamePtr->mainFSM()->_tcpIp->sendDelColor( fsm._color_id );
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmDisable " << fsm._color_id << std::endl;
			}
		};
		struct ScmPrepare : public msm::front::state<>
		{
			//typedef mpl::vector<scmGetReady> deferred_events;

			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmPrepare " << fsm._color_id << std::endl;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_PREPARE );

				if ( !(fsm._gamePtr->clientColors( ) & fsm._color_id) )
				{ fsm._gamePtr->mainFSM( )->_tcpIp->sendSetColor( static_cast<uint8_t>( fsm._color_id ) ); }
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmPrepare " << fsm._color_id << std::endl;
			}
		};
		struct ScmWait : public msm::front::state<>
		{
			//typedef mpl::vector<scmGetReady> deferred_events;

			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmWait " << fsm._color_id << std::endl;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_WAIT );
				if ( fsm._gamePtr->playboard()->addPlayer( fsm._color_id ) ) {
					fsm._gamePtr->addToClientColors( fsm._color_id );
					fsm.process_event( fsm::scmEvMeeplesOK( ) );
				}
				else
					fsm.process_event( fsm::scmEvMeeplesNotOK( ) );
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmWait " << fsm._color_id << std::endl;
			}
		};
		struct ScmStart : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmStart " << fsm._color_id << std::endl;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_START );
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmStart " << fsm._color_id << std::endl;
			}
		};
		struct ScmReady : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmReady " << fsm._color_id << std::endl;
				fsm._ready = true;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_WAIT );




				boost::shared_ptr< std::vector< boost::shared_ptr< msm::back::state_machine< SelectColorFSM_ > > > > ssms =
						fsm._gamePtr->mainFSM( )->_ssms;

				// check if everyone else is in ScmReady already
				bool everyoneReady = true;
				for ( std::vector< boost::shared_ptr< msm::back::state_machine< SelectColorFSM_ > > >::iterator it = ssms->begin( );
						  it != ssms->end( ); ++it )
				{
					if ( !(*it)->_ready && (*it)->_color_id != fsm._color_id )
					{	everyoneReady = false; break;	}
				}

				if ( everyoneReady ) // is ready, start the game
				{
					std::cout << "\teveryone is ready" << std::endl;
					if(fsm._gamePtr->mode() == Game::SelectColor)
						fsm._gamePtr->mainFSM( )->_tcpIp->sendStartGame( static_cast<uint8_t>(fsm._color_id) );
					//fsm.startGame( );
					fsm._gamePtr->mainFSM()->process_event( fsm::evInitGame() );
				}
				else // send a broadcast message that everyone gets ready
				{
					std::cout << "\tnot everyone is ready" << std::endl;
					for ( std::vector< boost::shared_ptr< msm::back::state_machine< SelectColorFSM_ > > >::iterator it = ssms->begin( );
						  it != ssms->end( ); ++it )
					{
						if ( !(*it)->_ready && (*it)->_color_id != fsm._color_id )
							(*it)->process_event( scmEvGetReady( ) );
					}
				}
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmReady " << fsm._color_id << std::endl;
			}
		};

		// actions
		struct delColor {
			template<class EVT, class FSM, class SourceState, class TargetState>
			void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
				std::cout << "transition with event:" << typeid(EVT).name() << " " << fsm._color_id << std::endl;

				fsm._gamePtr->mainFSM( )->_tcpIp->sendDelColor( static_cast<uint8_t>(fsm._color_id) );
				fsm._gamePtr->playboard( )->delPlayer( fsm._color_id );
				fsm._gamePtr->delFromClientColors( fsm._color_id );

			}
		};

		struct delPlayer {
			template<class EVT, class FSM, class SourceState, class TargetState>
			void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
				std::cout << "transition with event:" << typeid(EVT).name() << " " << fsm._color_id << std::endl;
				fsm._gamePtr->playboard()->delPlayer( fsm._color_id );
			}
		};

		// guard conditions
		struct gAccept
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				bool colorAvailable = fsm._gamePtr->availableColors( ) & fsm._color_id;
				bool rightClient = fsm._gamePtr->clientColors( ) & fsm._color_id;
				return ( colorAvailable && !rightClient )
					&& ( fsm._gamePtr->mode() == Game::SelectColor );
			}
		};
		struct gBlocked
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				bool colorAvailable = fsm._gamePtr->availableColors( ) & fsm._color_id;
				bool rightClient = fsm._gamePtr->clientColors( ) & fsm._color_id;
				return (!colorAvailable && !rightClient);
			}
		};
		struct gKnown
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				bool colorAvailable = fsm._gamePtr->availableColors( ) & fsm._color_id;
				bool rightClient = fsm._gamePtr->clientColors( ) & fsm._color_id;
				return ( !colorAvailable && rightClient);
			}
		};
		struct gLeftOut
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				bool colorAvailable = fsm._gamePtr->availableColors( ) & fsm._color_id;
				bool rightClient = fsm._gamePtr->clientColors( ) & fsm._color_id;
				return ( colorAvailable && !rightClient )
					&& ( fsm._gamePtr->mode() == Game::Gaming );
			}
		};
		struct gGameMode
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				return fsm._gamePtr->mode() == Game::Gaming;
			}
		};
		struct gSelectColorMode
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				return fsm._gamePtr->mode() == Game::SelectColor;
			}
		};
		struct gColorTaken
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				bool colorAvailable = fsm._gamePtr->availableColors( ) & fsm._color_id;
				return !colorAvailable;
			}
		};
		struct gColorAvailable
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				bool colorAvailable = fsm._gamePtr->availableColors( ) & fsm._color_id;
				return colorAvailable;
			}
		};


		// Initial state(s)
		typedef ScmSemm initial_state;
		typedef int activate_deferred_events;

		// Transition table for player
		struct transition_table : mpl::vector<
			//    Start            Event           Next                  Action        Guard
			//  +----------------+------------------+------------------+-------------+-----------------+
			Row < ScmSemm        , none             , ScmEnter         , none        , gAccept         >,
			Row < ScmSemm        , none             , ScmDisable       , none        , gBlocked        >,
			Row < ScmSemm        , none             , ScmPrepare       , none        , gKnown          >,
			Row < ScmSemm        , none             , ScmReady         , none        , gLeftOut        >,
			//Row < ScmSemm        , scmEvGetReady  , none             , none        , none            >,
			//  +----------------+------------------+------------------+--- ---------+-----------------+
			Row < ScmEnter       , scmEvEnter       , ScmPrepare       , none        , none            >,
			Row < ScmEnter       , scmEvGetReady    , ScmReady         , none        , none            >,
			Row < ScmEnter       , scmEvColors      , ScmSemm          , none        , gColorTaken     >,
			//  +----------------+------------------+------------------+--- ---------+-----------------+
			Row < ScmPrepare     , scmEvEnter       , ScmWait          , none        , none            >,
			Row < ScmPrepare     , scmEvGetReady    , none             , Defer       , none            >,
//			Row < ScmEnter       , scmEvColors      , none             , none        , none            >, // ignore event
			//  +----------------+------------------+------------------+--- ---------+-----------------+
			Row < ScmDisable     , scmEvGetReady    , ScmReady         , none        , none            >,
			Row < ScmDisable     , scmEvColors      , ScmSemm          , delPlayer   , gColorAvailable >,
			//  +----------------+------------------+------------------+--- ---------+-----------------+
			Row < ScmWait        , scmEvMeeplesNotOK, ScmEnter         , delColor    , none            >,
			Row < ScmWait        , scmEvMeeplesOK   , ScmStart         , none        , gSelectColorMode>,
			//Row < ScmWait        , none             , ScmStart         , none        , gSelectColorMode>, // TODO remove later
			Row < ScmWait        , scmEvGetReady    , none             , Defer       , none            >,
			Row < ScmWait        , scmEvMeeplesOK   , ScmReady         , none        , gGameMode       >,
			//  +----------------+------------------+------------------+--- ---------+-----------------+
			Row < ScmStart       , scmEvEnter       , ScmReady         , none        , none            >,
			Row < ScmStart       , scmEvCancel      , ScmEnter         , delColor , none            >,
			Row < ScmStart       , scmEvGetReady    , ScmReady         , none        , none            >,
			//  +----------------+------------------+------------------+--- ---------+-----------------+
			Row < ScmReady       , scmEvGetReady    , none             , none        , none            >
		> {};

		boost::shared_ptr< Game > _gamePtr;
		boost::shared_ptr< pimpl > _gui;

		int32_t _color_id;
		bool _ready;

		// Replaces the default no-transition response.
		template <class FSM,class Event>
		void no_transition(Event const& e, FSM&,int state)
		{
			std::cout << "no transition from state " << state
				<< " on event " << typeid(e).name() << std::endl;
		}
    };
    // Pick a back-end
    typedef msm::back::state_machine< SelectColorFSM_ > selectColorFSM;
}

#endif /* SELECTCOLORFSM_H_ */
