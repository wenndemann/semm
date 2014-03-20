#include "LedRing.h"

LedRing::LedRing(uint8_t i2cAddr) {
	setI2cAddr(i2cAddr);
	set(0, 0);
}

LedRing::~LedRing() {
	set(0, 0);
}

int LedRing::set(uint8_t mode, uint8_t color) {
	I2c::write(mode, &color, sizeof color);
	return 0;
}
