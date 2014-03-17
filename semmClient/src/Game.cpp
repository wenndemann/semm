#include "Game.h"

Game::Game() {
	m_availColors = 0;
	m_client = NULL;
	m_mode = 1;
	m_clientCounter = 0;
	m_myColor = 0;
	m_actPips = 0;
	m_actPlayer = 0;
	m_lastMoveByHand = false;

	m_XYdrive = new XYdrive(I2C_XYS_ADDR, I2C_RFID_ADDR, I2C_LR_ADDR,
	I2C_LS_ADDR);
	m_display[0] = new Display(I2C_DBEN_ADDR_0, 500);
	m_display[1] = new Display(I2C_DBEN_ADDR_1, 500);
	m_display[2] = new Display(I2C_DBEN_ADDR_2, 500);
	m_display[3] = new Display(I2C_DBEN_ADDR_3, 500);
}

Game::~Game() {
	delete m_XYdrive;
	for (int i = 0; i <= 3; i++)
		delete m_display[i];
}

void Game::start() {
	m_mode = 1;
	m_clientCounter = 0;
	for (int i = 0; i < 4; i++)
		m_display[i]->setPictures(I2C_DBEN_PIC_SEMM);
	m_initPlayground();
	boost::thread gameWorker(boost::bind(&Game::m_worker, this));
}

void Game::addCmd(uint8_t cmd[], int length) {
	m_cB.mutex.lock();
	while (((m_cB.in + 1) % CMDBUF_SIZE) == m_cB.out)  //while cmdBuffer is full
		m_cB.condRem.wait(m_cB.mutex);
	memcpy(&m_cB.cmd[m_cB.in][0], cmd, length);
	++m_cB.in %= CMDBUF_SIZE;
	m_cB.condAdd.notify_one();
	m_cB.mutex.unlock();
	//	debug(" ...done\n");
}

