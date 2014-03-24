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
		{	it->second->init( );	}

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
	}
};

struct GmMoveDone : public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> GmMoveDone" << std::endl;
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
	void on_entry(Event const&, FSM&) {
		std::cout << "-> GmDice" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmDice" << std::endl;
	}
};

struct GmWaitForShowDie: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> GmWaitForShowDie" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmWaitForShowDie" << std::endl;
	}
};

struct GmShowDie: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> GmShowDie" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmShowDie" << std::endl;
	}
};

struct GmMoveMeeple : public msm::front::state<>
{
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


/*
struct Empty: public msm::front::state<> {
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> Empty" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- Empty" << std::endl;
	}
};

struct Open: public msm::front::state<> {
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> Open" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- Open" << std::endl;
	}
};

struct Stopped: public msm::front::state<> {
	// when stopped, the CD is loaded
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> Stopped" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- Stopped" << std::endl;
	}
};

struct Playing: public msm::front::state<> {
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "-> Playing" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- Playing" << std::endl;
	}
};

// state not defining any entry or exit
struct Paused: public msm::front::state<> {
};
*/
// the initial state of the player SM. Must be defined
typedef Init initial_state;
typedef int activate_deferred_events;


#endif /* STATES_H_ */
