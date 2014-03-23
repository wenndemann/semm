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

	struct evInitGame { evInitGame( ){ FSM_EVENT_COUT( "evInitGame" ); } };
	struct evInitColors { evInitColors( ){ FSM_EVENT_COUT( "evInitColors" ); } };
	struct evClientColors { evClientColors( ){ FSM_EVENT_COUT( "evClientColors" ); } };
	struct evMove{ evMove( uint8_t from, uint8_t to )
	: _from( from ), _to( to )
	{
		FSM_EVENT_COUT( "evMove"); }
		uint8_t _from, _to;
	};
	struct evMoveDone { evMoveDone( ){ FSM_EVENT_COUT( "evMoveDone" ); } };

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
