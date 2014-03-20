
#include <SD.h>
#include "i2c_defs.h"
#include "displayDraw.h"
#include "tft18.h"

byte _selectedButton;
boolean _buttonPushed;
byte _i2c_adresse;
byte _currentPic = 0;

TFT18 TFT;

void initDisplay()
{
  TFT.initTFT();   // initialize a ST7735S chip, black tab

  TFT.fillScreen(ST7735_BLACK);
  TFT.setTextColor(ST7735_WHITE);
  TFT.setTextSize(1);
  TFT.setCursor(5, 20);
  TFT.print("Lade...");  

  if (!SD.begin(SD_CS))
  {
    TFT.setCursor(5, 35);
    TFT.print("FEHLER!!!");
    TFT.setCursor(5, 45);
    TFT.print("SD-Karte kann nicht");
    TFT.setCursor(5, 55);
    TFT.print("gelesen werden");
    while (1);
  }

  //Lese I2C-Adresse von SD-Karte. Angabe erfolgt in der Datei als Hex (00 bis FF)
  char buffer[3];
  File i2c_address_file = SD.open("i2c.txt");
  i2c_address_file.read(buffer, 2);
  i2c_address_file.close();
  buffer[2] = 0;
  _i2c_adresse = strtol(buffer, NULL, 16);
}


void drawPicture(byte picNo)
{
  byte variant = picNo & 0x60; //variante auslesen
  picNo &= 0x9F; //Variantenbits ausmaskieren
  
  if (_currentPic == I2C_DBEN_PIC_CANCEL && picNo != I2C_DBEN_PIC_SEMM) return;
  else if (_currentPic == I2C_DBEN_PIC_SEMM && picNo == I2C_DBEN_PIC_CANCEL) return;
  else if (picNo != _currentPic)
  {
    _selectedButton = 0;
    _buttonPushed = false; 
    _currentPic = picNo;

    TFT.fillScreen(SEMM_GREY);
    switch (picNo) //Über I2C
    {
      case I2C_DBEN_PIC_SEMM:
        bmpDraw("logo_1.bmp", 6, 22, 116, 116); // Logo
        break;
      case I2C_DBEN_PIC_PREPARE:
        bmpDraw("startf_b.bmp", 0, 6, 128, 46);
        bmpDraw("startf_1.bmp", 14, 52, 100, 76); 
        drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_ENTER:
        drawUnpushedButton();
        bmpDraw("enc_1.bmp", 26, 48, 76, 104);
        break;
      case I2C_DBEN_PIC_START:
        drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_WAIT:
        bmpDraw("wart_b.bmp", 0, 6, 128, 46);
        bmpDraw("sand_1.bmp", 34, 62, 60, 84);
        break;
      case I2C_DBEN_PIC_DICE:
        bmpDraw("bit_w_b.bmp", 0, 6, 128, 48);
        bmpDraw("bit_w1.bmp", 28, 56, 72, 72);
        drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_DICE_1:
        bmpDraw("w1.bmp", 10, 12, 108, 108);
        if (variant) drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_DICE_2:
        bmpDraw("w2.bmp", 10, 12, 108, 108);
        if (variant) drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_DICE_3:
        bmpDraw("w3.bmp", 10, 12, 108, 108);
        if (variant) drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_DICE_4:
        bmpDraw("w4.bmp", 10, 12, 108, 108);
        if (variant) drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_DICE_5:
        bmpDraw("w5.bmp", 10, 12, 108, 108);
        if (variant) drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_DICE_6:
        bmpDraw("w6.bmp", 10, 12, 108, 108);
        if (variant) drawUnpushedButton();
        break;        
      case I2C_DBEN_PIC_MOVE_ILLEGAL:
        bmpDraw("f_zug_b.bmp", 0, 6, 128, 46);
        bmpDraw("f_zug_1.bmp", 20, 58, 88, 88);
        break;
      case I2C_DBEN_PIC_NOT_FOUND:
        bmpDraw("korr_b.bmp", 0, 6, 128, 46);
        bmpDraw("korr_1.bmp", 29, 52, 70, 70); 
        drawUnpushedButton();
        break;
      case I2C_DBEN_PIC_WIN:
        bmpDraw("win.bmp", 0, 0, 128, 160);
        break;
      case I2C_DBEN_PIC_CANCEL:
        bmpDraw("abbr_b.bmp", 0, 6, 128, 46);
        bmpDraw("abbr_1.bmp", 36, 64, 56, 56);
        break;
      case I2C_DBEN_PIC_DISABLED:
        bmpDraw("dis_x_b.bmp", 0, 6, 128, 46);
        bmpDraw("dis_x_1.bmp", 18, 58, 92, 92);
        break;
    }   
  }
}

void drawUnpushedButton()
{
  _selectedButton = 1;
  
  switch (_currentPic) //nach draw nach ic2
  {
    case I2C_DBEN_PIC_ENTER: //Menüpunkt mit teilnehmen
      bmpDraw("teiln_g.bmp", 4, 6, 120, 30);
      break;     
    case I2C_DBEN_PIC_DICE: //ok button unten
      bmpDraw("OK_g.bmp", 44, 130, 40, 24);
      break;
    case I2C_DBEN_PIC_PREPARE: //gesetzt button
    case I2C_DBEN_PIC_NOT_FOUND:
    case I2C_DBEN_PIC_DICE_1:
    case I2C_DBEN_PIC_DICE_2:
    case I2C_DBEN_PIC_DICE_3:
    case I2C_DBEN_PIC_DICE_4:
    case I2C_DBEN_PIC_DICE_5:
    case I2C_DBEN_PIC_DICE_6:
      bmpDraw("gese_g.bmp", 19, 130, 90, 24);
      break;
    case I2C_DBEN_PIC_START:
      bmpDraw("beg_g.bmp", 4, 10, 120, 66);
      bmpDraw("nteil_w.bmp", 4, 86, 120, 66);
      break;
  }
}

