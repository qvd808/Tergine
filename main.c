#include <signal.h>
// #include <stdlib.h>
#include "draw.h"
#include "setup.h"
#include <ncurses.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int running = 1;
void exit_program_handler(int n);

struct Point3d {
	float x;
	float y;
	float z;
};

struct Cube {
	struct Point3d p1;
	struct Point3d p2;
	struct Point3d p3;
	struct Point3d p4;
	struct Point3d p5;
	struct Point3d p6;
};


int main() {

	signal(SIGINT, exit_program_handler);
	init_program();

	int size = 10;
	int num = 0;
	while (running) {
		attrset(COLOR_PAIR(1));
		// draw_right_triangle(0, 0, size, size);
		// draw_rect(0, 0, size, size);
		struct Point p1 = {
			.x = 8,
			.y = 8
		};
		struct Point p2 = {
			.x = 0,
			.y = 0
		};
		struct Point p3 = {
			.x = 0,
			.y = 8
		};
		draw_triangle(p1, p2, p3);
		p1.x = 9;
		p1.y = 0;
		p2.x = 9;
		p2.y = 9;
		p3.x = 18;
		p3.y = 9;

		draw_triangle(p1, p2, p3);

		// struct Point p1 = {
		// 	.x = 8,
		// 	.y = 8
		// };
		// struct Point p2 = {
		// 	.x = 0,
		// 	.y = 0
		// };
		// struct Point p3 = {
		// 	.x = 0,
		// 	.y = 8
		// };
		// draw_line(p2,p3);
		// draw_line(p1,p3);
		// attrset(COLOR_PAIR(2));
		// draw_line(p3,p1);
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
