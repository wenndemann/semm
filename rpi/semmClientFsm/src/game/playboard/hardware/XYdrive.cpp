#include "XYdrive.h"

XYdrive::XYdrive(uint8_t i2cAddrXYdrive) {
	setI2cAddr(i2cAddrXYdrive);

	_magPos = true;
	moveMagnet(false);
}

XYdrive::~XYdrive() {

}

void XYdrive::moveMagnet(bool value) {
	if (value == _magPos) return;

	char b[32];
	_magPos = value;
	sprintf(b, " %.2d", _magPos);
	write(I2C_XYS_SERVO, b, sizeof(b));
	usleep(600000);
}

void XYdrive::moveCarriage(uint8_t x, uint8_t y) {
	if(x == _x && y == _y) return;

	char b[32];
	bool running = true;
	_x = x; _y = y;

	float x2 = ((10-_x) * 172);
	float y2 = ((10-_y) * 172);

	sprintf(b, " %.2f %.2f", x2, y2);
	write(I2C_XYS_SETPOINTS, b, sizeof(b));

	memset(b, '\0', sizeof(b));
	while(running) {
		usleep(250000);
		read(I2C_XYS_RUNNING, b, sizeof(b));
		running = atoi(&b[2]);
	}
}

void XYdrive::moveCarriage(uint8_t x) {
	moveCarriage(x, _y);
}
