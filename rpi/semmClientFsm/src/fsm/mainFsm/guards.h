/*
 * guards.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef GUARDS_H_
#define GUARDS_H_

#include "../../game/Game.h"

// guard conditions
struct gMoveAllowed {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt) {
		return src._moveAllowed;
	}
};
struct gMoveNotAllowed {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt) {
		return !(src._moveAllowed);
	}
};
struct gMyColor {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt) {

		bool thisClient = false;
		if ( !fsm._ddm.empty( ) )
		{
			uint8_t player = ( fsm._ddm.size( ) > 1 ) ? fsm._ddm[ 1 ].player : fsm._ddm[ 0 ].player;
			fsm._gamePtr->playboard()->ledStripe()->set(LedStripes::Mode::ON, player);
			thisClient = static_cast<uint8_t>(fsm._gamePtr->clientColors( ) ) & player;
		}
		return thisClient;
	}
};
struct gNotMyColor {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM& fsm, SourceState& src, TargetState& tgt) {

		bool notThisClient = false;
		if ( !fsm._ddm.empty( ) )
		{
			uint8_t player = ( fsm._ddm.size( ) > 1 ) ? fsm._ddm[ 1 ].player : fsm._ddm[ 0 ].player;
			fsm._gamePtr->playboard()->ledStripe()->set(LedStripes::Mode::ON, player);
			notThisClient = !(static_cast<uint8_t>(fsm._gamePtr->clientColors( ) ) & player);
		}
		return notThisClient;
	}
};
/*
struct DummyGuard {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM& fsm, SourceState& src,
			TargetState& tgt) {
		return true;
	}
};
struct good_disk_format {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM&, SourceState&, TargetState&) {
		// to test a guard condition, let's say we understand only CDs, not DVD
		if (evt.disc_type != DISK_CD) {
			std::cout << "wrong disk, sorry" << std::endl;
			return false;
		}
		return true;
	}
};
struct always_true {
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool operator()(EVT const& evt, FSM&, SourceState&, TargetState&) {
		return true;
	}
};
*/

#endif /* GUARDS_H_ */
