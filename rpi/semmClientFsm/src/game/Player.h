/*
 * Player.h
 *
 *  Created on: Mar 21, 2014
 *      Author: reinhard
 */

#ifndef PLAYER_H_
#define PLAYER_H_

// locale includes
#include "playboard/Meeple.h"

// global includes
#include <boost/shared_ptr.hpp>
#include <vector>

typedef boost::shared_ptr<Meeple> MeeplePtr;
typedef std::vector<MeeplePtr> MeepleVec;
typedef MeepleVec::iterator MeepleVecIt;

class Player {
public:
	Player(int32_t colorId);
	virtual ~Player();

	void addMeeples( std::vector< uint16_t > tags );
	MeepleVec& meeples() { return _meeples; }
	int32_t getStartAreaPos(int32_t i);
	int32_t color() { return _color; }

	const std::vector<int32_t>& startAreaPos( ){ return _startAreaPos; }

private:
	int32_t _color;
	MeepleVec _meeples;
	std::vector<int32_t> _startAreaPos;
};

#endif /* PLAYER_H_ */
