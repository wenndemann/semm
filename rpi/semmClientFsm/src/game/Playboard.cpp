/*
 * Playboard.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "Playboard.h"

Playboard::Playboard() :
	_displays({
		DisplayPtr(new Display(I2C_DBEN_ADDR_0, Display::updateInterval)),
		DisplayPtr(new Display(I2C_DBEN_ADDR_1, Display::updateInterval)),
		DisplayPtr(new Display(I2C_DBEN_ADDR_2, Display::updateInterval)),
		DisplayPtr(new Display(I2C_DBEN_ADDR_3, Display::updateInterval))
	}),
	_ledRing(new LedRing(I2C_LR_ADDR)),
	_ledStripe(new LedStripes(I2C_LS_ADDR)),
	_rfid(new Rfid(I2C_RFID_ADDR)),
	_XYDrive(new XYdrive(I2C_XYS_ADDR))
{

}

Playboard::~Playboard() {



}

