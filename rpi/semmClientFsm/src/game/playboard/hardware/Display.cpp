#include "Display.h"

#include "../../../fsm/mainFsm.h"

using namespace std;

Display::Display(uint8_t i2cAddr, long msec, fsm::gameFSM* gameFsmPtr)
: _mainFSM( gameFsmPtr )
{
	std::cout << "    creating display " << static_cast<int32_t>(i2cAddr-80) << std::endl;
	setI2cAddr(i2cAddr);
	m_periode = msec;
	m_readEnable = false;
	m_blocked = false;
	m_encoder = 0;
	m_strand = new boost::asio::strand(m_io);
	m_timer = new boost::asio::deadline_timer(m_io, boost::posix_time::milliseconds(m_periode));
	m_timer->async_wait(m_strand->wrap(boost::bind(&Display::handler, this)));
	m_handlerThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));
}

Display::~Display() {

}

void Display::init( )
{
	_subFSM = _mainFSM->_ssms->at( static_cast<int32_t>(getI2cAddr())-80 );
}

int Display::setPictures(uint8_t number) {
	if(m_actPicNumber == number || m_blocked) return 0;
//	while((m_actPicNumber >= I2C_DBEN_PIC_DICE_1) && (m_actPicNumber <= I2C_DBEN_PIC_DICE_6) && !m_encoder)
//		usleep(250000);
	m_actPicNumber = number;
	I2c::write(I2C_DBEN_PICTURE, &number, sizeof number);
	//if(m_cliDisplay != NULL) m_cliDisplay->callPicture(number);
	cout << "set display " << pow(2,static_cast<int32_t>(getI2cAddr())-80) << " to pic " << static_cast<int32_t>(number) << endl;
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
		if ( m_encoder == 1 )
		{
			if ( _subFSM )
				_subFSM->process_event( fsm::scmEnter( ) );
			//else
				//_mainFSM->process_event( fsm::GEILES_ENTER_EVENT( ) );
		}
	} else if(m_encoder == 255) {

	}

	m_timer->expires_at(m_timer->expires_at() + boost::posix_time::milliseconds(m_periode));
	m_timer->async_wait(boost::bind(&Display::handler, this));
}
