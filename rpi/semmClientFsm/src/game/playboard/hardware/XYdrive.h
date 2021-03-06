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

	void liftMagnet(bool value);
	void moveMagnet(uint8_t x, uint8_t y);
	void moveCarriage(uint8_t x);

	uint8_t x() const { return _x; }
	uint8_t y() const { return _y; }

private:
	void move( uint8_t x, uint8_t y, int32_t offset );
	void resetDev();

	bool _magPos;
	uint8_t _x, _y;
};

#endif /* XYDRIVE_H_ */
