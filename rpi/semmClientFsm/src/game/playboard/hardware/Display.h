#ifndef DISPLAY_H_
#define DISPLAY_H_

// global includes
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <iostream>

// local includes
#include "I2c.h"
#include "../../../../../../arduino/display/i2c_defs.h"

class Display: protected I2c {

public:
	Display(uint8_t i2cAddr, long msec);
	virtual ~Display();
	int setPictures(uint8_t number);
	int getPicture() { return m_actPicNumber; }
	int setName(uint8_t player, const char* name);
	char* getName(uint8_t player);
	int getSelection() { return m_encoder; }
	uint8_t getVersion();
	uint8_t getEncoder() { return m_encoder; }
	void setBlocked(bool blocked) { m_blocked = blocked; }

	static const long updateInterval = 500;

private:
	int m_actPicNumber;
	long m_periode;
	uint8_t m_encoder;
	bool m_readEnable;
	bool m_blocked;
	boost::asio::io_service m_io;
	boost::asio::deadline_timer* m_timer;
	boost::thread* m_handlerThread;
	boost::asio::strand* m_strand;

	void handler();
};

#endif /* DISPALY_H_ */
