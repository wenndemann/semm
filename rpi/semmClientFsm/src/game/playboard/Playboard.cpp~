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
	ledShow();
//	sleep(15);
	_XYDrive->moveMagnet(5, 5);
}

Playboard::~Playboard() {



}

bool Playboard::addPlayer( int32_t colorId) {
	_players[colorId] = PlayerPtr(new Player(colorId));

	std::stringstream ss;
	ss << "addPlayer " << colorId;

	std::vector< uint16_t > tags( 3 );

	if ( !readPlayersMeepleAtStart( colorId, tags ) )
	{
		_players.erase(_players.find(colorId));
		cout << ss.str( ) << " failed!" << std::endl;
		return false;
	}

	_players[colorId]->addMeeples( tags );
	cout << ss.str( ) << " successful!" << std::endl;
	return true;
}

bool Playboard::delPlayer( int32_t colorId )
{
	cout << "delPlayer " << colorId << endl;
	PlayerMapIt it = _players.find( colorId );
	if ( it != _players.end( ) )
	{
		_players.erase( it );
		return true;
	}
	return false;
}

bool Playboard::readPlayersMeepleAtStart( int32_t colorId, std::vector< uint16_t >& tags )
{
	stringstream ss;
	ss << "Reading from fields ";

	for (uint32_t i = 0; i < tags.size( ); i++) {

		uint32_t fieldId = static_cast<int32_t>(_players[colorId]->getStartAreaPos(i));

		tags[ i ] = readId( fieldId );

		ss << " " << static_cast<int32_t>(fieldId);

		if ( tags[ i ] == 0 )
		{
			std::cout << ss.str( ) << " failed";
			return false;
		}
	}
	std::cout << ss.str( ) << " successful" << endl;
	return true;
}

bool Playboard::reconfigurePlayersMeeple( int32_t colorId, const std::vector< uint16_t >& tags )
{
	MeepleVec& meeples = _players[colorId]->meeples( );
	const std::vector<int32_t>& startAreaPos = _players[colorId]->startAreaPos( );
	if ( tags.size( ) != meeples.size( ) ){ return false; }

	for ( uint32_t i = 0; i < tags.size( ); i++ )
	{
		meeples.at( i )->fieldId( static_cast<uint16_t>(tags.at( i )) );
		meeples.at( i )->fieldId( static_cast<int32_t>(startAreaPos.at( i )) );
	}

	return true;
}

//void Playboard::moveMeeple(const Meeple& m, const Field& to) {
void Playboard::moveMeeple(uint8_t from, uint8_t to) {
	uint8_t color = static_cast<uint8_t>(getColorFromFieldId(static_cast<int32_t>(from)));
	moveMeepleXY(color, _fields[from]->x(), _fields[from]->y(), to);

	//move meeple
	setMeepleMove(color, from, to);
}

void Playboard::moveMeepleXY(uint8_t color, uint8_t fromX, uint8_t fromY, uint8_t to) {
	boost::lock_guard<boost::mutex> gard(_mutexXYDrive);

		if(fromX == _fields[to]->x() && fromY == _fields[to]->y() ) return;

//		MeeplePtr meeple = getMeepleFromFieldId( from );
//		if ( !meeple )
//			return;


		//calculate path
		Route r;
		// add actual positions of meeples to block list
		for(PlayerMapIt pIt = _players.begin(); pIt != _players.end(); ++pIt) {
			MeepleVec& m = pIt->second->meeples();
			for(MeepleVecIt mIt = m.begin(); mIt != m.end(); ++mIt)
				r.block( _fields[(*mIt)->fieldId()]->x(), _fields[(*mIt)->fieldId()]->y());
		}
		//calculate and optimize path to target position
		r.calcPath(fromX, fromY, _fields[to]->x(), _fields[to]->y());
		r.optimize();
		//move to start field if necessary
		if(fromX != _XYDrive->y() || fromY != _XYDrive->y())
				r.appendFront(fromX, fromY, false);

		// set LEDs
		if(to >= 32 && to <= 43) { // if someone is kicked out
			_ledRing->set(LedRing::BLINK, color);
		} else {
			_ledRing->set(LedRing::ON, color);
		}

		// move along path
		WaypointVec& wayPts = r.getList();
		for(WaypointVecIt it = wayPts.begin(); it != wayPts.end(); ++it) {
			_XYDrive->liftMagnet(it->mag());
			_XYDrive->moveMagnet(it->x(), it->y());
		}

		//turn LedRing off
		_ledRing->set(LedRing::OFF, color);

		//lower magnet
		_XYDrive->liftMagnet(false);
}

