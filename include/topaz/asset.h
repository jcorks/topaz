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


#ifndef H_TOPAZDC__COLOR__INCLUDED
#define H_TOPAZDC__COLOR__INCLUDED

#include <topaz/containers/string.h>

/*

    Asset
    -----

    An asset represents an external set of data, usually a separate file,
    that can be used to interface with 

*/

typedef struct topazAsset_t topazAsset_t;

typedef enum {
    topazAsset_Type_None,
    topazAsset_Type_Image,
    topazAsset_Type_Font,
    topazAsset_Type_Audio,
    topazAsset_Type_Model,
    topazAsset_Type_Particle,
    topazAsset_Type_RawData,
    topazAsset_Type_Actor,
    topazAsset_Type_Count,    
} topazAsset_Type;



topazString_t * topaz_asset_get_name(const topazAsset_t *);

topazAsset_Type topaz_asset_get_type(const topazAsset_t *);

#endif


