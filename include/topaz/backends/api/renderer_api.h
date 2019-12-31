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


#ifndef H_TOPAZDC__RENDERER_API__INCLUDED
#define H_TOPAZDC__RENDERER_API__INCLUDED

#include <stdint.h>
#include <topaz/backends/renderer.h>

typedef struct topazRenderer_t topazRenderer_t;
typedef struct topazRenderer_CoreAPI_t topazRenderer_CoreAPI_t;


/*

    RendererAPI
    -----
    
    The set of functions that define how the renderer abstraction should 
    behave. Creating renderer abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/renderer.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazRendererAPI_t    topazRendererAPI_t;




/*

    Renderer_TextureAPI
    -----
    

*/
typedef struct topazRenderer_TextureAPI_t    topazRenderer_TextureAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_texture.h>
///
struct topazRenderer_TextureAPI_t {


    void                    (*renderer_texture_create)              (topazRendererAPI_t *, topazRenderer_TextureAPI_t *,  int w, int h, const uint8_t * rgbaTextureData);
    void                    (*renderer_texture_destroy)             (topazRenderer_TextureAPI_t *);


    void                    (*renderer_texture_update)(topazRenderer_TextureAPI_t *, const uint8_t * newData);
    void                    (*renderer_texture_get)(topazRenderer_TextureAPI_t *, uint8_t *);


    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};

/*

    Renderer_ProgramAPI
    -----


*/
typedef struct topazRenderer_ProgramAPI_t    topazRenderer_ProgramAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_program.h>
///
struct topazRenderer_ProgramAPI_t {


    topazRenderer_Program_t * (*renderer_program_create)              (topazRendererAPI_t *,
                                                                        topazRenderer_ProgramAPI_t *,
                                                                        const topazString_t *, 
                                                                        const topazString_t *, 
                                                                        topazString_t *);
    topazRenderer_Program_t * (*renderer_program_create_preset)        (topazRendererAPI_t *,
                                                                        topazRenderer_ProgramAPI_t *,
                                                                        topazRenderer_PresetProgram);

    void                    (*renderer_program_destroy)             (topazRenderer_ProgramAPI_t *);



    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};











/*

    Renderer_2DAPI
    -----
    

*/
typedef struct topazRenderer_2DAPI_t    topazRenderer_2DAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_2d.h>
///
struct topazRenderer_2DAPI_t {


    void                    (*renderer_2d_create)              (topazRendererAPI_t *, topazRenderer_2DAPI_t *);
    void                    (*renderer_2d_destroy)             (topazRenderer_2DAPI_t *);



    int (*renderer_2d_add_objects)(topazRenderer_2DAPI_t *, uint32_t * output, uint32_t count);
    void (*renderer_2d_remove_objects)(topazRenderer_2DAPI_t *, uint32_t * ids, uint32_t count);


    void (*renderer_2d_queue_vertices)(
        topazRenderer_2DAPI_t *,
        const uint32_t * objects,
        uint32_t count
    );

    void (*renderer_2d_clear_queue)(topazRenderer_2DAPI_t *);
    int (*renderer_2d_add_vertices)(topazRenderer_2DAPI_t *, uint32_t * output, uint32_t count);
    void (*renderer_2d_remove_vertices)(topazRenderer_2DAPI_t *, uint32_t * objects, uint32_t count);
    void (*renderer_2d_set_vertices)(
        topazRenderer_2DAPI_t *, 
        uint32_t * vertices, 
        const topazRenderer_2D_Vertex_t *, 
        uint32_t count
    );

    void (*renderer_2d_get_vertices)(
        topazRenderer_2DAPI_t *, 
        const uint32_t * vertexIDs, 
        topazRenderer_2D_Vertex_t * output,
        uint32_t count
    );



    void (*renderer_2d_set_object_params)(
        topazRenderer_2DAPI_t *, 
        uint32_t object, 
        const topazRenderer_2D_ObjectParams_t *
    );



    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};















/*

    Renderer_LightAPI
    -----

*/
typedef struct topazRenderer_LightAPI_t    topazRenderer_LightAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_light.h>
///
struct topazRenderer_LightAPI_t {


    void                    (*renderer_light_create)              (topazRendererAPI_t *, topazRenderer_LightAPI_t *, topazRenderer_LightType);
    void                    (*renderer_light_destroy)             (topazRenderer_LightAPI_t *);

    void (*renderer_light_update_attribs)(topazRenderer_LightAPI_t *, float *);
    void (*renderer_light_enable)(topazRenderer_LightAPI_t *, int doIt);


    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};













