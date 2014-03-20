/*
 * CliDisplay.h
 *
 *  Created on: Sep 13, 2013
 *      Author: reinhard
 */

#ifndef CLIDISPLAY_H_
#define CLIDISPLAY_H_

#include <ncurses.h>
#include <menu.h>
#include "../general/CliPicture.h"
#include "../general/cli.h"


class CliDisplay {
public:
	CliDisplay(int x, int y, int width, int height, const char* name, int colorPairId);
	virtual ~CliDisplay();

	void refreshDisplay();
	void callPicture(int picNumber);
	void sendKey(int key);
	int getActMenuItem() { return m_pics[m_actPicNumber]->getActMenuItem(); }
	uint8_t getActPicNumber() { return m_actPicNumber; }

private:
	int m_x, m_y, m_width, m_height, m_colorPairId;
	uint8_t m_actPicNumber;
	const char* m_name;
	WINDOW* m_win;
	CliPicture* m_pics[34];

	void initMenues();
};

#endif /* CLIDISPLAY_H_ */
