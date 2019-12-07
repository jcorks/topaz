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
#include <topaz/containers/string.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



#define prealloc_size 32


struct topazString_t {
    char * cstr;
    uint32_t len;
    uint32_t alloc;

    topazString_t * delimiters;
    topazString_t * chain;
    uint32_t iter;

    topazString_t * lastSubstr;
};


topazString_t * topaz_string_create() {
    topazString_t * out = calloc(1, sizeof(topazString_t));
    out->alloc = prealloc_size;
    out->cstr = malloc(prealloc_size);
    out->cstr[0] = 0;
    return out;
}

topazString_t * topaz_string_create_from_c_str(const char * str) {
    topazString_t * out = topaz_string_create();
    topaz_string_set(out, topaz_string_cast_from_c_str(str));
    return out;
}

topazString_t * topaz_string_clone(const topazString_t * src) {
    topazString_t * out = topaz_string_create();
    topaz_string_set(out, src);
    return out;
}

void topaz_string_destroy(topazString_t * s) {
    free(s->cstr);
    if (s->delimiters) topaz_string_destroy(s->delimiters);
    if (s->chain) topaz_string_destroy(s->chain);
    if (s->lastSubstr) topaz_string_destroy(s->lastSubstr);
}



void topaz_string_set(topazString_t * s, const topazString_t * src) {
    free(s->cstr);
    s->len = src->len;
    s->alloc = src->alloc;
    s->cstr = malloc(s->alloc);
    memcpy(s->cstr, src->cstr, src->len);

    if (s->delimiters) topaz_string_destroy(s->delimiters);
    if (s->chain) topaz_string_destroy(s->chain);
    s->iter = src->iter;
    
    s->delimiters = topaz_string_clone(s->delimiters);
    s->chain = topaz_string_clone(s->chain);
    
}



void topaz_string_concat_printf(topazString_t * s, const char * format, ...) {
    va_list args;
    va_start(args, s);
    int lenReal = vsnprintf(NULL, 0, format, args);
    va_end(args);


    char * newBuffer = malloc(lenReal+1);
    va_start(args, s);    
    vsnprintf(newBuffer, lenReal, format, args);
    va_end(args);
    
    topaz_string_concat(s, topaz_string_cast_from_c_str(newBuffer));
    free(newBuffer;
}

void topaz_string_concat(topazString_t * s, const topazString_t * src) {
    if (s->len + src->len >= s->alloc) {
        s->alloc*=1.4;
        s->cstr = realloc(s->cstr, s->alloc);
    }

    memcpy(s->cstr+s->size, src->cstr, src->size);
    s->size+=src->size;
}



const topazString_t * topaz_string_get_substr(
    const topazString_t * s,
    uint32_t from,
    uint32_t to
) {
    #ifdef TOPAZDC_DEBUG
        assert(from < s->len);
        assert(to < s->len);
    #endif

    if (to < from) {
        uint32_t temp = to;
        to = from;
        from = temp;
    }

    if (s->lastSubstr) {
        topaz_string_destroy(s->lastSubstr);
    }

    char realAtTo = s->cstr[to];
    s->cstr[to] = 0;
    s->lastSubstr = topaz_string_create_from_c_str(s->cstr+from);
    s->cstr[to] = realAtTo;    

    return s->lastSubstr;    
}



const char * topaz_string_get_c_str(const topazString_t * t) {
    return t->cstr;
}

uint32_t topaz_string_get_length(const topazString_t * t) {
    return t->len;
}












