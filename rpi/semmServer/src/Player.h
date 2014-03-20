/**
 *  class Player.
 *  @file Player.h
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <boost/thread.hpp>

#include "Game.h"
#include "defs.h"

class Game;
struct field_s;

struct figure_s {
	uint8_t color;
	field_s *field;
	field_s *startField;
};
/**
 * Player represents a real player at the playground, whether or not it is a
 * human at the playground, a human at a tablet/smartphone or a computer
 * opponent
 */
class Player {
public:
	Player();
	virtual ~Player();

	/**
	 * send the command 'TCP_CMD_MOVE_NOT_ALLOWED_SC' to the player.
	 */
	void sendMoveNotAllowed();

	/**
	 * return the specific color of this instance of Player.
	 * @return players color
	 */
	uint8_t getColor() { return m_color; }

	/**
	 * returns the name of the player.
	 * @return
	 */
	const char *getName() { return m_name; }

	/**
	 * returns a pointer to a specific figure structure.
	 * @param i index of figure structure
	 * @return pointer to specific figure structure
	 */
	figure_s *getFigure(uint8_t i) { return &m_figures[i]; }

	/**
	 * returns a pointer to the field structure of the insert field of this
	 * instance of Player.
	 * @return pointer to insert field structure
	 */
	field_s *getInsertField() { return m_insertField; }

	/**
	 * returns a pointer to the field structure of the exit field of this
	 * instance of Player.
	 * @return pointer to exit field structure
	 */
	field_s *getExitField() { return m_exitField; }

	/**
	 * returns a pointer to the field structure of the first end field of this
	 * instance of Player.
	 * @return pointer to first end field structure
	 */
	field_s *getFirstEndField() { return m_firstEndField; }

	/**
	 * returns how many times a player is allowed to dice.
	 * @return no of dices
	 */
	uint8_t getActCountOfDice() { return m_actCountOfDice; }

	/**
	 * returns the client ID for this instance of Player.
	 * @return client ID
	 */
	int getClientId() { return m_client; }

	/**
	 * sets the client ID for this instance of Player.
	 * @param client new client ID
	 */
	void setClient(int client) { m_client = client; }

	/**
	 * sets the color for this instance of Player.
	 * @param color new color code
	 */
	void setColor(uint8_t color) {m_color = color; }

	/**
	 * sets the pointer to the corresponding instance of Game for this instance
	 * of Player.
	 * @param game pointer to corresponding instance of Game
	 */
	void setGame(Game *game) { m_game = game; }

	/**
	 * sets the name for this instance of Player.
	 * @param id new players name
	 */
	void setName(char id[]) { strcpy(m_name, id); }

	/**
	 * sets the pointer of the insert field for this instance of Player.
	 * @param field pointer to insert field
	 */
	void setInsertField(field_s *field) { m_insertField = field; }

	/**
	 * sets the pointer of the exit field for this instance of Player.
	 * @param field pointer to exit field
	 */
	void setExitField(field_s *field) { m_exitField = field; }

	/**
	 * sets the pointer of the first end field for this instance of Player.
	 * @param field pointer to first end field
	 */
	void setFirstEndField(field_s *field) { m_firstEndField = field; }

	/**
	 * sets how many timer this instance of player is allowed to dice.
	 * @param i no of dices
	 */
	void setActCountOfDice(uint8_t i) { m_actCountOfDice = i; }

private:
	char m_name[PLAYER_NAME_MAX_LENGTH];	///< name of player
	int m_client;							///< client ID
	Game *m_game;							///< pointer to class Game
	uint8_t m_color;						///< color of player
	figure_s m_figures[3];					/**< pointer array to players three
												 meeples*/
	field_s *m_insertField;					///< pointer to insert field
	field_s *m_exitField;					///< pointer to exit field
	field_s *m_firstEndField;				///< pointer to first end field
	uint8_t m_actCountOfDice;				///< no of dices
};

#endif /* PLAYER_H_ */
