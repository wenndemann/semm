/*
 * Playboard.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "Playboard.h"

Playboard::Playboard() :
	_displays({
		{1, DisplayPtr(new Display(I2C_DBEN_ADDR_0, Display::updateInterval))},
		{2, DisplayPtr(new Display(I2C_DBEN_ADDR_1, Display::updateInterval))},
		{4, DisplayPtr(new Display(I2C_DBEN_ADDR_2, Display::updateInterval))},
		{8, DisplayPtr(new Display(I2C_DBEN_ADDR_3, Display::updateInterval))}
	}),
 	_ledRing(new LedRing(I2C_LR_ADDR)),
	_ledStripe(new LedStripes(I2C_LS_ADDR)),
	_rfid(new Rfid(I2C_RFID_ADDR)),
	_XYDrive(new XYdrive(I2C_XYS_ADDR)),
	_fields({
		{  0, FieldPtr(new Field(  1,  4,  0)) },
		{  1, FieldPtr(new Field(  2,  4,  1)) },
		{  2, FieldPtr(new Field(  3,  4,  2)) },
		{  3, FieldPtr(new Field(  4,  4,  3)) },
		{  4, FieldPtr(new Field(  4,  3,  4)) },
		{  5, FieldPtr(new Field(  4,  2,  5)) },
		{  6, FieldPtr(new Field(  4,  1,  6)) },
		{  7, FieldPtr(new Field(  5,  1,  7)) },
		{  8, FieldPtr(new Field(  6,  1,  8)) },
		{  9, FieldPtr(new Field(  6,  2,  9)) },
		{ 10, FieldPtr(new Field(  6,  3, 10)) },
		{ 11, FieldPtr(new Field(  6,  4, 11)) },
		{ 12, FieldPtr(new Field(  7,  4, 12)) },
		{ 13, FieldPtr(new Field(  8,  4, 13)) },
		{ 14, FieldPtr(new Field(  9,  4, 14)) },
		{ 15, FieldPtr(new Field(  9,  5, 15)) },
		{ 16, FieldPtr(new Field(  9,  6, 16)) },
		{ 17, FieldPtr(new Field(  8,  6, 17)) },
		{ 18, FieldPtr(new Field(  7,  6, 18)) },
		{ 19, FieldPtr(new Field(  6,  6, 19)) },
		{ 20, FieldPtr(new Field(  6,  7, 20)) },
		{ 21, FieldPtr(new Field(  6,  8, 21)) },
		{ 22, FieldPtr(new Field(  6,  9, 22)) },
		{ 23, FieldPtr(new Field(  5,  9, 23)) },
		{ 24, FieldPtr(new Field(  4,  9, 24)) },
		{ 25, FieldPtr(new Field(  4,  8, 25)) },
		{ 26, FieldPtr(new Field(  4,  7, 26)) },
		{ 27, FieldPtr(new Field(  4,  6, 27)) },
		{ 28, FieldPtr(new Field(  3,  6, 28)) },
		{ 29, FieldPtr(new Field(  2,  6, 29)) },
		{ 30, FieldPtr(new Field(  1,  6, 30)) },
		{ 31, FieldPtr(new Field(  1,  5, 31)) },
		//start area
		{ 32, FieldPtr(new Field(  0,  1, 32)) },
		{ 33, FieldPtr(new Field(  1,  1, 33)) },
		{ 34, FieldPtr(new Field(  1,  0, 34)) },
		{ 35, FieldPtr(new Field(  9,  0, 35)) },
		{ 36, FieldPtr(new Field(  9,  1, 36)) },
		{ 37, FieldPtr(new Field( 10,  1, 37)) },
		{ 38, FieldPtr(new Field( 10,  9, 38)) },
		{ 39, FieldPtr(new Field(  9,  9, 39)) },
		{ 40, FieldPtr(new Field(  9, 10, 40)) },
		{ 41, FieldPtr(new Field(  1, 10, 41)) },
		{ 42, FieldPtr(new Field(  1,  9, 42)) },
		{ 43, FieldPtr(new Field(  0,  9, 43)) },
		//end area
		{ 44, FieldPtr(new Field(  2,  5, 44)) },
		{ 45, FieldPtr(new Field(  3,  5, 45)) },
		{ 46, FieldPtr(new Field(  4,  5, 46)) },
		{ 47, FieldPtr(new Field(  5,  2, 47)) },
		{ 48, FieldPtr(new Field(  5,  3, 48)) },
		{ 49, FieldPtr(new Field(  5,  4, 49)) },
		{ 50, FieldPtr(new Field(  8,  5, 50)) },
		{ 51, FieldPtr(new Field(  7,  5, 51)) },
		{ 52, FieldPtr(new Field(  6,  5, 52)) },
		{ 53, FieldPtr(new Field(  5,  8, 53)) },
		{ 54, FieldPtr(new Field(  5,  7, 54)) },
		{ 55, FieldPtr(new Field(  5,  6, 55)) }
//		//gap area
//		{ 56, FieldPtr(new Field(  1,  3, 56)) },
//		{ 57, FieldPtr(new Field(  2,  3, 57)) },
//		{ 58, FieldPtr(new Field(  3,  3, 58)) },
//		{ 58, FieldPtr(new Field(  3,  3, 58)) },
//		{ 58, FieldPtr(new Field(  3,  3, 58)) },
//		{ 59, FieldPtr(new Field(  3,  2, 59)) },
//		{ 60, FieldPtr(new Field(  3,  1, 60)) },
//		{ 61, FieldPtr(new Field(  3,  0, 61)) },
//		{ 62, FieldPtr(new Field(  4,  0, 62)) },
//		{ 63, FieldPtr(new Field(  5,  0, 63)) },
//		{ 64, FieldPtr(new Field(  6,  0, 64)) },
//		{ 65, FieldPtr(new Field(  7,  0, 65)) },
//		{ 66, FieldPtr(new Field(  7,  1, 66)) },
//		{ 67, FieldPtr(new Field(  7,  2, 67)) },
//		{ 68, FieldPtr(new Field(  7,  3, 68)) },
//		{ 68, FieldPtr(new Field(  7,  3, 68)) },
//		{ 68, FieldPtr(new Field(  7,  3, 68)) },
//		{ 69, FieldPtr(new Field(  8,  3, 69)) },
//		{ 70, FieldPtr(new Field(  9,  3, 70)) },
//		{ 71, FieldPtr(new Field( 10,  3, 71)) },
//		{ 72, FieldPtr(new Field( 10,  4, 72)) },
//		{ 73, FieldPtr(new Field( 10,  5, 73)) },
//		{ 74, FieldPtr(new Field( 10,  6, 74)) },
//		{ 75, FieldPtr(new Field( 10,  7, 75)) },
//		{ 76, FieldPtr(new Field(  9,  7, 76)) },
//		{ 77, FieldPtr(new Field(  8,  7, 77)) },
//		{ 78, FieldPtr(new Field(  7,  7, 78)) },
//		{ 78, FieldPtr(new Field(  7,  7, 78)) },
//		{ 78, FieldPtr(new Field(  7,  7, 78)) },
//		{ 79, FieldPtr(new Field(  7,  8, 79)) },
//		{ 80, FieldPtr(new Field(  7,  9, 80)) },
//		{ 81, FieldPtr(new Field(  7, 10, 81)) },
//		{ 82, FieldPtr(new Field(  6, 10, 82)) },
//		{ 83, FieldPtr(new Field(  5, 10, 83)) },
//		{ 84, FieldPtr(new Field(  4, 10, 84)) },
//		{ 85, FieldPtr(new Field(  3, 10, 85)) },
//		{ 86, FieldPtr(new Field(  3,  9, 86)) },
//		{ 87, FieldPtr(new Field(  3,  8, 87)) },
//		{ 88, FieldPtr(new Field(  3,  7, 88)) },
//		{ 88, FieldPtr(new Field(  3,  7, 88)) },
//		{ 88, FieldPtr(new Field(  3,  7, 88)) },
//		{ 89, FieldPtr(new Field(  2,  7, 89)) },
//		{ 90, FieldPtr(new Field(  1,  7, 90)) },
//		{ 91, FieldPtr(new Field(  0,  7, 91)) },
//		{ 92, FieldPtr(new Field(  0,  6, 92)) },
//		{ 93, FieldPtr(new Field(  0,  5, 93)) },
//		{ 94, FieldPtr(new Field(  0,  4, 94)) },
//		{ 95, FieldPtr(new Field(  0,  3, 95)) }
	})
{

}

Playboard::~Playboard() {



}


//void Playboard::moveMeeple(const Meeple& m, const Field& to) {
void Playboard::moveMeeple(uint8_t from, uint8_t to) {
	boost::lock_guard<boost::mutex> gard(_mutexXYDrive);

	if(from == to) return;

	//calculate path
	Route r;
	// add actual positions of meeples to block list
	for(PlayerMapIt pIt = _players.begin(); pIt != _players.end(); ++pIt) {
		MeepleVec& m = pIt->second->meeples();
		for(MeepleVecIt mIt = m.begin(); mIt != m.end(); ++mIt)
			r.block((*mIt)->field()->x(), (*mIt)->field()->y());
	}
	//calculate and optimize path to target position
	r.calcPath(_fields[from]->x(), _fields[from]->y(),
		_fields[to]->x(), _fields[to]->y());
	r.optimize();
	//move to start field if necessary
	if(_fields[from]->x() != _XYDrive->y()
		|| _fields[from]->y() != _XYDrive->y())
			r.appendFront(_fields[from]->x(), _fields[from]->y(), false);

	// set LEDs
	if(to >= 32 && to <= 43) { // if someone is kicked out
		_ledRing->set(LedRing::BLINK,
				_fields[to]->meeple()->color());
		_ledStripe->set(LedStripes::BLINK_FAST,
				_fields[to]->meeple()->color());
	} else {
		_ledRing->set(LedRing::ON,
				_fields[to]->meeple()->color());
		_ledStripe->set(LedStripes::ON,
				_fields[to]->meeple()->color());
	}

	// move along path
	WaypointVec& wayPts = r.getList();
	for(WaypointVecIt it = wayPts.begin(); it != wayPts.end(); ++it) {
		_XYDrive->moveMagnet(it->mag());
		_XYDrive->moveCarriage(it->x(), it->y());
	}

	//turn LedStripe off
	_ledStripe->set(LedStripes::OFF, 0);
}

uint16_t Playboard::readId(const Field& f) {
	boost::lock_guard<boost::mutex> gard(_mutexXYDrive);
	_XYDrive->moveMagnet(false);
	_XYDrive->moveCarriage((int)((10-f.no()) * 172 + 250),
			_XYDrive->y());
//	readRfid(tags, sizeof(tagLine_t));
}
