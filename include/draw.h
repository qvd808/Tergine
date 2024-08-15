#ifndef _DRAW_H_
#define _DRAW_H_

#pragma once

#include <curses.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>


// Define constant
static const char ASCII_CHARS[] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
static const unsigned int ASCII_LEN = 92;
static const float MAX_DEPTH = 30.0f;

struct Point {
	int x;
	int y;
};

void plotLineLow(int x0, int y0, int x1, int y1);
void plotLineHigh(int x0, int y0, int x1, int y1);
void draw_rect(int x, int y, int width, int height);
void draw_right_triangle(int x, int y, int width, int height);
int draw_line(struct Point start, struct Point end);
int draw_triangle(struct Point p1, struct Point p2, struct Point p3);
int draw_triangle_fill(struct Point p1, struct Point p2, struct Point p3);

#endif
