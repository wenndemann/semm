/*
 * transition_actions.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef TRANSITION_ACTIONS_H_
#define TRANSITION_ACTIONS_H_

// transition actions
// as the functors are generic on events, fsm and source/target state,
// you can reuse them in another machine if you wish


struct delSsms {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		int32_t clientColors = fsm._gamePtr->clientColors();
		int32_t availableColors = fsm._gamePtr->availableColors();

		Playboard::DisplayMap& displays = fsm._gamePtr->playboard( )->displays( );
		Playboard::DisplayMapIt it = displays.begin( );
		std::vector< Playboard::DisplayMapIt > vecDisplayToBeErased;
		for ( ; it != displays.end( ); ++it )
		{
			int32_t color = it->second->subFSM()->_color_id;
			// check whether its a player on playboard
			if ( !(color & clientColors) )
			{
				// check whether the player is active
				if ( color & availableColors )
				{
					it->second->setPictures( I2C_DBEN_PIC_SEMM );
				}
				else
				{
					it->second->setPictures( I2C_DBEN_PIC_DISABLED );
				}
				vecDisplayToBeErased.push_back( it );
			}
		}

		for ( uint32_t i = 0; i < vecDisplayToBeErased.size( ); i++ )
		{
			displays.erase( vecDisplayToBeErased[ i ] );
		}
	}
};

struct sendDiceDone {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const& ev, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		fsm._tcpIp->sendDieDone( fsm._currDD.player );
	}
};

struct showDice {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const& ev, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;


		/*Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.find( static_cast<int32_t>(fsm._curr.player) );
		if ( it == displayMap.end( ) )
		{
			std::cout << "No player " << static_cast<int32_t>(fsm._curr.player)
					  << " to move on dice " << static_cast<int32_t>(fsm._curr.dDice) << std::endl;
		}*/

		target._moveAllowed = ev._moveAllowed;
	}
};

struct popDiceData {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		if( fsm._ddm.size( ) > 0 )
		{
			std::cout << "Initiaizing the next DiceData with color " << static_cast<int32_t>(fsm._ddm.front().player)
								  << " and dice result " << static_cast<int32_t>(fsm._ddm.front().dice) << std::endl;
			std::cout << "pop from DiceData deque" << std::endl;
			fsm._currDD = fsm._ddm.front( );
			fsm._ddm.pop_front();

			std::cout << "LED___________ on for player " << static_cast<int32_t>(fsm._currDD.player) << std::endl;
			fsm._gamePtr->playboard()->ledStripe()->set(LedStripes::Mode::ON, fsm._currDD.player);
		}
		else
		{ std::cout << "No dicedata for popDiceData!" << std::endl; }

	}
};

struct waitSomeTime {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		boost::this_thread::sleep( boost::posix_time::seconds( 3 ) );
	}
};

struct sendMovedMeeple {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		target._fromFieldId = src._fromFieldId;
	}
};

struct meepleMoved {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		if ( fsm._gui )
		{
			// visualization
			uint8_t color = fsm._currDD.player;
			if ( fsm._gamePtr->playboard( )->getMeepleFromFieldId( src._from ) )
			{
				uint16_t tag = fsm._gamePtr->playboard( )->getMeepleFromFieldId( src._from )->tag( );
				fsm._gui->setMeeplePos( color, tag, src._to );
			}
		}

		fsm._gamePtr->playboard( )->setMeepleMove( src._from, src._to );
	}
};

struct transferFromTo {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		target._from = src._from;
		target._to = src._to;
	}
};

struct transferFromToIllegal {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		target._from = src._from;
		target._to = src._to;
		target._x = src._x;
		target._y = src._y;
	}
};

struct transferFromToAll {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		target._fromAll = src._fromAll;
		target._toAll = src._toAll;
	}
};

struct resetMove {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;
		std::cout << "This move was illegal. Searching for meeple." << std::endl;

		uint8_t x, y;
		bool found = fsm._gamePtr->playboard( )->searchForMeeple( src._fromFieldId, x, y );

		while( !found ) {
			std::cout << "  Not found. Please reset manual." << std::endl;
			Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
			Playboard::DisplayMapIt it = displayMap.find( fsm._currDD.player );
			if ( it != displayMap.end( ) )
				{	it->second->setPictures( I2C_DBEN_PIC_NOT_FOUND );	}
			boost::this_thread::sleep( boost::posix_time::seconds( 5 ) );
			found = fsm._gamePtr->playboard( )->searchForMeeple( src._fromFieldId, x, y );
		}

		std::cout << "  Found illegal moved meeple and set it "
				  << "back to its original Positition" << endl;
		fsm._gamePtr->playboard( )->moveMeepleXY( fsm._currDD.player, x, y, src._fromFieldId );
	}
};

/*

*/

#endif /* TRANSITION_ACTIONS_H_ */