void Game::m_worker() {
	std::map<int, Client*>::iterator itCli;
	std::map<int, Player*>::iterator itPla;
	int color, cmd;
	uint8_t buf[CMDBUF_MAX_CMD_LENGHT + 1];

	debug("start GAME worker\n");
	while (m_mode) {
		m_cB.mutex.lock();
		while (m_cB.in == m_cB.out) // while cmdBffer is empty
			m_cB.condAdd.wait(m_cB.mutex);

		cmd = m_cB.cmd[m_cB.out][1];
		color = m_cB.cmd[m_cB.out][2];
		memcpy(&buf, &m_cB.cmd[m_cB.out][2], CMDBUF_MAX_CMD_LENGHT - 1);
		++m_cB.out %= CMDBUF_SIZE;
		m_cB.condRem.notify_one();
		m_cB.mutex.unlock();

		switch (cmd) {
		// display pushed enter
		case DISPLAY_CMD_PUSH_ENTER: {
			int gColor = (1 << color);
			debug("got cmd from display[%d] = %d,%d,%d,%d\n", color, buf[0],
					buf[1], buf[2]);
			switch (buf[1]) {

			// pressed enter at enter-screen
			case I2C_DBEN_PIC_ENTER:
				debug("send \'setColor\' to server\n");
				m_display[color]->setPictures(I2C_DBEN_PIC_PREPARE);
				m_myColor |= gColor;
				m_client->sendSetColor(gColor);
				break;

			case I2C_DBEN_PIC_PREPARE:
				if (m_mode == 2) {
					m_display[color]->setPictures(I2C_DBEN_PIC_WAIT);

					if (m_addPlayer(gColor)) {
						m_display[color]->setPictures(I2C_DBEN_PIC_START);
					} else {
						m_display[color]->setPictures(I2C_DBEN_PIC_ENTER);
						m_client->sendDelColor(gColor);
					}

				} else if (m_mode == 3) {
					uint8_t pg[3];
					uint16_t tag[3];
					uint8_t oldPos[3];
					tagLine_t l0, l1;

					switch (1 << color) {
					case 8:  //stefan
						pg[0] = 32;
						pg[1] = 33;
						pg[2] = 34;
						m_XYdrive->scanLine(0, &l0);
						m_XYdrive->scanLine(1, &l1);
						while (m_XYdrive->getIsMoving())
							usleep(100000);
						tag[0] = l0.tag[1];
						tag[1] = l1.tag[1];
						tag[2] = l1.tag[0];
						break;
					case 4:  //ela
						pg[0] = 35;
						pg[1] = 36;
						pg[2] = 37;
						m_XYdrive->scanLine(9, &l0);
						m_XYdrive->scanLine(10, &l1);
						while (m_XYdrive->getIsMoving())
							usleep(100000);
						tag[0] = l0.tag[0];
						tag[1] = l0.tag[1];
						tag[2] = l1.tag[1];
						break;
					case 2:  //martina
						pg[0] = 38;
						pg[1] = 39;
						pg[2] = 40;
						m_XYdrive->scanLine(0, &l0);
						m_XYdrive->scanLine(10, &l1);
						while (m_XYdrive->getIsMoving())
							usleep(100000);
						tag[0] = l1.tag[9];
						tag[1] = l0.tag[9];
						tag[2] = l0.tag[10];
						break;
					case 1:  //marcel
						pg[0] = 41;
						pg[1] = 42;
						pg[2] = 43;
						m_XYdrive->scanLine(0, &l0);
						m_XYdrive->scanLine(1, &l1);
						while (m_XYdrive->getIsMoving())
							usleep(100000);
						tag[0] = l1.tag[10];
						tag[1] = l1.tag[9];
						tag[2] = l0.tag[9];
						break;
					default:
						return;
					}
					for(int i = 0; i < 3; i++) {
						oldPos[i] = m_playerMap.at(gColor).getFigure(i)->field->no;
						m_move(oldPos[i], pg[i]);
						m_playerMap.at(gColor).getFigure(i)->tagNum = tag[i];
						m_moveMechanical(pg[i], oldPos[i]);
						m_move(pg[i], oldPos[i]);
					}
				}
				break;

				// pressed enter at start-game-screen
			case I2C_DBEN_PIC_START:
				if (buf[2] == 1)
					m_client->sendStartGame(gColor);
				if (buf[2] == 2) {
					m_delPlayer(gColor);
					m_client->sendDelColor(gColor);
					m_myColor &= ~(gColor);
					m_display[color]->setPictures(I2C_DBEN_PIC_ENTER);
				}
				break;

				// pressed enter at dice-screen
			case I2C_DBEN_PIC_DICE:
				debug("  send show the dice to server\n");
				m_client->sendDieDone(gColor);
				break;

				// pressed enter select-meeple-screen
//			case I2C_DBEN_PIC_SELECT_FIGURE:
//				debug("  send moved figure to server\n");
//				m_client->sendSelectFigure(gColor, m_playerMap.at(gColor).getFigure(buf[1] -1)->field->no);
//				break;

				// pressed enter at a dice-number-screen or not-found-screen
			//case I2C_DBEN_PIC_NOT_FOUND:
			case (I2C_DBEN_PIC_DICE_1 + (1<<6)):
			case (I2C_DBEN_PIC_DICE_2 + (1<<6)):
			case (I2C_DBEN_PIC_DICE_3 + (1<<6)):
			case (I2C_DBEN_PIC_DICE_4 + (1<<6)):
			case (I2C_DBEN_PIC_DICE_5 + (1<<6)):
			case (I2C_DBEN_PIC_DICE_6 + (1<<6)): {
				if (m_allowedToSet) {
					for (int i = 0; i < 4; i++)
						m_display[i]->setPictures(I2C_DBEN_PIC_WAIT);

					int moved = m_getMovedMeeple(gColor);

					debug("moved = %d\n", moved);

					if (moved <= 0) {
						debug("no field recognized please select\n");
						m_display[color]->setPictures(
						I2C_DBEN_PIC_NOT_FOUND);
						sleep(2);
						m_display[color]->setPictures(
						I2C_DBEN_PIC_PREPARE);
					} else {
						debug("moved field %d\n", moved);
						m_display[color]->setPictures(I2C_DBEN_PIC_WAIT);
						m_client->sendSelectFigure(gColor, moved);
						m_lastMoveByHand = true;
					}
				}
			}
				break;
			}
		}
			break;

		case DISPLAY_CMD_CANCEL_GAME:
			m_client->sendCancelGame(1 << color);
			break;

			// goto select color mode
		case (TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_SELECT_COLOR):
			debug("goto \'select colormode\'\n");
			m_mode = 2;
//			for (int i = 0; i < 4; i++) {
//				if (color & (1 << i))
//					m_display[i]->setPictures(I2C_DBEN_PIC_ENTER);
//				else
//					m_display[i]->setPictures(I2C_DBEN_PIC_DISABLED);
//			}
			break;

			// server disaccept connection
		case (TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_USER_NOT_AUTHORIZED):
			debug("not autorized\n");
			break;

			// get available colors				debug("you are allowed to set\n");
		case TCP_CMD_AVAILABLE_COLORS_SC: {

			debug("got available colors=%d\tmode = %d\n", color, m_mode);
			m_availColors = color;
			if (m_mode == 2) {
				for (int i = 0; i < 4; i++) {
					debug("disable: %d\n", m_availColors & 0x0f);
					debug("my     : %d\n", m_myColor & 0x0f);
					debug("(1<<i) : %d\n", (1<<i) & 0x0f);
					debug("lala   : %d\n\n", (m_availColors) & m_myColor & (1<<i));
//					if (m_display[i]->getPicture() != I2C_DBEN_PIC_PREPARE || m_display[i]->getPicture() != I2C_DBEN_PIC_ENTER) {
					if((m_availColors) & m_myColor & (1<<i)) {
//					if (color & (1 << i)) {
							m_display[i]->setPictures(I2C_DBEN_PIC_DISABLED);
							m_display[i]->setBlocked(true);
						} else {
							if(!((1<<i) & m_myColor)){
								m_display[i]->setBlocked(false);
								m_display[i]->setPictures(I2C_DBEN_PIC_ENTER);
							}
//						}
					}
				}
			}
			//add or remove players that not belong to this unit
			uint8_t tmpColor = ~m_availColors & 15;
			debug("you are allowed to set\n");
			debug("colors to be checked = %d (a %d, m %d)\n", tmpColor,
					m_availColors, m_myColor);
			for (int i = 0; i < 4; i++) {
				uint8_t n = (tmpColor & (1 << i));
				if (n) {
					if (m_playerMap.find(n) == m_playerMap.end()) {
						debug(" -add %d\n", n);
						m_addPlayer(n);
					}
				} else {
					if (m_playerMap.find((1 << i)) != m_playerMap.end()) {
						debug(" -del %d\n", (1 << i));
						m_delPlayer((1 << i));
					}
				}
			}
			if (m_mode == 3) {
				for (int i = 0; i < 4; i++) {
					if (m_availColors & (1 << i)) {
						m_display[i]->setPictures(I2C_DBEN_PIC_SEMM);
						m_display[i]->setBlocked(true);
					}
					if (!(m_myColor & (1 << i))) {
						m_display[i]->setPictures(I2C_DBEN_PIC_DISABLED);
						m_display[i]->setBlocked(true);
					}
				}
			}
		}
			break;

			// goto game Mode
		case (TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_GAME):
			m_mode = 3;
			buf[1] = '\0';
			debug("goto game mode\n");
			for (int i = 0; i < 4; i++) {
				m_display[i]->setPictures(I2C_DBEN_PIC_WAIT);
			}
//			m_XYdrive->scan(&m_tagsActPg);
//			while(m_XYdrive->getIsMoving()) usleep(100000);
//			m_printTags(&m_tagsActPg);
			break;

			// define client colors
		case TCP_CMD_DEFINE_CLIENT_COLORS:
			debug("set myColors = %d\n", color);
			m_myColor = color;
			for (int i = 0; i < 4; i++)
				m_addPlayer(m_myColor & (1 << i));
			break;

			// move figure
		case (TCP_CMD_MOVE_SC):
			buf[0]--;
			buf[1]--;
//			debug("move figure from %d to %d\n", buf[0], buf[1]);

			if (!m_actPlayer & m_myColor) {
				m_lastMoveByHand = false;
				debug("tablet move\n");
			}

			if (m_lastMoveByHand) {
				if (buf[1] < 32)
					m_lastMoveByHand = false;
				m_move(buf[0], buf[1]);
				debug("check\n");
				if (!m_checkField(&m_playground[buf[1]])) {
					debug("you cheated\n");
					for (int i = 0; i < 4; i++) {
						if (m_actPlayer == (1 << i)) {
							m_display[i]->setPictures(
									I2C_DBEN_PIC_MOVE_ILLEGAL);
//
//					uint8_t newPos = m_findMeeple(m_playground[buf[1]].figure);
//					debug("newPos = %d\n", newPos);
//					if(newPos != 255) {
//						m_move(buf[1], newPos);
//						m_moveMechanical(newPos, buf[1]);
//						m_move(newPos, buf[1]);
//					}
//					else {
							debug("no field recognized please select\n");
							m_display[i]->setPictures(I2C_DBEN_PIC_NOT_FOUND);
							sleep(2);
							m_display[i]->setPictures(I2C_DBEN_PIC_PREPARE);
						}
					}
//					}
				}
			} else {
				m_moveMechanical(buf[0], buf[1]);
				m_move(buf[0], buf[1]);
			}
			break;

			// show dice
		case (TCP_CMD_SHOW_DIE_SC):

			if (m_actPlayer == m_myColor)
				debug("  you diced a %d, please select a figure to move\n",
						m_actPips);
			else
				debug("  %d diced a %d\n", m_actPlayer, m_actPips);

			m_allowedToSet = buf[0];
			for (int i = 0; i < 4; i++) {
				if (m_allowedToSet && (m_actPlayer == (1 << i))) {
					debug("you are allowed to set\n");
					m_display[i]->setPictures(
					I2C_DBEN_PIC_DICE_1 + m_actPips - 1 + (1 << 6));
				} else {
					debug("you are not allowed to set\n");
					m_display[i]->setPictures(
					I2C_DBEN_PIC_DICE_1 + m_actPips - 1);
				}
			}
			break;

			// dice
		case (TCP_CMD_DICE_SC): {
			static uint8_t lastPlayer;
			m_actPlayer = buf[0];
			m_actPips = buf[1];

			debug("dice\n");

			if (lastPlayer == m_actPlayer) {
				lastPlayer = m_actPlayer;
				sleep(1);
				for (int i = 0; i < 4; i++)
					if ((m_actPlayer & m_myColor) && (m_actPlayer & (1 << i))
							&& (m_display[i]->getPicture()
									>= I2C_DBEN_PIC_DICE_1)
							&& (m_display[i]->getPicture()
									<= I2C_DBEN_PIC_DICE_6))
						while (m_display[i]->getEncoder() == 0)
							usleep(250000);
			}
			if (m_actPlayer & m_myColor) {
				debug(
						"  it is your turn, you have to dice, please send %d %d to server\n",
						TCP_CMD_DIE_DONE_CS, color);
				//TODO do not like

				for (int i = 0; i < 4; i++) {
					if (m_actPlayer & (1 << i)) {
						m_display[i]->setPictures(I2C_DBEN_PIC_DICE);
						debug("player %d dice\n", (1 << i));
					} else
						m_display[i]->setPictures(I2C_DBEN_PIC_WAIT);
				}
			} else {
				debug("  it is %ds turn\n", color);
			}
		}
			break;

		case TCP_CMD_MOVE_NOT_ALLOWED_SC:
			m_display[color]->setPictures(I2C_DBEN_PIC_MOVE_ILLEGAL);
			break;

		case TCP_CMD_PLAYER_IN_HOME_SC:

			if (color == 16) {
				m_mode = 4;
			} else if (color & m_myColor)
				for (int i = 0; i < 4; i++)
					if (color == (1 << i))
						m_display[i]->setPictures(I2C_DBEN_PIC_WIN);
			break;

		default:
			debug("cmd unknown\n");
			break;
		}
	}
	buf[1] = '\0';
}

