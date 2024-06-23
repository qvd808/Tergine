
#ifndef _SETUP_H_
#define _SETUP_H_

#include <curses.h>
// #include <ncursesw/ncurses.h>
#include <unistd.h>
#include <curses.h>
#include <locale.h>
#include <sys/ioctl.h>
#include "draw.h"
// #include <unistd.h>

void init_program(void);
int getWinHeight(void);
int getWinWidth(void);
struct Point translate_coordinate(float x, float y);

#endif
