#include "XYdrive.h"

XYdrive::XYdrive(uint8_t i2cAddrXYdrive) {
/*
	m_isMoving = false;
	setI2cAddr(i2cAddrXYdrive);

	boost::thread hardwareWorker(boost::bind(&XYdrive::m_worker, this));
	m_mag = true;
	magnet(false);
	m_actX = 0; m_actY = 0;
*/
}
XYdrive::~XYdrive() {
}

/*

void XYdrive::m_moveTo(int x, int y) {
	uint8_t buf[10];

	buf[0] = DEV_XY_DRIVE;
	buf[1] = XY_DRIVE_CMD_MOVE;
	memcpy(&buf[2], &x, sizeof(x));
	memcpy(&buf[6], &y, sizeof(y));

	addCmd(buf, sizeof(buf));
}

void XYdrive::magnet(bool value) {
	addCmd(3, DEV_XY_DRIVE, XY_DRIVE_CMD_MAG, (uint8_t) value);
}

void XYdrive::ref() {
	addCmd(2, DEV_XY_DRIVE, XY_DRIVE_CMD_REF);
}

void XYdrive::moveToXY(uint8_t x, uint8_t y) {
	float x2 = ((10-x) * 172);
	float y2 = ((10-y) * 172);
	m_actX = x2; m_actY = y2;
	debug("move to (%d/%d)\n", x, y);
	m_moveTo((int) x2, (int) y2);
}

void XYdrive::scan(tags_t* tags) {
	for(int i = 0; i < 11; i++) {
		scanLine(i, &(tags->line[i]));
	}
}

void XYdrive::scanLine(uint8_t line, tagLine_t* tags) {
	magnet(0);
	m_moveTo((int)((10-line) * 172 + 250),m_actY);
	readRfid(tags, sizeof(tagLine_t));
}

void XYdrive::readRfid(void* scanDest, int length) {
	uint8_t buf[6];

	buf[0] = DEV_RFID;
	buf[1] = RFID_CMD_READ;
	memcpy(&buf[2], &scanDest, sizeof(scanDest));
//	memcpy(&buf[6], &length, sizeof(length));

	addCmd(buf, sizeof(buf));
}
void XYdrive::setLedRing(uint8_t mode, uint8_t color) {
	addCmd(4, DEV_LED_RING, LED_RING_CMD_SET, (uint8_t) mode, (uint8_t) color);
}

bool XYdrive::getIsMoving() {
	bool ret;

	m_cB.mutex.lock() ;
	ret = m_isMoving;
	m_cB.mutex.unlock() ;

	return ret;
}

void XYdrive::addCmd(uint8_t cmd[], int length) {
	m_cB.mutex.lock() ;
	while(((m_cB.in + 1) % CMDBUF_SIZE) == m_cB.out)  //while cmdBuffer is full
		m_cB.condRem.wait(m_cB.mutex);
	memcpy(&m_cB.cmd[m_cB.in][0], cmd, length);
	++m_cB.in %= CMDBUF_SIZE;
	m_cB.condAdd.notify_one();
	m_cB.mutex.unlock();
}

void XYdrive::addCmd(uint8_t n, ...) {
	va_list list;
	uint8_t buf[CMDBUF_MAX_CMD_LENGHT];

	va_start(list, n);
	for(int i = 0; i < n; i++)
		buf[i] = va_arg(list, int);
	addCmd(buf,n);
	va_end(list);
}

void XYdrive::m_worker() {
	uint8_t buf[CMDBUF_MAX_CMD_LENGHT + 1];
	uint8_t target, cmd;
	debug("start XYdrive worker\n");

	while(1) {
		m_cB.mutex.lock();
		while(m_cB.in == m_cB.out) {// while cmdBffer is empty
			m_isMoving = false;
			m_cB.condAdd.wait(m_cB.mutex);
		}
		m_isMoving = true;

		target = m_cB.cmd[m_cB.out][0]; // copy actual element
		cmd = m_cB.cmd[m_cB.out][1];
		memcpy(&buf, &m_cB.cmd[m_cB.out][2], CMDBUF_MAX_CMD_LENGHT - 1);

		++m_cB.out %= CMDBUF_SIZE; // next element
		m_cB.condRem.notify_one();
		m_cB.mutex.unlock();

		switch(target) {
		case DEV_XY_DRIVE: {
			switch (cmd) {
			case XY_DRIVE_CMD_MOVE: {
				int x, y;
				char b[32];
				bool running = true;

				memcpy(&x, &buf[0], sizeof(x));
				memcpy(&y, &buf[4], sizeof(y));
				sprintf(b, " %.2f %.2f", (float) x, (float) y);
				write(I2C_XYS_SETPOINTS, b, sizeof(b));

				memset(b, '\0', sizeof(b));
				while(running) {
					usleep(250000);
					read(I2C_XYS_RUNNING, b, sizeof(b));
					running = atoi(&b[2]);
				}
			}
			break;
			case XY_DRIVE_CMD_MAG:
				char b[32];
				if(buf[0] == m_mag) break;
				else {
					m_mag = buf[0];
					sprintf(b, " %.2d", m_mag);
					write(I2C_XYS_SERVO, b, sizeof(b));
					usleep(600000);
				}
				break;
			}
			break;
		}
		case DEV_RFID: {
			switch (cmd) {
			case RFID_CMD_READ: {
				tagLine_t* scanDest;
				memcpy(&scanDest, &buf[0], sizeof(scanDest));
				m_rfid->readLine(scanDest);
			}
			break;
			}
			break;

			default:
				debug("    - device unknown\n");
				break;
		}
		break;
		case DEV_LED_RING: {
			switch(cmd) {
			case LED_RING_CMD_SET:
				debug("LED RING set Color %d, %d\n", buf[1], buf[2]);
				m_ledRing->set(buf[0], buf[1]);
			}//
		}
		break;
		case DEV_LED_STRIPES: {

		}
		}
	}
}
*/