bool Game::m_addPlayer(uint8_t color) {
	uint8_t pg[3];
	uint16_t tagNum[3];

	switch (color) {
	case 8: { //stefan
		tagLine_t l0, l1;
		m_XYdrive->scanLine(0, &l0);
		m_XYdrive->scanLine(1, &l1);
		while (m_XYdrive->getIsMoving())
			usleep(100000);
		pg[0] = 32;
		tagNum[0] = l0.tag[1];
		pg[1] = 33;
		tagNum[1] = l1.tag[1];
		pg[2] = 34;
		tagNum[2] = l1.tag[0];
	}
		break;

	case 4: { //ela
		tagLine_t l9, l10;
		m_XYdrive->scanLine(9, &l9);
		m_XYdrive->scanLine(10, &l10);
		while (m_XYdrive->getIsMoving())
			usleep(100000);
		pg[0] = 35;
		tagNum[0] = l9.tag[0];
		pg[1] = 36;
		tagNum[1] = l9.tag[1];
		pg[2] = 37;
		tagNum[2] = l10.tag[1];
	}
		break;

	case 2: { //martina
		tagLine_t l9, l10;
		m_XYdrive->scanLine(9, &l9);
		m_XYdrive->scanLine(10, &l10);
		while (m_XYdrive->getIsMoving())
			usleep(100000);
		pg[0] = 38;
		tagNum[0] = l10.tag[9];
		pg[1] = 39;
		tagNum[1] = l9.tag[9];
		pg[2] = 40;
		tagNum[2] = l9.tag[10];
	}
		break;

	case 1: { //marcel
		tagLine_t l0, l1;
		m_XYdrive->scanLine(0, &l0);
		m_XYdrive->scanLine(1, &l1);
		while (m_XYdrive->getIsMoving())
			usleep(100000);
		pg[0] = 41;
		tagNum[0] = l1.tag[10];
		pg[1] = 42;
		tagNum[1] = l1.tag[9];
		pg[2] = 43;
		tagNum[2] = l0.tag[9];
	}
		break;

	default:
		return false;
	}

	//check if all tags are found
	if (!tagNum[0] || !tagNum[1] || !tagNum[2]) {
		debug("can't find all figures for player %d\n", color);
//		m_client->sendDelColor(color);
		return 0;
	}
	m_mutexPlayground.lock();
	debug("add player %d\n", color);

	//create players
	m_playerMap.insert(std::pair<int, Player>(color, Player()));

	//assign figures to its fields
	m_playerMap[color].setColor(color);
	for (int i = 0; i < 3; i++) {
		m_playerMap[color].getFigure(i)->field = &m_playground[pg[i]];
		m_playground[pg[i]].figure = m_playerMap[color].getFigure(i);
		m_playground[pg[i]].figure->color = color;
		m_playground[pg[i]].figure->tagNum = tagNum[i];
	}

	drawPlayground(m_playground);
	m_mutexPlayground.unlock();
	return true;
}

