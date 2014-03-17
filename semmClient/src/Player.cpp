/*
 * Player.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: reinhard
 */

#include "Player.h"

Player::Player() {
	m_id = 0;
	m_color = 0;
	m_game = NULL;
}

void Player::sendSetPlayerName(const char *name) {
	m_game->getClient()->sendSetPlayerName(m_id, name);
}

void Player::sendSetColor(int color) {
	m_game->getClient()->sendSetColor(color);
}

void Player::sendStartGame() {
	m_game->getClient()->sendStartGame(m_id);
}

void Player::sendDieDone() {
	m_game->getClient()->sendDieDone(m_id);
}

void Player::sendSelectFigure(uint8_t field) {
	m_game->getClient()->sendSelectFigure(m_id, field);
}
