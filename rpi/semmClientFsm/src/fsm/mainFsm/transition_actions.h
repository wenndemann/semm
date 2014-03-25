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

		for ( boost::uint32_t i = 0; i < fsm._ssms->size( ); i++ )
		{
			fsm._ssms->at( i ).reset( );
		}
		/*
		Playboard::DisplayMap displays = fsm._gamePtr->playboard( )->displays( );
		Playboard::DisplayMapIt it = displays.begin( );
		for ( ; it != displays.end( ); ++it )
		{
			it->second->subFSMPtr( ).reset( );
		}
		*/
	}
};

struct showDice {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const& ev, FSM& fsm, SourceState& src, TargetState& target) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;


		Playboard::DisplayMap displayMap = fsm._gamePtr->playboard()->displays( );
		Playboard::DisplayMapIt it = displayMap.find( static_cast<int32_t>(fsm._curr.player) );
		/*if ( it == displayMap.end( ) )
		{
			std::cout << "No player " << static_cast<int32_t>(fsm._curr.player)
					  << " to move on dice " << static_cast<int32_t>(fsm._curr.dDice) << std::endl;
		}*/
		for ( ; it != displayMap.end( ); ++it )
		{
			it->second->setPictureDice( fsm._curr.dice );
		}

		target._moveAllowed = ev._moveAllowed;
	}
};

struct checkDice {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		// if the first dice data is being sent during the GmMoveDone,
		// current (curr) DiceData is not set, so simply set it.
		if ( !fsm._curr.valid && fsm._next.valid )
		{
			fsm._curr = fsm._next;
		}
	}
};


/*

*/

#endif /* TRANSITION_ACTIONS_H_ */
