#ifndef LEDRING_H_
#define LEDRING_H_

// locale includes
#include "I2c.h"
#include "../../../../../../arduino/led_ring/i2c_defs.h"

class LedRing: protected I2c {
public:
	LedRing(uint8_t i2cAddr);
	virtual ~LedRing();
	int set(uint8_t mode, uint8_t color);
};

#endif /* LEDRING_H_ */
