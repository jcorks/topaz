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
#include <topaz/modules/console.h>
#include <topaz/topaz.h>
#include <topaz/system.h>
#include "glad.h"
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
    float w;
    float h;
    float x;
    float y;
    int lockClientPosition;
    GLint program;
    GLuint vbo;
    GLint vertexLocation;
    GLint textureSizeLocation;
    GLuint vao;
    topazDisplay_t * source;
} TopazGLFWWindow;

static void glfw_fb_size_change(
    GLFWwindow * win, 
    int w, 
    int h
) {
    TopazGLFWWindow * d = glfwGetWindowUserPointer(win);
    if ((int)d->w != (int)w) {
        d->w = w;
    }
    topaz_display_signal_parameter_change(
        d->source,
        topazDisplay_Parameter_Width,
        d->w
    );
    if ((int)d->h != (int)h) {
        d->h = h;
    }
    topaz_display_signal_parameter_change(
        d->source,
        topazDisplay_Parameter_Height,

        d->h
    );

}

static void glfw_fb_pos_change(
    GLFWwindow * win, 
    int x, 
    int y
) {
    TopazGLFWWindow * d = glfwGetWindowUserPointer(win);
    if ((int)d->x != (int)x) {
        d->x = x;
    }
    topaz_display_signal_parameter_change(
        d->source,
        topazDisplay_Parameter_X,
        d->x
    );
    if ((int)d->y != (int)y) {
        d->y = y;
    }
    topaz_display_signal_parameter_change(
        d->source,
        topazDisplay_Parameter_Y,

        d->y
    );
}

static void glfw_fb_focus_change(
    GLFWwindow * win, 
    int focused
) {
    TopazGLFWWindow * d = glfwGetWindowUserPointer(win);
    topaz_display_signal_parameter_change(
        d->source,
        topazDisplay_Parameter_InputFocus,
        focused
    );
}


static void glfw_fb_close(
    GLFWwindow * win
) {
    TopazGLFWWindow * d = glfwGetWindowUserPointer(win);
    topaz_display_signal_close(d->source);
}

topazString_t * topaz_glfw_set_post_process_shader(
    /// The display to modify.
    topazDisplay_t * dispSrc,

    void * api,

    /// The text for the vertex shader.
    const topazString_t * vertexSrc,

    /// The text for the fragment shader.
    const topazString_t * fragmentSrc

) {
    TopazGLFWWindow * w = api;

    if (w->program != -1) {
        glUseProgram(0);
        glDeleteProgram(w->program);
    }

    w->program = glCreateProgram();
    GLint result;
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char * source[2];
    topazString_t * vertexInput   = topaz_string_create_from_c_str("#define VERTEX\n%s", topaz_string_get_c_str(vertexSrc));
    topazString_t * fragmentInput = topaz_string_create_from_c_str("#define FRAGMENT\n%s", topaz_string_get_c_str(fragmentSrc));
    source[0] = topaz_string_get_c_str(vertexInput);
    source[1] = topaz_string_get_c_str(fragmentInput);

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
        topazString_t * output = topaz_string_create_from_c_str("GL3 (Screen renderer): Vertex shader failed to compile. Log:\n%s\n", log);
        free(log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);        
        glDeleteProgram(w->program);
        w->program = -1;
        return output;
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
        topazString_t * output = topaz_string_create_from_c_str("GL3 (Screen renderer): Fragment shader failed to compile. Log:\n%s\n", log);
        free(log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);        
        glDeleteProgram(w->program);
        w->program = -1;
        return output;
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
        topazString_t * output = topaz_string_create_from_c_str("GL3 (Screen renderer): Program failed to link. Log:\n%s\n", log);
        free(log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);        
        glDeleteProgram(w->program);
        w->program = -1;
        return output;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    
    // add reference to texture
    GLint i = glGetUniformLocation(w->program, "Texture");
    w->textureSizeLocation = glGetUniformLocation(w->program, "TextureSize");
    w->vertexLocation = glGetAttribLocation(w->program, "VertexCoord");
    glUseProgram(w->program);
    glUniform1i(i, 0);

    glUseProgram(0);
    return NULL;
}


