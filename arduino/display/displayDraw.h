#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "Arduino.h"

#define SD_CS    7  // Chip select line for SD card


  void drawPicture(byte picNo);
  void drawPushedButton();
  void drawUnpushedButton();
  void drawButtonSelection();
  void drawInfo(char* versionsNummer);
  byte getPushedButton();
  void initDisplay();

  void bmpDraw(char *filename, byte x, byte y, byte bmpWidth, byte bmpHeight);

  extern byte _i2c_adresse;

#endif //_DISPLAY_H_

