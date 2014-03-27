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

		//for ( boost::uint32_t i = 0; i < fsm._ssms->size( ); i++ )
		//{
		//	fsm._ssms->at( i ).reset( );
		//}
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

struct checkDice {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;

		// if the first dice data is being sent during the GmMoveDone,
		// current (curr) DiceData is not set, so simply set it.
		if ( !fsm._curr.valid && fsm._next.valid )
		{
			std::cout << "Setting the first DiceData in checkDice" << std::endl;
			fsm._curr = fsm._next;
		}
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

		fsm._gamePtr->playboard( )->setMeepleMove( fsm._curr.player, src._from, src._to );
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
		target._illegal = src._illegal;
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

/*

*/

#endif /* TRANSITION_ACTIONS_H_ */
