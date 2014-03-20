//============================================================================
// Name        : semmClientFsm.cpp
// Author      : Stefan Wenn
// Version     :
// Copyright   : copyleft
// Description : Hello World in C++, Ansi-style
//============================================================================

// global includes
#include <iostream>

// local includes
#include "fsm/mainFsm.h"
#include "game/Game.h"

using namespace std;

void test()
    {
        player p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start();
        // go to Open, call on_exit on Empty, then action, then on_entry on Open
        p.process_event(open_close()); pstate(p);
        p.process_event(open_close()); pstate(p);
        // will be rejected, wrong disk type
        p.process_event(
            cd_detected("louie, louie",DISK_DVD)); pstate(p);
        p.process_event(
            cd_detected("louie, louie",DISK_CD)); pstate(p);
        // no need to call play() as the previous event does it in its action method
        p.process_event(play());
        p.process_event(play());

        // at this point, Play is active
        p.process_event(paus()); pstate(p);
        // go back to Playing
        p.process_event(end_pause());  pstate(p);
        p.process_event(paus()); pstate(p);
        p.process_event(stop());  pstate(p);
        // event leading to the same state
        // no action method called as it is not present in the transition table
        p.process_event(stop());  pstate(p);
        std::cout << "stop fsm" << std::endl;
        p.stop();
    }

int main() {
	cout << "bla" << endl; // prints bla
	test();

	Game* g = new Game();
	delete g;

	return 0;
}
