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
#include <topaz/matrix.h>

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



topazVector_t topaz_vector_reflect_2d(
    /// The initial direction to reflect
    topazVector_t * direction,
    
    /// The surface to reflect from.
    topazVector_t * surface
) {
    topazVector_t out;
    topazVector_t normal = topaz_vector_cross(
        direction,
        surface
    );    
    
    topaz_vector_normalize(&normal);
    float dn = topaz_vector_dot(direction, &normal);
    
    out.x = direction->x - 2*dn*normal.x;
    out.y = direction->y - 2*dn*normal.y;
    return out;
}



// adapted from GluLookAt source
void glhLookAtf2(
    topazMatrix_t * matrix, 
    const topazVector_t * eyePosition3D,              
    const topazVector_t * center3D,
    const topazVector_t * upVector3D
) {
    topazVector_t forward, side, up;
    topazMatrix_t matrix2;
    topazMatrix_t resultMatrix;
    // --------------------
    forward.x = center3D->x - eyePosition3D->x;
    forward.y = center3D->y - eyePosition3D->y;
    forward.z = center3D->z - eyePosition3D->z;
    topaz_vector_normalize(&forward);
    // --------------------
    // Side = forward x up
    side = topaz_vector_cross(&forward, upVector3D);
    topaz_vector_normalize(&side);
    // --------------------
    // Recompute up as: up = side x forward
    up = topaz_vector_cross(&side, &forward);
    // --------------------
    matrix2.data[0] = side.x;
    matrix2.data[1] = side.y;
    matrix2.data[2] = side.z;
    matrix2.data[3] = 0.0;
    // --------------------
    matrix2.data[4] = up.x;
    matrix2.data[5] = up.y;
    matrix2.data[6] = up.z;
    matrix2.data[7] = 0.0;
    // --------------------
    matrix2.data[8] = -forward.x;
    matrix2.data[9] = -forward.y;
    matrix2.data[10] = -forward.z;
    matrix2.data[11] = 0.0;
    // --------------------
    matrix2.data[12] = matrix2.data[13] = matrix2.data[14] = 0.0;
    matrix2.data[15] = 1.0;
    // --------------------
    resultMatrix = topaz_matrix_multiply(matrix, &matrix2);
    topaz_matrix_translate(&resultMatrix,
        -eyePosition3D->x, 
        -eyePosition3D->y, 
        -eyePosition3D->z
    );
    // --------------------
    memcpy(matrix->data, resultMatrix.data, 16*sizeof(float));
}

topazVector_t topaz_vector_look_at_rotation(
    const topazVector_t * p0,
    const topazVector_t * p1,
    const topazVector_t * up
) {
    topazVector_t upReal = *up;
    topaz_vector_normalize(&upReal);
    topazMatrix_t m;
    topaz_matrix_set_identity(&m);
    glhLookAtf2(
        &m, 
        p0,
        p1,
        &upReal
    );
    
    
    // psi (yx) -> x
    // theta -> y 
    // phi (o/)-> z
    float psi, theta, phi;
    if (fabs(m.data[8]) != 1) {
        theta = -asin(m.data[8]); float ct = cos(theta);
        psi   = atan2(m.data[9] / ct, m.data[10] / ct);
        phi   = atan2(m.data[4] / ct, m.data[0]  / ct); 
    } else {
        phi   = 0;
        if (m.data[8] == -1) {
            theta = PI_DOUBLE / 2;
            psi   = atan2(m.data[1], m.data[2]);
        } else {
            theta = -PI_DOUBLE / 2;
            psi   = atan2(-m.data[1], -m.data[2]);
        }
    }
    topazVector_t rotation = {
        psi*(180/M_PI), theta*(180/M_PI), phi*(180/M_PI)    
    };
    printf("%f %f %f\n", rotation.x, rotation.y, rotation.z);
    return rotation;
}


float topaz_vector_point_at_2d(
    const topazVector_t * p0,
    const topazVector_t * p1
) {
    topazVector_t a = {
        p1->x - p0->x,
        p1->y - p0->y,
        0
    };
	float dProduct = (a.x); // reduced dot product based around 0-degree vector {1, 0, 0}
    float lengthPlane_this = 1.f;
    float lengthPlane_in   = sqrt(a.x*a.x + a.y* a.y);
    if (lengthPlane_in == 0.f)
        return 0.f;

    float cAngle = acos(dProduct / (lengthPlane_in * lengthPlane_this));
	if (a.y < 0) cAngle = -cAngle + (M_PI * 2.0);
	return (cAngle * (180.0 / M_PI));

}










