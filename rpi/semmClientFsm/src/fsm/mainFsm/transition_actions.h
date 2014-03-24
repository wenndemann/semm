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
	}
};
/*
struct TestFct {
	template<class EVT, class FSM, class SourceState, class TargetState>
	void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
		std::cout << "transition with event:" << typeid(EVT).name() << std::endl;
	}
};
*/

#endif /* TRANSITION_ACTIONS_H_ */
