#include "setup.h"

void init_program(void) {
	initscr();
	keypad(stdscr, TRUE);
	nonl();
	cbreak();
	echo();
	wtimeout(stdscr, 250);
	curs_set(0);
	setlocale(LC_ALL, "");

	if (has_colors()) {
		start_color();

		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);
		init_pair(4, COLOR_BLUE, COLOR_BLACK);
		init_pair(5, COLOR_CYAN, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(7, COLOR_WHITE, COLOR_BLACK);
	}
}

int getWinHeight(void) {

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	return size.ws_row;
}

int getWinWidth(void) {

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	return size.ws_col / 2;
}
