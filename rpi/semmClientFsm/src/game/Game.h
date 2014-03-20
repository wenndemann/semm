/*
 * Game.h
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#ifndef GAME_H_
#define GAME_H_

// global includes
#include <boost/shared_ptr.hpp>

// local includes
#include "../defs.h"
#include "Playboard.h"


class Game {

	typedef boost::shared_ptr<Playboard> PlayboardPtr;

public:
	Game();
	virtual ~Game();

private:
	PlayboardPtr _playboard;
};

#endif /* GAME_H_ */
