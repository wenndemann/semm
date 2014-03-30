/**
 *  class Game.
 *  @file Game.cpp
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */


#include "Game.h"


Game::Game() {
	m_clientCounter = 0;
	m_mode = false;
	m_availColor = 15;
	m_initPlayground();
	m_actPlayer = 16;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

void Game::start() {
	m_mode = 2;
	m_clientCounter = 0;
	boost::thread gameWorker(boost::bind(&Game::m_worker, this));
}

void Game::addCmd(uint8_t cmd[], int length) {
	//printf("add cmd\n");
	m_cB.mutex.lock() ;
	while(((m_cB.in + 1) % CMDBUF_SIZE) == m_cB.out)  //while cmdBuffer is full
		m_cB.condRem.wait(m_cB.mutex);
	memcpy(&m_cB.cmd[m_cB.in][0], cmd, length);
	++m_cB.in %= CMDBUF_SIZE;
	m_cB.condAdd.notify_one();
	m_cB.mutex.unlock();
}

int Game::addClient(Client *client) {
	m_clientMap.insert(std::pair<int, Client*>(++m_clientCounter, client));
	client->setId(m_clientCounter);
	client->setGame(this);
	return m_clientCounter;
}

uint8_t Game::addPlayer(Player *player, uint8_t color, uint8_t clientId) {
	m_playerMap.insert(std::pair<int, Player*>(color, player));
	player->setColor(color);
	player->setGame(this);
	player->setActCountOfDice(3);
	player->setClient(clientId);

	//assign special fields
	switch (color) {
	case 8:
		player->setInsertField(&m_playground[0]);
		player->setExitField(&m_playground[31]);
		player->setFirstEndField(&m_playground[44]);
		m_playground[32].figure = player->getFigure(0);
		m_playground[33].figure = player->getFigure(1);
		m_playground[34].figure = player->getFigure(2);
		player->getFigure(0)->startField=&m_playground[32];
		player->getFigure(1)->startField=&m_playground[33];
		player->getFigure(2)->startField=&m_playground[34];
		break;
	case 4:
		player->setInsertField(&m_playground[8]);
		player->setExitField(&m_playground[7]);
		player->setFirstEndField(&m_playground[47]);
		m_playground[35].figure = player->getFigure(0);
		m_playground[36].figure = player->getFigure(1);
		m_playground[37].figure = player->getFigure(2);
		player->getFigure(0)->startField=&m_playground[35];
		player->getFigure(1)->startField=&m_playground[36];
		player->getFigure(2)->startField=&m_playground[37];
		break;
	case 2:
		player->setInsertField(&m_playground[16]);
		player->setExitField(&m_playground[15]);
		player->setFirstEndField(&m_playground[50]);
		m_playground[38].figure = player->getFigure(0);
		m_playground[39].figure = player->getFigure(1);
		m_playground[40].figure = player->getFigure(2);
		player->getFigure(0)->startField=&m_playground[38];
		player->getFigure(1)->startField=&m_playground[39];
		player->getFigure(2)->startField=&m_playground[40];
		break;
	case 1:
		player->setInsertField(&m_playground[24]);
		player->setExitField(&m_playground[23]);
		player->setFirstEndField(&m_playground[53]);
		m_playground[41].figure = player->getFigure(0);
		m_playground[42].figure = player->getFigure(1);
		m_playground[43].figure = player->getFigure(2);
		player->getFigure(0)->startField=&m_playground[41];
		player->getFigure(1)->startField=&m_playground[42];
		player->getFigure(2)->startField=&m_playground[43];
		break;
	}

	//set figures colors and actual fields
	for(int i = 0; i < 3; i++) {
		player->getFigure(i)->color = color;
		player->getFigure(i)->field = player->getFigure(i)->startField;
	}

	return color;
}

void Game::m_worker() {
	std::map<int ,Client*>::iterator itCli;
	std::map<int ,Player*>::iterator itPla;
	int cid, color, cmd;
	uint8_t buf[CMDBUF_MAX_CMD_LENGHT + 1];

	oprintf(0, "start worker\n");

	while(m_mode) {
		m_cB.mutex.lock();
		while(m_cB.in == m_cB.out) // while cmdBffer is empty
			m_cB.condAdd.wait(m_cB.mutex);

		cid = m_cB.cmd[m_cB.out][0];
		cmd = m_cB.cmd[m_cB.out][1];
		color = m_cB.cmd[m_cB.out][2];
		memcpy(&buf, &m_cB.cmd[m_cB.out][2], CMDBUF_MAX_CMD_LENGHT - 1);

		++m_cB.out %= CMDBUF_SIZE;
		m_cB.condRem.notify_one();
		m_cB.mutex.unlock();

		printf("\ngot cmd: %d from cid=%d\n", cmd, cid);

		switch(cmd) {
		case TCP_CMD_CLOSE_CONNECTION_CS:
		//close connection to client
			printf("  close connection to cid=%d\n", cid);
			m_clientMap[cid]->setFd(0);
			m_printClientList();
			break;
		case TCP_CMD_ADD_CLIENT_CS:
			//get identity from client and verify it
			printf("  new client cid=%d (%s) connected fd=%d\n", cid, (char*)&buf, m_clientMap[cid]->getFd());
			cid = m_identifyClient(cid,(char*)buf);
			if(cid) {
				m_printClientList();
				printf("mode=%d\n",m_mode);

				//if 'select color' mode
				if(m_mode == 2) {
					m_clientMap[cid]->sendSelectColorMode();
					sleep(1);
					m_clientMap[cid]->sendAvailableColors(m_availColor);
					usleep(250000);
					m_clientMap[cid]->sendDefineClientColors();
				}

				//if 'game' mode
				if(m_mode == 3) {
					printf("client bla\n");
					m_clientMap[cid]->sendGameMode();
					sleep(1);
					m_clientMap[cid]->sendAvailableColors(m_availColor);
					usleep(250000);
					m_clientMap[cid]->sendDefineClientColors();
					usleep(250000);
					m_sendPlayground(cid);
					usleep(250000);
					m_clientMap[cid]->sendDice(m_actPlayer, m_actPips);
				}
			}
			break;

		case TCP_CMD_SET_PLAYER_NAME_CS:
			//set players name
			printf("  player pid=%d from cid=%d, set name=\"%s\"\n", color, cid, (char*)&buf);
			m_playerMap[color]->setName((char*)&buf[1]);
			break;

		case TCP_CMD_SET_COLOR_CS:
			//get color from client
			if(m_mode == 2) {
				if(m_availColor & buf[0]) {
					addPlayer(new Player, color, cid);
					printf("  player pid=%d choosed color:%d\n", buf[0], buf[0]);
					m_playerMap[color]->setColor(buf[0]);
					m_playerMap[color]->setClient(cid);
					m_availColor &= ~buf[0];
					m_clientMap[cid]->addSelectedColor(buf[0]);
					printf("  cid=%d colors=%d\n", cid, m_clientMap[cid]->getSelectedColors());
				}
				else {
					printf("\x1b[0;31mERROR: color not available\x1b[0m\n");
					m_clientMap[cid]->sendDisacceptColor(m_availColor);
				}
				m_sendAvailableColorsToAll();
			}
			break;

		case TCP_CMD_DELETE_COLOR_CS:
			//delete color
			if(m_mode == 2) {
				if(buf[0]) {
					m_availColor |= m_playerMap[buf[0]]->getColor();
					m_clientMap.find(color)->second->sendSelectColorMode();
					m_clientMap[cid]->delSelectedColor(buf[0]);
					printf("  cid=%d colors=%d\n", cid, m_clientMap[cid]->getSelectedColors());
					delete m_playerMap[color];
					m_playerMap.erase(color);
					printf("  player pid=%d deleted\n", buf[0]);
					m_sendAvailableColorsToAll();
				}
				else {
					printf("\x1b[0;31mERROR: cid=%d trys to delete a unspecified player\x1b[0m\n", cid);
					//TODO handle this
				}
			}
			break;

		case TCP_CMD_START_GAME_CS:
			//get start game command
			m_mode = 3;
			m_playersDone = m_availColor;
			printf("  pid=%d starts the game with %d\n", color, m_playersDone);
			m_sendGameModeToAll();
			m_sendPlaygroundToAll();
			m_printPlayground();

			m_selectNextPlayer();
			m_actPips = rand() % 6 + 1;
			m_playerMap[m_actPlayer]->setActCountOfDice(m_playerMap[m_actPlayer]->getActCountOfDice()-1);
			m_sendDiceToAll(m_actPlayer, m_actPips);
			break;

		case TCP_CMD_DIE_DONE_CS: {
			//client presses die-button so show the pips at al clients
			printf("  pid=%d pressed the die-button, actPlayer=%d\n", color, m_actPlayer);

			int to;
			bool movePossible = false;
			for(int i = 0; i < 3; i++){
				to = m_calculateNextField(m_playerMap[m_actPlayer]->getFigure(i)->field->no, m_actPips, m_actPlayer);
				if(to >= 0)	movePossible = true;
				printf("  to=%d mp=%d\n", to, movePossible);
			}

			m_sendShowDieToAll(movePossible);

			if(m_actPips==6 && m_calculateNextCntOfDice(m_actPlayer)==1)
				m_playerMap[m_actPlayer]->setActCountOfDice(m_playerMap[m_actPlayer]->getActCountOfDice() + 1);

			if(!movePossible) {
				sleep(2);
				if(m_playerMap[m_actPlayer]->getActCountOfDice()) {
					printf("  %d has %d times to dice left\n",m_actPlayer, m_playerMap[m_actPlayer]->getActCountOfDice());
				}
				else {
					m_playerMap[m_actPlayer]->setActCountOfDice(m_calculateNextCntOfDice(m_actPlayer));
					printf("%d hast %d times to dice the next time\n", m_actPlayer, m_playerMap[m_actPlayer]->getActCountOfDice());
					printf("next player\n");
					m_selectNextPlayer();
				}
				m_actPips = rand() % 6 + 1;
				m_playerMap[m_actPlayer]->setActCountOfDice(m_playerMap[m_actPlayer]->getActCountOfDice()-1);
				m_sendDiceToAll(m_actPlayer, m_actPips);
			}
			break; }

		case TCP_CMD_SELECT_FIGURE_CS:
		//client chooses a figure to move
		//TODO prüfen ob würfeltaste gedrückt wurde

			if(m_mode == 3){
				int from = -1, to = -1;

				if(color == m_actPlayer) {
					from = buf[1];
					to = m_calculateNextField(from, m_actPips, m_actPlayer);

					printf("  target field=%d\n", to);

					//if there is a target field
					if(to >= 0) {
						//if target field is occupied
						if(m_playground[to].figure != NULL) {
							//if occipied by firure from own color
							if(m_playground[to].figure->color == m_playerMap[m_actPlayer]->getColor()) {
								printf("%d this move is not allowed, because the is another figure at target field\n", m_actPlayer);
								m_clientMap[cid]->sendMoveNotAllowed(m_actPlayer);
								break;
							}
							//else is occupied by an other player, so kick him out
							else {
								uint8_t playerOut = m_playground[to].figure->color;
								printf("%d kicks %d out\n", m_actPlayer, playerOut);
								m_sendMoveToAll(to,m_playground[to].figure->startField->no);
								m_move(to,m_playground[to].figure->startField->no);
								m_playerMap[playerOut]->setActCountOfDice(m_calculateNextCntOfDice(playerOut));
								printf("  %d hast %d times to dice the next time\n", playerOut, m_playerMap[playerOut]->getActCountOfDice());
								m_sendMoveToAll(from, to);
								m_move(from, to);
								m_printPlayground();
							}
						}
						//if field is free
						else {
							m_sendMoveToAll(from, to);
							m_move(from, to);
							m_printPlayground();
						}
						if(to == m_playerMap[m_actPlayer]->getInsertField()->no)
							m_playerMap[m_actPlayer]->setActCountOfDice(1);
					}
					else {
						printf("%d this move is not allowed\n", m_actPlayer);
						m_clientMap[cid]->sendMoveNotAllowed(m_actPlayer);
						break;
					}

					printf("%d moves from %d to %d\n", m_actPlayer, from, to);

					if(m_playerMap[color]->getActCountOfDice()) {
						printf("  player %d again\n", m_actPlayer);
						m_actPips = rand() % 6 + 1;
						m_playerMap[m_actPlayer]->setActCountOfDice(m_playerMap[m_actPlayer]->getActCountOfDice()-1);
						m_sendDiceToAll(m_actPlayer, m_actPips);
					}
					else {
						int n = m_calculateNextCntOfDice(m_actPlayer);
						if(n > 0) {
							m_playerMap[m_actPlayer]->setActCountOfDice(n);
							printf("%d hast %d times to dice the next time\n", m_actPlayer, n);
						}
						else {
							printf("Congratulation %d, you are done\n", m_actPlayer);
							m_playersDone |= m_actPlayer;
							printf("m_playersDone=%d", m_playersDone);
							m_sendPlayerInHomeToAll(m_actPlayer);
						}
						printf("  next player\n");
						if(m_selectNextPlayer() <= 0) {
							printf("\n\n\nGAME OVER!!!\n");
							m_sendPlayerInHomeToAll(16);
							m_mode = 4;
							break;
						}
						m_actPips = rand() % 6 + 1;
						m_playerMap[m_actPlayer]->setActCountOfDice(m_playerMap[m_actPlayer]->getActCountOfDice()-1);
						m_sendDiceToAll(m_actPlayer, m_actPips);
					}
				}
				else {
					printf("\x1b[0;31mERROR: pid=%d trys to cheat\x1b[0m\n", color);
				}
			}
			break;

		case TCP_CMD_CANCEL_GAME_CS:
			m_mode = 4;
			break;

		default:
			printf("unknown command\n");
			break;
		}


	}
}

void Game::m_printClientList() {
	//prints all connected clients
	std::map<int ,Client*>::iterator itCli;
	printf("\nclientlist:\n");
	if(m_clientMap.empty()) printf("    empty\n");
	else for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++)
		printf("    cid=%d (%s) fd=%d \n",
				itCli->second->getId(),
				itCli->second->getName(),
				itCli->second->getFd());
}