void Game::m_delPlayer(uint8_t color) {
	uint8_t pg[3];

	switch (color) {
	case 8:  //stefan
		pg[0] = 32;
		pg[1] = 33;
		pg[2] = 34;
		break;
	case 4:  //ela
		pg[0] = 35;
		pg[1] = 36;
		pg[2] = 37;
		break;
	case 2:  //martina
		pg[0] = 38;
		pg[1] = 39;
		pg[2] = 40;
		break;
	case 1:  //marcel
		pg[0] = 41;
		pg[1] = 42;
		pg[2] = 43;
		break;
	default:
		return;
	}

	m_mutexPlayground.lock();
	//delete figures from playground
	for (int i = 0; i < 3; i++) {
		m_playground[pg[i]].figure = NULL;
	}
	m_playerMap.erase(color);
	drawPlayground(m_playground);
	m_mutexPlayground.unlock();
}

bool Game::m_checkField(field_s* f) {
	tagLine_t l;
	bool ret;
	uint8_t tries = 1;

	do {
		m_XYdrive->scanLine(f->x, &l);
		while (m_XYdrive->getIsMoving())
			usleep(100000);
		ret = m_playground[f->no].figure->tagNum == l.tag[f->y];
	} while (!ret && --tries);

	return ret;
}

int Game::m_getMovedMeeple(uint8_t color) {
	for (int i = 0; i < 3; i++) {
		uint8_t num = m_playerMap[color].getFigure(i)->field->no;
		debug("   ckeck c=%d i=%d\n", color, i);
		if (!m_checkField(m_playerMap[color].getFigure(i)->field))
			return num;
	}
	return -1;
}

