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


#ifndef H_TOPAZDC__RENDERER_BUFFER_API__INCLUDED
#define H_TOPAZDC__RENDERER_BUFFER_API__INCLUDED

#include <stdint.h>
#include <topaz/backends/renderer_buffer.h>


typedef struct topazRenderer_t topazRenderer_t;



/*

    Renderer_BufferAPI
    -----
    
    The set of functions that define how the renderer abstraction should 
    behave. Creating renderer abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/renderer_buffer.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazRenderer_BufferAPI_t    topazRenderer_BufferAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_buffer.h>
///
struct topazRenderer_BufferAPI_t {


    void                    (*renderer_buffer_create)              (topazRendererAPI_t *);
    void                    (*renderer_buffer_destroy)             (topazRenderer_BufferAPI_t *);


    void                    (*renderer_buffer_update)               (topazRenderer_BufferAPI_t * float * newData, int offset, int numElements);
    void                    (*renderer_buffer_read)                 (topazRenderer_BufferAPI_t *, float * ouputData, int offset, int numELements);

    int                     (*renderer_buffer_get_size)             (topazRenderer_BufferAPI_t *);


    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};


#endif
