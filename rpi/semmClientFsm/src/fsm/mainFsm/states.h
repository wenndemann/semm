/*
 * states.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef STATES_H_
#define STATES_H_

// The list of FSM states

struct Init : public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM& fsm) {
		std::cout << "-> Init" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.begin( );
		for ( ; it != displayMap.end( ); ++it )
		{	it->second->setPictures( I2C_DBEN_PIC_SEMM );	}

		fsm._tcpIp->start( fsm._gamePtr );
		while( !fsm._tcpIp->running( ) )
		{ boost::this_thread::sleep( boost::posix_time::milliseconds( 50 ) ); }
		fsm._tcpIp->sendAddClient( "SemmBox" );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- Init" << std::endl;
	}
};

struct WaitForAvailColors : public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> WaitForAvailColors" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- WaitForAvailColors" << std::endl;
	}
};

struct WaitForClientColors : public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> WaitForClientColors" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- WaitForClientColors" << std::endl;
	}
};

struct SelectColorMode : public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM& fsm) {
		std::cout << "-> SelectColorMode" << std::endl;

		for ( int32_t i = 0; i < fsm._ssms->size( ); i++ )
		{	fsm._ssms->at( i ).reset( new fsm::selectColorFSM( pow(2,i), fsm._gamePtr ) );	}
		std::cout << "   subFSMs created!" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		for ( Playboard::DisplayMapIt it = displayMap.begin( ); it != displayMap.end( ); ++it )
		{	it->second->enableSubFSMEvents( );	}

		for ( boost::uint32_t i = 0; i < fsm._ssms->size( ); i++ )
		{
			if ( fsm._ssms->at( i ) )
			{
				fsm._ssms->at( i )->start( );
				//fsm._ssms->at( i )->process_event( fsm::scmInitialize( ) );
			}
		}

	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM& fsm) {
		std::cout << "<- SelectColorMode" << std::endl;

		for ( boost::uint32_t i = 0; i < fsm._ssms->size( ); i++ )
		{
			fsm._ssms->at( i )->stop( );
			//fsm._ssms->at( i ).reset( );
		}
		Playboard::DisplayMap displays = fsm._gamePtr->playboard( )->displays( );
		Playboard::DisplayMapIt it = displays.begin( );
		for ( ; it != displays.end( ); ++it )
		{
			it->second->disableSubFSMEvents( );
		}
	}
};

struct GmMoveDone : public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM& fsm) {
		std::cout << "-> GmMoveDone" << std::endl;

		// if the dice data for the next turn has been sent already
		// save it to the current dice data and do the dice event
		// to start the next turn
		if ( fsm._next.valid &&
		     ( fsm.get_message_queue_size() + fsm.get_deferred_queue( ).size( ) ) == 0 )
		{
			fsm._curr = fsm._next;
			fsm.enqueue_event( fsm::evDice( ) );
		}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveDone" << std::endl;
	}
};

struct GmDice: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmDice" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt displayMapIt = displayMap.find( static_cast<int32_t>(fsm._curr.player) );
		if ( displayMapIt == displayMap.end( ) )
		{
			std::cout << "No player " << static_cast<int32_t>(fsm._curr.player)
					  << " to move on dice " << static_cast<int32_t>(fsm._curr.dice) << std::endl;
		}

		displayMapIt->second->setPictureDice( 0 ); //ev._dice );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM& fsm) {
		std::cout << "<- GmDice" << std::endl;
	}
};

struct GmWaitForShowDice: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM& fsm) {
		std::cout << "-> GmWaitForShowDice" << std::endl;
		fsm._tcpIp->sendDieDone( fsm._curr.player );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmWaitForShowDice" << std::endl;
	}
};

struct GmShowDice: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmShowDice" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmShowDice" << std::endl;
	}
	bool _moveAllowed;
};

struct GmMoveMeeple : public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmMoveMeeple" << std::endl;

		fsm._gamePtr->playboard( )->moveMeeple( ev._from, ev._to );

		fsm.process_event( fsm::evMoveDone( ) );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveMeeple" << std::endl;
	}
};

struct GmCheckMovedMeeple: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmCheckMovedMeeple" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmCheckMovedMeeple" << std::endl;
	}
};

struct GmSendMovedMeeple: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmSendMovedMeeple" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmSendMovedMeeple" << std::endl;
	}
};

struct GmCheckDestination: public msm::front::state<>
{
	typedef mpl::vector<fsm::evDice> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmCheckDestination" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmCheckDestination" << std::endl;
	}
};

struct GmSearchForMeeple: public msm::front::state<>
{
	typedef mpl::vector<fsm::evDice> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmCheckDestination" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmCheckDestination" << std::endl;
	}
};

struct GmFoundMeeple: public msm::front::state<>
{
	typedef mpl::vector<fsm::evDice> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmFoundMeeple" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmFoundMeeple" << std::endl;
	}
};

struct GmMoveMeeplesByHand: public msm::front::state<>
{
	typedef mpl::vector<fsm::evDice> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmMoveMeeplesByHand" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveMeeplesByHand" << std::endl;
	}
};

struct GmReconfigureMeepleIDs: public msm::front::state<>
{
	typedef mpl::vector<fsm::evDice> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmReconfigureMeepleIDs" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmReconfigureMeepleIDs" << std::endl;
	}
};

struct GmMoveMeeplesToCorrectPos: public msm::front::state<>
{
	typedef mpl::vector<fsm::evDice> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmMoveMeeplesToCorrectPos" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveMeeplesToCorrectPos" << std::endl;
	}
};

typedef Init initial_state;
typedef int activate_deferred_events;


#endif /* STATES_H_ */
