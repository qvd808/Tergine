#include <math.h>
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
	struct Point3d p[8];
};

struct mat4x4 {
	float m[4][4];
};

int main() {

	signal(SIGINT, exit_program_handler);
	init_program();

	// Initialize Points
	struct Point3d p1 = {
		.x = 0,
		.y = 0,
		.z = 0
	};
	struct Point3d p2 = {
		.x = 1,
		.y = 0,
		.z = 0
	};
	struct Point3d p3 = {
		.x = 0,
		.y = 1,
		.z = 0
	};
	struct Point3d p4 = {
		.x = 0,
		.y = 0,
		.z = 1
	};
	struct Point3d p5 = {
		.x = 1,
		.y = 0,
		.z = 1
	};
	struct Point3d p6 = {
		.x = 1,
		.y = 1,
		.z = 0
	};
	struct Point3d p7 = {
		.x = 1,
		.y = 1,
		.z = 1
	};
	struct Point3d p8 = {
		.x = 0,
		.y = 1,
		.z = 1
	};

	//Initialize the cube in space
	struct Cube cube;
	cube.p[0] = p1;
	cube.p[1] = p2;
	cube.p[2] = p3;
	cube.p[3] = p4;
	cube.p[4] = p5;
	cube.p[5] = p6;
	cube.p[6] = p7;
	cube.p[7] = p8;


	// Projection Matrix
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = getAspectRatio();
	float fFovRad = 1.0f / tan(fFov * 0.5 / 180.0f * 3.14159f);
	
	while (running) {
		attrset(COLOR_PAIR(1));

		struct Point p1 = translate_coordinate(cube.p[0].x, cube.p[0].y);
		struct Point p2 = translate_coordinate(cube.p[1].x, cube.p[1].y);

		draw_line(p1, p2);

		refresh();
		// erase();
		usleep(100000);
	}

	endwin();
	return 0;
}

void exit_program_handler(int n) {
	running = 0;
}
