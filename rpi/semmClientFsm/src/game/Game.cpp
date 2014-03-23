/*
 * Game.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "Game.h"

#include "../fsm/mainFsm.h"

using namespace std;

Game::Game( fsm::gameFSM* gameFsmPtr )
: _mainFSM( gameFsmPtr )
, _playboard( new Playboard( gameFsmPtr ) )
{
}

Game::~Game() {

}

void Game::parseCmd( uint8_t* buf, int32_t nR )
{

}
