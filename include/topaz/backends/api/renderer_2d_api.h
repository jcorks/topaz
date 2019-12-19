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


#ifndef H_TOPAZDC__RENDERER_2D_API__INCLUDED
#define H_TOPAZDC__RENDERER_2D_API__INCLUDED

#include <stdint.h>
#include <topaz/backends/renderer_2d.h>


typedef struct topazRenderer_t topazRenderer_t;



/*

    Renderer_2DAPI
    -----
    
    The set of functions that define how the renderer abstraction should 
    behave. Creating renderer abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/renderer_2d.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazRenderer_2DAPI_t    topazRenderer_2DAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_2d.h>
///
struct topazRenderer_2DAPI_t {


    void                    (*renderer_2d_create)              (topazRendererAPI_t *);
    void                    (*renderer_2d_destroy)             (topazRenderer_2DAPI_t *);



    int (*renderer_2d_add_objects)(topazRenderer_2DAPI_t *, uint32_t * output, uint32_t count);
    void (*renderer_2d_remove_objects)(topazRenderer_2DAPI_t *, uint32_t * idsuint32_t count);


    void (*renderer_2d_queue_vertices)(
        topazRenderer_2D_t *,
        const uint32_t * objects,
        uint32_t count
    );

    void (*renderer_2d_clear_queue)(topazRenderer_2DAPI_t *);
    int (*renderer_2d_add_vertices)(topazRenderer_2DAPI_t *, uint32_t * output, uint32_t count);
    void (*renderer_2d_remove_vertices)(topazRenderer_2DAPI_t *, uint32_t * objects, uint32_t count);
    void (*renderer_2d_set_vertices)(
        topazRenderer_2DAPI_t *, 
        uint32_t * vertices, 
        const topazRenderer_2D_Vertex *, 
        uint32_t count
    );

    void (*renderer_2d_get_vertices)(
        topazRenderer_2DAPI_t *, 
        const uint32_t * vertexIDs, 
        topazRenderer_2D_Vertex * output,
        uint32_t count
    );



    void (*renderer_2d_set_object_params)(
        topazRenderer_2DAPI_t *, 
        uint32_t object, 
        const topazRenderer_2D_ObjectParams *
    );



    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};


#endif
