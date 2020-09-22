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
#include "texture.h"
#include <stdlib.h>





topazBackend_t * topaz_system_renderer_ogles2__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("OpenGLES 2"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Hope it works!"),




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


static intptr_t api_nothing(){return 0;}
void topaz_system_renderer_ogles2__api(topazRendererAPI_t * api){
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


        topaz_es2_texture_destroy(t0);
        topaz_es2_texture_destroy(t1);
        topaz_es2_texture_destroy(t2);
           
    }


    api->core.renderer_create = (void (*)(topazRendererAPI_t *, topazRenderer_CoreAPI_t *))api_nothing;
    api->core.renderer_destroy = (void (*)(topazRendererAPI_t*))api_nothing;
    api->core.renderer_draw_2d = (void (*)(topazRendererAPI_t*, void *, const topazRenderer_2D_Context_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    api->core.renderer_draw_3d = (void (*)(topazRendererAPI_t *, topazRenderer_3D_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    api->core.renderer_set_3d_viewing_matrix = (void (*)(topazRendererAPI_t *, void *))api_nothing;
    api->core.renderer_set_3d_projection_matrix = (void (*)(topazRendererAPI_t *, void *))api_nothing;
    api->core.renderer_clear_layer = (void (*)(topazRendererAPI_t*, topazRenderer_DataLayer))api_nothing;
    api->core.renderer_get_parameters = (topazRenderer_Parameters_t (*)(topazRendererAPI_t*)) api_nothing;
    api->core.renderer_sync = (void (*)(topazRendererAPI_t*)) api_nothing;
    api->core.renderer_attach_target = (void (*)(topazRendererAPI_t*, void *)) api_nothing;
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





