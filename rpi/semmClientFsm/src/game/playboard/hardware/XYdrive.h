#ifndef XYDRIVE_H_
#define XYDRIVE_H_

// global includes
#include <boost/thread.hpp>

// locale includes
#include "I2c.h"
#include "../../../../../../arduino/xy_schlitten/src/i2c_defs.h"

class XYdrive : protected I2c{
public:
	XYdrive(uint8_t i2cAddrXYdrive);
	virtual ~XYdrive();

	void moveMagnet(bool value);
	void moveCarriage(uint8_t x, uint8_t y);

private:
	bool _magPos;
};

#endif /* XYDRIVE_H_ */
