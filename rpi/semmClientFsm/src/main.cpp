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

void test( const std::string ipaddr )
{
	TcpIp t;
	if ( !t.connect( ipaddr ) )
		return;

	std::cout << "start fsm" << std::endl;

	fsm::gameFSM p( &t );
	p.start( );

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

	test( std::string( argv[ 1 ] ) );

	//Game* g = new Game(string(argv[0]));
	//delete g;

	return 0;
}
