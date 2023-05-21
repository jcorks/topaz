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
#include <ctype.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



#define prealloc_size 32




static uint32_t utf8_next_char(uint8_t ** source) {
    uint8_t * iter = *source;
    uint32_t val = (*source)[0];
    if (val < 128 && *iter) {
        val = (iter[0]) & 0x7F;
        (*source)++;
        return val;
    } else if (val < 224 && *iter && iter[1]) {
        val = ((iter[0] & 0x1F)<<6) + (iter[1] & 0x3F);
        (*source)+=2;
        return val;
    } else if (val < 240 && *iter && iter[1] && iter[2]) {
        val = ((iter[0] & 0x0F)<<12) + ((iter[1] & 0x3F)<<6) + (iter[2] & 0x3F);
        (*source)+=3;
        return val;
    } else if (*iter && iter[1] && iter[2] && iter[3]) {
        val = ((iter[0] & 0x7)<<18) + ((iter[1] & 0x3F)<<12) + ((iter[2] & 0x3F)<<6) + (iter[3] & 0x3F);
        (*source)+=4;
        return val;
    }
    return 0;
}


static int utf8_put_char(uint32_t val, uint8_t * iter) {
    if (val < 0x80) {
        *(iter++) = val & 0x7F;
        return 1;
    } else if (val < 0x800) {
        *(iter++) = ((val & 0x7C0) >> 6) | 0xC0;
        *(iter++) = (val & 0x3F) | 0x80; 
        return 2;
    } else if (val < 0x10000) {
        *(iter++) = ((val & 0xF000) >> 12) | 0xE0; 
        *(iter++) = ((val & 0xFC0) >> 6) | 0x80; 
        *(iter++) = (val & 0x3F) | 0x80; 
        return 3;
    } else {
        *(iter++) = ((val & 0x1C0000) >> 18) | 0xF0; 
        *(iter++) = ((val & 0x3F000) >> 12) | 0x80; 
        *(iter++) = ((val & 0xFC0) >> 6) | 0x80; 
        *(iter++) = (val & 0x3F) | 0x80; 
        return 4;
    }
}




struct topazString_t {
    char * utf8; // only populated when requested
    uint32_t * codepoints; // real active use 
    uint32_t len;
    uint32_t alloc;

    topazString_t * delimiters;
    topazString_t * chain;
    uint32_t iter;

    topazString_t * lastSubstr;
};


static void topaz_string_resize_fit(topazString_t * s, uint32_t len) {
    while (s->len + len + 1 >= s->alloc) {
        s->alloc*=1.4;
        s->codepoints = realloc(s->codepoints, s->alloc * sizeof(uint32_t));
    }
}

static void topaz_string_concat_cstr(topazString_t * s, const char * cstr, uint32_t len) {
    if (!len) return;
    topaz_string_resize_fit(s, len);
    uint32_t point;
    uint32_t iter = 0;
    while((point = utf8_next_char((uint8_t**)&cstr))) {        
        iter++;
        s->codepoints[s->len++] = point;
        if (iter >= len) break;
    }
    if (s->utf8) free(s->utf8);
    s->utf8 = NULL;
}


static void topaz_string_concat_codepoints(topazString_t * s, const uint32_t * points, uint32_t len) {
    if (!len) return;
    topaz_string_resize_fit(s, len);
    uint32_t point;
    uint32_t i;
    for(i = 0; i < len; ++i) {
        s->codepoints[s->len++] = points[i];
    }
    
    if (s->utf8) free(s->utf8);
    s->utf8 = NULL;
}

static void topaz_string_set_cstr(topazString_t * s, const char * cstr, uint32_t len) {
    s->len = 0;
    topaz_string_concat_cstr(s, cstr, len);
}




topazString_t * topaz_string_create() {
    topazString_t * out = calloc(1, sizeof(topazString_t));
    out->alloc = prealloc_size;
    out->utf8 = NULL;
    out->codepoints = malloc(prealloc_size * sizeof(uint32_t));
    return out;
}

topazString_t * topaz_string_create_from_c_str(const char * format, ...) {
    va_list args;
    va_start(args, format);
    int lenReal = vsnprintf(NULL, 0, format, args);
    va_end(args);


    char * newBuffer = malloc(lenReal+2);
    va_start(args, format);    
    vsnprintf(newBuffer, lenReal+1, format, args);
    va_end(args);
    

    topazString_t * out = topaz_string_create();
    topaz_string_set_cstr(out, newBuffer, lenReal);
    free(newBuffer);
    return out;
}

