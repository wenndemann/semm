#include "LedStripes.h"

LedStripes::LedStripes(uint8_t i2cAddr) {
	setI2cAddr(i2cAddr);
	set(0, 0);
}

LedStripes::~LedStripes() {
	set(0, 0);
}

int LedStripes::set(uint8_t mode, uint8_t color) {
	I2c::write(mode, &color, sizeof color);
	return 0;
}

int LedStripes::setDemo(uint8_t demo) {
	I2c::write(4, &demo, sizeof demo);
	return 0;
}

