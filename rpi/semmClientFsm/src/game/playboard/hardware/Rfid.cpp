#include "Rfid.h"

Rfid::Rfid(uint8_t i2cAddr) {
	setI2cAddr(i2cAddr);
}

Rfid::~Rfid() {
	// TODO check if neessary
}

int Rfid::readLine(tagLine_t* buf) {
	write(I2C_RFID_IREAD, NULL, 0);
	usleep(RFID_TIME_US_READ);
	return read(I2C_RFID_TAGS, buf, sizeof(tagLine_t));
}
