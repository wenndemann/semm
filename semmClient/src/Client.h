#ifndef CLIENT_H_
#define CLIENT_H_

#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "defs.h"
#include "Game.h"
#include "../general/tools.h"
#include "../general/cli.h"

class Game;

class Client {
public:
	Client(int fd);
	virtual ~Client();
	void runCli();
	void closeCli() {
		close(m_fd);
	}

	void sendAddClient(const char *name);

	void sendSetPlayerName(uint8_t pid, const char *name);
	void sendSetColor(uint8_t color);
	void sendDelColor(uint8_t color);
	void sendStartGame(uint8_t pid);
	void sendDieDone(uint8_t pid);
	void sendCancelGame(uint8_t pid);
	void sendSelectFigure(uint8_t pid, uint8_t field);

	void sendCmd(uint8_t cmd[], int length);

	char *getName(int client) {
		return m_name;
	}
	int getFd() {
		return m_fd;
	}
	int getId() {
		return m_Id;
	}

	void setName(const char *id) {
		strcpy(m_name, id);
	}
	void setFd(int fd) {
		m_fd = fd;
	}
	void setGame(Game *game) {
		m_game = game;
	}
	void setId(int id) {
		m_Id = id;
	}
private:
	char m_name[CLIENT_NAME_MAX_LENGTH];
	boost::mutex m_mutex;
	Game *m_game;
	boost::thread *m_thread;
	int m_Id;
	int m_fd;

	void m_receiveTcpIp();

};

#endif /* CLIENT_H_ */
