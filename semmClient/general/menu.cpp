#include "menu.h"

MENU *me[MENUES];
ITEM **it[MENU_ENTRIES];

extern WINDOW *winDisplay[4];
extern uint8_t g_actScreen;

void sendKeyToMenu(int key) {
	menu_driver(me[g_actScreen], key);
}

int getActMenuItem() {
	return item_index(current_item(me[g_actScreen]));
}

void refreshMenu() {
	refresh();
	wrefresh(winDisplay[0]);
}

void initMenu() {
	it[0] = (ITEM **) calloc(5, sizeof(ITEM *));
	it[0][0] = new_item("Zahlen", "");
	it[0][1] = new_item("Buchstaben", "");
	it[0][2] = new_item("Namen", "");
	it[0][3] = new_item("Ende", "");
	it[0][4] = 0;
	me[0] = new_menu(it[0]);

	it[1] = (ITEM **) calloc(5, sizeof(ITEM *));
	it[1][0] = new_item("eins", "");
	it[1][1] = new_item("zwei", "");
	it[1][2] = new_item("drei", "");
	it[1][3] = new_item("zurueck", "");
	it[1][4] = 0;
	me[1] = new_menu(it[1]);

	it[2] = (ITEM **) calloc(5, sizeof(ITEM *));
	it[2][0] = new_item("AAA", "");
	it[2][1] = new_item("BBB", "");
	it[2][2] = new_item("CCC", "");
	it[2][3] = new_item("zurueck", "");
	it[2][4] = 0;
	me[2] = new_menu(it[2]);

	it[3] = (ITEM **) calloc(6, sizeof(ITEM *));
	it[3][0] = new_item("Stefan", "");
	it[3][1] = new_item("Ela", "");
	it[3][2] = new_item("Marcel", "");
	it[3][3] = new_item("Martina", "");
	it[3][4] = new_item("zurueck", "");
	it[3][5] = 0;
	me[3] = new_menu(it[3]);

	g_actScreen = 0;
	for (int i = 0; i < MENU_ENTRIES; i++) {
		set_menu_win(me[i], winDisplay[0]);
		set_menu_sub(me[i], derwin(winDisplay[0], 8,18,2,2));
	}
	setScreen(g_actScreen);
}

void setScreen(uint8_t number) {
	unpost_menu(me[g_actScreen]);
	g_actScreen = number;
	wclear(winDisplay[0]);
	post_menu(me[g_actScreen]);
	wattron(winDisplay[0], COLOR_PAIR(1)| A_STANDOUT);
	box(winDisplay[0],0,0);
	wattroff(winDisplay[0], COLOR_PAIR(1)| A_STANDOUT);
	refreshMenu();
}

