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
    
/// A hybrid class suitable for both 2D and 3D
/// positional and physical operations.
///
typedef struct topazVector_t topazVector_t;
struct topazVector_t {
    /// The first component of the vector.
    float x; 

    /// The second component of the vector.
    float y; 

    /// The third component of the vector.
    float z;
};


/// Initializes x, y, and z to 0.
///
void topaz_vector_reset(
    /// The vector to reset.
    topazVector_t * vector
);

/// Shorthand initializer for x, y, and z.
///
topazVector_t topaz_vector_from_xyz(
    /// The x component.
    float x, 

    /// The y component.
    float y, 

    /// The z component.
    float z
);

/// Attempts to convert an input string into a vector.
///
/// The format of the text is 2 to 3 floating point numbers separated by
/// any counts of either whitespace or generally accepted separation characters:
///    ,<>(){}[]
///
/// The point here is to allow instantiation from natural mathematical notations
/// which depend on the usage of the Vector. Examples of valid input:
///
/// topaz_vector_from_string(TOPAZ_STR_CAST("{1, 4, 5}"));
/// topaz_vector_from_string(TOPAZ_STR_CAST("1 4 5"));
/// topaz_vector_from_string(TOPAZ_STR_CAST("<1 4 5>"));
/// topaz_vector_from_string(TOPAZ_STR_CAST("[1 4 5]"));
///
topazVector_t topaz_vector_from_string(
    /// Formatted data for the vector.
    const topazString_t * input
);

/// Returns the positional X of the vector
///
#define topaz_vec_x(__V__) (((float*)(__V__))[0])

/// Returns the positional Y of the vector
///
#define topaz_vec_y(__V__) (((float*)(__V__))[1])

/// Returns the positional Z of the vector
///
#define topaz_vec_z(__V__) (((float*)(__V__))[2])



/// Returns the length of the vector.
///
float topaz_vector_get_length(
    /// The vector to query.
    const topazVector_t * vector
);

/// Returns the distance of this vector to the other vector.
///
float topaz_vector_get_distance(
    /// The first positional vector.
    const topazVector_t * pos0, 

    /// The second positional vector.
    const topazVector_t * pos1
);

/// Sets the vector to a magnitude of 1. The source vector is returned it.
///
topazVector_t * topaz_vector_normalize(
    /// The vector to modify.
    topazVector_t * a
);

/// Returns the cross product: A x B
///
topazVector_t topaz_vector_cross(
    /// Vector A
    const topazVector_t * A, 

    /// Vector B
    const topazVector_t * B
);

/// Returns the dot product.: A . B
float topaz_vector_dot(
    /// Vector A
    const topazVector_t * A,

    /// Vector B
    const topazVector_t * B
);

/// Sets the X Y Z to integer values.
///
topazVector_t * topaz_vector_floor(
    /// The vector whose components will be floored.
    topazVector_t * a
);




/// Returns the YZ angle between the 2
/// vectors in relation to the origin in degrees
float topaz_vector_rotation_x_diff(
    /// The positional vector to measure from.
    const topazVector_t * a, 

    /// The positional vector to measure to.
    const topazVector_t * b
);

/// Returns the angle between the 2
/// vectors in relation to (*this) in degrees
float topaz_vector_rotation_x_diff_relative(
    /// The positional vector to measure from.
    const topazVector_t * a, 

    /// The positional vector to measure to.
    const topazVector_t * b
);



/// Returns the angle of the vector in degrees about the X axis
///
/// If a vector has length zero in these dimensions
/// , the angle is always zero.
/// The rotation is about the directional vector (1, 0, 0) and
/// Positive rotation is towards the positive Z direction.
///
float topaz_vector_rotation_x(
    /// The positional vector to measure to (from 0,0,0).
    const topazVector_t * a
);






/// Returns the zx angle between the 2
/// vectors in relation to the origin in degrees
float topaz_vector_rotation_y_diff(
    /// The positional vector to measure from.
    const topazVector_t * a, 

    /// The positional vector to measure to.
    const topazVector_t * b
);

/// Returns the angle between the 2
/// vectors in relation to (*this) in degrees
float topaz_vector_rotation_y_diff_relative(
    /// The positional vector to measure from.
    const topazVector_t * a, 

    /// The positional vector to measure to.
    const topazVector_t * b
);

/// Returns the angle of the vector about the Y axis in degrees
///
/// If a vector has length zero in these dimensions
/// , the angle is always zero.
/// The rotation is about the directional vector (0, 1, 0)
/// Positive rotation is towards the positive X direction.
float topaz_vector_rotation_y(
    /// The positional vector to measure to (from 0,0,0).
    const topazVector_t * a
);






/// Returns the XY angle between the 2
/// vectors in relation to the origin in degrees
float topaz_vector_rotation_z_diff(
    /// The positional vector to measure from.
    const topazVector_t * a, 

    /// The positional vector to measure to.
    const topazVector_t * b
);

/// Returns the angle between the 2
/// vectors in relation to (*this) in degrees
float topaz_vector_rotation_z_diff_relative(
    /// The positional vector to measure from.
    const topazVector_t * a, 

    /// The positional vector to measure to.
    const topazVector_t * b
);


/// Returns the angle of the vector about the Z axis in degrees
/// These deals with angles between the x and y axes.
/// XY is most useful for 2D angles and transformations.
/// If a vector has length zero in these dimensions
/// , the angle is always zero.
/// The rotation is about the directional vector (0, 0, 1).
/// Positive rotation is towards the positive Y direction.
///
float topaz_vector_rotation_z(
    /// The positional vector to measure to (from 0,0,0).
    const topazVector_t * a
);



/// Returns the result of rotating the vector
/// by the specified degrees about the center
topazVector_t * topaz_vector_rotate_x(
    /// The vector to rotate.
    topazVector_t * vector, 

    /// The amount to rotate in degrees.
    float amt
);

/// Returns the result of rotating the vector
/// by the specified degrees about the center
topazVector_t * topaz_vector_rotate_y(
    /// The vector to rotate.
    topazVector_t * vector, 

    /// The amount to rotate in degrees.
    float amt
);

/// Returns the result of rotating the vector
/// by the specified degrees about the center
topazVector_t * topaz_vector_rotate_z(
    /// The vector to rotate.
    topazVector_t * vector, 

    /// The amount to rotate in degrees.
    float amt
);








#endif
