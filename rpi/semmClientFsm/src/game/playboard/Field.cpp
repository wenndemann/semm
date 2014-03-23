/*
 * Field.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: reinhard
 */

#include "Field.h"

Field::Field() :
	_x(0),
	_y(0),
	_no(0)
{

}

Field::Field(uint8_t x, uint8_t y, uint8_t no) :
	_x(x),
	_y(y),
	_no(no)
{

}

Field::~Field() {

}

