#ifndef LEDSTRIPES_H_
#define LEDSTRIPES_H_

#include "I2c.h"
#include "../../../../../../arduino/led_stripe/i2c_defs.h"

class LedStripes: protected I2c {
public:
	enum Mode{
		OFF = 0, ON = 1, BLINK_SLOW = 2, BLINK_FAST = 3
	};

	enum Color {
		BLUE = 1, PINK = 2, GREEN = 3, LIGHT_BLUE = 4,
		YELLOW = 5, RED = 6, ORANGE = 7, WHITE = 8
	};

	enum Corner {
		STEFAN = 8, ELLLA = 4, MARCEL = 2, MARTINA = 1
	};

	LedStripes(uint8_t i2cAddr);
	virtual ~LedStripes();
	int set(uint8_t mode8, uint8_t color8,
	        uint8_t mode4, uint8_t color4,
	        uint8_t mode2, uint8_t color2,
	        uint8_t mode1, uint8_t color1);
	int set(uint8_t mode, uint8_t color, uint8_t corner);
	int set(uint8_t mode, uint8_t color);
	int setAllOff();

private:
	uint8_t _data[8];

	int set();
};

#endif /* LEDSTRIPES_H_ */
