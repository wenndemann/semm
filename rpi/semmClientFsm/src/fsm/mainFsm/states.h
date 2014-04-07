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
		{
			it->second->enableSubFSMEvents( );
		}

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

//		if ( !fsm._gameStarted ){ fsm._gameStarted = true; }


//		for ( uint8_t i = 0; i < 4; i++ )
//		{ fsm._gamePtr->playboard( )->ledStripe( )->set(LedStripes::OFF, static_cast<uint8_t>(1<<i)); }
		fsm._gamePtr->playboard( )->ledStripe( )->setAllOff( );

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.begin( );
		for ( ; it != displayMap.end( ); ++it )
		{
			if ( it->second )
				it->second->setPictures( I2C_DBEN_PIC_WAIT );
		}

		// if the dice data for the next turn has been sent already
		// do the dice event to start the next turn
		if ( !fsm._movesAtBeginning.empty( ) )
		{
			fsm::evMove moveAtBeginning = fsm._movesAtBeginning.front( );
			fsm._movesAtBeginning.pop_front();
			fsm.process_event( moveAtBeginning );
		}
		else if ( /*fsm.get_deferred_queue( ).size( ) == 0 &&*/ fsm._ddm.size() > 0 )
		{
			fsm.process_event( fsm::evDice( ) );
		}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveDone" << std::endl;
	}
};

struct GmDice: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmDice" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt displayMapIt = displayMap.find( static_cast<int32_t>(fsm._currDD.player) );
		if ( displayMapIt == displayMap.end( ) )
		{
			std::cout << "No player " << static_cast<int32_t>(fsm._currDD.player)
					  << " to move on dice " << static_cast<int32_t>(fsm._currDD.dice) << std::endl;
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
	typedef mpl::vector<fsm::evMove> deferred_events;
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM& fsm) {
		std::cout << "-> GmWaitForShowDice" << std::endl;

		if ( !fsm._deque_show_dice.empty( ) )
		{
			std::cout << "deque_show_dice.pop( )" << std::endl;
			uint8_t moveAllowed = fsm._deque_show_dice.front( );
			fsm._deque_show_dice.pop_front( );
			fsm.process_event(fsm::evShowDice( moveAllowed ));
		}
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

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.begin( );
		for ( ; it != displayMap.end( ); ++it )
		{
			if ( _moveAllowed && it == displayMap.find( static_cast<int32_t>(fsm._currDD.player) )
			    && ( fsm._currDD.player & static_cast<uint8_t>(fsm._gamePtr->clientColors( ) ) ) )
			{ it->second->setPictureDice( fsm._currDD.dice, true ); }
			else
			{ it->second->setPictureDice( fsm._currDD.dice ); }
		}
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

//		uint8_t color = fsm._gamePtr->playboard( )->getColorFromFieldId( ev._from );
		uint16_t tag = 0;
		MeeplePtr miep = fsm._gamePtr->playboard( )->getMeepleFromFieldId( ev._from );
		if ( miep ){ tag = miep->tag( ); }

		if ( ev._from == ev._to )
		{
			fsm.process_event( fsm::evMoveDone( ) );
		}
		else
		{
			uint16_t fromTag = fsm._gamePtr->playboard( )->readId( static_cast<uint32_t>( ev._from ) );
			if ( fromTag == tag ) // check whether the meeple is still there
			{
				fsm._gamePtr->playboard( )->moveMeeple( ev._from, ev._to );
				fsm.process_event( fsm::evMoveDone( ) );
			}
			else // if not check the destination
			{
				fsm.process_event( fsm::evCheckDst( ev._from, ev._to ) );
			}
		}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveMeeple" << std::endl;
	}

	//uint8_t _from, _to; // TODO fill 'em
};

struct GmCheckMovedMeeple: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmCheckMovedMeeple" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.begin( );
		for ( ; it != displayMap.end( ); ++it )
		{
			it->second->setPictures( I2C_DBEN_PIC_WAIT );
		}

		uint8_t fieldId = fsm._gamePtr->playboard()->checkMovedMeeple( fsm._currDD.player );
		if ( fieldId == 255 ) // no moved meeple found
		{
			fsm.process_event( fsm::evMeepleNotOK( ) );
		}
		else
		{
			_fromFieldId = fieldId;
			fsm.process_event( fsm::evMeepleOK( ) );
		}

	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmCheckMovedMeeple" << std::endl;
	}

	uint8_t _fromFieldId;
};

struct GmSendMovedMeeple: public msm::front::state<>
{
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmSendMovedMeeple" << std::endl;

		std::cout << "Sending select meeple color " << static_cast<int32_t>(fsm._currDD.player)
				  << " from field " << static_cast<int32_t>(_fromFieldId) << std::endl;
		fsm._tcpIp->sendSelectMeeple( fsm._currDD.player, _fromFieldId );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmSendMovedMeeple" << std::endl;
	}

	uint8_t _fromFieldId;
};

struct GmCheckDestination: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmCheckDestination" << std::endl;

		_from = ev._from;
		_to = ev._to;

		bool meepleMoveOk = fsm._gamePtr->playboard( )->checkMeepleMove(
				static_cast<uint8_t>(ev._from), static_cast<uint8_t>(ev._to) );

		if ( meepleMoveOk )
		{
			fsm.process_event( fsm::evMoveDone( ) );
		}
		else
		{
			fsm.process_event( fsm::evMeepleNotOK( ) );
		}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmCheckDestination" << std::endl;
	}

	uint8_t _from, _to;
};

