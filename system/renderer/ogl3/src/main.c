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
#include "context.h"
#include "texture.h"
#include "2d.h"
#include "buffer.h"
#include "framebuffer.h"
#include "program.h"
#include <stdlib.h>







static void topaz_api_gl3__create(topazRendererAPI_t * api, topazRenderer_CoreAPI_t * core) {
    api->implementationData = topaz_gl3_create();
}

static void topaz_api_gl3__destroy(topazRendererAPI_t * api) {
    topaz_gl3_destroy(api->implementationData);
}

static void topaz_api_gl3__draw_2d(topazRendererAPI_t * api, void *d2, const topazRenderer_2D_Context_t * ctx, const topazRenderer_Attributes_t * attribs) {
    if (topaz_gl3_start(api->implementationData)) {
        topaz_gl3_commit_process_attribs(api->implementationData, attribs);
        topaz_gl3_2d_render(
            d2,
            ctx,
            attribs
        );
        topaz_gl3_end(api->implementationData);    
    }
}


static void topaz_api_gl3__renderer_draw_3d(
    topazRendererAPI_t * api, 
    void * vertices,
    topazArray_t * indices,

    void * program,
    void * material,

    void * sampleFramebuffer,

    void * sampleTexture0,
    void * sampleTexture1,
    void * sampleTexture2,

    void * modelviewMatrix,
    void * projectionMatrix,
    const topazRenderer_Attributes_t * attribs
) {
    if (!program) {
        #ifdef TOPAZDC_DEBUG
            assert(!"Cannot render without a program.");
        #endif
        return;
    }
    
    if (!(indices && topaz_array_get_size(indices))) {
        return;
    }

    topaz_gl3_program_update_dynamic(
        program,
        material ? topaz_gl3_buffer_get_offline_ptr(material) : NULL,
        modelviewMatrix ? topaz_gl3_buffer_get_offline_ptr(modelviewMatrix) : NULL,
        projectionMatrix ? topaz_gl3_buffer_get_offline_ptr(projectionMatrix) : NULL
    );
    
    if (sampleTexture0) {
        topaz_gl3_program_bind_texture(
            program,
            0,
            sampleTexture0
        );
    }
    if (sampleTexture1) {
        topaz_gl3_program_bind_texture(
            program,
            1,
            sampleTexture1
        );
    }
    if (sampleTexture2) {
        topaz_gl3_program_bind_texture(
            program,
            2,
            sampleTexture2
        );
    }
    


    topaz_gl3_start(api->implementationData);
    topaz_gl3_commit_process_attribs(api->implementationData, attribs);
    GLuint * ids = NULL;
    if (sampleFramebuffer)
        ids = topaz_gl3_fb_get_handle(sampleFramebuffer);
    topaz_gl3_program_render(
        program,
        vertices,
        ids ? ids[1] : 0,
        topaz_array_get_data(indices), 
        topaz_array_get_size(indices)
    );

    topaz_gl3_end(api->implementationData);
}



static void topaz_api_gl3__clear_layer(topazRendererAPI_t* api, topazRenderer_DataLayer layer) {
    topaz_gl3_fb_clear_layer(
        topaz_gl3_get_target(api->implementationData),
        layer
    );
}

topazRenderer_Parameters_t topaz_api_gl3__get_parameters(topazRendererAPI_t* nu) {
    static topazRenderer_Parameters_t p = {
        0, // variable,
        "GLSL",
    };

    return p;
}


void topaz_api_gl3__sync(topazRendererAPI_t * api) {
    topaz_gl3_sync(api->implementationData);
}

void topaz_api_gl3__attach_target(topazRendererAPI_t*api, void * fb) {
    topaz_gl3_set_target(api->implementationData, fb);
}


void * topaz_api_gl3__buffer_create(topazRendererAPI_t * api, float * data, uint32_t elements) {
    return topaz_gl3_buffer_create(data, elements);
}


void * topaz_api_gl3__texture_create(topazRendererAPI_t * api, int w, int h, const uint8_t * rgbaTextureData) {
    return topaz_gl3_texture_create(
        topaz_gl3_get_texture_manager(api->implementationData),
        w,
        h,
        rgbaTextureData
    ); 
}

void * topaz_api_gl3__2d_create(topazRendererAPI_t * api) {
    return topaz_gl3_2d_create(
        topaz_gl3_get_texture_manager(api->implementationData)
    );
}

void * topaz_api_gl3__framebuffer_create(topazRendererAPI_t * api, topazRenderer_FramebufferAPI_t * fb) {
    return topaz_gl3_fb_create();
}



 

static void * topaz_api_gl3__renderer_program_create(
    topazRendererAPI_t * apiSrc,
    const topazString_t * vtxSrc, 
    const topazString_t * fragSrc, 
    topazString_t * log
) {
    return topaz_gl3_program_create(
        vtxSrc,
        fragSrc,
        log
    );
}
static void * topaz_api_gl3__renderer_program_get_fallback(
    topazRendererAPI_t * api
) {
    
    return topaz_gl3_get_default_program(api->implementationData);
}

static void topaz_api_gl3__renderer_program_destroy(
    void * programData
) {
    
}

static topazRenderer_Buffer_t * topaz_api_gl3__renderer_program_get_global_buffer(
    void * programData
) {
    return (topazRenderer_Buffer_t*)topaz_gl3_program_get_static_buffer(programData);
}







