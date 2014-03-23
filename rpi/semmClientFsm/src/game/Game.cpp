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
, _mode(Mode::Init)
, _availColors(0)
, _clientColors(0)
{
}

Game::~Game() {

}

void Game::parseCmd( uint8_t* buf, int32_t nR )
{
	switch(buf[0]) {
	case TCP_CMD_MODE_SELECT_COLOR_SC:
		_mode = Mode::SelectColor;
		_mainFSM->process_event(fsm::initGame());
		break;

	case TCP_CMD_USER_NOT_AUTHORIZED_SC:

		break;

	case TCP_CMD_AVAILABLE_COLORS_SC:
		_availColors = static_cast<int32_t> (buf[1]);
		_mainFSM->process_event(fsm::initColors());
		break;

	case TCP_CMD_MODE_GAME:
		_mode = Mode::Gaming;
		_mainFSM->process_event(fsm::initGame());
		break;

	case TCP_CMD_DEFINE_CLIENT_COLORS:
		_clientColors = static_cast<int32_t> (buf[1]);
		_mainFSM->process_event(fsm::clientColors());
		break;

	case TCP_CMD_MOVE_SC:

		break;

	case TCP_CMD_SHOW_DIE_SC:

		break;

	case TCP_CMD_DICE_SC:

		break;

	case TCP_CMD_MOVE_NOT_ALLOWED_SC:

		break;

	case TCP_CMD_PLAYER_IN_HOME_SC:

		break;

	default:
		cout << "!!! received unknown TCP command" << endl;
		break;
	}
}
