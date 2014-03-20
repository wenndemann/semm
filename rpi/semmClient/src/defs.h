#ifndef DEFS_H_
#define DEFS_H_

// general defines
#define CLIENT_ID_LENGHT 16
#define CMDBUF_MAX_CMD_LENGHT 20
#define CMDBUF_SIZE 100
#define CLIENT_NAME_MAX_LENGTH 20
#define PLAYER_NAME_MAX_LENGTH 20

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
#define TCP_CMD_DEL_COLOR_CS 82
#define TCP_CMD_DEFINE_CLIENT_COLORS 91
#define TCP_CMD_START_GAME_CS 95
#define TCP_CMD_MOVE_SC 100
#define TCP_CMD_DIE_DONE_CS 105
#define TCP_CMD_SHOW_DIE_SC 106
#define TCP_CMD_DICE_SC 110
#define TCP_CMD_SELECT_FIGURE_CS 115
#define TCP_CMD_MOVE_NOT_ALLOWED_SC 116
#define TCP_CMD_CANCEL_GAME_CS 117

// Display Commands
#define DISPLAY_CMD_PUSH_ENTER 10
#define DISPLAY_CMD_CANCEL_GAME 11

// I²C
#define I2C_DEV_NAME "/dev/i2c-0"
#define I2C_POLLING_TIME_US 100000

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

// hardware devices
#define DEV_XY_DRIVE 0
#define DEV_RFID 1
#define DEV_LED_RING 2
#define DEV_LED_STRIPES 3

// XY drive commands
#define XY_DRIVE_CMD_MOVE 0
#define XY_DRIVE_CMD_MAG 1
#define XY_DRIVE_CMD_REF 2

#define XY_DRIVE_MAX_SPEED 150
#define XY_DRIVE_MAX_ACC 2
#define XY_DRIVE_MAX_DEC 2
#define XY_DRIVE_MIN_SERVO 45
#define XY_DRIVE_MAX_SERVO 105

// rfid commands
#define RFID_CMD_READ 0


// magnet
#define MAG_OFF 0
#define MAG_ON 1

// LED ring commands
#define LED_RING_CMD_SET 0

//LED mode
#define LED_MODE_OFF 0
#define LED_MODE_ON 1
#define LED_MODE_HB_FAST 2
#define LED_MODE_HB_SLOW 3



struct tcp_data {
	int newsockfd;
	//std::string name;

	tcp_data() {
		newsockfd = 0;
	}
	tcp_data(int newsock) {
		newsockfd = newsock;
	}
};

#endif
