#ifndef MAINFSM_H_
#define MAINFSM_H_

#include "../defs.h"

#include <vector>
#include <queue>
#include <algorithm>
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

//using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

#include "subFsm.h"

// Game
#include "../game/Game.h"

// TcpIp
#include "../tcp_ip/TcpIp.h"

// events
#include "mainFsm/events.h"

namespace fsm // Concrete FSM implementation
{
	typedef std::vector< boost::shared_ptr< selectColorFSM > > SsmsVec;
	typedef boost::shared_ptr< SsmsVec > SsmsVecPtr;

	// front-end: define the FSM structure
    struct GameFSM_ : public msm::front::state_machine_def<GameFSM_>
    {
    	GameFSM_( TcpIp* tcpIp )
    	: _tcpIp( tcpIp )
    	{ }

    	template <class Event,class FSM>
    	void on_entry(Event const&, FSM& fsm)
    	{
    		std::cout << "-> gameFSM" << std::endl;
    		_gamePtr.reset( new Game( &fsm ) );
    		std::cout << "   game created!" << std::endl;

    		fsm._ssms.reset( new fsm::SsmsVec(4) );
    	}

    	template <class Event,class FSM>
    	void on_exit(Event const&, FSM& fsm)
    	{
    		std::cout << "<- gameFSM" << std::endl;
    		_gamePtr.reset( );
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

        TcpIp* _tcpIp;
        boost::shared_ptr< Game > _gamePtr;
        SsmsVecPtr _ssms;

        struct DiceData
        {
        	DiceData( ) : player( 0 ), dice( 0 ), valid( false ){ }
        	uint8_t player, dice;
        	bool valid;
        };
        std::queue<DiceData> _ddm;
    };
    // Pick a back-end
    typedef msm::back::state_machine< GameFSM_ > gameFSM;

}

#endif /* MAINFSM_H_ */
