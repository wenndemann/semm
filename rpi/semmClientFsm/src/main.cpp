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

// global includes
#include <iostream>
#include <string>

// local includes
#include "fsm/mainFsm.h"

using namespace std;

void test()
{
	//fsm::SsmsVecPtr ssms( new fsm::SsmsVec( {
	//		boost::shared_ptr< fsm::selectColorFSM >( new fsm::selectColorFSM( 1 ) ),
	//		boost::shared_ptr< fsm::selectColorFSM >( new fsm::selectColorFSM( 2 ) ),
	//		boost::shared_ptr< fsm::selectColorFSM >( new fsm::selectColorFSM( 4 ) ),
	//		boost::shared_ptr< fsm::selectColorFSM >( new fsm::selectColorFSM( 8 ) )
	//} ) );

	fsm::gameFSM p;

	std::cout << "start fsm" << std::endl;
	p.start();
	p.process_event(fsm::initGame());
	p.process_event(fsm::initColors());
	p.process_event(fsm::clientColors());

	while( 1 )
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds( 50 ) );
	}

	//std::cout << "stop fsm" << std::endl;
	//p.stop();
}

int main(int argc, char *argv[])
{
	test();

	//Game* g = new Game(string(argv[0]));
	//delete g;

	return 0;
}
