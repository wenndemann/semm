#include "XYdrive.h"

#include <iostream>
#include <cstdio>

using namespace std;

XYdrive::XYdrive(uint8_t i2cAddrXYdrive)
: _x(10)
, _y(10)
{
	setI2cAddr(i2cAddrXYdrive);

	_magPos = true;
	liftMagnet(false);
	resetDev();
//	moveMagnet(5, 5);
//	moveCarriage(5);
}

XYdrive::~XYdrive() {

}

void XYdrive::liftMagnet(bool value) {
	if (value == _magPos) return;

	char b[32];
	_magPos = value;
	sprintf(b, " %.2d", _magPos);
	//write(I2C_XYS_SERVO, b, sizeof(b));
	usleep(600000);
}

void XYdrive::moveMagnet(uint8_t x, uint8_t y)
{
	move( x, y, 0 );
}

void XYdrive::moveCarriage(uint8_t x)
{
	move( x, _y, 200 );
	_x = 11;
}

void XYdrive::resetDev() {
	uint8_t one = '1';
	//write(I2C_XYS_RESET, &one, 1);
//	sleep(15);
}

void XYdrive::move( uint8_t x, uint8_t y, int32_t offset )
{
	if(x == _x && y == _y) return;

	cout << "move to " << static_cast<int32_t>(x) << " / " << static_cast<int32_t>(y) << endl;

	char b[32];
	bool running = true;
	float offsetX = 0.0f, offsetY = 0.0f;
	if(_x < x) offsetX = -10.0f;
	else if(_x > x) offsetX = 10.0f;
	if(_y < y) offsetY = -10.0f;
	else if(_y > y) offsetY = 10.0f;

	_x = x; _y = y;

	float x2 = static_cast<float>((10-static_cast<int32_t>(_x)) * 173 + offset + 80);
	float y2 = static_cast<float>((10-static_cast<int32_t>(_y)) * 173 - 30 );

	x2 += offsetX; y2 += offsetY;
	cout << "offset x = " << offsetX << " offsetY = " << offsetY << endl;

	sprintf(b, " %.2f %.2f", x2, y2);
	//write(I2C_XYS_SETPOINTS, b, sizeof(b));

	memset(b, '\0', sizeof(b));
	while(running) {
		usleep(250000);
		read(I2C_XYS_RUNNING, b, sizeof(b));
		running = atoi(&b[2]);
	}
}
