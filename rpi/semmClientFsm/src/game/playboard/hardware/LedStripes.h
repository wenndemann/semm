#ifndef LEDSTRIPES_H_
#define LEDSTRIPES_H_

#include "I2c.h"
#include "../../../../../../arduino/led_stripe/i2c_defs.h"

class LedStripes: protected I2c {
public:
	enum {
		OFF = 0, ON = 1, BLINK_SLOW = 2, BLINK_FAST = 3
	};

	LedStripes(uint8_t i2cAddr);
	virtual ~LedStripes();
	int set(uint8_t mode, uint8_t color);
	int setDemo(uint8_t demo);

};

#endif /* LEDSTRIPES_H_ */
