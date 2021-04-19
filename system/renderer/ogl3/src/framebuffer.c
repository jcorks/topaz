#include "framebuffer.h"
#include <stdlib.h>
struct topazGL3_FB_t {
    GLuint fbo;
    GLuint tex;
    GLuint rbo_depth;
    GLuint rbo_stencil;
    int w;
    int h;
    GLuint handle[2];
};

topazGL3_FB_t * topaz_gl3_fb_create() {
    TOPAZ_GLES_FN_IN;
    topazGL3_FB_t * out = calloc(1, sizeof(topazGL3_FB_t));
    glGenTextures(1, &out->tex);
    glBindTexture(GL_TEXTURE_2D, out->tex); TOPAZ_GLES_CALL_CHECK;

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST
    ); TOPAZ_GLES_CALL_CHECK;


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST
    ); TOPAZ_GLES_CALL_CHECK;



    out->w = 640;
    out->h = 480;


    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        out->w,
        out->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    ); TOPAZ_GLES_CALL_CHECK;

    glBindTexture(GL_TEXTURE_2D, 0); TOPAZ_GLES_CALL_CHECK;


    glGenFramebuffers(1, &out->fbo); TOPAZ_GLES_CALL_CHECK;
    glGenRenderbuffers(1, &out->rbo_depth); TOPAZ_GLES_CALL_CHECK;
    glGenRenderbuffers(1, &out->rbo_stencil); TOPAZ_GLES_CALL_CHECK;

    glBindFramebuffer(GL_FRAMEBUFFER, out->fbo); TOPAZ_GLES_CALL_CHECK;

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        out->tex,
        0
    ); TOPAZ_GLES_CALL_CHECK;

    glBindRenderbuffer(GL_RENDERBUFFER, out->rbo_depth); TOPAZ_GLES_CALL_CHECK;    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 640, 480); TOPAZ_GLES_CALL_CHECK;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, out->rbo_depth); TOPAZ_GLES_CALL_CHECK;


    #ifdef TOPAZDC_DEBUG
    int a = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (a != GL_FRAMEBUFFER_COMPLETE) {
        printf("GL FATAL ERROR: incomplete framebuffer attachment:");
        switch(a) {
          case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
          case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
          case GL_FRAMEBUFFER_UNSUPPORTED: printf("GL_FRAMEBUFFER_UNSUPPORTED"); break;
        }
        printf("\n");
        exit(991);
    }
    #endif


    glBindFramebuffer(GL_FRAMEBUFFER, 0); TOPAZ_GLES_CALL_CHECK;
    glBindRenderbuffer(GL_RENDERBUFFER, 0); TOPAZ_GLES_CALL_CHECK;

    out->handle[0] = out->fbo;
    out->handle[1] = out->tex;

    return out;

}

// Destroys the framebuffer
void topaz_gl3_fb_destroy(topazGL3_FB_t * fb) {
    glDeleteTextures(1, &fb->tex);
    glDeleteFramebuffers(1, &fb->rbo_depth);
    glDeleteRenderbuffers(1, &fb->rbo_depth);
    glDeleteFramebuffers(1, &fb->rbo_stencil);
    glDeleteRenderbuffers(1, &fb->rbo_stencil);
    free(fb);
}

// Resizes the framebuffer with success returned.
int topaz_gl3_fb_resize(topazGL3_FB_t * out, int w, int h) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, out->tex); TOPAZ_GLES_CALL_CHECK;
    out->w = w;
    out->h = h;
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        out->w,
        out->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    ); TOPAZ_GLES_CALL_CHECK;
    glBindTexture(GL_TEXTURE_2D, 0);TOPAZ_GLES_CALL_CHECK;
    glBindFramebuffer(GL_FRAMEBUFFER, out->fbo);TOPAZ_GLES_CALL_CHECK;    
    glBindRenderbuffer(GL_RENDERBUFFER, out->rbo_depth); TOPAZ_GLES_CALL_CHECK;    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, out->w, out->h); TOPAZ_GLES_CALL_CHECK;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, out->rbo_depth); TOPAZ_GLES_CALL_CHECK;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);TOPAZ_GLES_CALL_CHECK;

    #ifdef TOPAZDC_DEBUG
    int a = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (a != GL_FRAMEBUFFER_COMPLETE) {
        printf("GL FATAL ERROR: incomplete framebuffer attachment:");
        switch(a) {
          case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
          case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
          case GL_FRAMEBUFFER_UNSUPPORTED: printf("GL_FRAMEBUFFER_UNSUPPORTED"); break;
        }
        printf("\n");
        exit(991);
    }
    #endif
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return 1;
}

// Gets a handle to the framebuffer. This is a 2-element array,
// [0] == framebuffer object
// [1] == texture target object
GLuint * topaz_gl3_fb_get_handle(topazGL3_FB_t * fb) {
    return fb->handle;
}

int topaz_gl3_fb_get_width(topazGL3_FB_t * fb) {
    return fb->w;
}

int topaz_gl3_fb_get_height(topazGL3_FB_t * fb) {
    return fb->h;
}

// Gets pixel data from the framebuffer
//
int topaz_gl3_get_raw_data(topazGL3_FB_t * fb, uint8_t * data) {
    TOPAZ_GLES_FN_IN;

    int w = fb->w;
    int h = fb->h;


    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);TOPAZ_GLES_CALL_CHECK;
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);TOPAZ_GLES_CALL_CHECK;
    uint32_t * data32 = (uint32_t*)data;
    uint32_t temp;
    for(uint32_t y = 0; y < h/2; ++y) {
        for(uint32_t x = 0; x < w; ++x) {
            temp = data32[x+y*w];
            data32[x+y*w] = data32[x+w*(h-y-1)];
            data32[x+w*(h-y-1)] = temp;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;
    return 1;
}

// Sets the filtering hint
void topaz_gl3_set_filtered_hint(topazGL3_FB_t * fb, int doIt) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, fb->tex);TOPAZ_GLES_CALL_CHECK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, doIt ? GL_LINEAR : GL_NEAREST);TOPAZ_GLES_CALL_CHECK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, doIt ? GL_LINEAR : GL_NEAREST);TOPAZ_GLES_CALL_CHECK;
}

topazRenderer_Framebuffer_Handle topaz_gl3_fb_get_handle_type(topazGL3_FB_t * t) {
    return topazRenderer_Framebuffer_Handle_GLFBPacket;
}

void topaz_gl3_fb_clear_layer(topazGL3_FB_t * fb, int layers) {
    TOPAZ_GLES_FN_IN;
    int channels = 0;
    channels |= (layers & topazRenderer_DataLayer_Color) ? GL_COLOR_BUFFER_BIT   : 0;
    channels |= (layers & topazRenderer_DataLayer_Depth) ? GL_DEPTH_BUFFER_BIT   : 0;
    channels |= (layers & topazRenderer_DataLayer_Etch)  ? GL_STENCIL_BUFFER_BIT : 0;

    int prevMask[4];
    glGetIntegerv(GL_COLOR_WRITEMASK, prevMask);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo); TOPAZ_GLES_CALL_CHECK;
    glClear(channels);TOPAZ_GLES_CALL_CHECK;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); TOPAZ_GLES_CALL_CHECK;
    glColorMask(prevMask[0], prevMask[1], prevMask[2], prevMask[3]); 


}
