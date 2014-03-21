#ifndef LEDSTRIPES_H_
#define LEDSTRIPES_H_

#include "I2c.h"
#include "../../../../../../arduino/led_stripe/i2c_defs.h"

class LedStripes: protected I2c {
public:
	LedStripes(uint8_t i2cAddr);
	virtual ~LedStripes();
	int set(uint8_t mode, uint8_t color);
	int setDemo(uint8_t demo);

};

#endif /* LEDSTRIPES_H_ */
