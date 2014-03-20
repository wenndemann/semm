#include <SPI.h>
#include "tft18.h"
#include "glcdfont.c"
#include <avr/pgmspace.h>

//TFT18 TFT;

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80
PROGMEM static prog_uchar
  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,  
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color
  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F },           //     XEND = 159

  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay      

void TFT18::commonInit(uint8_t *cmdList) {

  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  csport    = portOutputRegister(digitalPinToPort(TFT_CS));
  cspinmask = digitalPinToBitMask(TFT_CS);
  rsport    = portOutputRegister(digitalPinToPort(TFT_DC));
  rspinmask = digitalPinToBitMask(TFT_DC);


    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2); // 4 MHz (half speed)
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
  

  // toggle RST low to reset; CS low so it'll listen to us
  *csport &= ~cspinmask;
  if (TFT_RST) {
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH);
    delay(100);
    digitalWrite(TFT_RST, LOW);
    delay(100);
    digitalWrite(TFT_RST, HIGH);
    delay(100);
  }

  if(cmdList) commandList(cmdList);
}

void TFT18::initTFT() { 
  cursor_y = cursor_x = 0;
  textsize = 1;
  textcolor = textbgcolor = 0xFFFF;
  
  commonInit(Rcmd1);
  commandList(Rcmd2red);
  commandList(Rcmd3);
}

void TFT18::fillScreen(uint16_t color) {
  fillRect(0, 0,  ST7735_TFTWIDTH, ST7735_TFTHEIGHT, color);
}



// fill a rectangle
void TFT18::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT)) return;
  if((x + w - 1) >= ST7735_TFTWIDTH)  w = ST7735_TFTWIDTH  - x;
  if((y + h - 1) >= ST7735_TFTHEIGHT) h = ST7735_TFTHEIGHT - y;

  color = swapcolor(color);

  setAddrWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;
  *rsport |=  rspinmask;
  *csport &= ~cspinmask;
  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      spiwrite(hi);
      spiwrite(lo);
    }
  }

  *csport |= cspinmask;
}




void TFT18::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}


void TFT18::setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}


void TFT18::setTextColor(uint16_t c) {
  textcolor = c;
  textbgcolor = c; 
  // for 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
}

// return the size of the display which depends on the rotation!
int16_t TFT18::width(void) { 
  return ST7735_TFTWIDTH; 
}
 
int16_t TFT18::height(void) { 
  return ST7735_TFTHEIGHT; 
}


void TFT18::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

  writecommand(ST7735_CASET); // Column addr set
  writedata(0x00);
  writedata(x0);     // XSTART 
  writedata(0x00);
  writedata(x1);     // XEND

  writecommand(ST7735_RASET); // Row addr set
  writedata(0x00);
  writedata(y0);     // YSTART
  writedata(0x00);
  writedata(y1);     // YEND

  writecommand(ST7735_RAMWR); // write to RAM
}

void TFT18::pushColor(uint16_t color) {
  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  color = swapcolor(color);
  spiwrite(color >> 8);
  spiwrite(color);

  *csport |= cspinmask;
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t TFT18::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void TFT18::commandList(uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    writecommand(pgm_read_byte(addr++)); //   Read, issue command
    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      writedata(pgm_read_byte(addr++));  //     Read, issue argument
    }

    if(ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
}



void TFT18::writecommand(uint8_t c) {
  *rsport &= ~rspinmask;
  *csport &= ~cspinmask;

  //Serial.print("C ");
  spiwrite(c);

  *csport |= cspinmask;
}


void TFT18::writedata(uint8_t c) {
  *rsport |=  rspinmask;
  *csport &= ~cspinmask;
    
  //Serial.print("D ");
  spiwrite(c);

  *csport |= cspinmask;
} 


size_t TFT18::write(uint8_t c) {
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*6;
    if ((cursor_x > (ST7735_TFTWIDTH - textsize*6))) {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
  return 1;
}

// draw a character
void TFT18::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {

  if((x >= ST7735_TFTWIDTH)            || // Clip right
     (y >= ST7735_TFTHEIGHT)           || // Clip bottom
     ((x + 5 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = pgm_read_byte(font+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        } 	
      }
      line >>= 1;
    }
  }
}

void TFT18::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= ST7735_TFTWIDTH) || (y < 0) || (y >= ST7735_TFTHEIGHT)) return;

  setAddrWindow(x,y,x+1,y+1);

  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  color = swapcolor(color);

  spiwrite(color >> 8);
  spiwrite(color);

  *csport |= cspinmask;
}
