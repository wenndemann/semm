/*
 * cli.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: reinhard
 */

#include "cli.h"
#include "../src/Game.h"

WINDOW *winMain;
//WINDOW *winTopLeft;
//WINDOW *winTopRight;
WINDOW *winConsole;

WINDOW* winDisplay[4];
WINDOW* winPlayground;
extern CliDisplay* g_cliDisplay[4];

//pthread_mutex_t mutexCli = PTHREAD_MUTEX_INITIALIZER;
boost::mutex mutexCli;


void initUi() {
//	debug("initUi\n");

	atexit(quit);
	winMain = initscr();

	wresize(winMain, 75,200);
	box(winMain,0,0);

	start_color();

	init_pair(8, COLOR_WHITE, COLOR_BLACK);   // Stefan
	init_pair(4, COLOR_CYAN, COLOR_BLACK);    // Ela
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK); // Martina
	init_pair(1, COLOR_BLUE, COLOR_BLACK);    // Marcel
	init_pair(5, COLOR_BLACK, COLOR_WHITE); // console
	init_pair(6, COLOR_YELLOW, COLOR_BLACK); // console


	curs_set(0);
	noecho();
	cbreak();
	keypad(stdscr, 1);

	// prepare winMain
//	pthread_mutex_lock(&mutexCli);
	mutexCli.lock();
	mvprintw( 4,8,"   SSSSSSSSSSSSSSS  EEEEEEEEEEEEEEEEEEEEEE MMMMMMMM               MMMMMMMM MMMMMMMM               MMMMMMMM      222222222222222          000000000        1111111           444444444  ");
	mvprintw( 5,8," SS:::::::::::::::S E::::::::::::::::::::E M:::::::M             M:::::::M M:::::::M             M:::::::M     2:::::::::::::::22      00:::::::::00     1::::::1          4::::::::4  ");
	mvprintw( 6,8,"S:::::SSSSSS::::::S E::::::::::::::::::::E M::::::::M           M::::::::M M::::::::M           M::::::::M     2::::::222222:::::2   00:::::::::::::00  1:::::::1         4:::::::::4  ");
	mvprintw( 7,8,"S:::::S     SSSSSSS EE::::::EEEEEEEEE::::E M:::::::::M         M:::::::::M M:::::::::M         M:::::::::M     2222222     2:::::2  0:::::::000:::::::0 111:::::1        4::::44::::4  ");
	mvprintw( 8,8,"S:::::S               E:::::E       EEEEEE M::::::::::M       M::::::::::M M::::::::::M       M::::::::::M                 2:::::2  0::::::0   0::::::0    1::::1       4::::4 4::::4  ");
	mvprintw( 9,8,"S:::::S               E:::::E              M:::::::::::M     M:::::::::::M M:::::::::::M     M:::::::::::M                 2:::::2  0:::::0     0:::::0    1::::1      4::::4  4::::4  ");
	mvprintw(10,8," S::::SSSS            E::::::EEEEEEEEEE    M:::::::M::::M   M::::M:::::::M M:::::::M::::M   M::::M:::::::M              2222::::2   0:::::0     0:::::0    1::::1     4::::4   4::::4  ");
	mvprintw(11,8,"  SS::::::SSSSS       E:::::::::::::::E    M::::::M M::::M M::::M M::::::M M::::::M M::::M M::::M M::::::M         22222::::::22    0:::::0 000 0:::::0    1::::l    4::::444444::::444");
	mvprintw(12,8,"    SSS::::::::SS     E:::::::::::::::E    M::::::M  M::::M::::M  M::::::M M::::::M  M::::M::::M  M::::::M       22::::::::222      0:::::0 000 0:::::0    1::::l    4::::::::::::::::4");
	mvprintw(13,8,"       SSSSSS::::S    E::::::EEEEEEEEEE    M::::::M   M:::::::M   M::::::M M::::::M   M:::::::M   M::::::M      2:::::22222         0:::::0     0:::::0    1::::l    4444444444:::::444");
	mvprintw(14,8,"            S:::::S   E:::::E              M::::::M    M:::::M    M::::::M M::::::M    M:::::M    M::::::M     2:::::2              0:::::0     0:::::0    1::::l              4::::4  ");
	mvprintw(15,8,"            S:::::S   E:::::E       EEEEEE M::::::M     MMMMM     M::::::M M::::::M     MMMMM     M::::::M     2:::::2              0::::::0   0::::::0    1::::l              4::::4  ");
	mvprintw(16,8,"SSSSSSS     S:::::S EE::::::EEEEEEEE:::::E M::::::M               M::::::M M::::::M               M::::::M     2:::::2       222222 0:::::::000:::::::0 111::::::111           4::::4  ");
	mvprintw(17,8,"S::::::SSSSSS:::::S E::::::::::::::::::::E M::::::M               M::::::M M::::::M               M::::::M     2::::::2222222:::::2  00:::::::::::::00  1::::::::::1         44::::::44");
	mvprintw(18,8,"S:::::::::::::::SS  E::::::::::::::::::::E M::::::M               M::::::M M::::::M               M::::::M     2::::::::::::::::::2    00:::::::::00    1::::::::::1         4::::::::4");
	mvprintw(19,8," SSSSSSSSSSSSSSS    EEEEEEEEEEEEEEEEEEEEEE MMMMMMMM               MMMMMMMM MMMMMMMM               MMMMMMMM     22222222222222222222      000000000      111111111111         4444444444");

	mvprintw(65,40, "                  CONTROL KEYS            ");
	mvprintw(66,40, "                                          ");
	mvprintw(67,40, "         Stefan   Ela   Martina   Marcel  ");
	mvprintw(68,40, "     up    Q       W       E        R     ");
	mvprintw(69,40, "  enter    A       S       D        F     ");
	mvprintw(70,40, "   down    Y       X       C        V     ");

	refresh();
	winConsole = newwin(30,71,20,125);
	wbkgd(winConsole, COLOR_PAIR(5)|A_DIM );
	wrefresh(winConsole);
	scrollok(winConsole, TRUE);
	mutexCli.unlock();


	winPlayground = newwin(37,69,25,25);
	g_cliDisplay[3] = new CliDisplay(4,26,20,10,"Stefan", 8);
	g_cliDisplay[2] = new CliDisplay(95,26,20,10,"Ela", 4);
	g_cliDisplay[1] = new CliDisplay(95,50,20,10,"Martina", 2);
	g_cliDisplay[0] = new CliDisplay(4,50,20,10,"Marcel", 1);

