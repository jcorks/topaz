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


#ifndef H_TOPAZDC__RENDERER_BUFFER__INCLUDED
#define H_TOPAZDC__RENDERER_BUFFER__INCLUDED




#include <topaz/backends/renderer.h>


/*
    RendererBuffer
    -----

*/
typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;


/// Allocates a new data store. It is not guaranteed where this data store is kept,
/// but if you are using a backend with hardware support, it is likely in VRAM.
/// If the allocation fails in some way, the id returned will be invalid
///
topazRenderer_Buffer_t * topaz_renderer_buffer_create(topazRenderer_t *, float * data, int numElements);

/// Frees the buffer store.
///
void topaz_renderer_buffer_destroy(topazRenderer_Buffer_t *);




/// Updates numElements elements of the buffer starting at the offset'th element
/// No check is made to see if this oversteps the original buffer size. If
/// overstepping occurs, the result is undefined.
///
void topaz_renderer_buffer_update(topazRenderer_Buffer_t * float * newData, int offset, int numElements);


/// Populates outputData with the relevant data contained in the buffer. On most implementations
/// this will require expensive communication, so use with caution.
///
void topaz_renderer_buffer_read(topazRenderer_Buffer_t *, float * ouputData, int offset, int numELements);

/// Returns the number of elements of the buffer.
///
int topaz_renderer_buffer_get_size(topazRenderer_Buffer_t *);




#endif