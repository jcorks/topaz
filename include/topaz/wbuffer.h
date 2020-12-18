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


#ifndef H_TOPAZDC__WBUFFER__INCLUDED
#define H_TOPAZDC__WBUFFER__INCLUDED


#include <stdint.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>

///
///    Wbuffer
///    -----
///    Wbuffers (Write Buffers) are the complement to Rbuffers. Once instantiated, the
///    user can easily and quickly write data to the buffer in a variety of
///    formats verbatim. Once the user is ready, they can write the buffer to
///    either a new file or either write over or append an existing file.
///    It is also possible to view already written data to the buffer
///    via get_data(). If they so wish, the user may also clear the entire
///    buffer that they have written to as well.
///
typedef struct topazWbuffer_t topazWbuffer_t;


/// Creates a new Wbuffer for writing.
///
topazWbuffer_t * topaz_wbuffer_create();

/// Destroys the given buffer
///
void topaz_wbuffer_destroy(topazWbuffer_t *);



/// Writes the given array of bytes to the Wbuffer.
///
void topaz_wbuffer_write_bytes(topazWbuffer_t *, const topazArray_t *);

/// Writes the given string to the Wbuffer as a C-String.
///
void topaz_wbuffer_write_c_string(topazWbuffer_t *, const topazString_t *);

/// Writes the given raw buffer to the Wbuffer.
///
void topaz_wbuffer_write_buffer(topazWbuffer_t *, const void * data, uint64_t len);


/// Writes the given addressable variable to the Wbuffer.
///
/// Example: 
///     int value = 42;
///     topaz_wbuffer_write(wbuffer, int, value);
///
///
#define topaz_wbuffer_write(__B__, __T__, __D__) (topaz_wbuffer_write_buffer(__B__, &__D__, sizeof(__T__)))



/// Set the buffer position to the specifified byte
///
void topaz_wbuffer_go_to_byte(topazWbuffer_t *, uint64_t n);

/// Returns the queued data.
///    
const topazArray_t * topaz_wbuffer_get_data(const topazWbuffer_t *);

/// Returns the size of the queued data in bytes.
///    
int topaz_wbuffer_get_size(const topazWbuffer_t *);

/// Resets the queued data buffer.
///    
void topaz_wbuffer_clear(const topazWbuffer_t *);



#endif
