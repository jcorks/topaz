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
#include <stdlib.h>
#include <stdio.h>
#include <topaz/backends/renderer.h>
#include <topaz/system.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#if _WIN64 || _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3native.h>





typedef struct {
    GLFWwindow * window;
    int w;
    int h;
    int lockClientPosition;
    GLint program;
    GLuint vbo;
    GLint vertexLocation;
    topazDisplay_t * source;
} TopazGLFWWindow;

static void glfw_fb_size_change(
    GLFWwindow * win, 
    int w, 
    int h
) {
    TopazGLFWWindow * d = glfwGetWindowUserPointer(win);
    d->w = w;
    d->h = h;
    topaz_display_signal_resize(
        d->source,
        w, 
        h
    );
}


static void * topaz_glfw_create(topazDisplay_t * api, topaz_t * t) {

    TopazGLFWWindow * w = calloc(1, sizeof(TopazGLFWWindow));
    glfwWindowHint(GLFW_VISIBLE, 1);
    w->window = glfwCreateWindow(640, 480, "topaz", NULL, glfwGetCurrentContext());
    glfwSetFramebufferSizeCallback(w->window, glfw_fb_size_change);
    glfwSetWindowUserPointer(w->window, w);
    w->w = 640;
    w->h = 480;
    w->source  = api;


    w->program = glCreateProgram();
    GLint result;
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char * source[2];
    source[0] = "#version 100\nattribute vec2 pos;\nvarying vec2 uvs;\nuniform sampler2D tex;\nvoid main(){\nuvs = vec2((pos.x + 1.0) / 2.0, (pos.y + 1.0) / 2.0);\ngl_Position = vec4(pos.x, pos.y, 0, 1);}";
    source[1] = "#version 100\nvarying mediump vec2 uvs;\nuniform sampler2D tex;\nvoid main() {\ngl_FragColor=texture2D(tex, uvs);\n}";

    // vertex shader
    glShaderSource(
        vertexShader,
        1,
        source,
        NULL
    );
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        int logLen = 2048;
        char * log = malloc(logLen);
        glGetShaderInfoLog(
            vertexShader,
            logLen,
            NULL,
            log
        );
        printf("ES2 (Screen renderer): Vertex shader failed to compile. Log:\n%s\n", log);
        free(log);
        exit(10);
    } 
    glAttachShader(w->program, vertexShader);

    // fragment shader 
    glShaderSource(
        fragmentShader,
        1,
        source+1,
        NULL
    );
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        int logLen = 2048;
        char * log = malloc(logLen);
        glGetShaderInfoLog(
            fragmentShader,
            logLen,
            NULL,
            log
        );
        printf("ES2 (Screen renderer): Fragment shader failed to compile. Log:\n%s\n", log);
        free(log);
        exit(11);
    }
    glAttachShader(w->program, fragmentShader);


    glLinkProgram(w->program);
    glGetProgramiv(w->program, GL_LINK_STATUS, &result);
    if (!result) {
        int logLen = 2048;
        char * log = malloc(logLen);
        glGetProgramInfoLog(
            w->program,
            logLen,
            NULL,
            log
        );
        printf("ES2 (Screen renderer): Program failed to link. Log:\n%s\n", log);
        free(log);
        exit(112);
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    
    // add reference to texture
    GLint i = glGetUniformLocation(w->program, "sampler");
    w->vertexLocation = glGetAttribLocation(w->program, "pos");
    glUseProgram(w->program);
    glUniform1i(i, 0);
    glUseProgram(0);
    
    // add the static vertices
    glGenBuffers(1, &w->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, w->vbo);
    float staticVertices[] = {
        -1.f, -1.f,
         1.f, -1.f,
         1.f,  1.f,

         1.f,  1.f,
        -1.f,  1.f,
        -1.f, -1.f,
    };
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), staticVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return w;
}



static void topaz_glfw_destroy(topazDisplay_t * dispSrc, void * api) {
    TopazGLFWWindow * d = api;
    glfwDestroyWindow(d->window);
    free(d);
}


static void topaz_glfw_resize(topazDisplay_t * dispSrc, void * api, int w, int h) {
    TopazGLFWWindow * d = api;  
    glfwSetFramebufferSizeCallback(d->window, NULL);
    glfwSetWindowSize(d->window, w, h);
    glfwSetFramebufferSizeCallback(d->window, glfw_fb_size_change);
    glfw_fb_size_change(d->window, w, h);
}

static void topaz_glfw_set_position(topazDisplay_t * dispSrc, void * api, int x, int y) {
    TopazGLFWWindow * d = api;  
    glfwSetWindowPos(d->window, x, y);
}

void topaz_glfw_hide(topazDisplay_t * dispSrc, void * api, int hide) {
    TopazGLFWWindow * d = api;  
    if (hide) 
        glfwHideWindow(d->window);
    else 
        glfwShowWindow(d->window);
}


static int topaz_glfw_has_input_focus(topazDisplay_t * dispSrc, void * api) {
    TopazGLFWWindow * d = api;  
    return glfwGetWindowAttrib(d->window, GLFW_FOCUSED);    
}

static void topaz_glfw_lock_client_position(topazDisplay_t * dispSrc, void * api, int p) {
}

static void topaz_glfw_lock_client_resize(topazDisplay_t * dispSrc, void * api, int p) {
}

static int topaz_glfw_get_height(topazDisplay_t * dispSrc, void * api) {
    TopazGLFWWindow * d = api;  
    return d->h;
}

static int topaz_glfw_get_width(topazDisplay_t * dispSrc, void * api) {
    TopazGLFWWindow * d = api;  
    return d->w;
}

