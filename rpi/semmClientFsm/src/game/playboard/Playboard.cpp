/*
 * Playboard.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: reinhard
 */

#include "Playboard.h"
#include "../../fsm/mainFsm.h"

Playboard::Playboard( fsm::gameFSM* gameFsmPtr ) :
	_displays({
		{1, DisplayPtr(new Display(I2C_DBEN_ADDR_0, Display::updateInterval, gameFsmPtr))},
		{2, DisplayPtr(new Display(I2C_DBEN_ADDR_1, Display::updateInterval, gameFsmPtr))},
		{4, DisplayPtr(new Display(I2C_DBEN_ADDR_2, Display::updateInterval, gameFsmPtr))},
		{8, DisplayPtr(new Display(I2C_DBEN_ADDR_3, Display::updateInterval, gameFsmPtr))}
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
	})
{

}

Playboard::~Playboard() {



}

bool Playboard::addPlayer( int32_t colorId) {
	_players[colorId] = PlayerPtr(new Player(colorId));

	std::vector< uint16_t > tags( 3 );
	for (uint32_t i = 0; i < tags.size( ); i++) {

		uint32_t fieldId = _players[colorId]->getStartAreaPos(i);

		tags[ i ] = readId( fieldId );

		if ( tags[ i ] == 0 )
		{
			_players.erase(_players.find(colorId));
			return false;
		}
	}
	_players[colorId]->addMeeples( tags );
	return true;
}

bool Playboard::delPlayer( int32_t colorId )
{
	PlayerMapIt it = _players.find( colorId );
	if ( it != _players.end( ) )
	{
		_players.erase( it );
	}
}

//void Playboard::moveMeeple(const Meeple& m, const Field& to) {
void Playboard::moveMeeple(uint8_t from, uint8_t to) {
	boost::lock_guard<boost::mutex> gard(_mutexXYDrive);

	if(from == to) return;

	MeeplePtr meeple = getMeepleFromFieldId( from );
	if ( !meeple )
		return;

	uint8_t color = static_cast<uint8_t>(getColorFromFieldId(from));

	//calculate path
	Route r;
	// add actual positions of meeples to block list
	for(PlayerMapIt pIt = _players.begin(); pIt != _players.end(); ++pIt) {
		MeepleVec& m = pIt->second->meeples();
		for(MeepleVecIt mIt = m.begin(); mIt != m.end(); ++mIt)
			r.block( _fields[(*mIt)->fieldId()]->x(), _fields[(*mIt)->fieldId()]->y());
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
		_ledRing->set(LedRing::BLINK, color);
		_ledStripe->set(LedStripes::BLINK_FAST, color);
	} else {
		_ledRing->set(LedRing::ON, color);
		_ledStripe->set(LedStripes::ON, color);
	}

	// move along path
	WaypointVec& wayPts = r.getList();
	for(WaypointVecIt it = wayPts.begin(); it != wayPts.end(); ++it) {
		_XYDrive->liftMagnet(it->mag());
		_XYDrive->moveMagnet(it->x(), it->y());
	}

	//turn LedStripe off
	_ledStripe->set(LedStripes::OFF, color);
	_ledRing->set(LedRing::OFF, color);

	//lower magnet
	_XYDrive->liftMagnet(false);

	//move meeple
	meeple->fieldId( to );
}

uint16_t Playboard::readId(uint32_t fieldId) {
	boost::lock_guard<boost::mutex> gard(_mutexXYDrive);
	uint8_t color = static_cast<uint8_t>(getColorFromFieldId(fieldId));
	_ledStripe->set(LedStripes::ON, color);
	uint8_t x = _fields[fieldId]->x();
	uint8_t y = _fields[fieldId]->y();
	_XYDrive->liftMagnet(false);
	_XYDrive->moveCarriage(x);//static_cast<uint8_t>((10-static_cast<int32_t>(x)) * 172 + 250));
	_ledStripe->set(LedStripes::OFF, color);
	return _rfid->readTag(y);
}

int32_t Playboard::getColorFromFieldId( int32_t fieldId) {
	for(PlayerMapIt pIt = _players.begin(); pIt != _players.end(); ++pIt) {
		MeepleVec& m = pIt->second->meeples();
		for(MeepleVecIt mIt = m.begin(); mIt != m.end(); ++mIt) {
			if ( (*mIt)->fieldId() == fieldId)
				return pIt->second->color();
		}
	}
	return -1;
}
MeeplePtr Playboard::getMeepleFromFieldId( int32_t fieldId) {
	for(PlayerMapIt pIt = _players.begin(); pIt != _players.end(); ++pIt) {
		MeepleVec& m = pIt->second->meeples();
		for(MeepleVecIt mIt = m.begin(); mIt != m.end(); ++mIt) {
			if ( (*mIt)->fieldId() == fieldId)
				return *mIt;
		}
	}
	return MeeplePtr();
}
