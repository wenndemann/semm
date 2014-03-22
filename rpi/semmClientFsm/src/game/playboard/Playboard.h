/*
 * Playboard.h
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#ifndef PLAYBOARD_H_
#define PLAYBOARD_H_


// global includes
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

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

class Playboard {
typedef boost::shared_ptr<Display> DisplayPtr;
typedef std::map<int, DisplayPtr> DisplayMap;
typedef DisplayMap::iterator DisplayMapIt;
typedef boost::shared_ptr<LedRing> LedRingPtr;
typedef boost::shared_ptr<LedStripes> LedStripesPtr;
typedef boost::shared_ptr<Rfid> RfidPtr;
typedef boost::shared_ptr<XYdrive> XYDrivePtr;
typedef boost::shared_ptr<Field> FieldPtr;
typedef std::map<uint8_t, FieldPtr> FieldMap;
typedef FieldMap::iterator FieldMapIt;
typedef boost::shared_ptr<Player> PlayerPtr;
typedef std::map<int, PlayerPtr> PlayerMap;
typedef PlayerMap::iterator PlayerMapIt;

public:
	Playboard();
	virtual ~Playboard();

	void moveMeeple(const Meeple& m, const Field& to);
	void moveMeeple(uint8_t from, uint8_t to);
	uint16_t readId(const Field& f);

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