int Game::m_findMeeple(figure_s* figure) {
	tagLine_t line;
	uint8_t x, y;

	debug("looking for tag = %d\n", figure->tagNum);
	for (x = 0; x < 11; x++) {
		m_XYdrive->scanLine(x, &line);
		while (m_XYdrive->getIsMoving())
			usleep(100000);
		for (y = 0; y < 11; y++)
			if (figure->tagNum == line.tag[y])
				return m_XYtoNUM(x, y);
	}
	return 255;
}

uint8_t Game::m_XYtoNUM(uint8_t x, uint8_t y) {
	for (int i = 0; i < 96; i++) {
		if (m_playground[i].x == x && m_playground[i].y == y)
			return m_playground[i].no;
	}
	return 255;
}

void Game::m_move(uint8_t from, uint8_t to) {
	if (from == to)
		return;

	m_mutexPlayground.lock();
	debug("move in m_playground\n");
	m_playground[to].figure = m_playground[from].figure;
	m_playground[to].figure->field = &m_playground[to];
	m_playground[from].figure = NULL;

	drawPlayground(&m_playground[0]);
	m_mutexPlayground.unlock();
}

void Game::m_moveMechanical(uint8_t from, uint8_t to) {
	if (from == to)
		return;

	m_mutexPlayground.lock();
	Route r;
	m_calculatePath(&r, from, to);

	if (to >= 32 && to <= 43) // someone is kicked out
		m_XYdrive->setLedRing(2, m_playground[from].figure->color);
	else
		//normal movement
		m_XYdrive->setLedRing(1, m_playground[from].figure->color);
	for (int i = 0; i < r.getCount(); i++) {
		WayPoint& wp = r.getElement(i);
		m_XYdrive->magnet(wp.mag());
		m_XYdrive->moveToXY(wp.x(), wp.y());
	}
	while (m_XYdrive->getIsMoving()) {
		usleep(100000);
	}
	m_XYdrive->setLedRing(0, 0);
	m_XYdrive->magnet(false);
	m_mutexPlayground.unlock();
}

