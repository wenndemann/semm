/*
 * cli.h
 *
 *  Created on: Sep 11, 2013
 *      Author: reinhard
 */

#ifndef CLI_H_
#define CLI_H_

#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <cstdlib>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <boost/thread/mutex.hpp>


#include "../general/CliDisplay.h"
//

struct field_s;

void initUi();
void drawBigDot(WINDOW* win, int x, int y, int colorPairId);
void drawMeeple(WINDOW* win, int x, int y, int colorPairId, char c);
void drawPlayground(field_s* playground);
void quit();

int debug(const char *fmt, ...);
int printPG(const char *fmt, ...);
int clearPG();

#endif /* CLI_H_ */
