/*
 * CliPicture.h
 *
 *  Created on: Sep 13, 2013
 *      Author: reinhard
 */

#ifndef CLIPICTURE_H_
#define CLIPICTURE_H_

#include <menu.h>
#include <cstdlib>

class CliPicture {
public:
	CliPicture(const char* name);
	virtual ~CliPicture();

	void addItem(const char* text);
	void assignDisplay(WINDOW* win);
	void postPicture(WINDOW* win);
	void unpostPicture(WINDOW* win);
	void sendKey(int key);
	int getActMenuItem() { return item_index(current_item(m_me)) + 1; }

	int count;

private:
	const char* m_name;
	MENU *m_me;
	ITEM **m_it;

};

#endif /* CLIPICTURE_H_ */
