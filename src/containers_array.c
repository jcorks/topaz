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
#include <topaz/containers/array.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


#define array_resize_amt 1.4
#define array_presize_amt 16

struct topazArray_t {
    uint32_t allocSize;
    uint32_t size;
    uint32_t sizeofType;
    uint8_t * data;
};


topazArray_t * topaz_array_create(uint32_t typesize) {
    topazArray_t * a = malloc(sizeof(topazArray_t));
    a->sizeofType = typesize;
    a->size = 0;
    a->allocSize = array_presize_amt;
    a->data = malloc(typesize*array_presize_amt);
    return a;
}

void topaz_array_destroy(topazArray_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
    #endif
    free(t->data);
    free(t);
}



topazArray_t * topaz_array_clone(const topazArray_t * src) {
    #ifdef TOPAZDC_DEBUG
        assert(src && "topazArray_t pointer cannot be NULL.");
    #endif
    topazArray_t * a = malloc(sizeof(topazArray_t));

    // do not clone pre-alloc size
    a->allocSize = src->size;
    a->size = src->size;
    a->sizeofType = src->sizeofType;
    a->data = malloc(src->size*a->sizeofType);
    return a;
}


uint32_t topaz_array_get_size(const topazArray_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
    #endif
    return t->size;
}

void topaz_array_push_n(topazArray_t * t, void * elements, uint32_t count) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
    #endif
    while(t->size + count > t->allocSize) {
        t->allocSize += t->allocSize*array_presize_amt;
        t->data = realloc(t->data, t->allocSize*t->sizeofType);
    }
    memcpy(
        (t->data)+(t->size*t->sizeofType), 
        elements, 
        count*t->sizeofType
    );
}


void topaz_array_remove(topazArray_t * t, uint32_t index) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
        assert(index < t->size);
    #endif

    uint32_t indexByte = index*t->sizeofType;
    memmove(
        t->data+(indexByte),
        t->data+(indexByte + t->sizeofType),
        (t->size-1)*t->sizeofType
    );
}



void * topaz_array_get_data(const topazArray_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
    #endif
    return t->data;
}


void topaz_array_clear(topazArray_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
    #endif
    t->size = 0;
}

void topaz_array_set_size(topazArray_t * t, uint32_t size) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazArray_t pointer cannot be NULL.");
    #endif
    while(size > t->allocSize) {
        t->allocSize += t->allocSize*array_presize_amt;
        t->data = realloc(t->data, t->allocSize*t->sizeofType);
    }
    t->size = size;
}
