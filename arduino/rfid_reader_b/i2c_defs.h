#ifndef I2C_DEFS_RFID_H_
#define I2C_DEFS_RFID_H_
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
/* RFID Reader                                                               */
/*                                                                           */
/*****************************************************************************/
// DEVICE-ID = "RFID"
#define I2C_RFID_ADDR         0x20

/*      COMMANDS                                                             */
/*      command name          value   note                                   */
#define I2C_RFID_VERSION      0x10  //Ausgabe der Versionsnummer
#define I2C_RFID_TAGS         0x0D  //Ausgabe TAGS (11 x 2 Bytes)
#define I2C_RFID_IREAD        0xB0  //Initiiere Lesen

/*      STRUCTS                                                              */
#endif
