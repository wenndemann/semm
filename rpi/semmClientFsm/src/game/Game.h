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
#include "../tcp_ip/TcpIp.h"
#include "playboard/Playboard.h"
#include "../fsm/mainFsm.h"

class Game {

	typedef boost::shared_ptr<Playboard> PlayboardPtr;
	typedef boost::shared_ptr<TcpIp> TcpIpPtr;

	typedef boost::shared_ptr<Player> PlayerPtr;
	typedef std::map<int, PlayerPtr> PlayerMap;
	typedef PlayerMap::iterator PlayerMapIt;

	typedef boost::shared_ptr<gameFSM> GameFSMPtr;
public:
	enum Mode{
		Init = 1,
		SelectColor = 2,
		Gaming = 3,
		GameOver = 4
	};

	Game(const std::string& ip);
	virtual ~Game();

	Mode mode() { return _mode;}

private:
	PlayboardPtr _playboard;
	TcpIpPtr _tcpIp;
	GameFSMPtr _fsm;

	Mode _mode;
};

#endif /* GAME_H_ */
