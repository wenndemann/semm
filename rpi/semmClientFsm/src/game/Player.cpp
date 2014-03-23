/*
 * Player.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: reinhard
 */

#include "Player.h"

#include <assert.h>

Player::Player(int32_t colorId)
: _color(colorId)
{
	switch (_color) {
	case 8:
		_startAreaPos.push_back(32);
		_startAreaPos.push_back(33);
		_startAreaPos.push_back(34);
		break;

	case 4:
		_startAreaPos.push_back(35);
		_startAreaPos.push_back(36);
		_startAreaPos.push_back(37);
		break;

	case 2:
		_startAreaPos.push_back(38);
		_startAreaPos.push_back(39);
		_startAreaPos.push_back(40);
		break;

	case 1:
		_startAreaPos.push_back(41);
		_startAreaPos.push_back(42);
		_startAreaPos.push_back(43);
		break;
	}

}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::addMeeples( std::vector< uint16_t > tags )
{
	assert( tags.size( ) == 3 );

	_meeples.clear( );
	for ( uint32_t i = 0; i < tags.size( ); i++ )
	{
		_meeples.push_back( MeeplePtr( new Meeple( tags[ i ], _startAreaPos[ i ] ) ) );
	}
}

int32_t Player::getStartAreaPos(int32_t i) {
	if(i <= static_cast<int32_t>(_startAreaPos.size()))
		return _startAreaPos[i];
	return -1;
}

