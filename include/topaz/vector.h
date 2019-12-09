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


#ifndef H_TOPAZDC__VECTOR__INCLUDED
#define H_TOPAZDC__VECTOR__INCLUDED

#include <topaz/containers/string.h>

/*

    Vector
    -----
    
    A hybrid class suitable for both 2D and 3D
    positional and physical operations.


*/
typedef struct {float x; float y; float z;} topazVector_t;


/// Initializes x, y, and z to 0.
///
void topaz_vector_reset(topazVector_t *);

/// Shorthand initializer for x, y, and z.
///
topazVector_t topaz_vector_from_xyz(float x, float y, float z);

/// Attempts to retrieve coordinates from formatted text.
///
/// THe format of the text is 2 to 3 floating point numbers separated by
/// any counts of either whitespace or generally accepted separation characters:
///    ,.<>(){}[]
///
/// The point here is to allow instantiation from natural mathematical notations
/// which depend on the usage of the Vector. Examples of valid input:
///
/// Vector("{1, 4, 5}");
/// Vector("1 4 5");
/// Vector("<1 4 5>");
/// Vector("[1 4 5]");
///
topazVector_t topaz_vector_from_string(const topazString_t *);

/// Returns the positional X Y Z of the vector
///
#define topaz_vec_x(__V__) (((float*)(__V__))[0])
#define topaz_vec_y(__V__) (((float*)(__V__))[1])
#define topaz_vec_z(__V__) (((float*)(__V__))[2])



/// Returns the length of the vector.
///
float topaz_vector_get_length(const topazVector_t *);

/// Returns the distance of this vector to the other vector.
///
float topaz_vector_get_distance(const topazVector_t * a, const topazVector_t * b);

/// Sets the vector to a magnitude of 1. The source vector is returned it.
///
topazVector_t * topaz_vector_normalize(topazVector_t * a);

/// Returns the cross product
///
topazVector_t topaz_vector_cross_product(const topazVector_t * a, const topazVector_t * b);

/// Sets the X Y Z to integer values.
///
topazVector_t * topaz_vector_floor(topazVector_t *);



/// X axis rotation
///
/// If a vector has length zero in these dimensions
/// , the angle is always zero.
/// The rotation is about the directional vector (1, 0, 0) and
/// Positive rotation is towards the positive Z direction.

/// Returns the YZ angle between the 2
/// vectors in relation to the origin in degrees
float topaz_vector_rotation_x_diff(const topazVector_t * a, const topazVector_t * b);

/// Returns the angle between the 2
/// vectors in relation to (*this) in degrees
float topaz_vector_rotation_x_diff_relative(const topazVector_t * a, const topazVector_t * b);

/// Returns the angle of the vector in degrees
///
float topaz_vector_rotation_x(const topazVector_t * a);






/// Y axis rotation
///
/// If a vector has length zero in these dimensions
/// , the angle is always zero.
/// The rotation is about the directional vector (0, 1, 0)
/// Positive rotation is towards the positive X direction.

/// Returns the zx angle between the 2
/// vectors in relation to the origin in degrees
float topaz_vector_rotation_y_diff(const topazVector_t * a, const topazVector_t * b);

/// Returns the angle between the 2
/// vectors in relation to (*this) in degrees
float topaz_vector_rotation_y_diff_relative(const topazVector_t * a, const topazVector_t * b);

/// Returns the angle of the vector in degrees.
///
float topaz_vector_rotation_y(const topazVector_t * a);



/// Z rotation axis
///
/// These deals with angles between the x and y axes.
/// XY is most useful for 2D angles and transformations.
/// If a vector has length zero in these dimensions
/// , the angle is always zero.
/// The rotation is about the directional vector (0, 0, 1).
/// Positive rotation is towards the positive Y direction.


/// Returns the XY angle between the 2
/// vectors in relation to the origin in degrees
float topaz_vector_rotation_z_diff(const topazVector_t * a, const topazVector_t * b);

/// Returns the angle between the 2
/// vectors in relation to (*this) in degrees
float topaz_vector_rotation_z_diff_relative(const topazVector_t * a, const topazVector_t * b);


/// Returns the angle of the vector in degrees
///
float topaz_vector_rotation_z(const topazVector_t * a);



/// Returns the result of rotating the vector
/// by the specified degrees about the center
topazVector_t * topaz_vector_rotate_x(topazVector_t *, float);
topazVector_t * topaz_vector_rotate_y(topazVector_t *, float);
topazVector_t * topaz_vector_rotate_z(topazVector_t *, float);







#endif