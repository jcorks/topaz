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


#ifndef H_TOPAZDC__DATA__INCLUDED
#define H_TOPAZDC__DATA__INCLUDED

#include <topaz/asset.h>
typedef struct topazArray_t topazArray_t;

/// Holds raw byte data.
typedef struct topazData_t topazData_t;




/// Creates a new data asset object
/// Use asset_destroy to release.
///
topazAsset_t * topaz_data_create(
    topaz_t *, 
    const topazString_t *
);

/// Creates a new data asset object, but without 
/// any loading behavior. 
///
topazAsset_t * topaz_data_create_empty(topaz_t *);

/// Sets the raw data for the asset. The input array may be of any 
/// kind, as it will be copied into a byte array.
///
void topaz_data_set(topazAsset_t *, const topazArray_t *);

/// Returns a read-only byte array of the raw data 
/// within the asset.
///
const topazArray_t * topaz_data_get_as_bytes(topazAsset_t *);

/// Creates a new string from the raw data stored in the asset.
///
topazString_t * topaz_data_get_as_string(topazAsset_t *);


#endif
