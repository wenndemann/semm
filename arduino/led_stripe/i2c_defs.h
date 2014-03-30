// i2c_defs.h zur led_streifen.ino

#ifndef I2C_DEFS_LS_H_
#define I2C_DEFS_LS_H_
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
/* LED Streifen und Bedieneinheit                                            */
/*                                                                           */
/*****************************************************************************/
// Gerätepräfix = "LSB"
#define I2C_LS_ADDR        	0x40

#define I2C_LS_ALL_OFF		1
#define I2C_LS_ALL_ON		2
#define I2C_LS_SN_ON		3
#define I2C_LS_EA_ON		4
#define I2C_LS_ML_ON		5
#define I2C_LS_MA_ON		6
#define I2C_LS_ALL_HB		7
#define I2C_LS_SN_HB		8
#define I2C_LS_EA_HB		9
#define I2C_LS_ML_HB		10
#define I2C_LS_MA_HB		11
#define I2C_LS_SN_HB_EA_A	12
#define I2C_LS_EA_HB_SN_A	13
#define I2C_LS_SN_HB_ML_A	14
#define I2C_LS_ML_HB_SN_A	15
#define I2C_LS_SN_HB_MA_A	16
#define I2C_LS_MA_HB_SN_A	17
#define I2C_LS_EA_HB_ML_A	18
#define I2C_LS_ML_HB_EA_A	19
#define I2C_LS_EA_HB_MA_A	20
#define I2C_LS_MA_HB_EA_A	21
#define I2C_LS_ML_HB_MA_A	22
#define I2C_LS_MA_HB_ML_A	23
#define I2C_LS_SEMM_ANI		24
#define I2C_LS_RED_ANI		25
#define I2C_LS_GREEN_ANI	26

/*      COMMANDS                                                             */
/*      command name          value   note                                   */

/*      STRUCTS                                                              */
#endif