void Game::m_calculatePath(Route* r, uint8_t from, uint8_t to) {
	//add actual positions of players to blocklist
	for (int i = 1; i <= 8; i *= 2) {
		if (m_playerMap.find(i) != m_playerMap.end()) {
			for (int j = 0; j < 3; j++) {
				size_t x = m_playerMap[i].getFigure(j)->field->x;
				size_t y = m_playerMap[i].getFigure(j)->field->y;
				r->block(x, y);
			}
		}
	}

	//calculate and optimize path
	r->calcPath(m_playground[from].x, m_playground[from].y, m_playground[to].x,
			m_playground[to].y);
	r->optimize();

	//move to start position if necessary
	if (m_playground[from].x != m_XYdrive->getActX()
			|| m_playground[from].y != m_XYdrive->getActY())
		r->appendFront(m_playground[from].x, m_playground[from].y, false);
}

void Game::m_initPlayground() {
	m_mutexPlayground.lock();

	for (int i = 0; i < 96; i++) {
		m_playground[i].no = i;
		m_playground[i].figure = NULL;
	}
	//playground
	m_playground[0].set(1, 4);
	m_playground[1].set(2, 4);
	m_playground[2].set(3, 4);
	m_playground[3].set(4, 4);
	m_playground[4].set(4, 3);
	m_playground[5].set(4, 2);
	m_playground[6].set(4, 1);
	m_playground[7].set(5, 1);
	m_playground[8].set(6, 1);
	m_playground[9].set(6, 2);
	m_playground[10].set(6, 3);
	m_playground[11].set(6, 4);
	m_playground[12].set(7, 4);
	m_playground[13].set(8, 4);
	m_playground[14].set(9, 4);
	m_playground[15].set(9, 5);
	m_playground[16].set(9, 6);
	m_playground[17].set(8, 6);
	m_playground[18].set(7, 6);
	m_playground[19].set(6, 6);
	m_playground[20].set(6, 7);
	m_playground[21].set(6, 8);
	m_playground[22].set(6, 9);
	m_playground[23].set(5, 9);
	m_playground[24].set(4, 9);
	m_playground[25].set(4, 8);
	m_playground[26].set(4, 7);
	m_playground[27].set(4, 6);
	m_playground[28].set(3, 6);
	m_playground[29].set(2, 6);
	m_playground[30].set(1, 6);
	m_playground[31].set(1, 5);
	//start area
	m_playground[32].set(0, 1);
	m_playground[33].set(1, 1);
	m_playground[34].set(1, 0);
	m_playground[35].set(9, 0);
	m_playground[36].set(9, 1);
	m_playground[37].set(10, 1);
	m_playground[38].set(10, 9);
	m_playground[39].set(9, 9);
	m_playground[40].set(9, 10);
	m_playground[41].set(1, 10);
	m_playground[42].set(1, 9);
	m_playground[43].set(0, 9);
	//end area
	m_playground[44].set(2, 5);
	m_playground[45].set(3, 5);
	m_playground[46].set(4, 5);
	m_playground[47].set(5, 2);
	m_playground[48].set(5, 3);
	m_playground[49].set(5, 4);
	m_playground[50].set(8, 5);
	m_playground[51].set(7, 5);
	m_playground[52].set(6, 5);
	m_playground[53].set(5, 8);
	m_playground[54].set(5, 7);
	m_playground[55].set(5, 6);
	//gap area
	m_playground[56].set(1, 3);
	m_playground[57].set(2, 3);
	m_playground[58].set(3, 3);
	m_playground[58].set(3, 3);
	m_playground[58].set(3, 3);
	m_playground[59].set(3, 2);
	m_playground[60].set(3, 1);
	m_playground[61].set(3, 0);
	m_playground[62].set(4, 0);
	m_playground[63].set(5, 0);
	m_playground[64].set(6, 0);
	m_playground[65].set(7, 0);
	m_playground[66].set(7, 1);
	m_playground[67].set(7, 2);
	m_playground[68].set(7, 3);
	m_playground[68].set(7, 3);
	m_playground[68].set(7, 3);
	m_playground[69].set(8, 3);
	m_playground[70].set(9, 3);
	m_playground[71].set(10, 3);
	m_playground[72].set(10, 4);
	m_playground[73].set(10, 5);
	m_playground[74].set(10, 6);
	m_playground[75].set(10, 7);
	m_playground[76].set(9, 7);
	m_playground[77].set(8, 7);
	m_playground[78].set(7, 7);
	m_playground[78].set(7, 7);
	m_playground[78].set(7, 7);
	m_playground[79].set(7, 8);
	m_playground[80].set(7, 9);
	m_playground[81].set(7, 10);
	m_playground[82].set(6, 10);
	m_playground[83].set(5, 10);
	m_playground[84].set(4, 10);
	m_playground[85].set(3, 10);
	m_playground[86].set(3, 9);
	m_playground[87].set(3, 8);
	m_playground[88].set(3, 7);
	m_playground[88].set(3, 7);
	m_playground[88].set(3, 7);
	m_playground[89].set(2, 7);
	m_playground[90].set(1, 7);
	m_playground[91].set(0, 7);
	m_playground[92].set(0, 6);
	m_playground[93].set(0, 5);
	m_playground[94].set(0, 4);
	m_playground[95].set(0, 3);

	drawPlayground(&m_playground[0]);

	m_mutexPlayground.unlock();
}
