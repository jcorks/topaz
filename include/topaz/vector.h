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
    topazVector_t * v
);

/// Shorthand initializer for x, y, and z.
///
/// (No script mapping)
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
/// (No script mapping)
///

#define topaz_vec_x(__V__) (((float*)(__V__))[0])

/// Returns the positional Y of the vector
///
/// (No script mapping)
///
#define topaz_vec_y(__V__) (((float*)(__V__))[1])

/// Returns the positional Z of the vector
///
/// (No script mapping)
///
#define topaz_vec_z(__V__) (((float*)(__V__))[2])



/// Returns the length of the vector.
///
float topaz_vector_get_length(
    /// The vector to query.
    const topazVector_t * v
);

/// Returns the distance of this vector to the other vector.
///
float topaz_vector_get_distance(
    /// The first positional vector.
    const topazVector_t * p0, 

    /// The second positional vector.
    const topazVector_t * p1
);

/// Sets the vector to a magnitude of 1. The source vector is returned it.
///
topazVector_t * topaz_vector_normalize(
    /// The vector to modify.
    topazVector_t * v
);

/// Returns the cross product: A x B
///
topazVector_t topaz_vector_cross(
    /// Vector A
    const topazVector_t * a, 

    /// Vector B
    const topazVector_t * b
);

/// Returns the dot product.: A . B
float topaz_vector_dot(
    /// Vector A
    const topazVector_t * a,

    /// Vector B
    const topazVector_t * b
);

/// Sets the X Y Z to integer values.
///
topazVector_t * topaz_vector_floor(
    /// The vector whose components will be floored.
    topazVector_t * v
);





/// Reflects a directional 2D vector "direction"
/// off a direction 2D vector "surface" and returns the result
/// as a direction vector of the reflection 
topazVector_t topaz_vector_reflect_2d(
    /// The initial direction to reflect
    topazVector_t * direction,
    
    /// The surface to reflect from.
    topazVector_t * surface
);


/// Returns an xyz rotation vector that, then used 
/// in a rotation matrix (such as in entities), 
/// will orient itself as if facing to the point p1
/// from point p0, where "up" is the direction 
/// of how to orient the face-wise rotation.
topazVector_t topaz_vector_look_at_rotation(
    /// Where we are looking from.
    const topazVector_t * p0,
    
    /// Where we are looking to.
    const topazVector_t * p1,
    
    /// Direction to anchor the rotation
    const topazVector_t * up
);


float topaz_vector_point_at_2d(
    const topazVector_t * p0,
    const topazVector_t * p1
);



#endif
