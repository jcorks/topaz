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
#include <stdlib.h>






/*
#include <stdio.h>
#include <stdlib.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

static const GLuint WIDTH = 800;
static const GLuint HEIGHT = 600;
static const GLchar* vertex_shader_source =
    "#version 100\n"
    "attribute vec3 position;\n"
    "void main() {\n"
    "   gl_Position = vec4(position, 1.0);\n"
    "}\n";
static const GLchar* fragment_shader_source =
    "#version 100\n"
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";
static const GLfloat vertices[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

static GLint common_get_shader_program(const char *vertex_shader_source, const char *fragment_shader_source) {
    enum Consts {INFOLOG_LEN = 512};
    GLchar infoLog[INFOLOG_LEN];
    GLint fragment_shader;
    GLint shader_program;
    GLint success;
    GLint vertex_shader;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

static int smain(void) {
    GLuint shader_program, vbo;
    GLint pos;
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(window);

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

    shader_program = common_get_shader_program(vertex_shader_source, fragment_shader_source);
    pos = glGetAttribLocation(shader_program, "position");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, WIDTH, HEIGHT);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return EXIT_SUCCESS;
}
*/

/*
static uint32_t create_test_object(topazES2_2D_t * t, float x, float y, float r) {
    uint32_t object;
    topaz_es2_2d_add_objects(t, &object, 1);
    topazRenderer_2D_Vertex_t v[] = {
        {x+0, y+0,   r, 0, 1, 1,   0, 0},
        {x+100, y+0,   r, 0, 1, 1,   0, 0},
        {x+100, y+100,   r, 0, 1, 1,   0, 0},

        {x+100, y+100,   r, 0, 1, 1,   0, 0},
        {x+0, y+100,   r, 0, 1, 1,   0, 0},
        {x+0, y+0,   r, 0, 1, 1,   0, 0}
    };

    topazES2_Buffer_t * b = topaz_es2_buffer_create((float*)v, 6*8);
    topaz_es2_2d_set_object_vertices(t, object, b);
    return object;
}

    {
        GLFWwindow* window;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        window = glfwCreateWindow(640, 480, __FILE__, NULL, NULL);
        glfwMakeContextCurrent(window);

        printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
        printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );




        uint8_t * iter = 0;
        int i;
        topazES2_TexMan_t * a = topaz_es2_texman_create();


        uint8_t * raw0 = calloc(8*8 * 4, 1);
        i = 0;
        for(i = 0, iter = raw0; i < 8*8; ++i) {iter[0] = 128; iter[3] = 255; iter+=4;}


        uint8_t * raw1 = calloc(16*16 * 4, 1);
        i = 0;
        for(i = 0, iter = raw1; i < 16*16; ++i) { iter[0] = 255; iter[3] = 255; iter+=4;}

        uint8_t * raw2 = calloc(4*4 * 4, 1);
        i = 0;
        for(i = 0, iter = raw2; i < 4*4; ++i) {iter[0] = 64; iter[3] = 255; iter+=4;}


        topazES2_Texture_t * t0 = topaz_es2_texture_create(a, 8, 8, raw0);
        topaz_es2_texman_print_section(
            a, 0, 0, 64, 64, 0
        );


        topazES2_Texture_t * t1 = topaz_es2_texture_create(a, 16, 16, raw1);
        topaz_es2_texman_print_section(
            a, 0, 0, 64, 64, 0
        );


        topazES2_Texture_t * t2 = topaz_es2_texture_create(a, 4, 4, raw2);
        topaz_es2_texman_print_section(
            a, 0, 0, 64, 64, 0
        );

        topazES2_2D_t * t = topaz_es2_2d_create(a);

        {
            uint32_t objects[32];
            uint32_t i;
            for(i = 0; i < 32; ++i) {
                objects[i] = create_test_object(t, i*14, i*6, i*4 / (255.0));
            }


            topaz_es2_2d_queue_objects(t, objects, 32);


            topazRenderer_2D_Context_t ctx;
            topazMatrix_t mtransform;
            ctx.width = 640;
            ctx.height = 480;
            ctx.transform = &mtransform;
            topaz_matrix_set_identity(ctx.transform);

            

            topazRenderer_ProcessAttribs_t attribs = {
                topazRenderer_Primitive_Triangle,
                topazRenderer_DepthTest_None,
                topazRenderer_AlphaRule_Allow,
                topazRenderer_EtchRule_NoEtching,

                topazRenderer_TextureFilterHint_None
            };

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glViewport(0, 0, 640, 480);

            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                glClear(GL_COLOR_BUFFER_BIT);

                topaz_es2_2d_render(
                    t,
                    &ctx,
                    &attribs
                );




                glfwSwapBuffers(window);

            }
        }


        topaz_es2_texture_destroy(t0);
        topaz_es2_texture_destroy(t1);
        topaz_es2_texture_destroy(t2);
           
    }
*/

