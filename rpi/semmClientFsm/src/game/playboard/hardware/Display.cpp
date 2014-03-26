#include "Display.h"

#include "../../../fsm/mainFsm.h"

//using namespace std;

Display::Display(uint8_t i2cAddr, long msec, fsm::gameFSM* gameFsmPtr)
: _mainFSM( gameFsmPtr )
, _subFsmEnabled( false )
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

void Display::enableSubFSMEvents( )
{
	_subFSM = _mainFSM->_ssms->at( static_cast<int32_t>(getI2cAddr())-80 );
	if ( _subFSM )
		_subFsmEnabled = true;
}

void Display::disableSubFSMEvents( )
{
	_subFsmEnabled = false;
}

int Display::setPictures(uint8_t number) {
	if(m_actPicNumber == number || m_blocked) return 0;
//	while((m_actPicNumber >= I2C_DBEN_PIC_DICE_1) && (m_actPicNumber <= I2C_DBEN_PIC_DICE_6) && !m_encoder)
//		usleep(250000);
	m_actPicNumber = number;
	I2c::write(I2C_DBEN_PICTURE, &number, sizeof number);
	//if(m_cliDisplay != NULL) m_cliDisplay->callPicture(number);
	std::cout << "set display " << pow(2,static_cast<int32_t>(getI2cAddr())-80) << " to pic " << static_cast<int32_t>(number) << std::endl;
	return 0;
}

void Display::setPictureDice( uint8_t dice, bool moveAllowedAndCurrPlayer )
{
	uint8_t moveAllowedFlag = 0;
	if ( moveAllowedAndCurrPlayer )
	{	moveAllowedFlag = 64; 	}

	if ( dice == 0 )
		this->setPictures( I2C_DBEN_PIC_DICE | moveAllowedFlag );
	else if ( dice == 1 )
		this->setPictures( I2C_DBEN_PIC_DICE_1 | moveAllowedFlag );
	else if ( dice == 2 )
		this->setPictures( I2C_DBEN_PIC_DICE_2 | moveAllowedFlag );
	else if ( dice == 3 )
		this->setPictures( I2C_DBEN_PIC_DICE_3 | moveAllowedFlag );
	else if ( dice == 4 )
		this->setPictures( I2C_DBEN_PIC_DICE_4 | moveAllowedFlag );
	else if ( dice == 5 )
		this->setPictures( I2C_DBEN_PIC_DICE_5 | moveAllowedFlag );
	else if ( dice == 6 )
		this->setPictures( I2C_DBEN_PIC_DICE_6 | moveAllowedFlag );
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
			std::cout << "Display " << pow(2,static_cast<int32_t>(getI2cAddr())-80) << " pressed Enter" << std::endl;
			if ( _subFsmEnabled )
				_subFSM->process_event( fsm::scmEvEnter( ) );
			else
				_mainFSM->process_event( fsm::evEnter( ) );
		}
		if ( m_encoder == 2 )
		{
			if ( _subFsmEnabled )
				_subFSM->process_event( fsm::scmEvCancel( ) );
			//else
				//_mainFSM->process_event( fsm::GEILES_ENTER_EVENT( ) );
		}
	} else if(m_encoder == 255) {

	}

	m_timer->expires_at(m_timer->expires_at() + boost::posix_time::milliseconds(m_periode));
	m_timer->async_wait(boost::bind(&Display::handler, this));
}
