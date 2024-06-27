#ifndef __UTIL_H__
#define __UTIL_H__

typedef float vec4d[4];
typedef float mat4x4[4][4];

void cross_product(vec4d *v1, vec4d *v2, vec4d *output);
void get_normal();

#endif
