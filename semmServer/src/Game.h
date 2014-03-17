/**
 *  class Game.
 *  @file Game.h
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

#ifndef GAME_H_
#define GAME_H_

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/bimap.hpp>
#include <boost/config.hpp>
#include <boost/geometry.hpp>
#include <stdio.h>
#include <pthread.h>
#include <string>
#include <iostream>

#include "defs.h"
#include "Client.h"
#include "Player.h"
#include "tools.h"

class Client;
class Player;

/**
 *
 */
struct cmdBuffer_s {
	uint8_t cmd[CMDBUF_SIZE][CMDBUF_MAX_CMD_LENGHT];
	uint8_t in, out;
	boost::mutex mutex;
	boost::condition_variable_any condAdd, condRem;
};

struct figure_s;

/**
 *
 */
struct field_s {
	uint8_t x, y, no;
	figure_s *figure;
	void set(uint8_t _x, uint8_t _y) {
		x = _x;
		y = _y;
	}
};

/**
 *
 */
class Game {

public:
	Game();
	virtual ~Game();

	/**
	 *
	 */
	void start();


	/**
	 *
	 * @param cmd
	 * @param length
	 */
	void addCmd(uint8_t cmd[], int length);

	/**
	 *
	 * @param client
	 * @return
	 */
	int addClient(Client *client);

	/**
	 *
	 * @param player
	 * @param color
	 * @param clientId
	 * @return
	 */
	uint8_t addPlayer(Player *player, uint8_t color, uint8_t clientId);

	/**
	 *
	 * @return
	 */
	bool getIsRunning() {
		return m_mode;
	}

	/**
	 *
	 * @return
	 */
	std::map<int, Client*> *getClientMap() {
		return &m_clientMap;
	}


//	;

	/**
	 *
	 * @return
	 */
	uint8_t getMode() {
		return m_mode;
	}

	/**
	 *
	 * @param val
	 */
	void setIsRunning(bool val) {
		m_mode = val;
	}

private:


	uint8_t m_mode;						///<
	cmdBuffer_s m_cB;					///<
	boost::thread m_threadGame;			///<
	std::map<int, Client*> m_clientMap;	///<
	std::map<int, Player*> m_playerMap;	///<
	int m_clientCounter;				///<
	int m_playerCounter;				///<
	field_s m_playground[56];			///<
	uint8_t m_availColor;				///<
	uint8_t m_playersDone;				///<
	uint8_t m_actPlayer;				///<
	uint8_t m_actPips;					///<
	uint8_t m_outputOffset;				///<

	/**
	 *
	 */
	void m_worker();

	/**
	 *
	 */
	void m_printClientList();

	/**
	 *
	 */
	void m_printPlayerList();

	/**
	 *
	 */
	void m_printPlayground();

	/**
	 *
	 * @param x
	 * @param y
	 * @return
	 */
	field_s* m_findInPlayground(uint8_t x, uint8_t y);

	/**
	 *
	 * @param cid
	 * @param buf
	 * @return
	 */
	int m_identifyClient(int cid, char buf[]);


	//int m_identifyPlayer(int pid, char buf[]);

	/**
	 *
	 * @return
	 */
	int m_selectNextPlayer();

	/**
	 *
	 * @param startField
	 * @param pips
	 * @param player
	 * @return
	 */
	int m_calculateNextField(uint8_t startField, uint8_t pips, uint8_t player);

	/**
	 *
	 * @param player
	 * @return
	 */
	int m_calculateNextCntOfDice(uint8_t player);

	/**
	 *
	 * @param color
	 * @return
	 */
	bool m_allStartArea(uint8_t color);

	/**
	 *
	 * @param from
	 * @param to
	 */
	void m_move(uint8_t from, uint8_t to);

	/**
	 *
	 * @param color
	 */
	void m_sendPlayerInHomeToAll(uint8_t color);

	/**
	 *
	 */
	void m_sendAvailableColorsToAll();

	/**
	 *
	 */
	void m_sendGameModeToAll();

	/**
	 *
	 * @param cid
	 */
	void m_sendPlayground(uint8_t cid);

	/**
	 *
	 */
	void m_sendPlaygroundToAll();

	/**
	 *
	 * @param color
	 * @param pips
	 */
	void m_sendDiceToAll(uint8_t color, uint8_t pips);

	/**
	 *
	 * @param set
	 */
	void m_sendShowDieToAll(bool set);

	/**
	 *
	 * @param from
	 * @param to
	 */
	void m_sendMoveToAll(uint8_t from, uint8_t to);

	/**
	 *
	 */
	void m_initPlayground();

};

#endif /* GAME_H_ */
