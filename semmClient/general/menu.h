/*
 * menu.h
 *
 *  Created on: Sep 4, 2013
 *      Author: reinhard
 */

#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>
#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>

#include "tools.h"

#define MENUES 10
#define MENU_ENTRIES 10

void sendKeyToMenu(int key);
int getActMenuItem();
void refreshMenu();
void initMenu();
void setScreen(uint8_t number);

#endif /* MENU_H_ */

