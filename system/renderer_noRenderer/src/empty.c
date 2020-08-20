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




#include "backend.h"
#include <topaz/version.h>







topazBackend_t * topaz_system_renderer_noRenderer__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("NoRenderer"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Placeholder for a renderer. Does nothing!"),




        // On init
        NULL,

        // On init late
        NULL,

        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );
}


static intptr_t api_nothing(){return 0;}
void topaz_system_renderer_noRenderer__api(topazRendererAPI_t * api){

    api->core.renderer_create = (void (*)(topazRenderer_CoreAPI_t*))api_nothing;
    api->core.renderer_destroy = (void (*)(topazRenderer_CoreAPI_t*))api_nothing;
    api->core.renderer_draw_2d = (void (*)(topazRenderer_CoreAPI_t*, void *, const topazRenderer_2D_Context_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    api->core.renderer_draw_3d = (void (*)(topazRenderer_CoreAPI_t *, topazRenderer_3D_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    api->core.renderer_set_3d_viewing_matrix = (void (*)(topazRenderer_CoreAPI_t *, const topazRenderer_Buffer_t *))api_nothing;
    api->core.renderer_set_3d_projection_matrix = (void (*)(topazRenderer_CoreAPI_t *, const topazRenderer_Buffer_t *))api_nothing;
    api->core.renderer_clear_layer = (void (*)(topazRenderer_CoreAPI_t*, topazRenderer_DataLayer))api_nothing;
    api->core.renderer_get_parameters = (topazRenderer_Parameters_t (*)(topazRenderer_CoreAPI_t*)) api_nothing;
    api->core.renderer_sync = (void (*)(topazRenderer_CoreAPI_t*)) api_nothing;
    api->core.renderer_attach_target = (void (*)(topazRenderer_CoreAPI_t*, topazRenderer_Framebuffer_t *)) api_nothing;
    api->core.renderer_get_supported_framebuffers = (const topazArray_t *  (*)(topazRenderer_CoreAPI_t*)) api_nothing;
    api->buffer.renderer_buffer_create = (void * (*)(topazRendererAPI_t *, float * data, uint32_t numElements)) api_nothing;
    api->buffer.renderer_buffer_destroy = (void (*)(void *)) api_nothing;
    api->buffer.renderer_buffer_update = (void (*)(void *, const float * newData, uint32_t offset, uint32_t numElements)) api_nothing;
    api->buffer.renderer_buffer_read = (void (*)(void *, float * ouputData, uint32_t offset, uint32_t numELements)) api_nothing;
    api->program.renderer_program_create = (void * (*)(topazRendererAPI_t *,
                                                                        const topazString_t *, 
                                                                        const topazString_t *, 
                                                                        topazString_t *)) api_nothing;
    api->program.renderer_program_get_preset = (void * (*)(topazRendererAPI_t *,
                                                                        topazRenderer_PresetProgram)) api_nothing;
    api->program.renderer_program_destroy = (void (*)(void *)) api_nothing;
    api->texture.renderer_texture_create = (void * (*)(topazRendererAPI_t *, int w, int h, const uint8_t * rgbaTextureData)) api_nothing;
    api->texture.renderer_texture_destroy = (void (*)(void *)) api_nothing;
    api->texture.renderer_texture_update = (void (*)(void *, const uint8_t * newData)) api_nothing;
    api->texture.renderer_texture_get = (void (*)(void *, uint8_t *)) api_nothing;
    api->twod.renderer_2d_create = (void * (*)(topazRendererAPI_t *)) api_nothing;
    api->twod.renderer_2d_destroy = (void (*)(void *)) api_nothing;
    api->twod.renderer_2d_add_objects = (int (*)( void *, uint32_t * output, uint32_t count)) api_nothing;
    api->twod.renderer_2d_remove_objects = (void (*)( void *, uint32_t * ids, uint32_t count)) api_nothing;
    api->twod.renderer_2d_queue_objects = (void (*)( void *,
        const uint32_t * objects,
        uint32_t count
    )) api_nothing;
    api->twod.renderer_2d_clear_queue = (void (*)( void *)) api_nothing;
    api->twod.renderer_2d_set_object_vertices = (void (*)(
         void *, 
        uint32_t object, 
        void *
    )) api_nothing;
    api->twod.renderer_2d_set_object_transform = (void (*)(
         void *, 
        uint32_t object, 
        const topazMatrix_t *
    )) api_nothing;
    api->twod.renderer_2d_set_object_texture = (void (*)(
         void *, 
        uint32_t object, 
        void *
    )) api_nothing;

    api->light.renderer_light_create = (void * (*)(topazRendererAPI_t *,  topazRenderer_LightType)) api_nothing;
    api->light.renderer_light_destroy = (void (*)( void *)) api_nothing;
    api->light.renderer_light_update_attribs = (void (*)(void *, float *)) api_nothing;
    api->light.renderer_light_enable = (void (*)(void *,  int doIt )) api_nothing;
    api->fb.renderer_framebuffer_create = (void * (*)(topazRendererAPI_t *, topazRenderer_FramebufferAPI_t *)) api_nothing;
    api->fb.renderer_framebuffer_destroy = (void (*)(void *)) api_nothing;
    api->fb.renderer_framebuffer_resize = (int (*)(void *, int w, int h)) api_nothing;
    api->fb.renderer_framebuffer_get_handle = (void * (*)(void *)) api_nothing;
    api->fb.renderer_framebuffer_get_raw_data = (int (*)(void *, uint8_t *)) api_nothing;
    api->fb.renderer_framebuffer_set_filtered_hint = (void (*)(void *, int)) api_nothing;
    api->fb.renderer_framebuffer_get_handle_type = (topazRenderer_Framebuffer_Handle (*)(void *)) api_nothing;


}





