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
#include <topaz/backends/renderer.h>
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





topazBackend_t * topaz_system_display_glfw__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("GLFW"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("OpenGL/Vulkan system window bridge"),




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


typedef struct {
    GLFWwindow * window;
    int w;
    int h;
    int lockClientPosition;
} TopazGLFWWindow;


static void * topaz_glfw_create(topazDisplayAPI_t * api) {
    static int isInit = 0;
    if (!isInit) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        isInit = 1;
    }

    TopazGLFWWindow * w = calloc(1, sizeof(TopazGLFWWindow));
    w->window = glfwCreateWindow(640, 480, "topaz", NULL, NULL);
    glfwMakeContextCurrent(w->window);
    return w;
}

static void topaz_glfw_destroy(void * api) {
    TopazGLFWWindow * d = api;
    glfwDestroyWindow(d->window);
    free(d);
}


static void topaz_glfw_resize(void * api, int w, int h) {
    TopazGLFWWindow * d = api;  
    glfwSetWindowSize(d->window, w, h);
    glViewport(0, 0, w, h);
}

static void topaz_glfw_set_position(void * api, int x, int y) {
    TopazGLFWWindow * d = api;  
    glfwSetWindowPos(d->window, x, y);
}

void topaz_glfw_hide(void * api, int hide) {
    TopazGLFWWindow * d = api;  
    if (hide) 
        glfwHideWindow(d->window);
    else 
        glfwShowWindow(d->window);
}


static int topaz_glfw_has_input_focus(void * api) {
    TopazGLFWWindow * d = api;  
    return glfwGetWindowAttrib(d->window, GLFW_FOCUSED);    
}

static void topaz_glfw_lock_client_position(void * api, int p) {
}

static void topaz_glfw_lock_client_resize(void * api, int p) {
}

static int topaz_glfw_get_height(void * api) {
    TopazGLFWWindow * d = api;  
    return d->h;
}

static int topaz_glfw_get_width(void * api) {
    TopazGLFWWindow * d = api;  
    return d->w;
}

static int topaz_glfw_get_x(void * api) {
    int x, y;    
    TopazGLFWWindow * d = api;  
    glfwGetWindowPos(d->window, &x, &y);
    return x;
}

static int topaz_glfw_get_y(void * api) {
    int x, y;    
    TopazGLFWWindow * d = api;  
    glfwGetWindowPos(d->window, &x, &y);
    return y;
}

static void topaz_glfw_set_name(void * api, const topazString_t * str) {
    TopazGLFWWindow * d = api;      
    glfwSetWindowTitle(d->window, topaz_string_get_c_str(str));
}


static int topaz_glfw_is_capable(void * api, topazDisplay_Capability c) {
    return 1;
}

static void topaz_glfw_update(void * api, topazRenderer_Framebuffer_t * fb) {
    TopazGLFWWindow * d = api;      
    glfwPollEvents();
    glBindFramebuffer(GL_FRAMEBUFFER, ((GLuint*)topaz_renderer_framebuffer_get_handle(fb))[0]);
    glfwSwapBuffers(d->window);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

static const topazArray_t * topaz_glfw_supported_framebuffers(void * api) {
    static int e = topazRenderer_Framebuffer_Handle_GLFBPacket;
    return TOPAZ_ARRAY_CAST(&e, int, 1);
}


static topazDisplay_Handle topaz_glfw_get_system_handle_type(void * api) {
    #if _WIN64 || _WIN32
        return topazDisplay_Handle_WINAPIHandle;
    #endif
    #ifdef __linux__
        return topazDisplay_Handle_X11Display;
    #endif
    return topazDisplay_Handle_Unknown;
}

static void * topaz_glfw_get_system_handle(void * api) {
    TopazGLFWWindow * d = api;      
    #if _WIN64 || _WIN32
        return glfwGetWin32Window(d->window);
    #endif
    #ifdef __linux__
        return glfwGetX11Display();
    #endif
    return 0;
}

static topazDisplay_Event topaz_glfw_get_system_event_type(void * api) {
    TopazGLFWWindow * d = api;      
    #if _WIN64 || _WIN32
        return topazDisplay_event_WINAPIMsg;
    #endif
    #ifdef __linux__
        return topazDisplay_Event_X11Event;
    #endif
    return topazDisplay_Event_Unknown;
    
}

static void * topaz_glfw_get_last_system_event(void * api) {
    return 0;
}


static intptr_t api_nothing(){return 0;}
void topaz_system_display_glfw__api(topazDisplayAPI_t * api) {
    api->display_create = topaz_glfw_create;
    api->display_destroy = topaz_glfw_destroy;
    api->display_resize = topaz_glfw_resize;
    api->display_set_position = topaz_glfw_set_position;
    api->display_hide = topaz_glfw_hide;
    api->display_has_input_focus = topaz_glfw_has_input_focus;
    api->display_lock_client_resize = topaz_glfw_lock_client_resize;
    api->display_lock_client_position = topaz_glfw_lock_client_position;
    api->display_get_height = topaz_glfw_get_height;
    api->display_get_width = topaz_glfw_get_width;
    api->display_get_x = topaz_glfw_get_x;
    api->display_get_y = topaz_glfw_get_y;
    api->display_set_name = topaz_glfw_set_name;

    api->display_add_resize_callback = (void (*)(void *, void(*)(int w, int h, void *), void *)) api_nothing;
    api->display_remove_resize_callback = (void (*)(void *, void(*)(int w, int h, void *))) api_nothing;
    api->display_add_close_callback = (void (*)(void *, void(*)(void *), void *)) api_nothing;
    api->display_remove_close_callback = (void (*)(void *, void(*)(void *))) api_nothing;

    api->display_is_capable = topaz_glfw_is_capable;
    api->display_update = topaz_glfw_update;
    api->display_supported_framebuffers = topaz_glfw_supported_framebuffers;
    api->display_get_system_handle_type = topaz_glfw_get_system_handle_type;
    api->display_get_system_handle =  topaz_glfw_get_system_handle;
    api->display_get_system_event_type = topaz_glfw_get_system_event_type;
    api->display_get_last_system_event = topaz_glfw_get_last_system_event;
    api->display_get_current_clipboard = (topazArray_t * (*)(void *)) api_nothing;
    api->display_set_current_clipboard = (void (*)(void *, const topazArray_t *)) api_nothing;

}





