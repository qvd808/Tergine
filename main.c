#include <signal.h>
// #include <stdlib.h>
#include "draw.h"
#include "setup.h"
#include <ncurses.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int running = 1;
void exit_program_handler(int n);

int main() {

	signal(SIGINT, exit_program_handler);
	init_program();

	int size = 5;
	int num = 0;
	while (running) {
		attrset(COLOR_PAIR(1));
		// draw_right_triangle(0, 0, size, size);
		draw_rect(0, 0, size, size);
		struct Point point1 = {
			.x = 0,
			.y = 0
		};
		struct Point point2 = {
			.x = 3,
			.y = 15 
		};
		draw_line(point1, point2);


		refresh();

		num += 1;
		num %= 36;

		// erase();
		usleep(100000);
	}

	endwin();
	return 0;
}

void exit_program_handler(int n) {
	running = 0;
}
