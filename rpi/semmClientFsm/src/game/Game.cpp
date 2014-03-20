/*
 * Game.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "Game.h"

#define cout cout << "Game: "

using namespace std;

Game::Game(const string& ip) :
	_playboard(new Playboard()),
	_tcpIp(new TcpIp())
{
	_tcpIp->connect(ip);
}

Game::~Game() {

}

