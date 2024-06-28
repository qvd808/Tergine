#include "util.h"
#include <math.h>


void cross_product(vec4d *v1, float output[]){

	// Extract and calculate magnitude
	float temp1[3] = {v1[1][0] - v1[0][0], v1[1][1] - v1[0][1], v1[1][2] - v1[0][2] };
	float temp2[3] = {v1[2][0] - v1[0][0], v1[2][1] - v1[0][1], v1[2][2] - v1[0][2] };

	// Calculate the cross product using magnitude
	output[0] = temp1[1] * temp2[2] - temp1[2] * temp2[1];
	output[1] = temp1[2] * temp2[0] - temp1[0] * temp2[2];
	output[2] = temp1[0] * temp2[1] - temp1[1] * temp2[0];
}


void get_normal() {

}
