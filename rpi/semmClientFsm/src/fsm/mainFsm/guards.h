/*
 * guards.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef GUARDS_H_
#define GUARDS_H_

// guard conditions
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

#endif /* GUARDS_H_ */
