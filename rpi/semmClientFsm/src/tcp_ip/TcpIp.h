/*
 * TcpIp.h
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#ifndef TCPIP_H_
#define TCPIP_H_

// global includes
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

// local includes
#include "../defs.h"

class TcpIp {
public:
	TcpIp();
	virtual ~TcpIp();

	bool connect(const std::string& ip);

	void sendAddClient(const std::string& clientName);
	void sendSetColor(uint8_t color);
	void sendDelColor(uint8_t color);
	void sendStartGame(uint8_t pid);
	void sendDieDone(uint8_t pid);
	void sendCancelGame(uint8_t pid);
	void sendSelectFigure(uint8_t pid, uint8_t field);

private:
	bool _connected;
	int _fd;
	boost::mutex _mutexForFd;
	static const int _port = 6665;

	int writeToSocket(uint8_t* buf, int length);
};

#endif /* TCPIP_H_ */
