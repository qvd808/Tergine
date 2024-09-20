#include "draw.h"
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "util.h"

void draw_rect(int x, int y, int width, int height) {

	char cBlock = (char)0x2588;

	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
	}

	for (int i = 0; i < height; i++) {
		move(y + i, x);
		for (int j = 0; j < width; j++) {

			// printw("\xc3\x7f");
			mvaddstr(y + i, x + 2*j, "^#");
			// mvaddch(y + i, x + 2*j, cBlock);
			// addch(A_REVERSE);

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

void test(struct Point p1, struct Point p2) {
	// y =  ax + b
	// a = y2 - y1/x2 - x1 => y = (y2 - y1)x/(x2 - x1) + b
	// => (x2 - x1) * y = (y2 - y1) * x + (x2 - x1)* b
	// Let B = -(x2 - x1), A = (y2 - y1), C = (x2 - x1) * b
	// => -By = Ax + C
	// => 0 = Ax + By + C
	// Let's assume p2 is furhter from the origin point than p1
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int prev_x = p1.x;
	int prev_y = p1.y;
	for (int i = 0; i <= dx; i++) {
		float x = prev_x + i;
		float y = prev_y + (dy * i / (float)dx); // Calculate y based on current x
		mvaddstr((int)y, (int)x, "*"); // Ensure you're using integer coordinates
	}
}

struct PointVector* draw_line_test(struct Point p1, struct Point p2) {

	struct PointVector *res = NULL;

	int dx = abs(p2.x - p1.x);
	int dy = abs(p2.y - p1.y);
	int length = MAX(dx, dy) +  1;

	res = malloc(sizeof(struct PointVector));
	if (res == NULL) return NULL;
	res->length = length;
	res->points = malloc(sizeof(struct Point) * length);
	if (res->points == NULL) {
		free(res);
		return NULL;
	};

	int x = p1.x;
	int y = p1.y;
	int err = dx - dy;

    for (int i = 0; i < length; i++) {
        res->points[i] = (struct Point){x, y};

        if (x == p2.x && y == p2.y) break;

		// e2 = 2 * (dx - dy) => 2 * (dx - dy) >= -dy
		// 2dx >= 2dy
		// dx/dy >= 1
        int e2 = 2 * err;
        if (e2 >= -dy) {
            err -= dy;
            x += 1;
        }
		// e2 = 2 * (dx - dy) => 2 * (dx - dy) <= dx
		// dx <= 2*dy
		// dy/dx >= 1/2
        if (e2 <= dx) {
            err += dx;
            y += 1;
        }
    }

	return res;
}

int draw_triangle_fill(struct Point p1, struct Point p2, struct Point p3) {

	test(p1, p2);
	return 1;
}

int draw_triangle(struct Point p1, struct Point p2, struct Point p3) {

	draw_line(p1, p2);
	draw_line(p2, p3);
	draw_line(p3, p1);

	return 1;
}

int draw_line(struct Point start, struct Point end) {
	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
		printf("TIOCGWINSZ error!\n");
		return 0;
	}

	start.x *= 2;
	end.x *= 2;

	// //Draw vertical
	if (end.x - start.x == 0) {
		int dy;
		if (end.y - start.y < 0) {
			dy = -1;
		} else {
			dy = 1;
		}
		for (int i = 0; i < abs(end.y - start.y); i++) {
			mvaddstr(start.y + dy*i, start.x, "*");
		}

		return 1;
	}
	
	//Draw horizontal
	if (end.y - start.y == 0) {
		int dx;
		if (end.x - start.x < 0) {
			dx = -1;
		} else {
			dx = 1;
		}
		for (int i = 0; i < abs(end.x - start.x); i+=2) {
			mvaddstr(start.y, start.x + dx*i , "*");
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

	int dx = (x1 - x0);
	int dy = y1 - y0;
	int yi = 1;
	
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int D = (2 * dy ) - dx;
	int y = y0;
	
	for (int i = 0; i <= (x1 - x0); i+=1) {
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

	int dx = (x1 - x0);
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
			D += ((dx - dy));
		} else {
			D += dx;
		}
	}
}

