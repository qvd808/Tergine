#ifndef _SETUP_H_
#define _SETUP_H_

#pragma once

#include <curses.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>
#include "draw.h"

void init_program(void);
int getWinHeight(void);
int getWinWidth(void);
struct Point translate_coordinate(float x, float y);
float getAspectRatio();

#endif
