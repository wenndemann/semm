#include "LedStripes.h"

LedStripes::LedStripes(uint8_t i2cAddr) {
	std::cout << "    creating led stripe" << std::endl;
	setI2cAddr(i2cAddr);
}

LedStripes::~LedStripes() {
	set(0, 0);
}

int LedStripes::set(uint8_t mode8, uint8_t color8,
        uint8_t mode4, uint8_t color4,
        uint8_t mode2, uint8_t color2,
        uint8_t mode1, uint8_t color1)
{
	_data[0] = mode8;
	_data[1] = color8;
	_data[2] = mode4;
	_data[3] = color4;
	_data[4] = mode2;
	_data[5] = color2;
	_data[6] = mode1;
	_data[7] = color1;

	set();
}

int LedStripes::set(uint8_t mode, uint8_t color, uint8_t corner) {
	switch(corner) {
	case Corner::STEFAN:
		_data[0] = mode;
		_data[1] = color;
		break;
	case Corner::ELLLA:
		_data[2] = mode;
		_data[3] = color;
		break;
	case Corner::MARCEL:
		_data[4] = mode;
		_data[5] = color;
		break;
	case Corner::MARTINA:
		_data[6] = mode;
		_data[7] = color;
		break;
	default:
		break;
	}

	return set();
}

int LedStripes::set(uint8_t mode, uint8_t color) {
	set(mode, color, color);
}

int LedStripes::setAllOff() {
	return set(0,0,0,0,0,0,0,0);
}

int LedStripes::set() {
	return I2c::write(_data[0], &_data[1], 7);
}
