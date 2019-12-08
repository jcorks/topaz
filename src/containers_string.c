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
#include <stdarg.h>
#include <stdio.h>

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

static void topaz_string_concat_cstr(topazString_t * s, const char * cstr, uint32_t len) {
    while (s->len + len + 1 >= s->alloc) {
        s->alloc*=1.4;
        s->cstr = realloc(s->cstr, s->alloc);
    }

    memcpy(s->cstr+s->len, cstr, len+1);
    s->len+=len;
}

static void topaz_string_set_cstr(topazString_t * s, const char * cstr, uint32_t len) {
    s->len = 0;
    topaz_string_concat_cstr(s, cstr, len);
}




topazString_t * topaz_string_create() {
    topazString_t * out = calloc(1, sizeof(topazString_t));
    out->alloc = prealloc_size;
    out->cstr = malloc(prealloc_size);
    out->cstr[0] = 0;
    return out;
}

topazString_t * topaz_string_create_from_c_str(const char * str) {
    topazString_t * out = topaz_string_create();
    topaz_string_set_cstr(out, str, strlen(str));
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
    free(s);
}

void topaz_string_clear(topazString_t * s) {
    s->len = 0;
}

void topaz_string_set(topazString_t * s, const topazString_t * src) {
    free(s->cstr);
    s->len = src->len;
    s->alloc = src->alloc;
    s->cstr = malloc(s->alloc);
    memcpy(s->cstr, src->cstr, src->len+1);

    if (s->delimiters) topaz_string_destroy(s->delimiters);
    if (s->chain) topaz_string_destroy(s->chain);

}



void topaz_string_concat_printf(topazString_t * s, const char * format, ...) {
    va_list args;
    va_start(args, format);
    int lenReal = vsnprintf(NULL, 0, format, args);
    va_end(args);


    char * newBuffer = malloc(lenReal+2);
    va_start(args, format);    
    vsnprintf(newBuffer, lenReal+1, format, args);
    va_end(args);
    


    topaz_string_concat_cstr(s, newBuffer, lenReal);
    free(newBuffer);
}

void topaz_string_concat(topazString_t * s, const topazString_t * src) {
    topaz_string_concat_cstr(s, src->cstr, src->len);
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

    if (!s->lastSubstr) {
        ((topazString_t *)s)->lastSubstr = topaz_string_create();
    }

    char realAtTo = s->cstr[to];
    s->cstr[to] = 0;
    topaz_string_set_cstr(
        s->lastSubstr, 
        s->cstr+from, 
        to - from
    );
    s->cstr[to] = realAtTo;    

    return s->lastSubstr;    
}



const char * topaz_string_get_c_str(const topazString_t * t) {
    return t->cstr;
}

uint32_t topaz_string_get_length(const topazString_t * t) {
    return t->len;
}



const topazString_t * topaz_string_chain_start(topazString_t * t, const topazString_t * delimiters) {
    t->iter = 0;
    if (!t->chain) {
        t->chain = topaz_string_create();
        t->delimiters = topaz_string_create();
    }
    topaz_string_set(t->delimiters, delimiters);
    return topaz_string_chain_proceed(t);
}

const topazString_t * topaz_string_chain_current(topazString_t * t) {
    if (!t->chain) {
        t->chain = topaz_string_create();
        t->delimiters = topaz_string_create();
    }
    return t->chain;
}

int topaz_string_chain_is_end(const topazString_t * t) {
    return t->iter > t->len;
}

const topazString_t * topaz_string_chain_proceed(topazString_t * t) {

    char * del = t->delimiters->cstr;
    char * iter;    

    #define chunk_size 32
    char chunk[chunk_size];
    uint32_t chunkLen = 0;


    char c;

    // skip over leading delimiters
    for(; t->iter < t->len; t->iter++) {
        c = t->cstr[t->iter];

        // delimiter marks the end.
        for(iter = del; *iter; ++iter) {
            if (*iter == c) {
                break;                
            }
        }
        if (!*iter) break;
    }

    // reset for next link
    topaz_string_set_cstr(t->chain, "", 0);

    // check if at end
    if (t->iter >= t->len) {
        t->iter = t->len+1;
        return t->chain;
    }

    for(; t->iter < t->len; t->iter++) {
        c = t->cstr[t->iter];

        // delimiter marks the end.
        for(iter = del; *iter; ++iter) {
            if (*iter == c && chunkLen) {
                chunk[chunkLen] = 0;
                topaz_string_concat_cstr(t->chain, chunk, chunkLen);                
                return t->chain;
            }
        }         

        if (chunkLen == chunk_size-1) {
            chunk[chunkLen] = 0;
            topaz_string_concat_cstr(t->chain, chunk, chunkLen);                
            chunkLen = 0;
        }
        chunk[chunkLen++] = c;
   }

    // reached the end of the string 
    t->iter = t->len;
    chunk[chunkLen] = 0;
    topaz_string_concat_cstr(t->chain, chunk, chunkLen);         
    return t->chain;       
}












