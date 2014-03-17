/*
 * Client.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: reinhard
 */

#include "Client.h"

Client::Client(int fd) :
m_fd(fd) {
	m_Id = 0;
	m_game = NULL;
	m_thread = NULL;
}

Client::~Client() {
	// TODO check if nesessary
}

void Client::runCli() {
	boost::thread threadTcpIpParse(boost::bind(&Client::m_receiveTcpIp, this));
}

void Client::m_receiveTcpIp() {
	int nR = 0;
	uint8_t inputBuf[TCP_MSG_LENGTH + 1];
	uint8_t buf[TCP_MSG_LENGTH + 1];
	debug("start reading data\n");
//	FILE* a;

	memset(&buf, 0 ,TCP_MSG_LENGTH);

//	do {
//		nR = read(m_fd, &input, 1);
//		if (nR < 0) {
//			if (errno != EBADF) debug("ERROR reading to TCP IP: %s", strerror(errno));
//		}
//		else if(nR > 0) {
//			if(input == '\0') {
//				debug("get %d chars \"%d\"\n",counter ,buf[1]);
//				m_game->addCmd(buf,counter);
//				counter = 1;
//				buf[0] = 0;
//				memset(&buf, 0 ,TCP_MSG_LENGTH);
//			}
//			else {
//				buf[counter++] = input;
//			}
//		}
//
////	}while( nR > 0);
//	}while( 1);

	do {
		memset(&inputBuf, 0 ,TCP_MSG_LENGTH);
		nR = read(m_fd, &inputBuf[0], TCP_MSG_LENGTH);

//		debug("got data (%d)\n",nR);
//		for(int j = 0; j < nR; j++)
//			debug("   %d\n",inputBuf[j]);

		if (nR < 0) {
			if (errno != EBADF) debug("ERROR reading to TCP IP: %s", strerror(errno));
		}
		else if(nR > 0) {
			int a = 1;
			for(int i = 0; i < nR; i++) {
				buf[a++] = inputBuf[i];
				if((inputBuf[i] == '\0') || i == (nR-1)) {
					buf[0] = 0;
					m_game->addCmd(buf, a-1);
//					debug("client addCmd (%d)\n", buf[1]);
//					for(int j = 0; j < (a); j++)
//						debug("   %d\n",buf[j]);
					a = 1;
					memset(&buf, 0 ,TCP_MSG_LENGTH);
				}
			}
		}
	}while( nR > 0);
//	}while( 1);


//	do {
//		memset(&buf, 0 ,TCP_MSG_LENGTH);
//		nR = read(m_fd, &buf[1], TCP_MSG_LENGTH);
//		if (nR < 0) {
//			if (errno != EBADF) debug("ERROR reading to TCP IP: %s", strerror(errno));
//		}
//		else if(nR > 0) {
//			debug("get %d chars \"%d\"\n",nR ,buf[1]);
//			buf[0] = 0;
//			m_game->addCmd(buf,nR);
//		}
//		/*else {
//			buf[0] = m_Id;
//			buf[1] = TCP_CMD_CLOSE_CONNECTION_CS;
//			m_game->addCmd(buf,2);
//		}*/
//	} while(nR > 0);


	debug("\nstop reading data\n");
	close(m_fd);
}

void Client::sendAddClient(const char *name) {
	m_mutex.lock();
	//add this client to servers clientlist
	uint8_t buf[20];
	int nS = 0;

	buf[0] = TCP_CMD_ADD_CLIENT_CS;
	memcpy(&buf[1], name, 18);
	nS = write(m_fd, &buf, 20);
	debug("send %d chars \"ADD CLIENT\"(%s) to cid=%d\n"
			,nS , name, m_Id);
	m_mutex.unlock();
}

void Client::sendSetPlayerName(uint8_t pid, const char *name) {
	m_mutex.lock();
	//send the selected color to server
	uint8_t buf[20];
	int nS = 0;

	buf[0] = TCP_CMD_SET_COLOR_CS;
	memcpy(&buf[1], name, 18);
	nS = write(m_fd, &buf, 20);
	debug("send %d chars \"SET_PLAYER_NAME\"(%s) to pid=%d (cid=%d)\n"
			,nS , name, pid, m_Id);
	m_mutex.unlock();
}

void Client::sendSetColor(uint8_t color) {
	m_mutex.lock();
	//send the selected color to server
	uint8_t buf[4];
	int nS = 0;

	buf[0] = TCP_CMD_SET_COLOR_CS;
	buf[1] = color;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	debug("send %d chars \"SET_COLOR\"(%d) (cid=%d)\n"
			,nS , color, m_Id);
	m_mutex.unlock();
}

void Client::sendDelColor(uint8_t color) {
	m_mutex.lock();
	//send the color to delete to server
	uint8_t buf[4];
	int nS = 0;

	buf[0] = TCP_CMD_DEL_COLOR_CS;
	buf[1] = color;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 3);
	debug("send %d chars \"DEL_COLOR\"(%d) (cid=%d)\n"
			,nS , color, m_Id);
	m_mutex.unlock();
}

void Client::sendStartGame(uint8_t pid) {
	m_mutex.lock();
	//send 'start game' to server
	uint8_t buf[2];
	int nS = 0;

	buf[0] = TCP_CMD_START_GAME_CS;
	buf[1] = '\0';
	nS = write(m_fd, &buf, 2);
	debug("send %d chars \"START_GAME\"(%d) to pid=%d (cid=%d)\n"
			,nS , TCP_CMD_START_GAME_CS, pid, m_Id);
	m_mutex.unlock();
}

void Client::sendDieDone(uint8_t pid) {
	m_mutex.lock();
	//send 'die done' to server
	uint8_t buf[2];
	int nS = 0;

	buf[0] = TCP_CMD_DIE_DONE_CS;
	buf[1] = '\0';
	nS = write(m_fd, &buf, 2);
	debug("send %d chars \"DIE DONE\"(%d) to pid=%d (cid=%d)\n"
			,nS , TCP_CMD_DIE_DONE_CS, pid, m_Id);
	m_mutex.unlock();
}

void Client::sendCancelGame(uint8_t pid) {
	m_mutex.lock();
	//send 'cancel game' to server
	uint8_t buf[3];
	int nS = 0;

	buf[0] = TCP_CMD_CANCEL_GAME_CS;
	buf[1] = pid;
	buf[2] = '\0';
	nS = write(m_fd, &buf, 2);
	debug("send %d chars \"Cancel Game from player %d\"(%d) to server (cid=%d)\n"
			, nS, pid, TCP_CMD_CANCEL_GAME_CS, m_Id);
	m_mutex.unlock();
}


void Client::sendSelectFigure(uint8_t pid, uint8_t field) {
	m_mutex.lock();
	//send selected figure to move
	uint8_t buf[4];
	int nS = 0;

	buf[0] = TCP_CMD_SELECT_FIGURE_CS;
	buf[1] = pid;
	buf[2] = field;
	buf[3] = '\0';
	nS = write(m_fd, &buf, 4);
	debug("send %d chars \"SELECT FIGURE\"(%d) to pid=%d (cid=%d)\n"
			,nS , field, pid, m_Id);
	m_mutex.unlock();
}

void Client::sendCmd(uint8_t cmd[], int length) {
	m_mutex.lock();
	int nS = 0;

	nS = write(m_fd, cmd, length);
	debug("send %d chars cid=%d\n" ,nS ,m_Id);
	m_mutex.unlock();
}
