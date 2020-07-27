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


#ifndef H_TOPAZDC__BIN__INCLUDED
#define H_TOPAZDC__BIN__INCLUDED


#include <stdint.h>

/*

    Bin
    -----

    Tags a pointer with a unique value ID. Mostly useful for internal 
    implementations of ID systems where validation is needed.

*/
typedef struct topazBin_t topazBin_t;

/// Creates a new bin to add objects to.
///
topazBin_t * topaz_bin_create();

/// Destroys the bin.
///
void topaz_bin_destroy(topazBin_t *);

/// Adds an object to the bin, its ID tag is returned.
/// The object must not be NULL.
///
uint32_t topaz_bin_add(topazBin_t *, void *);

/// Returns whether the bin contains the given value.
///
int topaz_bin_contains(const topazBin_t *, uint32_t);

/// Returns the object matched with the ID tag
///
void * topaz_bin_fetch(const topazBin_t *, uint32_t);

/// Removes an object from the bin.
///
void topaz_bin_remove(topazBin_t *, uint32_t);

/// Returns an array of void * containing all 
/// objects within the bin. Its a new array and must be destroyed by 
/// the user.
///
topazArray_t * topaz_bin_get_all(const topazBin_t *);


#endif