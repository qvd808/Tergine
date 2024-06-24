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

typedef float vec4d[4];
typedef float mat4x4[4][4];

void MultiplyMatrixVector(vec4d *input, vec4d *output, mat4x4 *m) {
	(*output)[0] = (*input)[0] * (*m)[0][0] + (*input)[1] * (*m)[1][0] + (*input)[2] * (*m)[2][0] + (*m)[3][0];
	(*output)[1] = (*input)[0] * (*m)[0][1] + (*input)[1] * (*m)[1][1] + (*input)[2] * (*m)[2][1] + (*m)[3][1];
	(*output)[2] = (*input)[0] * (*m)[0][2] + (*input)[1] * (*m)[1][2] + (*input)[2] * (*m)[2][2] + (*m)[3][2];
	(*output)[3] = (*input)[0] * (*m)[0][3] + (*input)[1] * (*m)[1][3] + (*input)[2] * (*m)[2][3] + (*m)[3][3];

	if ((*output)[3] !=  0.0f) {
		(*output)[0] /= (*output)[3];
		(*output)[1] /= (*output)[3];
		(*output)[2] /= (*output)[3];
	}
}

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

	mat4x4 matProj = {0};
	matProj[0][0] = fAspectRatio * fFovRad;
	matProj[1][1] = fFovRad;
	matProj[2][2] = fFar / (fFar - fNear);
	matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj[2][3] = 1.0f;
	matProj[3][3] = 0.0f;

	
	
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
