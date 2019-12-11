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
#include <topaz/wbuffer.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazWbuffer_t {
    topazArray_t * data;  
    uint64_t bufferPos;
};


topazWbuffer_t * topaz_wbuffer_create() {
    topazWbuffer_t * out = calloc(1, sizeof(topazWbuffer_t));
    out->data = topaz_array_create(sizeof(uint8_t));
    return out;
}

void topaz_wbuffer_destroy(topazWbuffer_t * t) {
    topaz_array_destroy(t->data);
    free(t);
}

void topaz_wbuffer_write_bytes(topazWbuffer_t * t, const topazArray_t * src) {
    topaz_wbuffer_write_buffer(
        t,
        topaz_array_get_data(src),
        topaz_array_get_size(src)*topaz_array_get_type_size(src)
    );

}

void topaz_wbuffer_write_c_string(topazWbuffer_t * t, const topazString_t * str) {
    topaz_wbuffer_write_buffer(
        t,
        topaz_string_get_c_str (str),
        topaz_string_get_length(str)
    );
    
}

void topaz_wbuffer_write_buffer(topazWbuffer_t * t, const void * src, uint64_t lenBytes) {
    uint8_t * data = topaz_array_get_data(t->data);

    topaz_array_set_size(t->data, lenBytes + t->bufferPos);
    memcpy(
        data+t->bufferPos, 
        src,
        lenBytes
    );
    t->bufferPos += lenBytes;
}



void topaz_wbuffer_go_to_byte(topazWbuffer_t * t, uint64_t n) {
    if (n < topaz_array_get_size(t->data))
        t->bufferPos = n;
    else 
        t->bufferPos = topaz_array_get_size(t->data);
}

const topazArray_t * topaz_wbuffer_get_data(const topazWbuffer_t * t) {
    return t->data;
}

int topaz_wbuffer_get_size(const topazWbuffer_t * t) {
    return topaz_array_get_size(t->data);
}

void topaz_wbuffer_clear(const topazWbuffer_t * t) {
    topaz_array_set_size(t->data, 0);
}

