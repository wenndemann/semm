#ifndef LEDRING_H_
#define LEDRING_H_

// locale includes
#include "I2c.h"
#include "../../../../../../arduino/led_ring/i2c_defs.h"

class LedRing: protected I2c {
public:
	enum Mode{
		OFF = 0, ON = 1, BLINK = 2
	};

	enum Color {
		BLUE = 1, PINK = 2, GREEN = 3, LIGHT_BLUE = 4,
		YELLOW = 5, RED = 6, ORANGE = 7, WHITE = 8
	};

	LedRing(uint8_t i2cAddr);
	virtual ~LedRing();
	int set(uint8_t mode, uint8_t color);
};

#endif /* LEDRING_H_ */
