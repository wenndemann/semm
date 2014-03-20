/*
 * CliPicture.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: reinhard
 */

#include "CliPicture.h"

CliPicture::CliPicture(const char* name){
	m_name = name;
	m_it = (ITEM **) calloc(5, sizeof(ITEM *));

	count = 0;
	m_it[count] = 0;
	m_me = new_menu(m_it);
}

CliPicture::~CliPicture() {
	// TODO Auto-generated destructor stub
}

void CliPicture::addItem(const char* text) {
	free_item(m_it[count]);
	free_menu(m_me);
	m_it[count++] = new_item(text, "");
	m_it[count] = 0;
	m_me = new_menu(m_it);
}

void CliPicture::assignDisplay(WINDOW* win) {
	set_menu_win(m_me, win);
	set_menu_sub(m_me, derwin(win, 5,17,5,2));
}

void CliPicture::postPicture(WINDOW* win) {

	for(int i = 2; i < 10; i++)
		mvwprintw(win, i, 1 ,"                  ");

	mvwprintw(win, 3, 2 ,"%s", m_name);
	post_menu(m_me);
}

void CliPicture::unpostPicture(WINDOW* win) {
	unpost_menu(m_me);
}

void CliPicture::sendKey(int key) {
	menu_driver(m_me, key);
}
