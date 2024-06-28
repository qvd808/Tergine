#include "util.h"
#include <curses.h>
#include <math.h>


void cross_product(struct Point3d *tri, vec4d *output){

	vec4d temp1 = {0};
	temp1[0] = tri[1].x - tri[0].x;
	temp1[1] = tri[1].y - tri[0].y;
	temp1[2] = tri[1].z - tri[0].z;

	vec4d temp2 = {0};
	temp2[0] = tri[2].x - tri[0].x;
	temp2[1] = tri[2].y - tri[0].y;
	temp2[2] = tri[2].z - tri[0].z;
	

	(*output)[0] = temp1[1] * temp2[2] - temp1[2] * temp2[1];
	(*output)[1] = temp1[2] * temp2[0] - temp1[0] * temp2[2];
	(*output)[2] = temp1[0] * temp2[1] - temp1[1] * temp2[0];

	// printw("%f", temp2[2]);

	float l = sqrtf((*output)[0] * (*output)[0] + (*output)[1] * (*output)[1] + (*output)[2] * (*output)[2]);
	(*output)[0] /= l;
	(*output)[1] /= l;
	(*output)[2] /= l;

}


void get_normal() {

}