void drawPushedButton()
{
  if (_buttonPushed == false && _selectedButton)
  {
    _buttonPushed = true; 

    switch (_currentPic) //nach draw nach ic2
    {
      case I2C_DBEN_PIC_ENTER: //Menüpunkt mit teilnehmen
        bmpDraw("teiln_s.bmp", 4, 6, 120, 30);
        break;
      case I2C_DBEN_PIC_PREPARE:
      case I2C_DBEN_PIC_NOT_FOUND:
      case I2C_DBEN_PIC_DICE_1:
      case I2C_DBEN_PIC_DICE_2:
      case I2C_DBEN_PIC_DICE_3:
      case I2C_DBEN_PIC_DICE_4:
      case I2C_DBEN_PIC_DICE_5:
      case I2C_DBEN_PIC_DICE_6:
        bmpDraw("gese_s.bmp", 19, 130, 90, 24);
        break;
      case I2C_DBEN_PIC_DICE: //ok button unten
        bmpDraw("OK_s.bmp", 44, 130, 40, 24);
        break;
      case I2C_DBEN_PIC_START:
        if (_selectedButton == 1)
        {
          bmpDraw("beg_s.bmp", 4, 10, 120, 66);
        }
        else if (_selectedButton == 2)
        {
          bmpDraw("nteil_s.bmp", 4, 86, 120, 66);
        }
        break;
    }
  }     
}

void drawButtonSelection()
{
  //Damit eine Selektion nur dann stattfindet, wenn noch kein Button gedrückt wurde
  if (_buttonPushed == false && _currentPic == I2C_DBEN_PIC_START)
  {
    if (_selectedButton == 1)
    {
      bmpDraw("beg_w.bmp", 4, 10, 120, 66);
      bmpDraw("nteil_g.bmp", 4, 86, 120, 66);
      _selectedButton = 2;
    }
    else if (_selectedButton == 2)
    {
      bmpDraw("nteil_w.bmp", 4, 86, 120, 66);
      bmpDraw("beg_g.bmp", 4, 10, 120, 66);
      _selectedButton = 1;
    }
  }
}

void drawInfo(char* versionsNummer)
{
  TFT.fillScreen(ST7735_BLACK);
  TFT.setTextColor(ST7735_WHITE);
  TFT.setTextSize(1);
  TFT.setCursor(5, 5);
  TFT.print(versionsNummer);
  TFT.setCursor(5, 20);
  TFT.print("I2C-Adresse: 0x"); 
  TFT.print(_i2c_adresse, 16);
}

byte getPushedButton()
{
  if (_currentPic == I2C_DBEN_PIC_CANCEL) return 0xFF;
  if (_buttonPushed) return _selectedButton;
  return 0;
}

void bmpDraw(char *filename, byte x, byte y, byte bmpWidth, byte bmpHeight) 
{
  File     bmpFile;
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[48]; // pixel buffer (R+G+B per pixel) also immer faktor 3
  uint8_t buffidx = 0;
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;
  uint8_t paddingKorrektur = 0;

  // Open requested file on SD card
  bmpFile = SD.open(filename);


  // BMP rows are padded (if needed) to 4-byte boundary
  rowSize = (bmpWidth * 3 + 3) & ~3;
  paddingKorrektur = rowSize - bmpWidth * 3;
  //Serial.println(paddingKorrektur);

  // Crop area to be loaded
  w = bmpWidth;
  h = bmpHeight;
  if ((x + w - 1) >= TFT.width())  w = TFT.width()  - x;
  if ((y + h - 1) >= TFT.height()) h = TFT.height() - y;

  // Set TFT address window to clipped image bounds
  TFT.setAddrWindow(x, y, x + w - 1, y + h - 1);

  for (row=0; row < h; ++row) { // For each scanline...
    pos = 54 + row * rowSize;

    if(bmpFile.position() != pos) { // Need seek?
      bmpFile.seek(pos);
    }
    buffidx = sizeof(sdbuffer);

    for (col=0; col<w; ++col) { // For each pixel...
      // Time to read more pixel data?
      if (buffidx == sizeof(sdbuffer)) { // Indeed

        if ((w - col) * 3 + paddingKorrektur >=  sizeof(sdbuffer))
        {
          bmpFile.read(sdbuffer, sizeof(sdbuffer));
        }
        else
        {
          bmpFile.read(sdbuffer, (w - col) * 3 + paddingKorrektur);
        }
        buffidx = 0; // Set index to beginning
      }
      //Serial.println("");
      // Convert pixel from BMP to TFT format, push to display
      b = sdbuffer[buffidx++];
      g = sdbuffer[buffidx++];
      r = sdbuffer[buffidx++];
      TFT.pushColor(TFT.Color565(r,g,b));
    } // end pixel 
  } // end scanline

  bmpFile.close();
}


