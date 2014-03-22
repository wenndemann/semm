/*
 * Player.h
 *
 *  Created on: Mar 21, 2014
 *      Author: reinhard
 */

#ifndef PLAYER_H_
#define PLAYER_H_

// global includes
#include <boost/shared_ptr.hpp>
#include <vector>

// locale includes
#include "playboard/Meeple.h"


typedef boost::shared_ptr<Meeple> MeeplePtr;
typedef std::vector<MeeplePtr> MeepleVec;
typedef MeepleVec::iterator MeepleVecIt;

class Player {
public:
	Player();
	virtual ~Player();

	MeepleVec& meeples() { return _meeples; }

private:
	MeepleVec _meeples;
};

#endif /* PLAYER_H_ */