uint16_t Playboard::readId(uint32_t fieldId ) {
	boost::lock_guard<boost::mutex> gard(_mutexXYDrive);
	uint8_t color = static_cast<uint8_t>(getColorFromFieldId(static_cast<int32_t>(fieldId)));
//	_ledStripe->set(LedStripes::ON, color);
	uint8_t x = _fields[static_cast<uint8_t>(fieldId)]->x();
	uint8_t y = _fields[static_cast<uint8_t>(fieldId)]->y();
	std::cout << "reading ID from field " << static_cast<int32_t>(fieldId)
			  << " of color " << static_cast<int32_t>(color)
			  << " at X=" << static_cast<int32_t>(x) << " Y=" << static_cast<int32_t>(y) << std::endl;
	_XYDrive->liftMagnet(false);
	_XYDrive->moveCarriage(x);//static_cast<uint8_t>((10-static_cast<int32_t>(x)) * 172 + 250));
//	_ledStripe->set(LedStripes::OFF, color);
	return _rfid->readTag(y);
}

uint8_t Playboard::checkMovedMeeple( uint8_t color )
{
	PlayerMapIt pIt = _players.find( static_cast<int32_t>(color) );
	if ( pIt == _players.end( ) || !pIt->second )
	{
		std::cout << "There is no player with id " << static_cast<int32_t>(color)
				  << "to check for moved meeple." << std::endl;
		return 255;
	}

	MeepleVec& meeples = pIt->second->meeples();
	for ( uint32_t i = 0; i < meeples.size( ); i++ )
	{
		if ( readId( static_cast<uint32_t>(meeples[ i ]->fieldId()) ) == 0 )
		{
			return static_cast<uint8_t>( meeples[ i ]->fieldId() );
		}
	}

	std::cout << "There is no meeple moved of player " << static_cast<int32_t>(color) << std::endl;
	return 255;
}

bool Playboard::checkMeepleMove( uint8_t from, uint8_t to )
{
	uint16_t fromTagId = getMeepleFromFieldId( static_cast<int32_t>( from ) )->tag();

	uint16_t toTagId = static_cast<uint16_t>( readId( static_cast<uint32_t>(to) ) );

	return (fromTagId == toTagId);
}

bool Playboard::setMeepleMove( uint8_t color, uint8_t from, uint8_t to )
{
	PlayerMapIt pIt = _players.find( static_cast<int32_t>(color) );
	if ( pIt == _players.end( ) || !pIt->second )
	{
		std::cout << "There is no player with id " << static_cast<int32_t>(color)
				  << "to set the moved meeple from " << static_cast<int32_t>(from)
				  << "to " << static_cast<int32_t>(to) << std::endl;
		return false;
	}

	MeepleVec& meeples = pIt->second->meeples();
	for ( uint32_t i = 0; i < meeples.size( ); i++ )
	{
		if ( static_cast<uint8_t>(meeples[ i ]->fieldId()) == from )
		{
			meeples[ i ]->fieldId( static_cast<int32_t>(to) );
			return true;
		}
	}

	std::cout << "There is no meeple on field " << static_cast<int32_t>(from)
			  << "of player " << static_cast<int32_t>(color)
			  << "to set a movement." << std::endl;
	return false;
}

