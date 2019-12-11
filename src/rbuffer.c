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
#include <topaz/rbuffer.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

struct topazRbuffer_t {
    uint8_t * buffer;
    uint64_t bufferPos;
    uint64_t size;

    topazString_t * readString;
    topazArray_t * readBytes;
    uint8_t * readOverflow;
};



topazRbuffer_t * topaz_rbuffer_create() {
    topazRbuffer_t * out = calloc(1, sizeof(topazRbuffer_t));
    return out;
}

void topaz_rbuffer_destroy(topazRbuffer_t * t) {
    if (t->readString) topaz_string_destroy(t->readString);
    if (t->readBytes) topaz_array_destroy(t->readBytes);
    free(t->readOverflow);
    free(t->buffer);
    free(t);
}


void topaz_rbuffer_open(topazRbuffer_t * t, const uint8_t * data, uint64_t size) {
    free(t->buffer);
    t->buffer = malloc(size);
    memcpy(t->buffer, data, size);
    t->bufferPos = 0;
    t->size = size;
}


const topazString_t * topaz_rbuffer_read_string(topazRbuffer_t * t, uint64_t nBytes) {
    if (!t->readString) {
        t->readString = topaz_string_create();
    }

    topaz_string_clear(t->readString);

    if (t->bufferPos == t->size) {
        return t->readString;        
    }

    uint64_t realEnd = t->bufferPos + nBytes < t->size ? t->bufferPos + nBytes : t->size-1;    
    char end = (char)t->buffer[realEnd];
    t->buffer[realEnd] = 0;
    topaz_string_concat_printf(t->readString, "%s", t->buffer);
    t->buffer[realEnd] = end;
    if (realEnd == t->size-1) {
        topaz_string_concat_printf(t->readString, "%c", end);
        t->bufferPos = t->size; // end
    } else {
        t->bufferPos = realEnd;
    }
    return t->readString;
}

const topazArray_t * topaz_rbuffer_read_bytes(topazRbuffer_t * t, uint64_t nBytes) {
    if (!t->readBytes) {
        t->readBytes = topaz_array_create(sizeof(uint8_t));
    }

    topaz_array_set_size(t->readBytes, 0);

    if (t->bufferPos == t->size) {
        return t->readBytes;
    }

    uint64_t realSize = t->bufferPos + nBytes < t->size ? nBytes : (t->size - t->bufferPos);
    topaz_array_set_size(t->readBytes, realSize);
    memcpy(topaz_array_get_data(t->readBytes), t->buffer+t->bufferPos, realSize);
    t->bufferPos += realSize;
    return t->readBytes;
}


const void * topaz_rbuffer_get_buffer(topazRbuffer_t * t, uint64_t numBytes) {
    if (t->bufferPos + numBytes >= t->size) {
        return NULL;
    }

    const void * output = t->buffer+t->bufferPos;
    t->bufferPos += numBytes;
    return output;
}


// same as getPtr but does not return null;
const void * topaz_rbuffer_get_buffer_nnull(topazRbuffer_t * t, uint64_t numBytes) {
    if (t->bufferPos + numBytes >= t->size) {
        free(t->readOverflow);
        t->readOverflow = calloc(1, numBytes);
        t->bufferPos = t->size;
        return t->readOverflow;
    }

    const void * output = t->buffer+t->bufferPos;
    t->bufferPos += numBytes;
    return output;
}



void topaz_rbuffer_go_to_byte(topazRbuffer_t * t, uint64_t i) {
    if (i < t->size)
        t->bufferPos = i;
    else 
        t->bufferPos = t->size;
}


uint64_t topaz_rbuffer_get_size(const topazRbuffer_t * t) {
    return t->size;
}

uint64_t topaz_rbuffer_get_bytes_left(const topazRbuffer_t * t) {
    return t->size - t->bufferPos;
}

int topaz_rbuffer_is_empty(const topazRbuffer_t * t) {
    return t->size == t->bufferPos;
}






