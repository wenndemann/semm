/**
 *  global defines.
 *  @file defs.h
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

#ifndef DEFS_H_
#define DEFS_H_

// general defines
#define CLIENT_ID_LENGHT 16
#define CMDBUF_MAX_CMD_LENGHT 20
#define CMDBUF_SIZE 20
#define CLIENT_NAME_MAX_LENGTH 20
#define PLAYER_NAME_MAX_LENGTH 20
#define DELAY_SEND 0

// TCP/IP DEFINES
// SC = server -> client
// CS = client -> server
#define TCP_PORT_GUI 6665
#define TCP_MSG_LENGTH CMDBUF_SIZE
#define TCP_CMD_PLAYER_IN_HOME_SC 40
#define TCP_CMD_CLOSE_CONNECTION_CS 60
#define TCP_CMD_IDENTIFY_SC 65
#define TCP_CMD_SET_PLAYER_NAME_CS 67
#define TCP_CMD_ADD_CLIENT_CS 66
#define TCP_CMD_SEND_PID_SC 68
#define TCP_CMD_ACT_GAME_MODE_SC 70
#define TCP_CMD_AVAILABLE_COLORS_SC 80
#define TCP_CMD_SET_COLOR_CS 81
#define TCP_CMD_DELETE_COLOR_CS 82
#define TCP_CMD_DEFINE_CLIENT_COLORS_SC 91
#define TCP_CMD_START_GAME_CS 95
#define TCP_CMD_MOVE_SC 100
#define TCP_CMD_DIE_DONE_CS 105
#define TCP_CMD_SHOW_DIE_SC 106
#define TCP_CMD_DICE_SC 110
#define TCP_CMD_SELECT_FIGURE_CS 115
#define TCP_CMD_MOVE_NOT_ALLOWED_SC 116
#define TCP_CMD_CANCEL_GAME_CS 117


// GAME MODES
#define GAME_MODE_SELECT_COLOR 0
#define GAME_MODE_USER_NOT_AUTHORIZED 1
#define GAME_MODE_GAME 20

// GAME COLORS
// 1 = color free
#define GAME_COLOR_MARCEL 1
#define GAME_COLOR_MARTINA 2
#define GAME_COLOR_ELA 4
#define GAME_COLOR_STEFAN 8

// playground
#define PLAYGROUND_ROWS 11
#define PLAYGROUND_COLUMNS 11
#define PLAYGROUND_FIELD_EMPTY 255

struct tcp_data {
	int newsockfd;
	std::string name;

	tcp_data() { newsockfd = 0; }
	tcp_data(int newsock) { newsockfd = newsock; }
};

#endif
