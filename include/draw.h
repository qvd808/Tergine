#ifndef _DRAW_H_
#define _DRAW_H_

#include <curses.h>
// #include <ncursesw/ncurses.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>
// #include <unistd.h>

struct Point {
	int x;
	int y;
};

void draw_rect(int x, int y, int width, int height);
void draw_right_triangle(int x, int y, int width, int height);
void draw_line(struct Point start, struct Point end);

#endif
