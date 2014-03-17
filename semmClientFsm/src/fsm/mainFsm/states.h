/*
 * states.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef STATES_H_
#define STATES_H_

// The list of FSM states
struct Empty: public msm::front::state<> {
	// every (optional) entry/exit methods get the event passed.
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "entering: Empty" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "leaving: Empty" << std::endl;
	}
};

struct Open: public msm::front::state<> {
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "entering: Open" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "leaving: Open" << std::endl;
	}
};

struct Stopped: public msm::front::state<> {
	// when stopped, the CD is loaded
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "entering: Stopped" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "leaving: Stopped" << std::endl;
	}
};

struct Playing: public msm::front::state<> {
	template<class Event, class FSM>
	void on_entry(Event const&, FSM&) {
		std::cout << "entering: Playing" << std::endl;
	}
	template<class Event, class FSM>
	void on_exit(Event const&, FSM&) {
		std::cout << "leaving: Playing" << std::endl;
	}
};

// state not defining any entry or exit
struct Paused: public msm::front::state<> {
};

// the initial state of the player SM. Must be defined
typedef Empty initial_state;

#endif /* STATES_H_ */