void Game::m_printPlayerList() {

}

void Game::m_printPlayground() {
	field_s *field;

	printf("\nplayground\n");
	for(int y = 0; y < PLAYGROUND_ROWS; y++) {
		for(int x = 0; x < PLAYGROUND_COLUMNS; x++) {
			field = m_findInPlayground(x,y);
			if(field  != NULL) {
				if(field->figure == NULL) printf(" .");
				else printf(" %d", field->figure->color);
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}
}

field_s* Game::m_findInPlayground(uint8_t x, uint8_t y){
	for(int i = 0; i < 56; i++) {
		if(m_playground[i].x == x && m_playground[i].y == y)
			return &m_playground[i];
	}
	return NULL;
}

int Game::m_identifyClient(int cid, char buf[]) {
	//identyfi new client. if client is known, then update the known one.
	//else add new client to client list
	int known = 0;
	std::map<int ,Client*>::iterator itCli;

	//search in clientmap for received name
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		if (!strcmp(itCli->second->getName(),(char*)buf)) {
			known = itCli->first;
			break;
		}
	}
	//update new fd to known cid and delete old connected client instance
	if (known && (m_availColor || m_clientMap[known]->getSelectedColors())) {
		printf("  client known, update cid=%d\n", known);
		m_clientMap[known]->setId(cid);
		m_clientMap[known]->closeCli();
		m_clientMap[cid]->setSelectedColors(m_clientMap[known]->getSelectedColors());
		delete m_clientMap[known];
		m_clientMap[known] = m_clientMap[cid];
		m_clientMap.erase(cid);
		m_clientMap[known]->setName(buf);
		m_clientMap[known]->setId(known);
		return known;
	}
	//set name to cid
	else if(m_mode == 2 && m_availColor){
		printf("  client unknown, add cid=%d to clientlist\n", cid);
		m_clientMap[cid]->setName(buf);
		return cid;
	}
	else {
		printf("  client not authorized (cid=%d)\n", cid);
		m_clientMap[cid]->sendUserNotAuthorized();
		delete m_clientMap[cid];
		m_clientMap.erase(cid);
		return 0;
	}
	//return 0;
}

int Game::m_selectNextPlayer() {
	int n = 0;
	do {
		n++;
		if(n > 4) return -1;
		m_actPlayer = m_actPlayer>>1;
		if(!m_actPlayer) m_actPlayer = 8;
	} while(m_actPlayer & m_playersDone);
	printf("actPlayer=%d\n", m_actPlayer);
	return 1;
}

int Game::m_calculateNextField(uint8_t startField, uint8_t pips, uint8_t player) {
	uint8_t field = startField;

	if(m_playground[field].figure == NULL)
		return -1;

	if(m_playground[field].figure->color != player)
		return -1;

	if(m_playground[startField].figure->startField->no == startField) {
		if(pips == 6)
			return m_playerMap[player]->getInsertField()->no;
		else
			return -1;
	}

	while(pips) {
		if(m_playerMap[player]->getExitField()->no == field) {
			field = m_playerMap[player]->getFirstEndField()->no;
			pips--;
		}
		else if((m_playerMap[player]->getFirstEndField()->no + 2) == field){
			return -1;
		}
		else {
			field++;
			if(field > 31 && field < 44) field -= 32;
			pips--;
		}
		if(field >= 44 && m_playground[field].figure != NULL)
			return -1;
	}
	if(m_playground[field].figure != NULL) {
		if(m_playground[field].figure->color != m_playerMap[player]->getColor())
			return field;
		else
			return -1;
	}
	return field;
}

int Game::m_calculateNextCntOfDice(uint8_t player) {
	int to = -1;
	bool movePossible = false;

	for(int pips = 1; pips <= 6; pips++) {
		for(int i = 0; i < 3; i++) {
			to = m_calculateNextField(m_playerMap[player]->getFigure(i)->field->no, pips, player);
			if(to != -1) {
				movePossible = true;
				if( to != m_playerMap[player]->getInsertField()->no)
					return 1;
			}
		}
	}
	if(movePossible) return 3;
	return -1;
}

bool Game::m_allStartArea(uint8_t color) {
	printf("allStartArea? %d\n", color);
	printf("fields are: %d %d %d\n",m_playerMap[color]->getFigure(0)->field->no,
			m_playerMap[color]->getFigure(1)->field->no,
			m_playerMap[color]->getFigure(2)->field->no);

	if(m_playerMap[color]->getFigure(0)->field->no > 31 &&
			m_playerMap[color]->getFigure(1)->field->no > 31 &&
			m_playerMap[color]->getFigure(2)->field->no > 31 ) return true;
	else return false;
}

void Game::m_move(uint8_t from, uint8_t to) {
	m_playground[to].figure = m_playground[from].figure;
	m_playground[to].figure->field = &m_playground[to];
	m_playground[from].figure = NULL;
}

void Game::m_sendAvailableColorsToAll() {
	printf("  avaliable color=%d\n",m_availColor);
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    > ");
		itCli->second->sendAvailableColors(m_availColor);
	}

}


