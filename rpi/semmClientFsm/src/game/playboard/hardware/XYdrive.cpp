#include "XYdrive.h"

XYdrive::XYdrive(uint8_t i2cAddrXYdrive) {
	setI2cAddr(i2cAddrXYdrive);

	_magPos = true;
	moveMagnet(false);
}

XYdrive::~XYdrive() {

}

void XYdrive::moveMagnet(bool value) {
	char b[32];
	if (value == _magPos) return;
	else {
		_magPos = value;
		sprintf(b, " %.2d", _magPos);
		write(I2C_XYS_SERVO, b, sizeof(b));
		usleep(600000);
	}
}

void XYdrive::moveCarriage(uint8_t x, uint8_t y) {
	char b[32];
	bool running = true;

	sprintf(b, " %.2f %.2f", (float) x, (float) y);
	write(I2C_XYS_SETPOINTS, b, sizeof(b));

	memset(b, '\0', sizeof(b));
	while(running) {
		usleep(250000);
		read(I2C_XYS_RUNNING, b, sizeof(b));
		running = atoi(&b[2]);
	}
}