topazString_t * topaz_string_clone(const topazString_t * src) {
    topazString_t * out = topaz_string_create();
    topaz_string_set(out, src);
    return out;
}

void topaz_string_destroy(topazString_t * s) {
    free(s->utf8);
    free(s->codepoints);
    if (s->delimiters) topaz_string_destroy(s->delimiters);
    if (s->chain) topaz_string_destroy(s->chain);
    if (s->lastSubstr) topaz_string_destroy(s->lastSubstr);
    free(s);
}

void topaz_string_clear(topazString_t * s) {
    s->len = 0;
    if (s->utf8) free(s->utf8);
    s->utf8 = NULL;
}

void topaz_string_set(topazString_t * s, const topazString_t * src) {
    topaz_string_resize_fit(s, src->len);
    memcpy(s->codepoints, src->codepoints, (src->len*sizeof(uint32_t)));
    s->len = src->len;


    if (s->delimiters) topaz_string_destroy(s->delimiters);
    if (s->chain) topaz_string_destroy(s->chain);
    if (s->utf8) free(s->utf8);
    s->utf8 = NULL;
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
    topaz_string_concat_cstr(s, topaz_string_get_c_str(src), src->len);
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

    topaz_string_concat_codepoints(
        s->lastSubstr,
        s->codepoints+from,
        (to-from)+1
    );

    return s->lastSubstr;    
}



const char * topaz_string_get_c_str(const topazString_t * t) {
    if (t->utf8) return t->utf8;
    ((topazString_t*)t)->utf8 = malloc((t->len+1)*sizeof(uint32_t));
    char * iter = t->utf8;
    uint32_t i;
    for(i = 0; i < t->len; ++i) {
        int offset = utf8_put_char(t->codepoints[i], iter);
        iter += offset;
    }
    return t->utf8;
}

uint32_t topaz_string_get_length(const topazString_t * t) {
    return t->len;
}

uint32_t topaz_string_get_char(const topazString_t * t, uint32_t p) {
    if (p >= t->len) return 0;
    return t->codepoints[p];
}

void topaz_string_set_char(topazString_t * t, uint32_t p, uint32_t value) {
    if (p >= t->len) return;
    t->codepoints[p] = value;
    if (t->utf8) free(t->utf8);
    t->utf8 = NULL;
}

void topaz_string_append_char(topazString_t * t, int value) {
    char str[2];
    str[1] = 0;
    str[0] = value;
    topaz_string_concat_cstr(t, str, 1);
}


uint32_t topaz_string_get_byte_length(const topazString_t * t) {
    // for now same as string length. will change when unicode is supported.
    return strlen(topaz_string_get_c_str(t));
}

void * topaz_string_get_byte_data(const topazString_t * t) {
    return (void*)topaz_string_get_c_str(t);
}

int topaz_string_test_contains(const topazString_t * a, const topazString_t * b) {
    const char * a_str = topaz_string_get_c_str(a);
    const char * b_str = topaz_string_get_c_str(b);
    return strstr(a_str, b_str) != NULL;
}

// TODO: just compare codepoints, thanks!
int topaz_string_test_eq(const topazString_t * a, const topazString_t * b) {
    const char * a_str = topaz_string_get_c_str(a);
    const char * b_str = topaz_string_get_c_str(b);
    return strcmp(a_str, b_str) == 0;
}

int topaz_string_topaz_compare(const topazString_t * a, const topazString_t * b) {
    const char * a_str = topaz_string_get_c_str(a);
    const char * b_str = topaz_string_get_c_str(b);
    return strcmp(a_str, b_str);
}

topazString_t * topaz_string_base64_from_bytes(
    const uint8_t * buffer,
    uint32_t size
) {
    topazString_t * out = topaz_string_create();
    const char * key = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint32_t i, j;
    int groupOfSix[4];
    for(i = 0; i < size; i+=3) {
        groupOfSix[0] = 
        groupOfSix[1] = 
        groupOfSix[2] = 
        groupOfSix[3] = -1;

        groupOfSix[0] = buffer[i] >> 2;
        groupOfSix[1] = (buffer[i] & 0x03) << 4;
        if (size > i + 1) {
            groupOfSix[1] |= buffer[i+1] >> 4;
            groupOfSix[2]  = (buffer[i+1] & 0x0f) << 2;
        }
        if (size > i + 2) {
            groupOfSix[2] |= buffer[i+2] >> 6;
            groupOfSix[3] = buffer[i+2] & 0x3f; 
        }

        for(j = 0; j < 4; ++j) {
            if (groupOfSix[j] < 0) {
                topaz_string_append_char(out, '=');
            } else {
                topaz_string_append_char(out, key[groupOfSix[j]]);
            }
        }
    }
    return out;
}


