#ifndef GAME_H_
#define GAME_H_

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <map>

#include "defs.h"
#include "Client.h"
#include "Player.h"
#include "route.h"
//#include "../hardware/Hardware.h"

#include "../../../arduino/led_ring/i2c_defs.h"
#include "../../../arduino/led_stripe/i2c_defs.h"
#include "../../../arduino/rfid_reader/i2c_defs.h"
#include "../../../arduino/xy_schlitten/src/i2c_defs.h"
#include "../../../arduino/display/i2c_defs.h"

#include "../hardware/LedRing.h"
#include "../hardware/LedStripes.h"
#include "../hardware/XYdrive.h"
#include "../hardware/Display.h"
#include "../general/tools.h"
#include "../general/CliDisplay.h"
//#include "../general/cli.h"

class Client;
class Player;

struct figure_s;

struct field_s {
	uint8_t x, y, no;
	figure_s *figure;

	void set(uint8_t _x, uint8_t _y) {
		x = _x;
		y = _y;
	}
	field_s &operator=(const field_s& cmp) {
		return *this;
	}
	bool operator!=(field_s* cmp) {
		return !(this->no == cmp->no);
	}
};

class Game {
public:
	Game();
	virtual ~Game();
	void addCmd(uint8_t cmd[], int length);
	void start();

	Client* getClient() {
		return m_client;
	}

	void setClient(Client* client) {
		m_client = client;
	}

	void setCliDisplay(int number, CliDisplay* CliDisplay)
		{ m_display[number]->setCliDisplay(CliDisplay); }

	uint8_t getMode() { return m_mode; }

private:
	uint8_t m_actPlayer;
	uint8_t m_actPips;
	uint8_t m_availColors;
	cmdBuffer_s m_cB;
	Client *m_client;
	int m_clientCounter;
	Display* m_display[4];
	bool m_lastMoveByHand;
	uint8_t m_mode;
	boost::mutex m_mutexPlayground;
	uint8_t m_myColor;
	std::map<int, Player> m_playerMap;
	field_s m_playground[96];
	tags_t m_tagsActPg, m_tagsActGame;
	XYdrive* m_XYdrive;
	bool m_allowedToSet;

	bool m_addPlayer(uint8_t color);
	void m_calculatePath(Route* r, uint8_t from, uint8_t to);
	bool m_checkField(field_s* f);
	int m_getMovedMeeple(uint8_t color);
	void m_delPlayer(uint8_t color);
	int m_findMeeple(figure_s* figure);
	uint8_t m_XYtoNUM(uint8_t x, uint8_t y);
	void m_initPlayground();
	void m_move(uint8_t from, uint8_t to);
	void m_moveMechanical(uint8_t from, uint8_t to);
	void m_printTags(tags_t* tags);
	void m_worker();
};

#endif /* GAME_H_ */
