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
struct TestFct {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "transition with event:" << typeid(EVT).name() << endl;
	}
};
struct start_playback {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::start_playback" << endl;
	}
};
struct open_drawer {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::open_drawer" << endl;
	}
};
struct close_drawer {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::close_drawer" << endl;
	}
};
struct store_cd_info {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
		cout << "player::store_cd_info" << endl;
		fsm.process_event(play());
	}
};
struct stop_playback {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::stop_playback" << endl;
	}
};
struct pause_playback {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::pause_playback" << endl;
	}
};
struct resume_playback {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::resume_playback" << endl;
	}
};
struct stop_and_open {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::stop_and_open" << endl;
	}
};
struct stopped_again {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		cout << "player::stopped_again" << endl;
	}
};

#endif /* TRANSITION_ACTIONS_H_ */
