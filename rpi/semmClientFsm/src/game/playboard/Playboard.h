/*
 * Playboard.h
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#ifndef PLAYBOARD_H_
#define PLAYBOARD_H_


// local includes
#include "../Player.h"
#include "Field.h"
#include "hardware/Display.h"
#include "hardware/LedRing.h"
#include "hardware/LedStripes.h"
#include "hardware/Rfid.h"
#include "hardware/XYdrive.h"
#include "Meeple.h"
#include "route.h"

// global includes
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

#include <boost/msm/back/state_machine.hpp>
namespace fsm
{
	class GameFSM_;
}

class Playboard {
public:
	typedef boost::shared_ptr<Display> 		DisplayPtr;
	typedef std::map<int32_t, DisplayPtr> 	DisplayMap;
	typedef DisplayMap::iterator 			DisplayMapIt;
	typedef boost::shared_ptr<LedRing> 		LedRingPtr;
	typedef boost::shared_ptr<LedStripes> 	LedStripesPtr;
	typedef boost::shared_ptr<Rfid> 		RfidPtr;
	typedef boost::shared_ptr<XYdrive> 		XYDrivePtr;
	typedef boost::shared_ptr<Field> 		FieldPtr;
	typedef std::map<uint8_t, FieldPtr> 	FieldMap;
	typedef FieldMap::iterator 				FieldMapIt;
	typedef boost::shared_ptr<Player> 		PlayerPtr;
	typedef std::map<int32_t, PlayerPtr> 	PlayerMap;
	typedef PlayerMap::iterator 			PlayerMapIt;

public:
	Playboard( boost::msm::back::state_machine< fsm::GameFSM_ >* gameFsmPtr );
	virtual ~Playboard( );

	bool addPlayer( int32_t );
	bool delPlayer( int32_t );
	bool readPlayersMeepleAtStart( int32_t colorId, std::vector< uint16_t >& tags );
	bool reconfigurePlayersMeeple( int32_t colorId, const std::vector< uint16_t >& tags );

	void moveMeeple(const Meeple& m, const Field& to);
	void moveMeeple(uint8_t from, uint8_t to);
	/// read id and set to target color
	uint16_t readId(uint32_t fieldId);

	uint8_t checkMovedMeeple( uint8_t color );
	bool checkMeepleMove( uint8_t from, uint8_t to );
	bool setMeepleMove( uint8_t color, uint8_t from, uint8_t to );

	// TODO optimize! do not search in fieldId order for efficiency
	bool searchForMeeple( uint8_t from, uint8_t& illegalPos );

	DisplayPtr display( int32_t id )
	{
		if ( _displays.find( id ) != _displays.end( ) )
			return _displays.at( id );

		return DisplayPtr( );
	}
	DisplayMap& displays( ){ return _displays; }

	XYDrivePtr drive( ){ return _XYDrive; }

	LedStripesPtr ledStripe( ){ return _ledStripe; }

	PlayerMap& players( ){ return _players; }

	int32_t getColorFromFieldId( int32_t fieldId);
	MeeplePtr getMeepleFromFieldId( int32_t fieldId);

private:
	DisplayMap _displays;
	LedRingPtr _ledRing;
	LedStripesPtr _ledStripe;
	RfidPtr _rfid;
	XYDrivePtr _XYDrive;

	FieldMap _fields;
	PlayerMap _players;
	boost::mutex _mutexXYDrive;
};

#endif /* PLAYBOARD_H_ */
