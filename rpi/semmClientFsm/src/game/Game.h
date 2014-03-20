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
#include "Playboard.h"

class Game {

	typedef boost::shared_ptr<Playboard> PlayboardPtr;
	typedef boost::shared_ptr<TcpIp> TcpIpPtr;

public:
	Game(const std::string& ip);
	virtual ~Game();

private:
	PlayboardPtr _playboard;
	TcpIpPtr _tcpIp;
};

#endif /* GAME_H_ */
