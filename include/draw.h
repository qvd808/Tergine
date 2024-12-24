#ifndef _DRAW_H_
#define _DRAW_H_

#pragma once

#include <curses.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Define constant
static const unsigned int ASCII_CHARS_LEN = 42;
static const char ASCII_CHARS[] = "`.-':_,^=;>+!rc*ya]2ESwqkP6h94VGUm8RD#Q%&@";
static const char ASCII_CHARS_FILL[] = "AKXHm8RD#$Bg0MNWQ%&@";
static const unsigned int ASCII_FILL_LEN = 20;
static const char ASCII_CHARS_TOP[] = "'-^=<>+*/T7FC][EZ";
static const unsigned int ASCII_TOP_LEN = 17;
static const char ASCII_CHARS_BOTTOM[] = "_,;rcfunypqg";
static const unsigned int ASCII_BOTTOM_LEN = 12;
static const float MAX_DEPTH = 30.0f;

struct Point {
  int x;
  int y;
};

struct PointVector {
  struct Point *points;
  int length;
};

void plotLineLow(int x0, int y0, int x1, int y1);
void plotLineHigh(int x0, int y0, int x1, int y1);
void draw_rect(int x, int y, int width, int height);
void draw_right_triangle(int x, int y, int width, int height);
int draw_line(struct Point start, struct Point end);
struct PointVector *draw_line_test(struct Point start, struct Point end);
int draw_triangle(struct Point p1, struct Point p2, struct Point p3);
int draw_triangle_fill(struct Point p1, struct Point p2, struct Point p3);
void drawWuLine(struct Point p1, struct Point p2);
void drawWuTriangle(struct Point p1, struct Point p2, struct Point p3);
void plotPixel(int x, int y, float brightness);
void rasterizeTriangle(struct Point p0, struct Point p1, struct Point p2);

#endif
