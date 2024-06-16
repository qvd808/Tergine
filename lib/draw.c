#include "draw.h"
#include <stdlib.h>
#include <curses.h>
// #include <curses.h>
// #include <wchar.h>

void draw_rect(int x, int y, int width, int height) {

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	for (int i = 0; i < height; i++) {
		// move(y + i, x);
		for (int j = 0; j < width; j++) {

			// printw("\xc3\x7f");
			mvaddstr(y + i, x + j, "*");
			// printw(" *");

			if (j + x >= size.ws_col - 1){
				break;
			}
		}
		if (i + y >= size.ws_row - 1){
			break;
		}
	}
}


void draw_right_triangle(int x, int y, int width, int height) {

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < i + 1; j++) {

			// printw("\xc3\x7f");
			if (j == 0 || j == i || i == height - 1) {
				// printw(" *");
				mvaddstr(y + i, x, " *");
			} else {
				// printw(" .");
				// printw("  ");
				mvaddstr(y + i, x, "||");
			}

			if (j + x >= size.ws_col - 1){
				break;
			}
		}

		if (i + y >= size.ws_row - 1){
			break;
		}
	}
}



int draw_line(struct Point start, struct Point end) {
	
	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
		return 0;
	}

	//If end.x - start.x == 0. Vertical line
	
	if (end.x - start.x == 0) {
		for (int i = 0; i < end.y - start.y; i++) {
			mvaddstr(start.y + i, start.x, "*");
		}

		return 1;
	}
	
	if (end.y - start.y == 0) {
		for (int i = 0; i < end.x - start.x; i++) {
			mvaddstr(start.y, start.x + i, "*");
		}
		return 1;
	}

	if (abs(end.y - start.y) < abs(end.x - start.x)) {
		if (start.x > end.x) {
			plotLineLow(end.x, end.y, start.x, start.y);
		} else {
			plotLineLow(start.x, start.y, end.x, end.y);
		}
	} else {

		if (start.y > end.y) {
			plotLineHigh(end.x, end.y, start.x, start.y);
		} else {
			plotLineHigh(start.x, start.y, end.x, end.y);
		}
	}

	return 1;
}


	// Bresenhams's line algorithm or Xiaolin Wu's Line algorithm
void plotLineLow(int x0, int y0, int x1, int y1) {

	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int D = (2 * dy ) - dx;
	int y = y0;
	
	for (int i = 0; i <= x1 - x0; i++) {
		mvaddstr(y, x0 + i, "*");
		if (D > 0) {
			y = y + yi;
			D += (2 * (dy - dx));
		} else {
			D += 2 * dy;
		}
	}
}

void plotLineHigh(int x0, int y0, int x1, int y1) {

	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int D = (2 * dx) - dy;
	int x = x0;
	
	for (int i = 0; i <= y1 - y0; i++) {
		mvaddstr(y0 + i, x, "*");
		if (D > 0) {
			x = x + xi;
			D += (2 * (dx - dy));
		} else {
			D += 2 * dx;
		}
	}
}

