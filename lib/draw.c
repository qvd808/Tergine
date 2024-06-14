#include "draw.h"
#include <curses.h>
// #include <curses.h>
// #include <wchar.h>

void draw_rect(int x, int y, int width, int height) {

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	for (int i = 0; i < height; i++) {
		move(y + i, x);
		for (int j = 0; j < width; j++) {

			// printw("\xc3\x7f");
			printw(" *");

			if (j + x >= size.ws_col - 1){
				break;
			}
		}
		if (i + y >= size.ws_row - 1){
			break;
		}
	}
	refresh();
}


void draw_right_triangle(int x, int y, int width, int height) {

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	for (int i = 0; i < height; i++) {
		move(y + i, x);
		for (int j = 0; j < i + 1; j++) {

			// printw("\xc3\x7f");
			if (j == 0 || j == i || i == height - 1) {
				printw(" *");
			} else {
				// printw(" .");
				printw("  ");
			}

			if (j + x >= size.ws_col - 1){
				break;
			}
		}

		if (i + y >= size.ws_row - 1){
			break;
		}
	}
	refresh();
}

