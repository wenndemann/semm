#ifndef XYDRIVE_H_
#define XYDRIVE_H_

#include "I2c.h"
#include "../hardware/Rfid.h"
#include "../hardware/LedRing.h"
#include "../hardware/LedStripes.h"
#include "../defs.h"
#include "../../../../arduino/xy_schlitten/src/i2c_defs.h"
#include <boost/thread.hpp>
#include <cstring>

struct cmdBuffer_s{
	uint8_t cmd[CMDBUF_SIZE][CMDBUF_MAX_CMD_LENGHT];
	uint8_t in, out;
	boost::mutex mutex;
	boost::condition_variable_any condAdd, condRem;
};

class Rfid;
class tags_t;
class tagLine_t;

class XYdrive : protected I2c{
public:
	XYdrive(uint8_t i2cAddrXYdrive);
	virtual ~XYdrive();
/*
	void magnet(bool value);
	void ref();
	void moveToXY(uint8_t x, uint8_t y);
	void scan(tags_t* tags);
	void scanLine(uint8_t line, tagLine_t* tags);
	void readRfid(void* scanDest, int length);
	void setLedRing(uint8_t mode, uint8_t color);
	bool getIsMoving();
	uint8_t getActX() { return m_actX; }
	uint8_t getActY() { return m_actY; }

private:
	cmdBuffer_s m_cB;
	volatile bool m_isMoving;
	uint8_t m_actX, m_actY;

	void m_worker();
	void addCmd(uint8_t cmd[], int length);
	void addCmd(uint8_t cmd, ...);
	void m_moveTo(int x, int y);

	bool m_mag;
*/

};

#endif /* XYDRIVE_H_ */
