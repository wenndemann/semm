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
	stateMachine._gui->delMeeples( 4 );

	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );
	stateMachine.process_event( fsm::evInitGame( ) );

	stateMachine._gamePtr->playboard( )->setMeepleMove( 41, 24 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	stateMachine._gamePtr->playboard( )->setMeepleMove( 38, 16 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	stateMachine._gamePtr->playboard( )->setMeepleMove( 35, 8 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

	stateMachine._gamePtr->playboard( )->setMeepleMove( 32, 0 );
	boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );

}

void start_game( const std::string ipaddr, bool guiEnabled )
{
	TcpIp t;
	if ( !t.connect( ipaddr ) )
		return;

	std::cout << "start fsm" << std::endl;

	fsm::gameFSM p( &t, guiEnabled );
	p.start( );

	//test( p );


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

	bool guiEnabled = false;
	if ( argc > 2 && strcmp( argv[ 2 ], "gui" ) == 0 ){ guiEnabled = true; }

	start_game( std::string( argv[ 1 ] ), guiEnabled );

	return 0;
}
