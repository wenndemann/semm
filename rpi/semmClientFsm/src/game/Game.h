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
#include <string>

// local includes
#include "../defs.h"
#include "playboard/Playboard.h"

#include <boost/msm/back/state_machine.hpp>
namespace fsm
{
	class GameFSM_;
}

class Game {

	typedef boost::shared_ptr<Playboard> PlayboardPtr;

	typedef boost::shared_ptr<Player> PlayerPtr;
	typedef std::map<int, PlayerPtr> PlayerMap;
	typedef PlayerMap::iterator PlayerMapIt;

public:
	enum Mode{
		Init = 1,
		SelectColor = 2,
		Gaming = 3,
		GameOver = 4
	};

	Game( boost::msm::back::state_machine< fsm::GameFSM_ >* gameFsmPtr );
	virtual ~Game();

	Mode mode() { return _mode;}
	PlayboardPtr playboard( ){ return _playboard; }

	void parseCmd( uint8_t* buf, int32_t nR );

private:
	PlayboardPtr _playboard;
	boost::msm::back::state_machine< fsm::GameFSM_ >* _mainFSM;

	Mode _mode;
};

#endif /* GAME_H_ */
