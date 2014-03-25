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
	const int32_t availableColors( ) { return _availColors; }
	const int32_t clientColors( ) { return _clientColors; }

	PlayboardPtr playboard( ){ return _playboard; }
	boost::msm::back::state_machine< fsm::GameFSM_ >* mainFSM( ) { return _mainFSM; }

	void parseCmd( uint8_t* buf, int32_t nR );

private:
	boost::msm::back::state_machine< fsm::GameFSM_ >* _mainFSM;
	PlayboardPtr _playboard;

	Mode _mode;
	int32_t _availColors;
	int32_t _clientColors;
};

#endif /* GAME_H_ */
