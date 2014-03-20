#ifndef I2C_DEFS_XYS_H_
#define I2C_DEFS_XYS_H_

/*****************************************************************************/
/*                                                                           */
/* Jedes #define entält das Präfix "I2C" gefolgt von seinem Gerätepräfix,    */
/* gefolgt vom Befehlsnamen und evtl einer Bitnummer. Die einzelnen Teile    */
/* sind durch ein "_" zu trennen.                                            */
/* bsp.: I2C_XYS_POS_X                                                       */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/* XY-Schlitten                                                              */
/*                                                                           */
/*****************************************************************************/
/* Gerätepräfix = "XYS"                                                      */
#define I2C_XYS_ADDR        	0x10

/*      COMMANDS                                                             */
/*      command name          value   note                                   */
#define I2C_XYS_SETPOINTS       'a'  // RW control byte
#define I2C_XYS_MAX_SPEED       'b'  // RW max speed of pos. mode
#define I2C_XYS_ACCELERATION    'c'  // RW acceleration of pos. mode
#define I2C_XYS_DECELERATION    'd'  // RW deceleration of pos. mode
#define I2C_XYS_SERVO_MIN       'e'  // RW servo min value
#define I2C_XYS_SERVO_MAX       'f'  // RW servo max value
#define I2C_XYS_SERVO           'g'  // RW servo on/off
#define I2C_XYS_RUNNING         'h'  // R  motorcontroller is running
#define I2C_XYS_RESET           'i'  // W  reset arduino

#endif
