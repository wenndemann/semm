/**
 *  class Player.
 *  @file Player.cpp
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */
#include "Player.h"

Player::Player() {
	// TODO Auto-generated constructor stub
	m_actCountOfDice = 0;
	m_insertField = NULL;
	m_exitField = NULL;
	m_firstEndField = NULL;
	m_client = 0;
	m_color = 0;
	m_game = NULL;
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::sendMoveNotAllowed() {
	printf("player: move not allowed %d\n", m_client);
	m_game->getClientMap()->at(m_client)->sendMoveNotAllowed(m_color);
}