// TODO optimize! do not search in fieldId order for efficiency
bool Playboard::searchForMeeple( uint8_t from, uint8_t& _x, uint8_t& _y )
{
	// Get the tag id from current playboard inner state where the meeple comes from.
	// The new position ( to ) is not set yet
	uint16_t origTagId = getMeepleFromFieldId( static_cast<int32_t>( from ) )->tag();

	for(uint8_t x = 0; x <= 10; x++) {
		std::vector<uint16_t> line(11);
		_XYDrive->moveCarriage(x);
		_rfid->readLine(&line);

		for(uint8_t y = 0; y <= 10; y++)
		{
			if( line[y] == origTagId) {
				_x = x; _y = y;
				return true;
			}
		}
	}

	// not found
	return false;
}

int32_t Playboard::getColorFromFieldId( int32_t fieldId) {
	for(PlayerMapIt pIt = _players.begin(); pIt != _players.end(); ++pIt) {
		MeepleVec& m = pIt->second->meeples();
		for(MeepleVecIt mIt = m.begin(); mIt != m.end(); ++mIt) {
			if ( (*mIt)->fieldId() == fieldId) {
				return pIt->second->color();
			}
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

uint8_t Playboard::getFieldIdFromXY( uint8_t x, uint8_t y) {
	FieldMapIt it = _fields.begin();
	for( ; it != _fields.end() ; ++it ) {
		if( ( it->second->x() == x ) && ( it->second->y() == y ) )
			return it->second->no();
	}
	return 255;
}

void Playboard::ledShow() {
		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::RED);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::RED       , LedStripes::Corner::STEFAN);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::YELLOW);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::YELLOW    , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::RED       , LedStripes::Corner::ELLLA);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::ORANGE);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::ORANGE    , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::YELLOW    , LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::RED       , LedStripes::Corner::MARCEL);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::GREEN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::GREEN     , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::ORANGE    , LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::YELLOW    , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::RED       , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::BLUE);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::BLUE      , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::GREEN     , LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::ORANGE    , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::YELLOW    , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::PINK);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::PINK      , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::BLUE      , LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::GREEN     , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::ORANGE    , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::LIGHT_BLUE);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::LIGHT_BLUE, LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::PINK      , LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::BLUE      , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::GREEN     , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::ON, LedStripes::Color::WHITE);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::WHITE     , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::LIGHT_BLUE, LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::PINK      , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::ON, LedStripes::Color::BLUE      , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::WHITE);
		_ledStripe->set(LedStripes::Mode::BLINK_SLOW, LedStripes::Color::WHITE     , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::BLINK_SLOW, LedStripes::Color::LIGHT_BLUE, LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::BLINK_SLOW, LedStripes::Color::PINK      , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::BLINK_SLOW, LedStripes::Color::BLUE      , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::LIGHT_BLUE);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::PINK);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::BLUE);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::WHITE);
		_ledStripe->set(LedStripes::Mode::BLINK_FAST, LedStripes::Color::WHITE     , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::BLINK_FAST, LedStripes::Color::LIGHT_BLUE, LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::BLINK_FAST, LedStripes::Color::PINK      , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::BLINK_FAST, LedStripes::Color::BLUE      , LedStripes::Corner::MARTINA);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::LIGHT_BLUE);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::PINK);
		sleep(1);

		_ledRing->set(LedRing::Mode::BLINK, LedStripes::Color::BLUE);
		sleep(1);

		_ledRing->set(LedRing::Mode::OFF, LedStripes::Color::WHITE);
		_ledStripe->set(LedStripes::Mode::OFF, LedStripes::Color::WHITE     , LedStripes::Corner::STEFAN);
		_ledStripe->set(LedStripes::Mode::OFF, LedStripes::Color::LIGHT_BLUE, LedStripes::Corner::ELLLA);
		_ledStripe->set(LedStripes::Mode::OFF, LedStripes::Color::PINK      , LedStripes::Corner::MARCEL);
		_ledStripe->set(LedStripes::Mode::OFF, LedStripes::Color::BLUE      , LedStripes::Corner::MARTINA);
}