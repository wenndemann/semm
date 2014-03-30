#include "Rfid.h"

Rfid::Rfid(uint8_t i2cAddr) {
	setI2cAddr(i2cAddr);
}

Rfid::~Rfid() {
	// TODO check if neessary
}

uint16_t Rfid::readTag(uint8_t y) {
//	tagLine_t line;
	std::vector<uint16_t> line(11);
	readLine(&line);
	return line[y];
}

int Rfid::readLine(std::vector<uint16_t>* tag) {
	write(I2C_RFID_IREAD, NULL, 0);
	usleep(RFID_TIME_US_READ);
	uint16_t buf[11];
	int n =read(I2C_RFID_TAGS, buf, sizeof(buf));
	for(int i = 0 ; i < 11 ; i++)
		tag->at(i) = buf[i];
	return n;
}
