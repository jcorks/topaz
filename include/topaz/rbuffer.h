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


#ifndef H_TOPAZDC__RBUFFER__INCLUDED
#define H_TOPAZDC__RBUFFER__INCLUDED


#include <stdint.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>


///
///    Rbuffer ("Read Buffer") handles input data. Once instantiated
///    with a target, the Rbuffer can then easily read and process binary data.
///    Rbuffers are the most effective for unpacking data, especially data
///    that is packed in accordance to OutBuffer routines, as all
///    OutBuffer writing functions mirror the reading functions of Rbuffer.
///
typedef struct topazRbuffer_t topazRbuffer_t;

/// Creates a new, empty input buffer 
///
topazRbuffer_t * topaz_rbuffer_create();

/// Destroys an input buffer 
///
void topaz_rbuffer_destroy(
    /// The Rbuffer to destroy.
    topazRbuffer_t * r
);


///  Opens a raw buffer to read from.
///
void topaz_rbuffer_open(
    /// The Rbuffer to modify. Any prior Rbuffer information 
    /// is cleared.
    topazRbuffer_t * r, 

    /// The data buffer of raw bytes to read from.
    /// The data is copied into the rbuffer.
    const uint8_t * data, 

    /// Number of bytes of the data buffer to read validly.
    uint64_t size
);


/// Returns the next nBytes as a topazString.
/// The string is valid until the next call to this function with this buffer.
///
const topazString_t * topaz_rbuffer_read_string(
    /// The rbuffer to read from.
    topazRbuffer_t * r, 

    /// The number of bytes to read.
    uint64_t nBytes
);

/// Returns the next n bytes.
/// 
/// If there are less than numBytes remaining,
/// the remaining bytes are returned.
/// The array is valid until the next call to this function with this buffer.
const topazArray_t * topaz_rbuffer_read_bytes(
    /// The rbuffer to read from.
    topazRbuffer_t * r, 

    /// Byte count to read.
    uint64_t numBytes
);


/// Returns a pointer to raw data 
/// of length numBytes. If there isnt enough space to service 
/// numBytes, NULL is returned.
///
const void * topaz_rbuffer_get_buffer(
    /// The rbuffer to query.
    topazRbuffer_t * r, 

    /// The number of bytes to read validly.
    uint64_t numBytes
);


/// Similar to topaz_rbuffer_get_buffer, but 
/// in the case that numBytes exceeds size, an 
/// empty buffer is returned rather than NULL.
///
const void * topaz_rbuffer_get_buffer_nnull(
    /// The rbuffer to query.
    topazRbuffer_t * r, 

    /// The number of bytes to read validly.
    uint64_t numBytes
);



/// Reads the next sizeof(T) bytes and returns a T object with
/// its state set to the read contents.
///
/// Note: If used with user objects, be aware that this only performs a shallow copy
/// If not enough bytes are left to acoomodate the data object, the contents are undefined.
///
/// __B__ is the buffer.
/// __T__ is the type.
///
#define topaz_rbuffer_read(__B__,__T__) (*(T*)topaz_rbuffer_get_buffer_nnull(__B__, sizeof(__T__))) 


/// Set the buffer position byte.
///
/// If this operation were to go beyond the last valid index,
/// it will move to the very last index.
void topaz_rbuffer_go_to_byte(
    /// The rbuffer to seek within.
    topazRbuffer_t * r, 

    /// The position in bytes to seet to.
    uint64_t byte
);


/// returns the size of the buffer in bytes.
///
uint64_t topaz_rbuffer_get_size(
    /// The rbuffer to query.
    const topazRbuffer_t * r
);

/// Returns the number of bytes left in the buffer.
///
uint64_t topaz_rbuffer_get_bytes_left(
    /// The rbuffer to query.
    const topazRbuffer_t * r
);

/// Returns whether or not all data has been read.
///
int topaz_rbuffer_is_empty(
    /// The rbuffer to query. 
    const topazRbuffer_t * r
);


#endif
