/*
 * TcpIp.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "TcpIp.h"

#include "../game/Game.h"

using namespace std;

TcpIp::TcpIp( boost::shared_ptr< Game > gamePtr )
: _connected(false)
, _fd(0)
, _gamePtr(gamePtr)
{

}

TcpIp::~TcpIp() {

}

bool TcpIp::connect(const std::string& ip) {
	if(_connected) return false;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	struct hostent *server = NULL;
	if (sockfd < 0) {
		cout << "ERROR opening socket" << endl;
		return false;
	}
	server = gethostbyname(ip.c_str());
	if (server == NULL) {
		cout << "ERROR, no such host\n" << endl;
		return false;
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr,
	server->h_length);
	serv_addr.sin_port = htons(_port);
	cout << "connecting to " << ip << "(" //%d.%d.%d.%d)\n", ip,
		 << int( serv_addr.sin_addr.s_addr & 0x000000FF) <<        "."
		 <<	int((serv_addr.sin_addr.s_addr & 0x0000FF00) >>  8) << "."
		 <<	int((serv_addr.sin_addr.s_addr & 0x00FF0000) >> 16) << "."
		 <<	int((serv_addr.sin_addr.s_addr & 0xFF000000) >> 24) << ")"
		 << endl;
	if (::connect(sockfd, (struct sockaddr *) &serv_addr,
				  sizeof(serv_addr)) < 0) {
		perror("ERROR connecting");
		return false;
	}
	_fd = sockfd;
	_connected = true;

	_thread.reset( new boost::thread( boost::bind(&TcpIp::worker, this) ) );

	return true;
}

void TcpIp::worker( )
{
	int32_t nR = 0;
	uint8_t inputBuf[TCP_MSG_LENGTH + 1];
	uint8_t buf[TCP_MSG_LENGTH + 1];
	std::cout << "start reading data\n" << std::endl;

	memset(&buf, 0 ,TCP_MSG_LENGTH);

	do {
		memset(&inputBuf, 0 ,TCP_MSG_LENGTH);
		nR = read(_fd, &inputBuf[0], TCP_MSG_LENGTH);

		if (nR < 0) {
			if (errno != EBADF) std::cout << "ERROR reading to TCP IP: " << strerror(errno) << std::endl;
		}
		else if(nR > 0) {
			std::stringstream ss;
			for ( int32_t i = 0; i < nR; i++ )
			{
				ss << static_cast<int32_t>( inputBuf[ i ] ) << " ";
			}
			std::cout << ss.str( ) << std::endl;
			_gamePtr->parseCmd(inputBuf, nR); // TODO
		}
	}while( nR > 0);

	std::cout << "stop reading data" << std::endl;
	close(_fd);
}

void TcpIp::sendAddClient(const string& name) {
	//add this client to servers clientlist
	uint8_t buf[20];
	buf[0] = TCP_CMD_ADD_CLIENT_CS;
	memcpy(&buf[1], &name, 18);
	writeToSocket(buf, 20);
	cout << "send command \"ADD CLIENT\" to server" << endl;
}

void TcpIp::sendSetColor(uint8_t color) {
	//send the selected color to server
	uint8_t buf[4];
	buf[0] = TCP_CMD_SET_COLOR_CS;
	buf[1] = color;
	buf[2] = '\0';
	writeToSocket(buf, 3);
	cout << "send command \"SET_COLOR\" (" << color << ") to server" << endl;
}

void TcpIp::sendDelColor(uint8_t color) {
	//send the color to delete to server
	uint8_t buf[4];
	buf[0] = TCP_CMD_DEL_COLOR_CS;
	buf[1] = color;
	buf[2] = '\0';
	writeToSocket(buf, 3);
	cout << "send command\"DEL_COLOR\" (" <<  color << ") to server" << endl;
}

void TcpIp::sendStartGame(uint8_t pid) {
	//send 'start game' to server
	uint8_t buf[2];
	buf[0] = TCP_CMD_START_GAME_CS;
	buf[1] = '\0';
	writeToSocket(buf, 2);
	cout << "send command\"START_GAME\" (" << pid << ") to server" << endl;
}

void TcpIp::sendDieDone(uint8_t pid) {
	//send 'die done' to server
	uint8_t buf[2];
	buf[0] = TCP_CMD_DIE_DONE_CS;
	buf[1] = '\0';
	writeToSocket(buf, 2);
	cout << "send command\"DIE DONE\" (" << pid << ") to server" << endl;
}

void TcpIp::sendCancelGame(uint8_t pid) {
	//send 'cancel game' to server
	uint8_t buf[3];
	buf[0] = TCP_CMD_CANCEL_GAME_CS;
	buf[1] = pid;
	buf[2] = '\0';
	writeToSocket(buf, 3);
	cout << "send command\"Cancel Game\" (" << pid << ") to server" << endl;
}


void TcpIp::sendSelectFigure(uint8_t pid, uint8_t field) {
	//send selected figure to move
	uint8_t buf[4];
	buf[0] = TCP_CMD_SELECT_FIGURE_CS;
	buf[1] = pid;
	buf[2] = field;
	buf[3] = '\0';
	writeToSocket(buf, 4);
	cout << "send command\"SELECT FIGURE\" (" << pid << ", "
											  << field << ") to server" << endl;
}

int TcpIp::writeToSocket(uint8_t* buf, int length) {
	boost::lock_guard<boost::mutex> guard(_mutexForFd);
	return write(_fd, buf, length);
}