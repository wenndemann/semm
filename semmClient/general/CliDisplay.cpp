/*
 * CliDisplay.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: reinhard
 */

#include "CliDisplay.h"

CliDisplay::CliDisplay(int x, int y, int width, int height, const char* name, int colorPairId){
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height + 1;
	m_win = newwin(m_height ,m_width, m_y ,m_x);
	m_colorPairId = colorPairId;
	m_actPicNumber = 1;
	m_name = name;

	initMenues();

	mvwprintw(m_win, 0, 0 ,"%s", m_name);

	wattron(m_win, COLOR_PAIR(m_colorPairId)| A_STANDOUT);
	box( derwin(m_win, m_height-1, m_width ,1,0),0,0);
	wattroff(m_win, COLOR_PAIR(m_colorPairId)| A_STANDOUT);

	refreshDisplay();
}

CliDisplay::~CliDisplay() {
	// TODO Auto-generated destructor stub
}

void CliDisplay::initMenues() {

	m_pics[1] = new CliPicture("SEMM");
	m_pics[1]->assignDisplay(m_win);

	m_pics[2] = new CliPicture("Teilnahme");
	m_pics[2]->addItem("teilnehmen");
	m_pics[2]->assignDisplay(m_win);

	m_pics[3]= new CliPicture("Starten");
	m_pics[3]->addItem("Spiel beginnen");
	m_pics[3]->addItem("abbrechen");
	m_pics[3]->assignDisplay(m_win);

	m_pics[4] = new CliPicture("Bitte warten...");
	m_pics[4]->assignDisplay(m_win);

	m_pics[5] = new CliPicture("Bitte Würfel");
	m_pics[5]->addItem("OK");
	m_pics[5]->assignDisplay(m_win);

	m_pics[6] = new CliPicture("Würfelzahl 1");
	m_pics[6]->assignDisplay(m_win);

	m_pics[7] = new CliPicture("Würfelzahl 2");
	m_pics[7]->assignDisplay(m_win);

	m_pics[8] = new CliPicture("Würfelzahl 3");
	m_pics[8]->assignDisplay(m_win);

	m_pics[9] = new CliPicture("Würfelzahl 4");
	m_pics[9]->assignDisplay(m_win);

	m_pics[10] = new CliPicture("Würfelzahl 5");
	m_pics[10]->assignDisplay(m_win);

	m_pics[11] = new CliPicture("Würfelzahl 6");
	m_pics[11]->assignDisplay(m_win);

	m_pics[12] = new CliPicture("Spielfigur heraussetzten");
	m_pics[12]->addItem("OK");
	m_pics[12]->assignDisplay(m_win);

	m_pics[15] = new CliPicture("Figur waehlen");
	m_pics[15]->addItem("1");
	m_pics[15]->addItem("2");
	m_pics[15]->addItem("3");
	m_pics[15]->assignDisplay(m_win);

	m_pics[16] = new CliPicture("Spielfigur setzen");
	m_pics[16]->addItem("OK");
	m_pics[16]->assignDisplay(m_win);

	m_pics[20] = new CliPicture("Ungültiger Spielzug");
	m_pics[20]->assignDisplay(m_win);

	m_pics[21] = new CliPicture("Gewonnen");
	m_pics[21]->assignDisplay(m_win);

	m_pics[25] = new CliPicture("Spiel abbrechen");
	m_pics[25]->addItem("OK"); // Habe den OK button eingefügt, ist aber noch nicht entschieden ob das reinkommt in die Bilder
	m_pics[25]->addItem("Nein"); // Habe den Nein button eingefügt, ist aber noch nicht entschieden ob das reinkommt in die Bilder
	m_pics[25]->assignDisplay(m_win);

	m_pics[26] = new CliPicture("Display\n  gesperrt");
	m_pics[26]->assignDisplay(m_win);


}

void CliDisplay::callPicture(int picNumber) {
	m_pics[m_actPicNumber]->unpostPicture(m_win);
	m_actPicNumber = picNumber;
//	wclear(m_win);

	m_pics[m_actPicNumber]->postPicture(m_win);
	mvwprintw(m_win, 0, 0 ,"%s", m_name);
	wattron(m_win, COLOR_PAIR(m_colorPairId) | A_STANDOUT);
	box( derwin(m_win, m_height-1, m_width ,1,0),0,0);
	wattroff(m_win, COLOR_PAIR(m_colorPairId)| A_STANDOUT);

	refreshDisplay();
}

void CliDisplay::sendKey(int key) {
	m_pics[m_actPicNumber]->sendKey(key);
	refreshDisplay();
}



void CliDisplay::refreshDisplay(){
//	refresh();
	wrefresh(m_win);
}

