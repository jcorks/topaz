/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/

#include <topaz/compat.h>
#include <topaz/vector.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

#ifndef PI_DOUBLE
#define PI_DOUBLE 3.14159265358979323846
#endif

void topaz_vector_reset(topazVector_t * v) {
    v->x = 0.f;
    v->y = 0.f;
    v->z = 0.f;
}

topazVector_t topaz_vector_from_xyz(float x, float y, float z) {
    topazVector_t v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

topazVector_t topaz_vector_from_string(const topazString_t * v) {
    topazString_t * c = topaz_string_clone(v);
    topazString_t * delimiters = topaz_string_create_from_c_str(" \n\t\r,<>(){}[]");

    int i = 0;
    const topazString_t * iter;

    topazVector_t out;
    topaz_vector_reset(&out);

    float * data = &out.x;

    for(iter = topaz_string_chain_start(c, delimiters);
        i < 3 && !topaz_string_chain_is_end(c);
        iter = topaz_string_chain_proceed(c)) {

        data[i] = atof(topaz_string_get_c_str(iter));
    }

    topaz_string_destroy(c);
    topaz_string_destroy(delimiters);
    return out;
}


float topaz_vector_get_length(const topazVector_t * t) {
    return sqrt(t->x*t->x +t->y*t->y + t->z*t->z);
}



float topaz_vector_get_distance(const topazVector_t * a, const topazVector_t * b) {
    return sqrt(pow(b->x - a->x, 2) + pow(b->y - a->y, 2) + pow(b->z - a->z, 2));
}

topazVector_t * topaz_vector_normalize(topazVector_t * t) {
	float l = topaz_vector_get_length(t);
	if (!l) {
        topaz_vector_reset(t);
        return t;
	}
	t->x = t->x / l;
	t->y = t->y / l;
	t->z = t->z / l;
	return t;
}

topazVector_t topaz_vector_cross(const topazVector_t * a, const topazVector_t * b) {
	topazVector_t out;
	out.x = a->y *  b->z - a->z * b->y;
	out.y = a->z *  b->x - a->x * b->z;
	out.z = a->x *  b->y - a->y * b->x;
	return out;
}

float topaz_vector_dot(const topazVector_t * a, const topazVector_t * b) {
    return 
        a->x * b->x + 
        a->y * b->y + 
        a->z * b->z;
}


topazVector_t * topaz_vector_floor(topazVector_t * t) {
    t->x = (int)t->x;
    t->y = (int)t->y;
    t->z = (int)t->z;
    return t;
}




float topaz_vector_rotation_x_diff(const topazVector_t * a, const topazVector_t * b) {
    // If any inputs are undefined in this plane, return 0
    if ((a->y == 0.f && a->z == 0.f) || (b->y == 0.f && b->z == 0.f)) return 0;
	float out = topaz_vector_rotation_x(b) - topaz_vector_rotation_x(a);
	if (out < 0) out += 360.f;
	return out;
}

float topaz_vector_rotation_x_diff_relative(const topazVector_t * a, const topazVector_t * b) {
    topazVector_t o;
    o.x = b->x - a->x;
    o.y = b->y - a->y;
    o.z = b->z - a->z;
	return topaz_vector_rotation_x(&o);
}

float topaz_vector_rotation_x(const topazVector_t * a) {
	float dProduct = (a->y); // reduced dot product based around 0-degree vector {0, 1, 0}
    float lengthPlane_this = 1.f;
    float lengthPlane_in   = sqrt(a->z*a->z + a->y* a->y);
    if (lengthPlane_in == 0.f)
        return 0.f;

    float cAngle = acos(dProduct / (lengthPlane_in * lengthPlane_this));
	if (a->z < 0) cAngle = -cAngle + (PI_DOUBLE * 2.0);
	return (cAngle * (180.0 / PI_DOUBLE));
}






float topaz_vector_rotation_y_diff(const topazVector_t * a, const topazVector_t * b) {
    // If any inputs are undefined in this plane, return 0
    if ((a->x == 0.f && a->z == 0.f) || (b->x == 0.f && b->z == 0.f)) return 0;
	float out = topaz_vector_rotation_y(b) - topaz_vector_rotation_y(a);
	if (out < 0) out += 360.f;
	return out;
}

float topaz_vector_rotation_y_diff_relative(const topazVector_t * a, const topazVector_t * b) {
    topazVector_t o;
    o.x = b->x - a->x;
    o.y = b->y - a->y;
    o.z = b->z - a->z;
	return topaz_vector_rotation_y(&o);
}

float topaz_vector_rotation_y(const topazVector_t * a) {
	float dProduct = (a->z); // reduced dot product based around 0-degree vector {0, 1, 0}
    float lengthPlane_this = 1.f;
    float lengthPlane_in   = sqrt(a->x*a->x + a->z* a->z);
    if (lengthPlane_in == 0.f)
        return 0.f;

    float cAngle = acos(dProduct / (lengthPlane_in * lengthPlane_this));
	if (a->x < 0) cAngle = -cAngle + (PI_DOUBLE * 2.0);
	return (cAngle * (180.0 / PI_DOUBLE));
}




float topaz_vector_rotation_z_diff(const topazVector_t * a, const topazVector_t * b) {
    // If any inputs are undefined in this plane, return 0
    if ((a->x == 0.f && a->y == 0.f) || (b->x == 0.f && b->y == 0.f)) return 0;
	float out = topaz_vector_rotation_z(b) - topaz_vector_rotation_z(a);
	if (out < 0) out += 360.f;
	return out;
}

float topaz_vector_rotation_z_diff_relative(const topazVector_t * a, const topazVector_t * b) {
    topazVector_t o;
    o.x = b->x - a->x;
    o.y = b->y - a->y;
    o.z = b->z - a->z;
	return topaz_vector_rotation_z(&o);
}

float topaz_vector_rotation_z(const topazVector_t * a) {
	float dProduct = (a->x); // reduced dot product based around 0-degree vector {1, 0, 0}
    float lengthPlane_this = 1.f;
    float lengthPlane_in   = sqrt(a->x*a->x + a->y* a->y);
    if (lengthPlane_in == 0.f)
        return 0.f;

    float cAngle = acos(dProduct / (lengthPlane_in * lengthPlane_this));
	if (a->y < 0) cAngle = -cAngle + (PI_DOUBLE * 2.0);
	return (cAngle * (180.0 / PI_DOUBLE));
}



topazVector_t * topaz_vector_rotate_x(topazVector_t * a, float f) {
    float x = a->x;
    float y = a->y;
    float z = a->z;

    a->x = x;
    a->y = y * cos(f * (PI_DOUBLE / 180.0)) - z * sin(f * (PI_DOUBLE / 180.0));
    a->z = z * cos(f * (PI_DOUBLE / 180.0)) + y * sin(f * (PI_DOUBLE / 180.0));

    return a;
}


topazVector_t * topaz_vector_rotate_y(topazVector_t * a, float f) {
    float x = a->x;
    float y = a->y;
    float z = a->z;

    a->x = x * cos(f * (PI_DOUBLE / 180.0)) + z * sin(f * (PI_DOUBLE / 180.0));
    a->y = y;
    a->z = z * cos(f * (PI_DOUBLE / 180.0)) - x * sin(f * (PI_DOUBLE / 180.0));

    return a;
}


topazVector_t * topaz_vector_rotate_z(topazVector_t * a, float f) {
    float x = a->x;
    float y = a->y;
    float z = a->z;

    a->x = x * cos(f * (PI_DOUBLE / 180.0)) - y * sin(f * (PI_DOUBLE / 180.0));
    a->y = y * cos(f * (PI_DOUBLE / 180.0)) + x * sin(f * (PI_DOUBLE / 180.0));
    a->z = z;

    return a;
}







