#include <curses.h>
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
	float x, y, z;
};

struct Triangle{
	struct Point3d vec[3];
};

struct Cube {
	struct Triangle mesh[12];
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

	//Initialize the cube in space
	struct Cube cube;
	// SOUTH
	struct Point3d tri1[3] = {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}};
	struct Point3d tri2[3] = {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};

	// EAST
	struct Point3d tri3[3] = {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
	struct Point3d tri4[3] = {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}};

	//NORTH
	struct Point3d tri5[3] = {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}};
	struct Point3d tri6[3] = {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};

	// WEST
	struct Point3d tri7[3] = {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};
	struct Point3d tri8[3] = {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

	// TOP
	struct Point3d tri9[3] = {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};
	struct Point3d tri10[3] = {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}};

	//BOTTOM
	struct Point3d tri11[3] = {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
	struct Point3d tri12[3] = {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};

	struct Point3d *list_points[12];
	list_points[0] = tri1;
	list_points[1] = tri2;
	list_points[2] = tri3;
	list_points[3] = tri4;
	list_points[4] = tri5;
	list_points[5] = tri6;
	list_points[6] = tri7;
	list_points[7] = tri8;
	list_points[8] = tri9;
	list_points[9] = tri10;
	list_points[10] = tri11;
	list_points[11] = tri12;

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

		for (int i = 0; i < 12; i++) {

			vec4d triProjected[3];

			for (int j = 0; j < 3; j++) {

				vec4d temp = {0};
				temp[0] = list_points[i][j].x;
				temp[1] = list_points[i][j].y;
				temp[2] = list_points[i][j].z;

				MultiplyMatrixVector(&temp, &triProjected[j], &matProj);
			}

			struct Point projected_p1 = translate_coordinate(triProjected[0][0], triProjected[0][1]);
			struct Point projected_p2 = translate_coordinate(triProjected[1][0], triProjected[1][1]);
			struct Point projected_p3 = translate_coordinate(triProjected[2][0], triProjected[2][1]);

			draw_triangle(projected_p1, projected_p2, projected_p3);

		}

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
