#ifndef I2C_DEFS_DBEN_H_
#define I2C_DEFS_DBEN_H_
/*****************************************************************************/
/*                                                                           */
/* Jedes #define entÃ¤lt das PrÃ¤fix "I2C" gefolgt von seinem GerÃ¤teprÃ¤fix,*/
/* gefolgt vom Befehlsnamen und evtl einer Bitnummer. Die einzelnen Teile    */
/* sind durch ein "_" zu trennen.                                            */
/* bsp.: I2C_XYS_POS_X                                                       */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Display-Bedieneinheiten                                                   */
/*                                                                           */
/*****************************************************************************/
// TVVDDDDD
// T     = TYPE       ( 0  - command;    1 - picture) 
// VV    = variant    (00 - Variante A; 01 - Variante B) 
// DDDDD = definition 

// GerÃ¤teprÃ¤fix = "DBEN"
#define I2C_DBEN_ADDR_0               0x50
#define I2C_DBEN_ADDR_1               0x51
#define I2C_DBEN_ADDR_2               0x52
#define I2C_DBEN_ADDR_3               0x53

/*      PICTURES                                                             */
#define I2C_DBEN_PIC_SEMM             0x81
#define I2C_DBEN_PIC_PREPARE          0x82  //neu
#define I2C_DBEN_PIC_ENTER            0x83
#define I2C_DBEN_PIC_START            0x84
#define I2C_DBEN_PIC_WAIT             0x85
#define I2C_DBEN_PIC_DICE             0x86
#define I2C_DBEN_PIC_DICE_1           0x87  //Variante B - C7 (7. Bit gesetzt)
#define I2C_DBEN_PIC_DICE_2           0x88  //Variante B - C8 (7. Bit gesetzt)
#define I2C_DBEN_PIC_DICE_3           0x89  //Variante B - C9 (7. Bit gesetzt)
#define I2C_DBEN_PIC_DICE_4           0x8A  //Variante B - CA (7. Bit gesetzt)
#define I2C_DBEN_PIC_DICE_5           0x8B  //Variante B - CB (7. Bit gesetzt)
#define I2C_DBEN_PIC_DICE_6           0x8C  //Variante B - CC (7. Bit gesetzt)
#define I2C_DBEN_PIC_MOVE_ILLEGAL     0x8D  //wird in server eingebaut 23.01.2014 
#define I2C_DBEN_PIC_NOT_FOUND        0x8E  //neu
#define I2C_DBEN_PIC_WIN              0x8F
#define I2C_DBEN_PIC_CANCEL           0x90
#define I2C_DBEN_PIC_DISABLED         0x91

/*      COMMANDS                                                             */
/*      command name                  value   note                           */
#define I2C_DBEN_VERSION              0x01  //Ausgabe der Versionsnummer
#define I2C_DBEN_SHOW_CONFIG          0x02  //Ausgabe der Konfigurationsdaten auf dem Display
#define I2C_DBEN_PICTURE              0x03  //Anzeige Menübild
#define I2C_DBEN_BUTTON               0x04  //Ausgabe der Menüauswahl
/*      STRUCTS                                                              */
#endif

