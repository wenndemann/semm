#ifndef DISPLAY_H_
#define DISPLAY_H_

// local includes
#include "I2c.h"
#include "../../../../../../arduino/display/i2c_defs.h"
#include "../../../defs.h"

// global includes
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <iostream>

#include <boost/msm/back/state_machine.hpp>
namespace fsm
{
	class GameFSM_;
	class SelectColorFSM_;
}

class Display: protected I2c {

public:
	Display(uint8_t i2cAddr, long msec, boost::msm::back::state_machine< fsm::GameFSM_ >* gameFsmPtr);
	virtual ~Display();
	void enableSubFSMEvents( );
	void disableSubFSMEvents( );
	int setPictures(uint8_t number);
	void setPictureDice( uint8_t dice, bool moveAllowedAndCurrPlayer = false );
	int getPicture() { return m_actPicNumber; }
	int setName(uint8_t player, const char* name);
	char* getName(uint8_t player);
	int getSelection() { return m_encoder; }
	uint8_t getVersion();
	uint8_t getEncoder() { return m_encoder; }
	void setBlocked(bool blocked) { m_blocked = blocked; }
	boost::shared_ptr< boost::msm::back::state_machine< fsm::SelectColorFSM_ > > subFSM(){ return _subFSM; }

	static const long updateInterval = 500;

private:
	int m_actPicNumber;
	long m_periode;
	uint8_t m_encoder;
	bool m_readEnable;
	bool m_blocked;
	bool _subFsmEnabled;
	boost::asio::io_service m_io;
	boost::asio::deadline_timer* m_timer;
	boost::thread* m_handlerThread;
	boost::asio::strand* m_strand;

	boost::msm::back::state_machine< fsm::GameFSM_ >* _mainFSM;
	boost::shared_ptr< boost::msm::back::state_machine< fsm::SelectColorFSM_ > > _subFSM;

	void handler();
};

#endif /* DISPALY_H_ */
