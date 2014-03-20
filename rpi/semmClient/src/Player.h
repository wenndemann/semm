/*
 * Player.h
 *
 *  Created on: Apr 24, 2013
 *      Author: reinhard
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Player.h"
#include "Game.h"
#include "../general/tools.h"

class Game;
struct field_s;

struct figure_s {
	uint8_t color;
	field_s *field;
	uint16_t tagNum;
//	char sym;
};

class Player {
public:
	Player();

	void sendSetPlayerName(const char *name);
	void sendSetColor(int color);
	void sendStartGame();
	void sendDieDone();
	void sendSelectFigure(uint8_t field);

	figure_s *getFigure(uint8_t i) {
		return &m_figures[i];
	}

	void setColor(uint8_t color) {
		m_color = color;
	}
	void setGame(Game *game) {
		m_game = game;
	}

private:
	Game *m_game;
	int m_color;
	int m_id;
	figure_s m_figures[3];
};

#endif /* PLAYER_H_ */
