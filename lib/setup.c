#include "setup.h"
#include <locale.h>

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

struct Point translate_coordinate(float x, float y) {

	int width = getWinWidth();
	int height = getWinHeight();
	int x_new = (width * (x + 1)) / 2;
	int y_new = (height * (y + 1)) / 2;

	struct Point res = {
		.x = x_new,
		.y = y_new,
	};

	return res;
}

float getAspectRatio() {
	float w = (float) getWinWidth();
	float h = (float) getWinHeight();

	return h/w;
}
