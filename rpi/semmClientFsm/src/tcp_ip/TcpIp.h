/*
 * TcpIp.h
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#ifndef TCPIP_H_
#define TCPIP_H_

// global includes
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

// local includes
#include "../defs.h"

class Game;

class TcpIp {
public:
	TcpIp( );
	virtual ~TcpIp();

	bool connect(const std::string& ip);
	void start( boost::shared_ptr< Game > gamePtr );
	bool running( ){ return _running; }

	void sendAddClient(const std::string& clientName);
	void sendSetColor(uint8_t color);
	void sendDelColor(uint8_t color);
	void sendStartGame(uint8_t pid);
	void sendDieDone(uint8_t pid);
	void sendCancelGame(uint8_t pid);
	void sendSelectMeeple(uint8_t pid, uint8_t field);

private:
	bool _connected;
	bool _running;
	int _fd;
	boost::shared_ptr< Game > _gamePtr;
	boost::mutex _mutexForFd;
	boost::shared_ptr< boost::thread > _thread;
	static const int _port = 6665;

	void worker( );
	int writeToSocket(uint8_t* buf, int length);
};

#endif /* TCPIP_H_ */