static void * topaz_glfw_create(topazDisplay_t * api, topaz_t * t) {

    TopazGLFWWindow * w = calloc(1, sizeof(TopazGLFWWindow));
    glfwWindowHint(GLFW_VISIBLE, 1);

    GLFWwindow * wOld = glfwGetCurrentContext();
    w->window = glfwCreateWindow(640, 480, "topaz", NULL, glfwGetCurrentContext());
    w->program = -1;
    glfwSetFramebufferSizeCallback(w->window, glfw_fb_size_change);
    glfwSetWindowFocusCallback(w->window, glfw_fb_focus_change);

    glfwSetWindowCloseCallback(w->window, glfw_fb_close);
    glfwSetWindowUserPointer(w->window, w);

    glfwMakeContextCurrent(w->window);
    w->source  = api;


    w->w = 64;
    w->h = 64;
    w->x = w->y = 0;

    topazString_t * vert = topaz_string_create_from_c_str("%s", "in vec2 VertexCoord;\nout vec2 uvs;\nuniform sampler2D Texture;\nvoid main(){\nuvs = vec2((VertexCoord.x + 1.0) / 2.0, (VertexCoord.y + 1.0) / 2.0);\ngl_Position = vec4(VertexCoord, 0, 1);}");
    topazString_t * frag = topaz_string_create_from_c_str("%s", "in vec2 uvs;\nuniform sampler2D Texture;\nvoid main() {\ngl_FragColor=texture2D(Texture, uvs);\n}");


    topazString_t * error = topaz_glfw_set_post_process_shader(
        api, 
        w,
        vert,
        frag
    );
    glGenVertexArrays(1, &w->vao);
    glBindVertexArray(w->vao);
    glEnableVertexAttribArray(w->vertexLocation);

    if (error) {
        topaz_console_print(
            topaz_context_get_console(t),
            error
        );
    }

    topaz_string_destroy(vert);
    topaz_string_destroy(frag);
    
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
    glBindVertexArray(0);
    glfwMakeContextCurrent(wOld);


    int x, y;    
    glfwGetWindowPos(w->window, &x, &y);
    w->x = x;
    w->y = y;
    topaz_display_signal_parameter_change(api, topazDisplay_Parameter_X, x);
    topaz_display_signal_parameter_change(api, topazDisplay_Parameter_Y, y);

    return w;
}

static void topaz_glfw_destroy(topazDisplay_t * dispSrc, void * api) {
    TopazGLFWWindow * d = api;
    glfwDestroyWindow(d->window);
    free(d);
}


static void topaz_glfw_request_parameter_change(
    topazDisplay_t * dispSrc, 
    void * api, 
    topazDisplay_Parameter param, 
    float value
) {
    TopazGLFWWindow * d = api;  

    switch(param) {
      case topazDisplay_Parameter_Width:
        d->w = value;
        glfwSetFramebufferSizeCallback(d->window, NULL);
        glfwSetWindowSize(d->window, d->w, d->h);
        glfwSetFramebufferSizeCallback(d->window, glfw_fb_size_change);
        glfw_fb_size_change(d->window, d->w, d->h);
        break;
      case topazDisplay_Parameter_Height:
        d->h = value;
        glfwSetFramebufferSizeCallback(d->window, NULL);
        glfwSetWindowSize(d->window, d->w, d->h);
        glfwSetFramebufferSizeCallback(d->window, glfw_fb_size_change);
        glfw_fb_size_change(d->window, d->w, d->h);
        break;
      case topazDisplay_Parameter_X:
        d->x = value;
        glfwSetWindowPosCallback(d->window, NULL);
        glfwSetWindowPos(d->window, d->x, d->y);
        glfwSetWindowPosCallback(d->window, glfw_fb_pos_change);
        glfw_fb_pos_change(d->window, d->x, d->y);
        break;
      case topazDisplay_Parameter_Y:
        d->y = value;
        glfwSetWindowPosCallback(d->window, NULL);
        glfwSetWindowPos(d->window, d->x, d->y);
        glfwSetWindowPosCallback(d->window, glfw_fb_pos_change);
        glfw_fb_pos_change(d->window, d->x, d->y);
        break;

      case topazDisplay_Parameter_Show:
        if (value) 
            glfwShowWindow(d->window);
        else 
            glfwHideWindow(d->window);
            
        break;
        

      case topazDisplay_Parameter_InputFocus: 
        if (value > 0.5) 
            glfwRequestWindowAttention(d->window);
        break;

      case topazDisplay_Parameter_Decorated:
        glfwSetWindowAttrib(d->window, GLFW_DECORATED, (int)value);
        break;
      case topazDisplay_Parameter_Fullscreen:
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
        break;
      default:;  
    }
}




