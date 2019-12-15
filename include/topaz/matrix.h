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

/*

    Matrix
    -----

    Transform matrix

*/
typedef struct {float data[16];} topazMatrix_t;

/// Sets the matrix to the identity matrix, removing all transformations.
///
void topaz_matrix_set_identity(topazMatrix_t *);

/// Transforms the given point and returns its result;
///
topazVector_t topaz_matrix_transform(const topazMatrix_t *, const topazVector_t *);



/// Returns a string containing info on the transform.
///
topazString_t * topaz_matrix_to_string(const topazMatrix_t *);

/// Transposes the matrix.
///
void topaz_matrix_transpose(topazMatrix_t *);

/// Inverts the matrix.
///
void topaz_matrix_invert(topazMatrix_t *);

/// Reverse the majority of the matrix.
///
void topaz_matrix_reverse_majority(topazMatrix_t *);

/// Returns the internal representation of the TransformMatrix.
///
#define topaz_matrix_ptr(__M__)((float*)__M__.data)


/// Multiplies 2 matrices.
///
topazMatrix_t topaz_matrix_multiply(const topazMatrix_t *, const topazMatrix_t *);


/// Rotates the matrix about the Euler angles psi, theta, and phi.
///
void topaz_matrix_rotate_by_angles(topazMatrix_t *, float x, float y, float z);

/// Expresses a translation by x, y, and z
///
void topaz_matrix_translate(topazMatrix_t *, float x, float y, float z);

/// Expresses a scaling in the x, y, and z directions.
///
void topaz_matrix_scale(topazMatrix_t *, float x, float y, float z);


    


#endif