static char byteKey[] = {
// 0 
    ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', 
    ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', 
    ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', 
    ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', 
// 32
    ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', 
    ' ', ' ', ' ', 62 ,    ' ', ' ', ' ', 63 , 
    52 , 53 , 54 , 55 ,    56 , 57 , 58 , 59 , 
    60 , 61 , ' ', ' ',    ' ', ' ', ' ', ' ', 
// 64
    ' ',  0 ,  1 ,  2 ,     3 ,  4 ,  5 ,  6 , 
     7 ,  8 ,  9 , 10 ,    11 , 12 , 13 , 14 , 
    15 , 16 , 17 , 18 ,    19 , 20 , 21 , 22 , 
    23 , 24 , 25 , ' ',    ' ', ' ', ' ', ' ', 

// 96
    ' ', 26 , 27 , 28 ,    29 , 30 , 31 , 32 , 
    33 , 34 , 35 , 36 ,    37 , 38 , 39 , 40 , 
    41 , 42 , 43 , 44 ,    45 , 46 , 47 , 48 , 
    49 , 50 , 51, ' ',    ' ', ' ', ' ', ' ', 


};

uint8_t * topaz_string_base64_to_bytes(
    const topazString_t * in,
    uint32_t * size
) {
    uint32_t i;
    uint32_t len = topaz_string_get_length(in);
    uint8_t val;
    if (!len) return NULL;

    *size = 0;
    uint8_t * out = malloc(len); // raw length is always bigger than real length;

    while(len && topaz_string_get_char(in, len-1) == '=') len--;
    int buffer = 0;
    int chr;
    int accumulatedBits = 0;
    for(i = 0; i < len; ++i) {
        chr = topaz_string_get_char(in, i);
        if (isspace(chr)) continue;
        // invalid string
        if (!(isalnum(chr) || chr == '+' || chr == '/')) {
            *size = 0;
            free(out);
            return NULL;
        }

        buffer <<= 6;
        buffer |= byteKey[chr];
        accumulatedBits += 6;
        if (accumulatedBits == 24) {
            val = (buffer & 0xff0000) >> 16;
            out[(*size)++] = val;
            val = (buffer & 0xff00) >> 8;
            out[(*size)++] = val;
            val = (buffer & 0xff);
            out[(*size)++] = val;
            buffer = 0;
            accumulatedBits = 0;            
        }
    }

    if (accumulatedBits == 12) {
        buffer >>= 4;
        val = buffer;
        out[(*size)++] = val;
    } else if (accumulatedBits == 18) {
        buffer >>= 2;        
        val = (buffer & 0xff00) >> 8;
        out[(*size)++] = val;
        val = (buffer & 0xff);
        out[(*size)++] = val;
    }

    return out;
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

    uint32_t * del = t->delimiters->codepoints;
    uint32_t * iter;    

    #define chunk_size 32
    uint32_t chunk[chunk_size];
    uint32_t chunkLen = 0;


    uint32_t c;

    // skip over leading delimiters
    for(; t->iter < t->len; t->iter++) {
        c = t->codepoints[t->iter];

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
        c = t->codepoints[t->iter];

        // delimiter marks the end.
        for(iter = del; *iter; ++iter) {
            if (*iter == c && chunkLen) {
                topaz_string_concat_codepoints(t->chain, chunk, chunkLen);                
                return t->chain;
            }
        }         

        if (chunkLen == chunk_size-1) {
            topaz_string_concat_codepoints(t->chain, chunk, chunkLen);                
            chunkLen = 0;
        }
        chunk[chunkLen++] = c;
   }

    // reached the end of the string 
    t->iter = t->len;
    topaz_string_concat_codepoints(t->chain, chunk, chunkLen);         
    return t->chain;       
}







#define topaz_string_temp_max_calls 128
static topazString_t * tempVals[topaz_string_temp_max_calls];
static int tempIter = 0;
static int tempInit = 0;

const topazString_t * topaz_string_temporary_from_c_str(const char * s) {
    if (!tempInit) {
        uint32_t i;
        for(i = 0; i < topaz_string_temp_max_calls; ++i)
            tempVals[i] = topaz_string_create();
        tempInit = 1;
    }    

    if (tempIter >= topaz_string_temp_max_calls) tempIter = 0;
    topazString_t * t = tempVals[tempIter++];
    topaz_string_set_cstr(t, s, strlen(s));
    return t;
}








