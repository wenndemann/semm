/** @mainpage semmServer

 *  @section sec1 Introduction
 *
 *  //TODO
 *
 *  @section sec2 Subsystems
 *  @subsection sec2_1 TCP/IP
 *  @subsection sec2_2 course of the game
 */

/**
 *  main file of the project.
 *  @file semmServer.cpp
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

//* include files
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
//#include <strings.h>
//#include <string>
//#include <map>
//#include <algorithm>

//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <pthread.h>
//#include <math.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "Client.h"
#include "Game.h"
#include "Player.h"

#include "tools.h"
#include "defs.h"

//* defines section
#define SEMM_NUM_THREADS 8

//* global variables section
//pthread_t* pthreadTcpIpListen;
//std::map<int, Client*> g_clientMap;

//* prototype section
void* tcpIpListen(void* g);

/**
 * main function of semmServer.
 * @param argc number of cli arguments
 * @param argv value array of cli arguments
 * @return 0=success
 * 		   1=fail
 *
 * The main function initializes and starts the TCP/IP listen thread and whiles
 * in a loop during the hole g_game. If the game mode becomes 4 it will leave
 * the loop and exit the program.
 */
int main(int argc, char** argv)
{
	Game* game; ///< global instance of Game
	boost::thread* tcpIpThread;

	srand (time(NULL));
	while(true) {
		game = new Game;
		tcpIpThread = new boost::thread(boost::bind(tcpIpListen, game));

		while(game->getMode() != 4)
			sleep(1);

		tcpIpThread->join();

		delete tcpIpThread;
		delete game;
	}
	return 0;
}

/**
 * connects to enquiring clients
 * @param arg not used
 *
 * the function is splited in three sections.
 *  1. start game
 *  	The game is started by the call of the start method by the global
 *  	instance class Game.
 *  2. initialize server
 *  	The server is initialized by preparing its address structure and bind
 *  	this to port=TCP_PORT_GUI.
 *  3. listening for enquiring clients
 *  	Listen in an infinity loop to port=TCP_PORT_GUI and accept its
 *  	enquirement. After this a new clients will be arranged and added to
 *  	the global instance of Game by its addClient(Client) method.
 */
void* tcpIpListen(void* g) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	Client *cli;
	Game* game = static_cast<Game*>(g);

	// start game
	game->start();

	// initialize server
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	perror("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = TCP_PORT_GUI;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}

	// listening for enquiring clients
	printf("listening to port %d\n", TCP_PORT_GUI);

	while(game->getMode() != 4) {
		listen(sockfd,5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		  perror("ERROR on accept");
		cli = new Client(newsockfd);
		game->addClient(cli);
		cli->runCli();
	}
	return NULL;
}
