/*
 * Client.cpp *
 *  Created on: Mar 18, 2013
 *      Author: reinhard
 */

#include "Client.h"

Client::Client(int fd)
 :m_fd(fd){
	m_game = NULL;
	m_Id = 0;
	m_thread = NULL;
	m_selectedColors = 0;
}

Client::~Client() {
	if (m_fd) close(m_fd);
}

void Client::runCli() {
	//pthread_mutex_init(&m_mutex, NULL);
	//boost::thread threadRepeatedSend(boost::bind(&Client::m_repeatedSend, this));
	boost::thread threadTcpIpParse(boost::bind(&Client::m_receiveTcpIp, this));
}

void Client::m_receiveTcpIp() {
	int nR = 0;
	uint8_t buf[TCP_MSG_LENGTH + 1];
	printf("\nstart reading data from id=%d\n", m_Id);
	do {
		memset(&buf, 0 ,TCP_MSG_LENGTH);
		nR = read(m_fd, &buf[1], TCP_MSG_LENGTH);
		if (nR < 0) {
			if (errno != EBADF) perror("ERROR reading to TCP IP: ");
		}
		else if(nR > 0) {
			printf("\nget %d chars \"%d\" from id=%d\n",nR ,buf[1], m_Id);
			printf("  %d %d %d %d %d %d\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
			buf[0] = m_Id;
			m_game->addCmd(buf,nR+1);
		}
	} while(nR > 0);
	printf("\nstop reading data from id=%d\n", m_Id);
	close(m_fd);
}

void Client::sendPlayerInHome(uint8_t color) {
	/** amount of data to be send = 3
	 * @li byte[0] = TCP_CMD_PLAYER_IN_HOME_SC
	 * @li byte[1] = color
	 * @li byte[2] = '\0'
	 */
	uint8_t buf[3];
	int nS = 0;

	buf[0] = TCP_CMD_PLAYER_IN_HOME_SC;
	buf[1] = color;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	printf("send %d chars \"player in home\"(%d) to cid=%d\n",nS , color, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendAvailableColors(uint8_t color) {
	/** amount of data to be send = 3
	 * @li byte[0] = TCP_CMD_AVAILABLE_COLORS_SC
	 * @li byte[1] = color
	 * @li byte[2] = '\0'
	 */
	uint8_t buf[3];
	int nS = 0;

	buf[0] = TCP_CMD_AVAILABLE_COLORS_SC;
	buf[1] = color;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	printf("send %d chars \"available colors\"(%d) to cid=%d\n",nS , color, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendSelectColorMode() {
	/** amount of data to be send = 2
	 * @li byte[0] = TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_SELECT_COLOR
	 * @li byte[1] ='\0'
	 */
	uint8_t buf[2];
	int nS = 0;

	buf[0] = TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_SELECT_COLOR;
	buf[1] = '\0';
	nS = write(m_fd, &buf, 2);
	printf("send %d chars \"select color mode\"(%d) cid=%d\n",nS , TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_SELECT_COLOR, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendUserNotAuthorized() {
	/** amount of data to be send = 2
	 * @li byte[0] = TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_USER_NOT_AUTHORIZED
	 * @li byte[1] ='\0'
	 */
	uint8_t buf[2];
	int nS = 0;

	buf[0] = TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_USER_NOT_AUTHORIZED;
	buf[1] = '\0';
	nS = write(m_fd, &buf, 2);
	printf("send %d chars \"USER NOT AUTHORIZED\"(%d) to cid=%d\n",
			nS , TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_USER_NOT_AUTHORIZED, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendGameMode() {
	/** amount of data to be send = 2
	 * @li byte[0] = TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_GAME
	 * @li byte[1] ='\0'
	 */
	uint8_t buf[2];
	int nS = 0;

	buf[0] = TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_GAME;
	buf[1] = '\0';
	nS = write(m_fd, &buf, 2);
	printf("send %d chars \"game mode\"(%d) to cid=%d\n",nS , TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_GAME, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendDefineClientColors() {
	/** amount of data to be send = 3
	 * @li byte[0] = TCP_CMD_DEFINE_CLIENT_COLORS_SC
	 * @li byte[1] = m_selectedColors
	 * @li byte[3] = '\0'
	 */
	uint8_t buf[3];
	int nS = 0;

	buf[0] = TCP_CMD_DEFINE_CLIENT_COLORS_SC;
	buf[1] = m_selectedColors;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	printf("send %d chars \"DEFINE CLIENT COLORS\"(%d) to cid=%d\n",nS , m_selectedColors, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendDisacceptColor(uint8_t color) {
	//go back to select color and send available colors
	sendSelectColorMode();
	sendAvailableColors(color);
	usleep(DELAY_SEND);
}

void Client::sendMove(uint8_t start, uint8_t end) {
	/** amount of data to be send = 4
	 * @li byte[0] = TCP_CMD_MOVE_SC
	 * @li byte[1] = start + 1
	 * @li byte[2] = end + 1
	 * @li byte[3] = '\0'
	 */
	uint8_t buf[4];
	int nS = 0;

	buf[0] = TCP_CMD_MOVE_SC;
	buf[1] = start + 1;
	buf[2] = end + 1;
	buf[3] = '\0';
	nS = write(m_fd, &buf, 4);
	printf("send %d chars \"MOVE\"(%d-%d) cid=%d\n",nS , start, end, m_Id);
	usleep(1);
//	usleep(DELAY_SEND);
}

void Client::sendShowDie(bool set) {
	/** amount of data to be send = 3
	 * @li byte[0] = TCP_CMD_SHOW_DIE_SC
	 * @li byte[1] = set
	 * @li byte[2] = '\0'
	 */
	uint8_t buf[3];
	int nS = 0;

	buf[0] = TCP_CMD_SHOW_DIE_SC;
	buf[1] = set;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	printf("send %d chars \"SHOW DIE\"(%d) to cid=%d set=%d\n",nS , TCP_CMD_SHOW_DIE_SC, m_Id, set);
	usleep(DELAY_SEND);
}

void Client::sendMoveNotAllowed(uint8_t color) {
	/** amount of data to be send = 3
	 * @li byte[0] = TCP_CMD_MOVE_NOT_ALLOWED_SC
	 * @li byte[1] = color
	 * @li byte[2] = '\0'
	 */
	uint8_t buf[3];
	int nS = 0;

	buf[0] = TCP_CMD_MOVE_NOT_ALLOWED_SC;
	buf[1] = color;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	printf("send %d chars \"MOVE NOT ALLOWED\"(%d) to cid=%d\n",nS , TCP_CMD_MOVE_NOT_ALLOWED_SC, m_Id);
	usleep(DELAY_SEND);
}

void Client::sendDice(uint8_t color, uint8_t pips) {
	/** amount of data to be send = 4
	 * @li byte[0] = TCP_CMD_DICE_SC
	 * @li byte[1] = color
	 * @li byte[2] = pips
	 * @li byte[3] = '\0'
	 */
	uint8_t buf[4];
	int nS = 0;

	buf[0] = TCP_CMD_DICE_SC;
	buf[1] = color;
	buf[2] = pips;
	buf[3] = '\0';
	nS = write(m_fd, &buf, 4);
	printf("send %d chars \"DICE\"(%d) cid=%d\n",nS , pips, m_Id);
	printf("      > %d %d %d\n", buf[0], buf[1], buf[2]);
	usleep(DELAY_SEND);
}