struct GmSearchForMeeple: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmSearchForMeeple" << std::endl;

		// searchForMeeple consists of the following steps:
		// - All fields get scanned for the meeple which has been on _from before
		// - If found it's illegal position will be saved to _illegal (by reference)
		// - the state whether the meeple has been found or not will be returned as a boolean
		_x = 0; _y = 0;
		if ( fsm._gamePtr->playboard( )->searchForMeeple( _from, _x, _y ) )
		{
			std::cout << "Illegal meeple found on fieldId ("
					<< static_cast<int32_t>(_x) << "/"
					<< static_cast<int32_t>(_y) << ")" << std::endl;
			fsm.process_event( fsm::evMeepleOK( ) );
		}
		else
		{
			std::cout << "Illegal meeple not found!" << std::endl;
			fsm.process_event( fsm::evMeepleNotOK( ) );
		}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmSearchForMeeple" << std::endl;
	}

	uint8_t _from, _to, _x, _y;
};

struct GmFoundMeeple: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmFoundMeeple" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.find( fsm._currDD.player );
		if ( it != displayMap.end( ) )
		{	it->second->setPictures( I2C_DBEN_PIC_MOVE_ILLEGAL );	}

		// move the meeple found from its illegal position to the actual target position to
		std::cout << "move meeple from illegal fieldId "
			<< "(" << static_cast<int32_t>(_x) << "/" << static_cast<int32_t>(_y) << ")"
			<< " to " << " actual target fieldId " << static_cast<int32_t>(_to) << std::endl;
		fsm._gamePtr->playboard( )->moveMeepleXY( fsm._currDD.player, _x, _y, _to );
		fsm._gamePtr->playboard( )->setMeepleMove( _from, _to);

		//boost::this_thread::sleep( boost::posix_time::seconds( 1 ) );

		fsm.process_event( fsm::evMoveDone( ) );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmFoundMeeple" << std::endl;
	}

	uint8_t _from, _to, _x, _y;
};

struct GmMoveMeeplesByHand: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmMoveMeeplesByHand" << std::endl;

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.find( fsm._gamePtr->playboard()->getColorFromFieldId(_from) );//fsm._currDD.player );
		if ( it != displayMap.end( ) )
		{	it->second->setPictures( I2C_DBEN_PIC_PREPARE );	}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveMeeplesByHand" << std::endl;
	}

	uint8_t _from, _to;
};

struct GmReconfigureMeepleIDs: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmReconfigureMeepleIDs" << std::endl;

		int32_t color = fsm._gamePtr->playboard()->getColorFromFieldId(_from);

		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.find( color );
		if ( it != displayMap.end( ) )
		{	it->second->setPictures( I2C_DBEN_PIC_WAIT );	}

		// All meeple of current player should be on their starting positions.
		// Check it by reading their tags and reconfigure if successful.
		std::vector< uint16_t > tags( 3 );
		if ( fsm._gamePtr->playboard( )->readPlayersMeepleAtStart( color, tags ) )
		{
			// save the new "to" field to the missing's meeple's "to" field, to move the meeples
			// to the correct position in the next state when moving them from the starting position
			fsm._gamePtr->playboard( )->setMeepleMove( _from, _to );

			// saving all "to" fields
			Playboard::PlayerMapIt playerIt =
				fsm._gamePtr->playboard( )->players( ).find( color );
			// error handling
			if ( playerIt == fsm._gamePtr->playboard( )->players( ).end( ) )
			{	 fsm.process_event( fsm::evMeepleNotOK( ) ); 	}
			// find the right meeple
			// copy the new "to" to the old "to"
			MeepleVec& meeples = playerIt->second->meeples( );
			// error handling
			if ( meeples.size( ) != tags.size( ) )
			{	 fsm.process_event( fsm::evMeepleNotOK( ) ); 	}
			// save their fieldIds
			_toAll.resize( meeples.size( ), 0 ); // initialize with meeples size, should be 3 and value 0
			assert( _toAll.size( ) == 3 );
			for ( uint8_t i = 0; i < meeples.size( ); i++ )
			{ _toAll.at( i ) = static_cast<uint8_t>(meeples.at( i )->fieldId( )); }

			if ( fsm._gamePtr->playboard( )->reconfigurePlayersMeeple( color, tags ) )

			// saving the new fromFieldIds
			// error handling
			if ( meeples.size( ) != tags.size( ) )
			{	 fsm.process_event( fsm::evMeepleNotOK( ) ); 	}
			// save their fieldIds
			_fromAll.resize( meeples.size( ), 0 ); // initialize with meeples size, should be 3 and value 0
			assert( _fromAll.size( ) == 3 );
			for ( uint8_t i = 0; i < meeples.size( ); i++ )
			{ _fromAll.at( i ) = static_cast<uint8_t>(meeples.at( i )->fieldId( )); }

			fsm.process_event( fsm::evMeepleOK( ) );
		}
		else
		{
			fsm.process_event( fsm::evMeepleNotOK( ) );
		}
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmReconfigureMeepleIDs" << std::endl;
	}

	uint8_t _from, _to;
	std::vector< uint8_t > _fromAll, _toAll;
};

struct GmMoveMeeplesToCorrectPos: public msm::front::state<>
{
	typedef mpl::vector<fsm::evMove> deferred_events;

	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const& ev, FSM& fsm) {
		std::cout << "-> GmMoveMeeplesToCorrectPos" << std::endl;

		assert(_fromAll.size( ) == _toAll.size( ) );
		for ( uint32_t i = 0; i < _fromAll.size( ); i++ )
		{
			fsm._gamePtr->playboard( )->moveMeeple( _fromAll.at( i ), _toAll.at( i ) );
		}
		fsm.process_event( fsm::evMoveDone( ) );
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "<- GmMoveMeeplesToCorrectPos" << std::endl;
	}

	uint8_t _from, _to;
	std::vector< uint8_t > _fromAll, _toAll;
};

typedef Init initial_state;
typedef int activate_deferred_events;


#endif /* STATES_H_ */
