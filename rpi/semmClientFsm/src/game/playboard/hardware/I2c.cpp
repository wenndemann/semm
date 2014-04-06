#include "I2c.h"

using namespace std;

int I2c::fd;
int I2c::count;
boost::mutex I2c::i2cMutex;

I2c::I2c() {
	m_i2cAddr = 0;

	count++;
}

I2c::~I2c() {
	count--;
	if (!count)
		close();
}

int I2c::open(const char* devName) {
//	return 0;

	i2cMutex.lock();
	if((fd = ::open(devName, O_RDWR)) < 0) {
		cout << "I2C open error! " << static_cast<int32_t>(fd) << endl;
	}
	i2cMutex.unlock();
	return 0;
}

int I2c::write(void* data, uint8_t length) {
	//return 0;
	int nW = 0;

	if(!fd) open(I2C_DEV_NAME);
	i2cMutex.lock();
	if(ioctl(fd, I2C_SLAVE, m_i2cAddr) < 0)
	cout << "I2C set adress error!" << endl;

	int tries = 0;

	do {
		nW = ::write(fd, data, length);
		if(nW != length) {
			cout << "write error to addr: " << static_cast<int32_t>(m_i2cAddr) <<
					" (" << (char*)data << ")" << endl;
			tries++;
			usleep(10000);
		}
	}while(nW != length && tries < 5);
	i2cMutex.unlock();
	return nW;
}

int I2c::write(uint8_t i2cCmd, void* data, uint8_t length) {
//	return 0;
	uint8_t buf[length + 1];

	buf[0] = i2cCmd;
	memcpy(&buf[1], (uint8_t*) data, length);
	return write(buf, length + 1);
}

int I2c::read(void* data, uint8_t length) {
	int nR = 0;
	//return 0;

	if(!fd) open(I2C_DEV_NAME);
	i2cMutex.lock();
	if(ioctl(fd, I2C_SLAVE, m_i2cAddr) < 0)
	cout << "I2C set adress error!" << endl;
	int tries = 0;
	do {
		nR = ::read(fd, data, length);
		if(nR != length) {
			cout << "read error from addr: " << static_cast<int32_t>(m_i2cAddr) <<
					" (" << (char*)data << ")" << endl;
			tries++;
			usleep(10000);
		}
	} while(nR != length && tries < 5);
	i2cMutex.unlock();
	return nR;
}

int I2c::read(uint8_t i2cCmd, void* data, uint8_t length) {
	write(&i2cCmd, 1);
	usleep(10000);
	return read(data, length);
}

int I2c::close() {
	return 0;
	i2cMutex.lock();
	::close(fd);
	i2cMutex.unlock();
	return 0;
}
