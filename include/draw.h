#ifndef _DRAW_H_
#define _DRAW_H_

#include <curses.h>
// #include <ncursesw/ncurses.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>
// #include <unistd.h>

void draw_rect(int x, int y, int width, int height);

#endif
