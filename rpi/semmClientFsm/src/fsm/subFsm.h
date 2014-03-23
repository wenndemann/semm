/*
 * selectColorFsm.h
 *
 *  Created on: 22.03.2014
 *      Author: micha
 */

#ifndef SELECTCOLORFSM_H_
#define SELECTCOLORFSM_H_

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

using namespace std;
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
	struct scmInitialize { scmInitialize( ){ FSM_EVENT_COUT( "scmInitialize" ); } };
	struct scmEnter { scmEnter( ){ FSM_EVENT_COUT( "scmEnter" ); } };
	struct scmMeeplesNotOK { scmMeeplesNotOK( ){ FSM_EVENT_COUT( "scmMeeplesNotOK" ); } };
	struct scmMeeplesOK { scmMeeplesOK( ){ FSM_EVENT_COUT( "scmMeeplesOK" ); } };

	// front-end: define the FSM structure
	struct SelectColorFSM_ : public msm::front::state_machine_def<SelectColorFSM_>
    {
		SelectColorFSM_( int32_t color_id, boost::shared_ptr< Game > gamePtr )
		: _gamePtr( gamePtr )
		, _color_id( color_id )
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

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_SEMM );
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

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_ENTER );
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
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmDisable " << fsm._color_id << std::endl;
			}
		};
		struct ScmPrepare : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmPrepare " << fsm._color_id << std::endl;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_PREPARE );
			}
			template<class Event, class FSM>
			void on_exit(Event const&, FSM& fsm) {
				std::cout << "<- ScmPrepare " << fsm._color_id << std::endl;
			}
		};
		struct ScmWait : public msm::front::state<>
		{
			// every (optional) entry/exit methods get the event passed.
			template<class Event, class FSM>
			void on_entry(Event const&, FSM& fsm) {
				std::cout << "-> ScmWait " << fsm._color_id << std::endl;

				fsm._gamePtr->playboard()->display( fsm._color_id )->setPictures( I2C_DBEN_PIC_WAIT );
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

		// guard conditions
		struct accept
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				return true;
				//return ( fsm.gamePtr->mode( ) == Game::Mode::SelectColor );
			}
		};
		struct blocked
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				return false;
				//return ( fsm.gamePtr->mode( ) == Game::Mode::SelectColor );
			}
		};
		struct known
		{
			template<class EVT, class FSM, class SourceState, class TargetState>
			bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt)
			{
				return false;
				//return ( fsm.gamePtr->mode( ) == Game::Mode::SelectColor );
			}
		};

		// Initial state(s)
		typedef ScmSemm initial_state;

		// Transition table for player
		struct transition_table : mpl::vector<
			//    Start                     Event           Next                      Action        Guard
			//  +------------------------+----------------+------------------------+--- ---------+-----------------+
			Row < ScmSemm                , scmInitialize  , ScmEnter               , none        , accept          >,
			Row < ScmSemm                , scmInitialize  , ScmDisable             , none        , blocked         >,
			Row < ScmSemm                , scmInitialize  , ScmPrepare             , none        , known           >,
			//  +------------------------+----------------+------------------------+--- ---------+-----------------+
			Row < ScmEnter               , scmEnter       , ScmPrepare             , none        , none            >,
			//  +------------------------+----------------+------------------------+--- ---------+-----------------+
			Row < ScmPrepare             , scmEnter       , ScmWait                , none        , none            >,
			//  +------------------------+----------------+------------------------+--- ---------+-----------------+
			Row < ScmWait                , scmMeeplesNotOK, ScmEnter               , none        , none            >,
			//  +------------------------+----------------+------------------------+--- ---------+-----------------+
			Row < ScmWait                , scmMeeplesOK   , ScmStart               , none        , none            >
		> {};

		// Replaces the default no-transition response.
		template <class FSM,class Event>
		void no_transition(Event const& e, FSM&,int state)
		{
			std::cout << "no transition from state " << state
				<< " on event " << typeid(e).name() << std::endl;
		}

		boost::shared_ptr< Game > _gamePtr;
		int32_t _color_id;
    };
    // Pick a back-end
    typedef msm::back::state_machine< SelectColorFSM_ > selectColorFSM;
}

#endif /* SELECTCOLORFSM_H_ */