//	debug("new Displays done\n");




}

void drawBigDot(WINDOW* win, int x, int y, int colorPairId){
//	pthread_mutex_lock(&mutexPrintPG);
	wattron(win, COLOR_PAIR(colorPairId));
	mvwprintw(win, y-1, x-2, " --- ");
	mvwprintw(win, y  , x-2, "|   |");
	mvwprintw(win, y+1, x-2, " --- ");
	wattroff(win, COLOR_PAIR(colorPairId));
//	pthread_mutex_unlock(&mutexPrintPG);
}

void drawMeeple(WINDOW* win, int x, int y, int colorPairId){
//	pthread_mutex_lock(&mutexPrintPG);
	wattron(win, COLOR_PAIR(colorPairId));
	mvwprintw(win, y  , x, "X");
	wattroff(win, COLOR_PAIR(colorPairId));
//	pthread_mutex_unlock(&mutexPrintPG);
}


void drawPlayground(field_s* playground) {
	mutexCli.lock();

	//print playground
	//Stefan
	drawBigDot(winPlayground,10, 3,8);
	drawBigDot(winPlayground, 4, 6,8);
	drawBigDot(winPlayground,10, 6,8);
	drawBigDot(winPlayground,10,15,8);
	drawBigDot(winPlayground,16,18,8);
	drawBigDot(winPlayground,22,18,8);
	drawBigDot(winPlayground,28,18,8);

	//Ela
	drawBigDot(winPlayground,58, 3,4);
	drawBigDot(winPlayground,58, 6,4);
	drawBigDot(winPlayground,64, 6,4);
	drawBigDot(winPlayground,40, 6,4);
	drawBigDot(winPlayground,34, 9,4);
	drawBigDot(winPlayground,34,12,4);
	drawBigDot(winPlayground,34,15,4);

	//Martina
	drawBigDot(winPlayground,64,30,2);
	drawBigDot(winPlayground,58,30,2);
	drawBigDot(winPlayground,58,33,2);
	drawBigDot(winPlayground,58,21,2);
	drawBigDot(winPlayground,52,18,2);
	drawBigDot(winPlayground,46,18,2);
	drawBigDot(winPlayground,40,18,2);

	//Marcel
	drawBigDot(winPlayground,10,33,1);
	drawBigDot(winPlayground,10,30,1);
	drawBigDot(winPlayground, 4,30,1);
	drawBigDot(winPlayground,28,30,1);
	drawBigDot(winPlayground,34,27,1);
	drawBigDot(winPlayground,34,24,1);
	drawBigDot(winPlayground,34,21,1);

	//rest
	drawBigDot(winPlayground,16,15,6);
	drawBigDot(winPlayground,22,15,6);
	drawBigDot(winPlayground,28,15,6);
	drawBigDot(winPlayground,28,12,6);
	drawBigDot(winPlayground,28, 9,6);
	drawBigDot(winPlayground,28, 6,6);
	drawBigDot(winPlayground,34, 6,6);
	drawBigDot(winPlayground,40, 9,6);
	drawBigDot(winPlayground,40,12,6);
	drawBigDot(winPlayground,40,15,6);
	drawBigDot(winPlayground,46,15,6);
	drawBigDot(winPlayground,52,15,6);
	drawBigDot(winPlayground,58,15,6);
	drawBigDot(winPlayground,58,18,6);
	drawBigDot(winPlayground,52,21,6);
	drawBigDot(winPlayground,46,21,6);
	drawBigDot(winPlayground,40,21,6);
	drawBigDot(winPlayground,40,24,6);
	drawBigDot(winPlayground,40,27,6);
	drawBigDot(winPlayground,40,30,6);
	drawBigDot(winPlayground,34,30,6);
	drawBigDot(winPlayground,28,27,6);
	drawBigDot(winPlayground,28,24,6);
	drawBigDot(winPlayground,28,21,6);
	drawBigDot(winPlayground,28,21,6);
	drawBigDot(winPlayground,22,21,6);
	drawBigDot(winPlayground,16,21,6);
	drawBigDot(winPlayground,10,21,6);
	drawBigDot(winPlayground,10,18,6);

	for(int i = 0; i < 96; i++){
		if(playground[i].figure != NULL) {
			drawMeeple(winPlayground, (playground[i].x * 6) + 4, (playground[i].y * 3) + 3, playground[i].figure->color);
		}
	}

	box(winPlayground,0,0);
	wrefresh(winPlayground);
	mutexCli.unlock();
}



void quit() {
	endwin();
	std::cout << "exit" << std::endl;
}

int debug(const char *fmt, ...) {
	mutexCli.lock();

	int ret, y;

	y = getmaxy(winConsole);
	va_list list;
	va_start(list, fmt);

	wmove(winConsole, y - 1, 1);
	ret = vwprintw(winConsole, fmt, list);
	wrefresh(winConsole);
	fflush(stdout);
	mutexCli.unlock();
	return ret;
}

int printPG(const char *fmt, ...) {
	return 0;
}

int clearPG() {
	return 0;
}