static void topaz_glfw_set_name(topazDisplay_t * dispSrc, void * api, const topazString_t * str) {
    TopazGLFWWindow * d = api;      
    glfwSetWindowTitle(d->window, topaz_string_get_c_str(str));
}


static int topaz_glfw_is_parameter_modifiable(topazDisplay_t * dispSrc, void * api, topazDisplay_Parameter c) {
    return 1;
}


static void render_to_screen(TopazGLFWWindow * w, GLuint tex, int width, int height) { 
    if (w->program == -1) return;
    GLFWwindow * wOld = glfwGetCurrentContext();
    glfwMakeContextCurrent(w->window);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, w->w, w->h);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(w->vao);
    glUseProgram(w->program);

    if (w->textureSizeLocation >= 0) {
        glUniform2f(w->textureSizeLocation, (float)width, (float)height);
    }

    glBindBuffer(GL_ARRAY_BUFFER, w->vbo);    
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
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (stateBlend) glEnable(stateBlend);
    if (stateCull)  glEnable(stateCull);
    if (stateDepth) glEnable(stateDepth);

    glBindVertexArray(0);

    glfwMakeContextCurrent(wOld);

}

static void topaz_glfw_update(topazDisplay_t * dispSrc, void * api, topazRenderer_Framebuffer_t * fb) {
    TopazGLFWWindow * d = api;      
    glfwPollEvents();


    render_to_screen(
        d, 
        ((GLuint*)topaz_renderer_framebuffer_get_handle(fb))[1],
        topaz_renderer_framebuffer_get_width(fb),
        topaz_renderer_framebuffer_get_height(fb)
    );
    glFinish();
    //glfwSwapBuffers(d->window);
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


static intptr_t api_nothing(){return 0;}

void topaz_system_display_ogl3__backend(
    topazSystem_t *         system, 
    topazSystem_Backend_t * backend, 
    topazDisplayAPI_t *     api
) {
    topaz_system_backend_bind(
        backend,
        
        // name
        TOPAZ_STR_CAST("OGL3"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("OpenGL 3.1 windowing bridge using GLFW"),





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
    api->display_request_parameter_change = topaz_glfw_request_parameter_change;
    api->display_set_name = topaz_glfw_set_name;

    api->display_is_parameter_modifiable = topaz_glfw_is_parameter_modifiable;
    api->display_update = topaz_glfw_update;
    api->display_supported_framebuffers = topaz_glfw_supported_framebuffers;
    api->display_get_system_handle_type = topaz_glfw_get_system_handle_type;
    api->display_get_system_handle =  topaz_glfw_get_system_handle;
    api->display_get_system_event_type = topaz_glfw_get_system_event_type;
    api->display_get_last_system_event = topaz_glfw_get_last_system_event;
    api->display_set_post_process_shader = topaz_glfw_set_post_process_shader;
    api->display_get_current_clipboard = (topazArray_t * (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_set_current_clipboard = (void (*)(topazDisplay_t *, void *, const topazArray_t *)) api_nothing;

}