static int topaz_glfw_get_x(topazDisplay_t * dispSrc, void * api) {
    int x, y;    
    TopazGLFWWindow * d = api;  
    glfwGetWindowPos(d->window, &x, &y);
    return x;
}

static int topaz_glfw_get_y(topazDisplay_t * dispSrc, void * api) {
    int x, y;    
    TopazGLFWWindow * d = api;  
    glfwGetWindowPos(d->window, &x, &y);
    return y;
}

static void topaz_glfw_set_name(topazDisplay_t * dispSrc, void * api, const topazString_t * str) {
    TopazGLFWWindow * d = api;      
    glfwSetWindowTitle(d->window, topaz_string_get_c_str(str));
}


static int topaz_glfw_is_capable(topazDisplay_t * dispSrc, void * api, topazDisplay_Capability c) {
    return 1;
}


static void render_to_screen(TopazGLFWWindow * w, GLuint tex) {
    GLFWwindow * wOld = glfwGetCurrentContext();
    glfwMakeContextCurrent(w->window);
    glViewport(0, 0, w->w, w->h);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUseProgram(w->program);
    glBindBuffer(GL_ARRAY_BUFFER, w->vbo);    
    glEnableVertexAttribArray(w->vertexLocation);
    glVertexAttribPointer(w->vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    int stateBlend, 
        stateCull, 
        stateDepth;

    glGetIntegerv(GL_BLEND,      &stateBlend);
    glGetIntegerv(GL_CULL_FACE,  &stateCull);
    glGetIntegerv(GL_DEPTH_TEST, &stateDepth);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(w->vertexLocation);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (stateBlend) glEnable(stateBlend);
    if (stateCull)  glEnable(stateCull);
    if (stateDepth) glEnable(stateDepth);


    glfwMakeContextCurrent(wOld);

}

static void topaz_glfw_update(topazDisplay_t * dispSrc, void * api, topazRenderer_Framebuffer_t * fb) {
    TopazGLFWWindow * d = api;      
    glfwPollEvents();


    render_to_screen(d, ((GLuint*)topaz_renderer_framebuffer_get_handle(fb))[1]);
    glfwSwapBuffers(d->window);
};

static const topazArray_t * topaz_glfw_supported_framebuffers(topazDisplay_t * dispSrc, void * api) {
    static int e = topazRenderer_Framebuffer_Handle_GLFBPacket;
    return TOPAZ_ARRAY_CAST(&e, int, 1);
}


static topazDisplay_Handle topaz_glfw_get_system_handle_type(topazDisplay_t * dispSrc, void * api) {
    return topazDisplay_Handle_GLFWwindow;
}

static void * topaz_glfw_get_system_handle(topazDisplay_t * dispSrc, void * api) {
    TopazGLFWWindow * d = api;      
    return d->window;
}
static topazDisplay_Event topaz_glfw_get_system_event_type(topazDisplay_t * dispSrc, void * api) {
    return topazDisplay_Event_Unknown;    
}

static void * topaz_glfw_get_last_system_event(topazDisplay_t * dispSrc, void * api) {
    return 0;
}

static void topaz_glfw_fullscreen(topazDisplay_t * dispSrc, void * api, int doIt) {
    /*
    TopazGLFWWindow * d = api;      
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (doIt) {
        glfwSetWindowMonitor(
            d->window,
            glfwGetPrimaryMonitor(),
            0,
            0,
            mode->width,
            mode->height,
            mode->refreshRate
        );
    } else {
        glfwSetWindowMonitor(
            d->window,
            NULL,
            0,
            0,
            d->w,
            d->h,
            mode->refreshRate
        );
    } 
    */
}



static intptr_t api_nothing(){return 0;}

void topaz_system_display_ogles2__backend(
    topazSystem_t *         system, 
    topazSystem_Backend_t * backend, 
    topazDisplayAPI_t *     api
) {
    topaz_system_backend_bind(
        backend,
        
        // name
        TOPAZ_STR_CAST("OGLES2"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("OpenGLES2 windowing bridge using GLFW"),





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

    api->display_create = topaz_glfw_create;
    api->display_destroy = topaz_glfw_destroy;
    api->display_resize = topaz_glfw_resize;
    api->display_set_position = topaz_glfw_set_position;
    api->display_fullscreen = topaz_glfw_fullscreen;
    api->display_hide = topaz_glfw_hide;
    api->display_has_input_focus = topaz_glfw_has_input_focus;
    api->display_lock_client_resize = topaz_glfw_lock_client_resize;
    api->display_lock_client_position = topaz_glfw_lock_client_position;
    api->display_get_height = topaz_glfw_get_height;
    api->display_get_width = topaz_glfw_get_width;
    api->display_get_x = topaz_glfw_get_x;
    api->display_get_y = topaz_glfw_get_y;
    api->display_set_name = topaz_glfw_set_name;

    api->display_is_capable = topaz_glfw_is_capable;
    api->display_update = topaz_glfw_update;
    api->display_supported_framebuffers = topaz_glfw_supported_framebuffers;
    api->display_get_system_handle_type = topaz_glfw_get_system_handle_type;
    api->display_get_system_handle =  topaz_glfw_get_system_handle;
    api->display_get_system_event_type = topaz_glfw_get_system_event_type;
    api->display_get_last_system_event = topaz_glfw_get_last_system_event;
    api->display_get_current_clipboard = (topazArray_t * (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_set_current_clipboard = (void (*)(topazDisplay_t *, void *, const topazArray_t *)) api_nothing;

}




