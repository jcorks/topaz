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


#ifndef H_TOPAZDC__MATRIX__INCLUDED
#define H_TOPAZDC__MATRIX__INCLUDED


#include <topaz/vector.h>
#include <topaz/containers/string.h>


/// A generic, row-major matrix 
///
typedef struct topazMatrix_t topazMatrix_t;
struct topazMatrix_t {
    /// Data for the matrix, elements are laid out 
    /// linearly in memory, starting with the topleft,
    /// then laying out rows.
    float data[16];
};

/// Sets the matrix to the identity matrix, removing all transformations.
///
void topaz_matrix_set_identity(
    /// Matrix to modify.
    topazMatrix_t * m
);

/// Convenience function that transforms the 
/// given point and returns its result.
///
topazVector_t topaz_matrix_transform(
    /// The matrix to use as source.
    const topazMatrix_t * m, 

    /// The point to transform.
    const topazVector_t * point
);



/// Returns a string containing info on the transform.
///
topazString_t * topaz_matrix_to_string(
    /// The source matrix.
    const topazMatrix_t * m
);

/// Transposes the matrix.
///
void topaz_matrix_transpose(
    /// The matrix to modify.
    topazMatrix_t * m
);

/// Inverts the matrix.
///
void topaz_matrix_invert(
    /// The matrix to modify.
    topazMatrix_t * m
);

/// Reverse the majority of the matrix.
///
void topaz_matrix_reverse_majority(
    /// The matrix to modify.
    topazMatrix_t * m
);

/// Returns the internal representation of the TransformMatrix.
///
/// (No script mapping) 
///
#define topaz_matrix_ptr(__M__) ((float*)__M__.data)


/// Multiplies 2 matrices.
///
topazMatrix_t topaz_matrix_multiply(
    /// The first operand of the multiplication.
    const topazMatrix_t * a, 

    /// The second operand of the multiplication.
    const topazMatrix_t * b
);


/// Rotates the matrix about the Euler angles psi, theta, and phi.
///
void topaz_matrix_rotate_by_angles(
    /// The transform matrix to modify.
    topazMatrix_t * m,  

    /// The psi (x) rotation angle.
    float x, 

    /// The theta (y) rotation angle.
    float y, 

    /// The phi (z) rotation angle.
    float z
);

/// Rotates the matrix about an axies
///
void topaz_matrix_rotate_by_axis(
    /// The transform matrix to modify.
    topazMatrix_t * m,  

    /// The axis X
    float x, 

    /// The axis Z
    float y, 

    /// The axis Z
    float z,
    
    /// The amount to rotate
    float degrees
);

/// Expresses a translation by x, y, and z
///
void topaz_matrix_translate(
    /// The transform matrix to modify.
    topazMatrix_t * m,  

    /// The x displacement.
    float x, 

    /// The y displacement.
    float y, 

    /// The z displacement.
    float z
);

/// Expresses a scaling in the x, y, and z directions.
///
void topaz_matrix_scale(
    /// The transform matrix to modify.
    topazMatrix_t * m,  

    /// The x scale modifier.
    float x, 

    /// The y scale modifier.
    float y, 

    /// The z scale modifier.
    float z
);

    


#endif


