//============================================================================
// Name        : semmClientFsm.cpp
// Author      : Stefan Wenn
// Version     :
// Copyright   : copyleft
// Description : Hello World in C++, Ansi-style
//============================================================================

// global includes
#include <iostream>
#include <string>

// local includes
#include "fsm/mainFsm.h"
#include "fsm/mainFsm/events.h"
#include "game/Game.h"

using namespace std;

void test()
    {
        gameFsm p;

        std::cout << "start fsm" << std::endl;
        p.start();
        p.process_event(initGame()); pstate(p);
        p.process_event(initColors()); pstate(p);
        std::cout << "stop fsm" << std::endl;
        p.stop();
    }

int main(int argc, char *argv[]) {
	cout << "bla" << endl; // prints bla
	test();

	//Game* g = new Game(string(argv[0]));
	//delete g;

	return 0;
}
