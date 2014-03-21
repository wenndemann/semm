#ifndef I2C_H_
#define I2C_H_

// global includes
#include <boost/thread.hpp>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// local includes
#include "../../../defs.h"
//#define NO_I2C

class I2c {
public:
	I2c();
	virtual ~I2c();

private:
	void setCmd(uint8_t I2cCmd, int i2cAddr, int fd);
	static int fd;
	static int count;
	static boost::mutex i2cMutex;
	int open(const char* devName);
	int close();

protected:
	uint8_t m_i2cAddr;

	void setI2cAddr(uint8_t i2cAddr) {
		m_i2cAddr = i2cAddr;
	}

	uint8_t getI2cAddr() { return m_i2cAddr; }

	int read(void* data, uint8_t length);
	int read(uint8_t i2cCmd, void* data, uint8_t length);
	int write(void* data, uint8_t length);
	int write(uint8_t i2cCmd, void* data, uint8_t length);
};

#endif
