//============================================================================
// Name        : semmClientFsm.cpp
// Author      : Stefan Wenn
// Version     :
// Copyright   : copyleft
// Description : Hello World in C++, Ansi-style
//============================================================================

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30
#define BOOST_MPL_LIMIT_MAP_SIZE 30
//#define FUSION_MAX_VECTOR_SIZE 30

// global includes
#include <iostream>
#include <string>

// local includes
#include "fsm/mainFsm.h"
#include "tcp_ip/TcpIp.h"

using namespace std;

void test( fsm::gameFSM& stateMachine )
{
	uint8_t* buf = new uint8_t[ 10 ];

	stateMachine.process_event( fsm::evInitGame( ) );

	buf[ 0 ] = 16; buf[ 1 ] = 0;
	stateMachine._gamePtr->parseCmd( buf, 2 );
	stateMachine._gamePtr->_clientColors = 16;
	stateMachine.process_event( fsm::evInitColors( ) );
	stateMachine.process_event( fsm::evClientColors( ) );

	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );
	for ( boost::uint32_t i = 0; i < stateMachine._ssms->size( ); i++ )
	{
		if ( stateMachine._ssms->at( i ) )
		{
			stateMachine._ssms->at( i )->process_event( fsm::scmEvGetReady( ) );
		}
	}
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	stateMachine.process_event( fsm::evInitGame( ) );

	uint16_t tag;

	tag = stateMachine._gamePtr->playboard( )->getMeepleFromFieldId( 41 )->tag( );
	stateMachine._gui->setMeeplePos( 1, tag, 24 );
	stateMachine._gamePtr->playboard( )->setMeepleMove( 1, 41, 24 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	tag = stateMachine._gamePtr->playboard( )->getMeepleFromFieldId( 38 )->tag( );
	stateMachine._gui->setMeeplePos( 2, tag, 16 );
	stateMachine._gamePtr->playboard( )->setMeepleMove( 2, 38, 16 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	tag = stateMachine._gamePtr->playboard( )->getMeepleFromFieldId( 35 )->tag( );
	stateMachine._gui->setMeeplePos( 4, tag, 8 );
	stateMachine._gamePtr->playboard( )->setMeepleMove( 4, 35, 8 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	tag = stateMachine._gamePtr->playboard( )->getMeepleFromFieldId( 32 )->tag( );
	stateMachine._gui->setMeeplePos( 8, tag, 0 );
	stateMachine._gamePtr->playboard( )->setMeepleMove( 8, 32, 0 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

}

void start_game( const std::string ipaddr )
{
	TcpIp t;
	//if ( !t.connect( ipaddr ) )
	//	return;

	std::cout << "start fsm" << std::endl;

	fsm::gameFSM p( &t );
	p.start( );

	test( p );


	while( 1 )
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds( 50 ) );
	}

	//std::cout << "stop fsm" << std::endl;
	//p.stop();
}

int main(int argc, char *argv[])
{
	if ( argc < 2 )
	{
		std::cout << "Not enough arguments!" << std::endl;
		return 1;
	}

	start_game( std::string( argv[ 1 ] ) );

	return 0;
}
