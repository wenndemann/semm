#ifndef RFID_H_
#define RFID_H_

// globale includes
#include <vector>

// locale includes
#include "I2c.h"
#include "../../../../../../arduino/rfid_reader/i2c_defs.h"

#define RFID_TIME_US_READ 1200000



class Rfid: protected I2c {
private:
	class tagLine_t {
	public:
		tagLine_t() { for(int i = 0; i < 11; i++) tag[i] = 0; }
		uint16_t tag[11];
	};

	class tags_t {
	public:
		tagLine_t line[11];
	};

public:
	Rfid(uint8_t i2cAddr);
	virtual ~Rfid();

	uint16_t readTag(uint8_t y);
	int readLine(std::vector<uint16_t>* buf);
};

#endif /* RFID_H_ */
