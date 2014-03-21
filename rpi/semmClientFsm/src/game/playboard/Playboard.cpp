/*
 * Playboard.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "Playboard.h"

Playboard::Playboard() :
	_displays({
		{ 1, DisplayPtr(new Display(I2C_DBEN_ADDR_0, Display::updateInterval)) },
		{ 2, DisplayPtr(new Display(I2C_DBEN_ADDR_1, Display::updateInterval)) },
		{ 4, DisplayPtr(new Display(I2C_DBEN_ADDR_2, Display::updateInterval)) },
		{ 8, DisplayPtr(new Display(I2C_DBEN_ADDR_3, Display::updateInterval)) }
	}),
 	_ledRing(new LedRing(I2C_LR_ADDR)),
	_ledStripe(new LedStripes(I2C_LS_ADDR)),
	_rfid(new Rfid(I2C_RFID_ADDR)),
	_XYDrive(new XYdrive(I2C_XYS_ADDR))
{

}

Playboard::~Playboard() {



}

