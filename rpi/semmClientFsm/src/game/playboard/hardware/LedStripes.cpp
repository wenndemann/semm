#include "LedStripes.h"

LedStripes::LedStripes(uint8_t i2cAddr) {
	std::cout << "    creating led stripe" << std::endl;
	setI2cAddr(i2cAddr);
}

LedStripes::~LedStripes() {
	set(0, 0);
}

int LedStripes::set(uint8_t mode, uint8_t color) {
	set(mode, color, color);
}

int LedStripes::set(uint8_t mode, uint8_t color, uint8_t corner) {
	//cout << "set led stripe to mode " << static_cast<int32_t>(mode)
	//	 << "with color " << static_cast<int32_t>(color)
	//	 << "in corner " << static_cast<int32_t>(corner) << endl;

	return 0; // remove if led will work

	uint8_t buf[2];
	buf[0] = color;
	buf[1] = corner;

	I2c::write(mode, buf, 2);
	usleep(10000);
	return 0;
}

