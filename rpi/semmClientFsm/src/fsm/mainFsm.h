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

#include <boost/shared_ptr.hpp>

using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

// cout manipulation
#ifdef FSM_COUT_HAS_BEEN_MODIFIED
#undef FSM_COUT_HAS_BEEN_MODIFIED
#endif

#ifdef cout
#define FSM_COUT_HAS_BEEN_MODIFIED
#pragma push_macro("cout")
#endif

#define cout cout << "FSM: "

// forward declarations
class Game;

namespace  // Concrete FSM implementation
{
	// events
	#include "mainFsm/events.h"

    // front-end: define the FSM structure
    struct GameFSM_ : public msm::front::state_machine_def<GameFSM_>
    {
    	GameFSM_( Game* game )
		{
    		gamePtr.reset( game );
		}

        template <class Event,class FSM>
        void on_entry(Event const& ,FSM&)
        {
            std::cout << "entering: Game" << std::endl;
        }
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& )
        {
            std::cout << "leaving: Game" << std::endl;
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

        boost::shared_ptr< Game > gamePtr;
    };
    // Pick a back-end
    typedef msm::back::state_machine< GameFSM_ > gameFSM;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = {
    	"Init",
    	"WaitForAvailColors",
    	"WaitForClientColors",
    	"SelectColorMode",
    	"GmWaitForPlayGround"
    };

    void pstate(game const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }
}

// revert cout manipulation
#undef cout
#ifdef FSM_COUT_HAS_BEEN_MODIFIED
#pragma pop_macro( "cout" )
#undef FSM_COUT_HAS_BEEN_MODIFIED
#endif

#endif /* MAINFSM_H_ */
