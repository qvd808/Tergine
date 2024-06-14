#include <signal.h>
// #include <stdlib.h>
#include "draw.h"
#include "setup.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int running = 1;
void exit_program_handler(int n);

int main() {

	signal(SIGINT, exit_program_handler);
	init_program();

	int size = 10;
	int num = 0;
	while (running) {
		attrset(COLOR_PAIR(1));
		draw_triangle(0, 0, size, size);
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