static intptr_t api_nothing(){return 0;}



static void topaz_api_es2__create(topazRendererAPI_t * api, topazRenderer_CoreAPI_t * core) {
    api->implementationData = topaz_es2_create();
}

static void topaz_api_es2__destroy(topazRendererAPI_t * api, topazRenderer_CoreAPI_t * core) {
    topaz_es2_destroy(api->implementationData);
}

static void topaz_api_es2__draw_2d(topazRendererAPI_t * api, void *d2, const topazRenderer_2D_Context_t * ctx, const topazRenderer_ProcessAttribs_t * attribs) {
    topaz_es2_start(api->implementationData);
    topaz_es2_2d_render(
        d2,
        ctx,
        attribs
    );
    topaz_es2_end(api->implementationData);    
}

static void topaz_api_es2__clear_layer(topazRendererAPI_t* api, topazRenderer_DataLayer layer) {
    topaz_es2_fb_clear_layer(
        topaz_es2_get_target(api->implementationData),
        layer
    );
}

topazRenderer_Parameters_t topaz_api_es2__get_parameters(topazRendererAPI_t* nu) {
    static topazRenderer_Parameters_t p = {
        0, // variable,
        0,
        0,
        "GLSL",
    };

    return p;
}


void topaz_api_es2__sync(topazRendererAPI_t * api) {
    topaz_es2_sync(api->implementationData);
}

void topaz_api_es2__attach_target(topazRendererAPI_t*api, void * fb) {
    topaz_es2_set_target(api->implementationData, fb);
}


void * topaz_api_es2__buffer_create(topazRendererAPI_t * api, float * data, uint32_t elements) {
    return topaz_es2_buffer_create(data, elements);
}


void * topaz_api_es2__texture_create(topazRendererAPI_t * api, int w, int h, const uint8_t * rgbaTextureData) {
    return topaz_es2_texture_create(
        topaz_es2_get_texture_manager(api->implementationData),
        w,
        h,
        rgbaTextureData
    ); 
}

void * topaz_api_es2__2d_create(topazRendererAPI_t * api) {
    return topaz_es2_2d_create(
        topaz_es2_get_texture_manager(api->implementationData)
    );
}

void * topaz_api_es2__framebuffer_create(topazRendererAPI_t * api, topazRenderer_FramebufferAPI_t * fb) {
    return topaz_es2_fb_create();
}



