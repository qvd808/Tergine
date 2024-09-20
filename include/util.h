#ifndef __UTIL_H__
#define __UTIL_H__

#pragma once

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

struct Point3d {
	float x, y, z;
};

typedef float vec4d[4];
typedef float mat4x4[4][4];

void cross_product(struct Point3d *v1, vec4d*output);
// void get_normal();

#endif
