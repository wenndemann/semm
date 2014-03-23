/*
 * events.h
 *
 *  Created on: Mar 16, 2014
 *      Author: reinhard
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#ifdef FSM_EVENT_COUT
#undef FSM_EVENT_COUT
#endif
#define FSM_EVENT_COUT( event_name ) ( std::cout << "E " << event_name << std::endl )

namespace fsm
{

	// events
	/*
	struct play { };
	struct end_pause { };
	struct stop { };
	struct paus { };
	struct open_close { };
	*/

	struct initGame { initGame( ){ FSM_EVENT_COUT( "initGame" ); } };
	struct initColors { initColors( ){ FSM_EVENT_COUT( "initColors" ); } };
	struct clientColors { clientColors( ){ FSM_EVENT_COUT( "clientColors" ); } };

	// A "complicated" event type that carries some data.
	/*
	enum DiskTypeEnum {
		DISK_CD = 0, DISK_DVD = 1
	};
	struct cd_detected {
		cd_detected(std::string name, DiskTypeEnum diskType) :
				name(name), disc_type(diskType) {
		}

		std::string name;
		DiskTypeEnum disc_type;
	};
	*/
}

#endif /* EVENTS_H_ */
