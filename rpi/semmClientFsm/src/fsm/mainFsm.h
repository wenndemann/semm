#ifndef MAINFSM_H_
#define MAINFSM_H_

#include <vector>
#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>


using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

namespace  // Concrete FSM implementation
{
	// events
	#include "mainFsm/events.h"

    // front-end: define the FSM structure
    struct player_ : public msm::front::state_machine_def<player_>
    {
        template <class Event,class FSM>
        void on_entry(Event const& ,FSM&)
        {
            std::cout << "entering: Player" << std::endl;
        }
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& )
        {
            std::cout << "leaving: Player" << std::endl;
        }

        // The list of FSM states
		#include "mainFsm/states.h"

        // The list of FSM transition actions
		#include "mainFsm/transition_actions.h"

        // guard conditions
		#include "mainFsm/guards.h"

        // Transition table for player
		#include "mainFsm/transition_table.h"

        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
    // Pick a back-end
    typedef msm::back::state_machine<player_> player;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Open", "Empty", "Playing", "Paused" };
    void pstate(player const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }
}

#endif /* MAINFSM_H_ */