/*

    Renderer_BufferAPI
    -----


*/
typedef struct topazRenderer_BufferAPI_t    topazRenderer_BufferAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_buffer.h>
///
struct topazRenderer_BufferAPI_t {


    void                    (*renderer_buffer_create)              (topazRendererAPI_t *, topazRenderer_BufferAPI_t *, float * data, int numElements);
    void                    (*renderer_buffer_destroy)             (topazRenderer_BufferAPI_t *);


    void                    (*renderer_buffer_update)               (topazRenderer_BufferAPI_t *, float * newData, int offset, int numElements);
    void                    (*renderer_buffer_read)                 (topazRenderer_BufferAPI_t *, float * ouputData, int offset, int numELements);


    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};






/*

    Renderer_FramebufferAPI
    -----


*/
typedef struct topazRenderer_FramebufferAPI_t    topazRenderer_FramebufferAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_buffer.h>
///
struct topazRenderer_FramebufferAPI_t {


    void                    (*renderer_framebuffer_create)              (topazRendererAPI_t *, topazRenderer_FramebufferAPI_t *);
    void                    (*renderer_framebuffer_destroy)             (topazRenderer_FramebufferAPI_t *);


    int      (*renderer_framebuffer_resize)(topazRenderer_FramebufferAPI_t *, int w, int h);
    void *     (*renderer_framebuffer_get_handle)(topazRenderer_FramebufferAPI_t *);
    int      (*renderer_framebuffer_get_raw_data)(topazRenderer_FramebufferAPI_t *, uint8_t *);
    void     (*renderer_framebuffer_set_filtered_hint)(topazRenderer_FramebufferAPI_t *, int);
    topazRenderer_Framebuffer_Handle (*renderer_framebuffer_get_handle_type)(topazRenderer_FramebufferAPI_t *);

    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};








/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/Renderer.h>
///
typedef struct topazRenderer_CoreAPI topazRenderer_CoreAPI;

struct topazRenderer_CoreAPI_t {


    void                    (*renderer_create)              (topazRenderer_CoreAPI_t *);
    void                    (*renderer_destroy)             (topazRenderer_CoreAPI_t *);




    void                    (*renderer_draw_2d)             (topazRenderer_CoreAPI_t *, topazRenderer_2DAPI_t *, const topazRenderer_2D_Context_t *, const topazRenderer_ProcessAttribs_t *);
    void                    (*renderer_draw_3d)             (topazRenderer_CoreAPI_t *, topazRenderer_3D_t *, const topazRenderer_ProcessAttribs_t *);
    void                    (*renderer_set_3d_viewing_matrix)(topazRenderer_CoreAPI_t *, const topazMatrix_t *);
    void                    (*renderer_set_3d_projection_matrix)(topazRenderer_CoreAPI_t *, const topazMatrix_t *);


    void                    (*renderer_clear_data)          (topazRenderer_CoreAPI_t *);
    void                    (*renderer_clear_layer)         (topazRenderer_CoreAPI_t *, topazRenderer_DataLayer);

    topazRenderer_Parameters_t(*renderer_get_parameters)      (topazRenderer_CoreAPI_t *);


    void                    (*renderer_sync)                (topazRenderer_CoreAPI_t *);
    void                    (*renderer_attach_target)       (topazRenderer_CoreAPI_t *, topazRenderer_Framebuffer_t *);
    const topazArray_t *    (*renderer_get_supported_framebuffers)(topazRenderer_CoreAPI_t *);

    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};





/// composite struct composed of all the subAPIs that 
/// together for the renderer.
///
struct topazRendererAPI_t {
    /// API for the renderer direct functions    
    ///
    topazRenderer_CoreAPI_t core;

    /// API for renderer buffers, which provide renderer 
    /// storage.
    ///
    topazRenderer_BufferAPI_t buffer;

    /// API for renderer programs, which provide a way to 
    /// generate dynamic renderer programs such as shaders.
    ///
    topazRenderer_ProgramAPI_t program;

    /// API for renderer textures, which provide image data.
    /// 
    topazRenderer_TextureAPI_t texture;

    /// API for the 2d renderer, which provides operations optimized 
    /// for 2d geometry (small batch, dynamic visuals)
    ///
    topazRenderer_2DAPI_t twod;

    /// API for the light renderer, which provides lighting control 
    /// for 3d scenes.
    ///
    topazRenderer_LightAPI_t light;

    /// API for the renderer framebuffer, which is the target of rendering 
    /// operations.
    ///
    topazRenderer_FramebufferAPI_t fb;
};



#endif