#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>
#include <signal.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <cerrno>

#include "defs.h"
#include "Client.h"
#include "Game.h"
#include "../general/tools.h"
#include "../general/menu.h"
#include "../general/cli.h"

pthread_t g_pthreadTcpConnect;
Client* g_client;
Game* g_game;
uint8_t g_actScreen;
CliDisplay* g_cliDisplay[4];

int tcpConnect(const char* ip);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf(" not enough arguments\n");
		printf(" usage: semmClient IP\n");
		return 1;
	} else {
		int fd = 0;
		int actItem = 0;
		char ch;
		uint8_t buf[5];
		buf[0] = 0;
		buf[1] = DISPLAY_CMD_PUSH_ENTER;

		initUi();
		refresh();

		debug("********************\n");
		debug("* SEMM Client 2014 *\n");
		debug("********************\n");
		fd = tcpConnect(argv[1]);
		if (fd < 0) {
			debug("failed to connect to %s\n", argv[1]);
			return 1;
		} else {
			while (1) {
				g_game = new Game;

				for (int i = 0; i < 4; i++)
					g_game->setCliDisplay(i, g_cliDisplay[i]);

				debug("connected to %s\n", argv[1]);
				g_game->start();
				g_client = new Client(fd);
				g_client->setGame(g_game);
				g_game->setClient(g_client);
				g_client->runCli();
				g_client->sendAddClient("semmbox");

				while ((ch = getch()) != 104 && (g_game->getMode() <= 4)) {
					switch (ch) {
					case 'y':
						g_cliDisplay[3]->sendKey(REQ_DOWN_ITEM);
						break;
					case 'q':
						g_cliDisplay[3]->sendKey(REQ_UP_ITEM);
						break;
					case 'x':
						g_cliDisplay[2]->sendKey(REQ_DOWN_ITEM);
						break;
					case 'w':
						g_cliDisplay[2]->sendKey(REQ_UP_ITEM);
						break;
					case 'c':
						g_cliDisplay[1]->sendKey(REQ_DOWN_ITEM);
						break;
					case 'e':
						g_cliDisplay[1]->sendKey(REQ_UP_ITEM);
						break;
					case 'v':
						g_cliDisplay[0]->sendKey(REQ_DOWN_ITEM);
						break;
					case 'r':
						g_cliDisplay[0]->sendKey(REQ_UP_ITEM);
						break;

					case 'f':
						actItem = g_cliDisplay[0]->getActMenuItem();
						debug("Display[0]item:->getActMenuItem() = %d\n",
								actItem);
						if (actItem > 0) {
							buf[2] = 0;
							buf[3] = g_cliDisplay[0]->getActPicNumber();
							buf[4] = actItem;
							g_game->addCmd(buf, 5);
						}
						break;
					case 'd':
						actItem = g_cliDisplay[1]->getActMenuItem();
						debug("Display[1]item:->getActMenuItem() = %d\n",
								actItem);
						if (actItem > 0) {
							buf[2] = 1;
							buf[3] = g_cliDisplay[1]->getActPicNumber();
							buf[4] = actItem;
							g_game->addCmd(buf, 5);
						}
						break;
					case 's':
						actItem = g_cliDisplay[2]->getActMenuItem();
						debug("Display[2]item:->getActMenuItem() = %d\n",
								actItem);
						if (actItem > 0) {
							buf[2] = 2;
							buf[3] = g_cliDisplay[2]->getActPicNumber();
							buf[4] = actItem;
							g_game->addCmd(buf, 5);
						}
						break;
					case 'a':
						actItem = g_cliDisplay[3]->getActMenuItem();
						debug("Display[3]item:->getActMenuItem() = %d\n",
								actItem);
						if (actItem > 0) {
							buf[2] = 3;
							buf[3] = g_cliDisplay[3]->getActPicNumber();
							buf[4] = actItem;
							g_game->addCmd(buf, 5);
						}
						break;
					}
				}
				delete g_client;
				delete g_game;
			}
		}
	}

	return 0;
}

int tcpConnect(const char* ip) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	struct hostent *server = NULL;
	if (sockfd < 0) {
		perror("ERROR opening socket");
		return -1;
	}
	server = gethostbyname(ip);
	if (server == NULL) {
		perror("ERROR, no such host\n");
		return -1;
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr,
	server->h_length);
	serv_addr.sin_port = htons(6665);
	debug("connecting to %s (%d.%d.%d.%d)\n", ip,
			int( serv_addr.sin_addr.s_addr & 0x000000FF),
			int((serv_addr.sin_addr.s_addr & 0x0000FF00) >> 8),
			int((serv_addr.sin_addr.s_addr & 0x00FF0000) >> 16),
			int((serv_addr.sin_addr.s_addr & 0xFF000000) >> 24));
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		perror("ERROR connecting");
		return -1;
	}
	return sockfd;
}
