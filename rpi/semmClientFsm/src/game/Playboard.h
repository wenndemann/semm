/*
 * Playboard.h
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#ifndef PLAYBOARD_H_
#define PLAYBOARD_H_


// global includes
#include <boost/shared_ptr.hpp>
#include <vector>

// local includes
#include "../hardware/Display.h"
#include "../hardware/LedRing.h"
#include "../hardware/LedStripes.h"
#include "../hardware/Rfid.h"
#include "../hardware/XYdrive.h"

class Playboard {

	typedef boost::shared_ptr<Display> DisplayPtr;
	typedef std::vector<DisplayPtr> DisplayVector;
	typedef DisplayVector::iterator DisplayVectorIt;

	typedef boost::shared_ptr<LedRing> LedRingPtr;
	typedef boost::shared_ptr<LedStripes> LedStripesPtr;
	typedef boost::shared_ptr<Rfid> RfidPtr;
	typedef boost::shared_ptr<XYdrive> XYDrivePtr;

public:
	Playboard();
	virtual ~Playboard();

private:
	std::vector<DisplayPtr> _displays;
	LedRingPtr _ledRing;
	LedStripesPtr _ledStripe;
	RfidPtr _rfid;
	XYDrivePtr _XYDrive;
};

#endif /* PLAYBOARD_H_ */
