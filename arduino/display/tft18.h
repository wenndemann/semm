#ifndef _TFT18_H_
#define _TFT18_H_

//#include "Arduino.h"
#include "Print.h"
 
#define TFT_CS   8  // Chip select line for TFT display
#define TFT_DC  10  // Data/command line for TFT
#define TFT_RST  9  // Reset line for TFT (or connect to +5V) 
 
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT  0x11
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_INVOFF  0x20
#define ST7735_MADCTL  0x36
#define ST7735_COLMOD  0x3A
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
#define ST7735_NORON   0x13
#define ST7735_DISPON  0x29


#define SEMM_GREY      0xCE59
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0  
#define ST7735_WHITE   0xFFFF


class TFT18 : public Print 
{
  public:
    void initTFT();
    void fillScreen(uint16_t color);
    void setCursor(int16_t x, int16_t y);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    int16_t width(void);
    int16_t height(void);
    void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
    void pushColor(uint16_t color);
 
    virtual size_t write(uint8_t);
  private:
    void commonInit(uint8_t *cmdList);
    void commandList(uint8_t *addr);
    inline uint16_t swapcolor(uint16_t x);
    inline void spiwrite(uint8_t c);
    void writecommand(uint8_t c);
    void writedata(uint8_t c);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    
    uint8_t cspinmask, rspinmask, datapinmask, clkpinmask;
    int16_t  cursor_x, cursor_y;
    uint16_t textcolor, textbgcolor;
    uint8_t  textsize;
    volatile uint8_t *csport, *rsport, *dataport, *clkport;
};

//extern TFT18 TFT;

uint16_t TFT18::swapcolor(uint16_t x) { 
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}

void TFT18::spiwrite(uint8_t c) {
  SPDR = c;
  while(!(SPSR & _BV(SPIF)));
}

#endif //_TFT18_H_


