/**
 *  class Client.
 *  @file Client.h
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "stdio.h"
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "defs.h"
#include "Game.h"

class Game;

/**
 * class Client represents a TCP/IP connection to a client, whether or not it
 * is the hardware playground client or a tablet/smartphone
 */
class Client {
public:
	/**
	 * constructor.
	 * @param fd file descriptor to TCP/IP connection
	 */
	Client(int fd);

	virtual ~Client();

	/**
	 * starts the client.
	 * method initializes and starts a thread for receiving data from client
	 * by the specified file descriptor fd
	 */
	void runCli();

	/**
	 * closes connection to client.
	 */
	void closeCli() { close(m_fd); } //TODO clean up

	/**
	 * sends the 'TCP_CMD_PLAYER_IN_HOME_SC' command to client.
	 * @param color color of player which is done
	 */
	void sendPlayerInHome(uint8_t color);

	/**
	 * sends the 'TCP_CMD_AVAILABLE_COLORS_SC' command to client.
	 * @param color colors which are available
	 */
	void sendAvailableColors(uint8_t color);

	/**
	 * sends the 'TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_SELECT_COLOR'.
	 * command to client
	 */
	void sendSelectColorMode();

	/**
	 * sends the 'TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_USER_NOT_AUTHORIZED'.
	 * command to client
	 */
	void sendUserNotAuthorized();

	/**
	 * sends the 'TCP_CMD_ACT_GAME_MODE_SC + GAME_MODE_GAME'.
	 * command to client
	 */
	void sendGameMode();

	/**
	 * sends the 'TCP_CMD_DEFINE_CLIENT_COLORS_SC' command to client.
	 */
	void sendDefineClientColors();

	/**
	 * calls sendSelectColorMode and sendAvailableColors.
	 * @param color
	 */
	void sendDisacceptColor(uint8_t color);

	/**
	 * sends the 'TCP_CMD_MOVE_SC' command to client.
	 * @param start index of field where to move from
	 * @param end index of field where to move to
	 */
	void sendMove(uint8_t start, uint8_t end);

	/**
	 * sends the 'TCP_CMD_SHOW_DIE_SC' command to client.
	 * @param set true if it is possible to move a meeple for the player
	 */
	void sendShowDie(bool set);

	/**
	 * sends the 'TCP_CMD_MOVE_NOT_ALLOWED_SC' command to client.
	 * @param color color of player who tried to cheat
	 */
	void sendMoveNotAllowed(uint8_t color);

	/**
	 * sends the 'TCP_CMD_DICE_SC' command to client.
	 * @param color color of player who is moving next
	 * @param pips no of pips
	 */
	void sendDice(uint8_t color, uint8_t pips);

	/**
	 * adds a color to the selected colors.
	 * @param color color to be set
	 */
	void addSelectedColor(uint8_t color) {m_selectedColors |= color; }

	/**
	 * removes color.
	 * @param color to be removed
	 */
	void delSelectedColor(uint8_t color) {m_selectedColors &= ~color; }

	/**
	 * returns the name of the client.
	 * @return pointer to clients name char array
	 */
	char *getName() {return m_name; }

	/**
	 * returns the file descriptor for this instance of Client.
	 * @return file descriptor
	 */
	int getFd() { return m_fd; }

	/**
	 * returns ID for this instance of Client.
	 * @return cleint ID
	 */
	int getId() { return m_Id; }

	/**
	 * returns colors that are assigned to this instance of Client
	 * @return colors
	 */
	uint8_t getSelectedColors() { return m_selectedColors; }

	/**
	 * sets the name for this instance of Client.
	 * @param name name for this client
	 */
	void setName(char name[]) { strcpy(m_name, name); }

	/**
	 * sets the file descriptor for this instance of Client.
	 * @param fd file descriptor for this instance
	 */
	void setFd(int fd) { m_fd = fd; }

	/**
	 * sets the pointer to the corresponding instance of Game for this instance
	 * of Client.
	 * @param game pointer to corresponding instance of Game
	 */
	void setGame(Game *game) { m_game = game; }

	/**
	 * sets the ID for this instance of Client.
	 * @param id client ID
	 */
	void setId(int id) { m_Id = id; }

	/**
	 * sets colors that are assigned to this instance of Client.
	 * @param color colors
	 */
	void setSelectedColors(uint8_t color) { m_selectedColors = color; }



private:
	char m_name[CLIENT_NAME_MAX_LENGTH];	///< name of Client
	//pthread_mutex_t m_mutex;				///<
	Game *m_game;							///< pointer to class Game
	boost::thread *m_thread;				///< TCP/IP receive thread
	int m_Id;								///< client ID
	int m_fd;								///< file descriptor
	uint8_t m_selectedColors;				/**< colors that belogs to this
	 	 	 	 	 	 	 	 	 	 	 	 client*/

	/**
	 * member function to receive data from TCP/IP connection.
	 *
	 * this function runs in a separated thread and whiles as long as a
	 * client sends data. It is blocked during the time of no communication.
	 * When data is receied it will be copied to the command buffer of the
	 * Game instance, referenced by m_game.
	 */
	void m_receiveTcpIp();
};
#endif /* CLIENT_H_ */