void Game::m_sendPlayerInHomeToAll(uint8_t color) {
	printf("  send 'player in home'\n");
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    > ");
		itCli->second->sendPlayerInHome(color);
	}
}


void Game::m_sendGameModeToAll() {
	printf("  send 'enter game mode'\n");
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    > ");
		itCli->second->sendGameMode();
	}
}

void Game::m_sendPlayground(uint8_t cid) {
	//printf("    send playground information to cid=%d\n", cid);
	std::map<int ,Player*>::iterator itPla;
	for(itPla = m_playerMap.begin(); itPla != m_playerMap.end(); itPla++) {
		for(int i = 0; i < 3; i++) {
			printf("     > ");
			m_clientMap[cid]->sendMove(itPla->second->getFigure(i)->startField->no, itPla->second->getFigure(i)->field->no);
			usleep(10000);
		}
	}
}

void Game::m_sendPlaygroundToAll() {
	printf("  send playground information to clients\n");
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    cid=%d\n", itCli->first);
		m_sendPlayground(itCli->first);
	}
}

void Game::m_sendDiceToAll(uint8_t color, uint8_t pips){
	printf("  send diced pips to clients\n");
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    > ");
		itCli->second->sendDice(color, pips);
	}
}

void Game::m_sendShowDieToAll(bool set) {
	printf("  send 'show die' to clients\n");
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    > ");
		itCli->second->sendShowDie(set);
	}
}

