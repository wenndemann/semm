#include "Display.h"
#include "../src/Game.h"

extern Game* g_game;

Display::Display(uint8_t i2cAddr, long msec) {
	setI2cAddr(i2cAddr);
	m_periode = msec;
	m_readEnable = false;
	m_blocked = false;
	m_encoder = 0;
	m_strand = new boost::asio::strand(m_io);
	m_timer = new boost::asio::deadline_timer(m_io, boost::posix_time::milliseconds(m_periode));
	m_timer->async_wait(m_strand->wrap(boost::bind(&Display::handler, this)));
	m_handlerThread = new boost::thread (boost::bind(&boost::asio::io_service::run, &m_io));
	setPictures(I2C_DBEN_PIC_SEMM);
}

Display::~Display() {

}

int Display::setPictures(uint8_t number) {
	if(m_actPicNumber == number || m_blocked) return 0;
//	while((m_actPicNumber >= I2C_DBEN_PIC_DICE_1) && (m_actPicNumber <= I2C_DBEN_PIC_DICE_6) && !m_encoder)
//		usleep(250000);
	m_actPicNumber = number;
	I2c::write(I2C_DBEN_PICTURE, &number, sizeof number);
	//if(m_cliDisplay != NULL) m_cliDisplay->callPicture(number);
	debug("set display 0x%x to pic 0x%x\n", getI2cAddr(), number);
	return 0;
}

int Display::setName(uint8_t player, const char* name) {
	return 0;
}

char* Display::getName(uint8_t player) {
	return 0;
}

uint8_t Display::getVersion() {
	uint8_t version;

	I2c::read(I2C_DBEN_VERSION, &version, sizeof version);
	return version;
}

void Display::handler() {
	int n = I2c::read(I2C_DBEN_BUTTON, &m_encoder, sizeof(m_encoder));
	if(n != sizeof(m_encoder)) {
		m_encoder = 0;
//		debug("display read error, n = %d\n",n);
	}
	if(m_encoder < 255 && m_encoder > 0) {
//		debug("display %d = %d\n", getI2cAddr(), m_encoder);
		uint8_t buf[5];
		m_readEnable = false;
		buf[0] = 0;
		buf[1] = DISPLAY_CMD_PUSH_ENTER;
		buf[2] = getI2cAddr() - 0x50;
		buf[3] = m_actPicNumber;
		buf[4] = m_encoder;
		g_game->addCmd(buf, sizeof(buf));
		if(m_actPicNumber == I2C_DBEN_PIC_ENTER)
			setPictures(I2C_DBEN_PIC_WAIT);
	} else if(m_encoder == 255) {
		uint8_t buf[3];
		buf[0] = 0;
		buf[1] = DISPLAY_CMD_CANCEL_GAME;
		buf[2] = getI2cAddr() - 0x50;
		g_game->addCmd(buf, sizeof(buf));
	}

	m_timer->expires_at(m_timer->expires_at() + boost::posix_time::milliseconds(m_periode));
	m_timer->async_wait(boost::bind(&Display::handler, this));
}