void topaz_system_renderer_ogl3__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazRendererAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("OpenGL 3.1"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Hope it works!"),





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


    static int isInit = 0;
    static GLFWwindow * context = NULL;
    if (!isInit) {
        glfwInit();
        glfwSwapInterval(1);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_VISIBLE, 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);

        glfwWindowHint(GLFW_DEPTH_BITS, 16);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        context = glfwCreateWindow(640, 480, __FILE__, NULL, NULL);
        #ifdef TOPAZDC_DEBUG
            assert(context);
        #endif
        glfwMakeContextCurrent(context);
        int res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        #ifdef TOPAZDC_DEBUG
            assert(res == 1);
        #endif
        glGetError();

        #ifdef TOPAZDC_DEBUG
            printf("CHOSEN ATTRIBS FOR GL:\n");
            int st;
            glGetIntegerv(GL_RED_BITS, &st);     printf("R:       %d\n", st);
            glGetIntegerv(GL_GREEN_BITS, &st);   printf("G:       %d\n", st);
            glGetIntegerv(GL_BLUE_BITS, &st);    printf("B:       %d\n", st);
            glGetIntegerv(GL_DEPTH_BITS, &st);   printf("Depth:   %d\n", st);
            glGetIntegerv(GL_STENCIL_BITS, &st); printf("Stencil: %d\n", st);
        #endif

        isInit = 1;
    }
    

    // ctx
    api->core.renderer_create = topaz_api_gl3__create;
    api->core.renderer_destroy = topaz_api_gl3__destroy;
    api->core.renderer_draw_2d =topaz_api_gl3__draw_2d;
    api->core.renderer_clear_layer = topaz_api_gl3__clear_layer;
    api->core.renderer_get_parameters = topaz_api_gl3__get_parameters;
    api->core.renderer_sync = topaz_api_gl3__sync;
    api->core.renderer_attach_target = topaz_api_gl3__attach_target;


    // buffer
    api->buffer.renderer_buffer_create = topaz_api_gl3__buffer_create;
    api->buffer.renderer_buffer_destroy = (void (*)(void *)) topaz_gl3_buffer_destroy;
    api->buffer.renderer_buffer_update = (void (*)(void *, const float * newData, uint32_t offset, uint32_t numElements)) topaz_gl3_buffer_update;
    api->buffer.renderer_buffer_read = (void (*)(void *, float * ouputData, uint32_t offset, uint32_t numELements)) topaz_gl3_buffer_read;


    // textur
    api->texture.renderer_texture_create = topaz_api_gl3__texture_create;
    api->texture.renderer_texture_destroy = (void (*)(void *)) topaz_gl3_texture_destroy;
    api->texture.renderer_texture_update = (void (*)(void *, const uint8_t * newData)) topaz_gl3_texture_update;
    api->texture.renderer_texture_get = (void (*)(void *, uint8_t *)) topaz_gl3_texture_get;

    // 2D
    api->twod.renderer_2d_create = topaz_api_gl3__2d_create;
    api->twod.renderer_2d_destroy = (void (*)(void *)) topaz_gl3_2d_destroy;
    api->twod.renderer_2d_add_objects = (int (*)( void *, uint32_t * output, uint32_t count)) topaz_gl3_2d_add_objects;
    api->twod.renderer_2d_remove_objects = (void (*)( void *, uint32_t * ids, uint32_t count)) topaz_gl3_2d_remove_objects;
    api->twod.renderer_2d_queue_objects = (void (*)( void *,
        const uint32_t * objects,
        uint32_t count
    )) topaz_gl3_2d_queue_objects;;
    api->twod.renderer_2d_clear_queue = (void (*)( void *)) topaz_gl3_2d_clear_queue;
    api->twod.renderer_2d_set_object_vertices = (void (*)(
         void *, 
        uint32_t object, 
        void *
    )) topaz_gl3_2d_set_object_vertices;
    api->twod.renderer_2d_set_object_transform = (void (*)(
         void *, 
        uint32_t object, 
        const topazMatrix_t *
    )) topaz_gl3_2d_set_object_transform;
    api->twod.renderer_2d_set_object_texture = (void (*)(
         void *, 
        uint32_t object, 
        void *
    )) topaz_gl3_2d_set_object_texture;


    api->fb.renderer_framebuffer_create = topaz_api_gl3__framebuffer_create;
    api->fb.renderer_framebuffer_destroy = (void (*)(void *)) topaz_gl3_fb_destroy;
    api->fb.renderer_framebuffer_resize = (int (*)(void *, int w, int h)) topaz_gl3_fb_resize;
    api->fb.renderer_framebuffer_get_handle = (void * (*)(void *)) topaz_gl3_fb_get_handle;
    api->fb.renderer_framebuffer_get_raw_data = (int (*)(void *, uint8_t *)) topaz_gl3_get_raw_data;
    api->fb.renderer_framebuffer_set_filtered_hint = (void (*)(void *, int)) topaz_gl3_set_filtered_hint;
    api->fb.renderer_framebuffer_get_handle_type = (topazRenderer_Framebuffer_Handle (*)(void *)) topaz_gl3_fb_get_handle_type;




    // missing
    api->core.renderer_draw_3d = topaz_api_gl3__renderer_draw_3d;



    api->program.renderer_program_create = topaz_api_gl3__renderer_program_create;
    api->program.renderer_program_get_fallback = topaz_api_gl3__renderer_program_get_fallback;
    api->program.renderer_program_destroy = topaz_api_gl3__renderer_program_destroy;
    api->program.renderer_program_get_global_buffer = topaz_api_gl3__renderer_program_get_global_buffer;



}