void Game::m_sendMoveToAll(uint8_t from, uint8_t to) {
	printf("  send move figure to clients\n");
	std::map<int ,Client*>::iterator itCli;
	for(itCli = m_clientMap.begin(); itCli != m_clientMap.end(); itCli++) {
		printf("    > ");
		itCli->second->sendMove(from, to);
	}
}

void Game::m_initPlayground() {
	// assign fields to its coordinates
	//m_playground.insert(std::pair<uint8_t, coordinate_s>(0, new coordinate_s(1,4)));

	for(int i = 0; i < 56; i++)
		m_playground[i].no = i;

	m_playground[0].set(1,4);
	m_playground[1].set(2,4);
	m_playground[2].set(3,4);
	m_playground[3].set(4,4);
	m_playground[4].set(4,3);
	m_playground[5].set(4,2);
	m_playground[6].set(4,1);
	m_playground[7].set(5,1);
	m_playground[8].set(6,1);
	m_playground[9].set(6,2);
	m_playground[10].set(6,3);
	m_playground[11].set(6,4);
	m_playground[12].set(7,4);
	m_playground[13].set(8,4);
	m_playground[14].set(9,4);
	m_playground[15].set(9,5);
	m_playground[16].set(9,6);
	m_playground[17].set(8,6);
	m_playground[18].set(7,6);
	m_playground[19].set(6,6);
	m_playground[20].set(6,7);
	m_playground[21].set(6,8);
	m_playground[22].set(6,9);
	m_playground[23].set(5,9);
	m_playground[24].set(4,9);
	m_playground[25].set(4,8);
	m_playground[26].set(4,7);
	m_playground[27].set(4,6);
	m_playground[28].set(3,6);
	m_playground[29].set(2,6);
	m_playground[30].set(1,6);
	m_playground[31].set(1,5);
	m_playground[32].set(0,1);
	m_playground[33].set(1,1);
	m_playground[34].set(1,0);
	m_playground[35].set(9,0);
	m_playground[36].set(9,1);
	m_playground[37].set(10,1);
	m_playground[38].set(10,9);
	m_playground[39].set(9,9);
	m_playground[40].set(9,10);
	m_playground[41].set(1,10);
	m_playground[42].set(1,9);
	m_playground[43].set(0,9);
	m_playground[44].set(2,5);
	m_playground[45].set(3,5);
	m_playground[46].set(4,5);
	m_playground[47].set(5,2);
	m_playground[48].set(5,3);
	m_playground[49].set(5,4);
	m_playground[50].set(8,5);
	m_playground[51].set(7,5);
	m_playground[52].set(6,5);
	m_playground[53].set(5,8);
	m_playground[54].set(5,7);
	m_playground[55].set(5,6);
}
