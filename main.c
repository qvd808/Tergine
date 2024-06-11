#include <signal.h>
#include <unistd.h>
// #include <stdlib.h>
#include <curses.h>
#include <sys/ioctl.h>
#include "draw.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int running = 1;
void exit_program_handler(int n);
void init_program(void);

int main() {

	signal(SIGINT, exit_program_handler);
	init_program();

	int num = 0;
	while (running) {
		attrset(COLOR_PAIR(1));
		draw_rect(num, 33, 3, 3);
		num += 1;
		num %= 36;

		erase();
		usleep(100000);
	}

	endwin();
	return 0;
}

void exit_program_handler(int n) {
	running = 0;
}
void init_program(void) {
	initscr();
	keypad(stdscr, TRUE);
	nonl();
	cbreak();
	echo();
	wtimeout(stdscr, 250);
	curs_set(0);

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