void topaz_system_renderer_ogles2__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazRendererAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("OpenGLES 2"),

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
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_VISIBLE, 0);

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
        isInit = 1;
    }
    

    // ctx
    api->core.renderer_create = topaz_api_es2__create;
    api->core.renderer_destroy = topaz_api_es2__destroy;
    api->core.renderer_draw_2d =topaz_api_es2__draw_2d;
    api->core.renderer_clear_layer = topaz_api_es2__clear_layer;
    api->core.renderer_get_parameters = topaz_api_es2__get_parameters;
    api->core.renderer_sync = topaz_api_es2__sync;
    api->core.renderer_attach_target = topaz_api_es2__attach_target;


    // buffer
    api->buffer.renderer_buffer_create = topaz_api_es2__buffer_create;
    api->buffer.renderer_buffer_destroy = (void (*)(void *)) topaz_es2_buffer_destroy;
    api->buffer.renderer_buffer_update = (void (*)(void *, const float * newData, uint32_t offset, uint32_t numElements)) topaz_es2_buffer_update;
    api->buffer.renderer_buffer_read = (void (*)(void *, float * ouputData, uint32_t offset, uint32_t numELements)) topaz_es2_buffer_read;


    // textur
    api->texture.renderer_texture_create = topaz_api_es2__texture_create;
    api->texture.renderer_texture_destroy = (void (*)(void *)) topaz_es2_texture_destroy;
    api->texture.renderer_texture_update = (void (*)(void *, const uint8_t * newData)) topaz_es2_texture_update;
    api->texture.renderer_texture_get = (void (*)(void *, uint8_t *)) topaz_es2_texture_get;

    // 2D
    api->twod.renderer_2d_create = topaz_api_es2__2d_create;
    api->twod.renderer_2d_destroy = (void (*)(void *)) topaz_es2_2d_destroy;
    api->twod.renderer_2d_add_objects = (int (*)( void *, uint32_t * output, uint32_t count)) topaz_es2_2d_add_objects;
    api->twod.renderer_2d_remove_objects = (void (*)( void *, uint32_t * ids, uint32_t count)) topaz_es2_2d_remove_objects;
    api->twod.renderer_2d_queue_objects = (void (*)( void *,
        const uint32_t * objects,
        uint32_t count
    )) topaz_es2_2d_queue_objects;;
    api->twod.renderer_2d_clear_queue = (void (*)( void *)) topaz_es2_2d_clear_queue;
    api->twod.renderer_2d_set_object_vertices = (void (*)(
         void *, 
        uint32_t object, 
        void *
    )) topaz_es2_2d_set_object_vertices;
    api->twod.renderer_2d_set_object_transform = (void (*)(
         void *, 
        uint32_t object, 
        const topazMatrix_t *
    )) topaz_es2_2d_set_object_transform;
    api->twod.renderer_2d_set_object_texture = (void (*)(
         void *, 
        uint32_t object, 
        void *
    )) topaz_es2_2d_set_object_texture;


    api->fb.renderer_framebuffer_create = topaz_api_es2__framebuffer_create;
    api->fb.renderer_framebuffer_destroy = (void (*)(void *)) topaz_es2_fb_destroy;
    api->fb.renderer_framebuffer_resize = (int (*)(void *, int w, int h)) topaz_es2_fb_resize;
    api->fb.renderer_framebuffer_get_handle = (void * (*)(void *)) topaz_es2_fb_get_handle;
    api->fb.renderer_framebuffer_get_raw_data = (int (*)(void *, uint8_t *)) topaz_es2_get_raw_data;
    api->fb.renderer_framebuffer_set_filtered_hint = (void (*)(void *, int)) topaz_es2_set_filtered_hint;
    api->fb.renderer_framebuffer_get_handle_type = (topazRenderer_Framebuffer_Handle (*)(void *)) topaz_es2_fb_get_handle_type;




    // missing
    api->core.renderer_draw_3d = (void (*)(topazRendererAPI_t *, topazRenderer_3D_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    api->core.renderer_set_3d_viewing_matrix = (void (*)(topazRendererAPI_t *, void *))api_nothing;
    api->core.renderer_set_3d_projection_matrix = (void (*)(topazRendererAPI_t *, void *))api_nothing;


    api->light.renderer_light_create = (void * (*)(topazRendererAPI_t *,  topazRenderer_LightType)) api_nothing;
    api->light.renderer_light_destroy = (void (*)( void *)) api_nothing;
    api->light.renderer_light_update_attribs = (void (*)(void *, float *)) api_nothing;
    api->light.renderer_light_enable = (void (*)(void *,  int doIt )) api_nothing;

    api->program.renderer_program_create = (void * (*)(topazRendererAPI_t *,
                                                                        const topazString_t *, 
                                                                        const topazString_t *, 
                                                                        topazString_t *)) api_nothing;
    api->program.renderer_program_get_preset = (void * (*)(topazRendererAPI_t *,
                                                                        topazRenderer_PresetProgram)) api_nothing;
    api->program.renderer_program_destroy = (void (*)(void *)) api_nothing;



}



