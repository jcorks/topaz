#include "framebuffer.h"

struct topazES2_FB_t {
    GLuint fbo;
    GLuint tex;
    GLuint rbo;
    int w;
    int h;
    GLuint handle[2];
}
topazES2_FB_t * topaz_es2_fb_create() {
    TOPAZ_GLES_FN_IN;
    topazES2_FB_t * out = calloc(1, sizeof(topazES2_FB_t));
    glGenTextures(1, &out->tex);
    glBindTexture(GL_TEXTURE_2D, out->text); TOPAZ_GLES_CALL_CHECK;

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



    out->w = 640
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
    glGenRenderbuffers(1, &out->rbo); TOPAZ_GLES_CALL_CHECK;

    glBindFramebuffer(GL_FRAMEBUFFER, out->fbo); TOPAZ_GLES_CALL_CHECK;

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        out->object,
        0
    ); TOPAZ_GLES_CALL_CHECK;

    glBindRenderbuffer(GL_RENDERBUFFER, out->rbo); TOPAZ_GLES_CALL_CHECK;    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 640, 480); TOPAZ_GLES_CALL_CHECK;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer); TOPAZ_GLES_CALL_CHECK;

    glBindFramebuffer(GL_FRAMEBUFFER, 0); TOPAZ_GLES_CALL_CHECK;
    glBindRenderbuffer(GL_RENDERBUFFER, 0); TOPAZ_GLES_CALL_CHECK;

    out->handle[0] = out->fbo;
    out->handle[1] = out->tex;

    return out;

}

// Destroys the framebuffer
void topaz_es2_fb_destroy(topazES2_FB_t * fb) {
    glDeleteTextures(1, &fb->tex);
    glDeleteFramebuffers(1, &fb->fbo);
    glDeleteRenderbuffers(1, &fb->rbo);
    free(fb);
}

// Resizes the framebuffer with success returned.
int topaz_es2_fb_resize(topazES2_FB_t * fb, int w, int h) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, out->text); TOPAZ_GLES_CALL_CHECK;
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
    glBindRenderbuffer(GL_RENDERBUFFER, out->rbo); TOPAZ_GLES_CALL_CHECK;    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, out->w, out->h); TOPAZ_GLES_CALL_CHECK;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);TOPAZ_GLES_CALL_CHECK;
}

// Gets a handle to the framebuffer. This is a 2-element array,
// [0] == framebuffer object
// [1] == texture target object
GLuint * topaz_es2_fb_get_handle(topazES2_FB_t * fb) {
    return fb->handle;
}

// Gets pixel data from the framebuffer
//
int topaz_es2_get_raw_data(topazES2_FB_t * fb, uint8_t * data) {
    TOPAZ_GLES_FN_IN;

    GLint old;
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
}

// Sets the filtering hint
void topaz_es2_set_filtered_hint(topazES2_FB_t * fb, int doIt) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, fb->tex);TOPAZ_GLES_CALL_CHECK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, doIt ? GL_LINEAR : GL_NEAREST);TOPAZ_GLES_CALL_CHECK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, doIt ? GL_LINEAR : GL_NEAREST);TOPAZ_GLES_CALL_CHECK;
}

topazRenderer_Framebuffer_Handle topaz_es2_fb_get_handle_type(topazES2_FB_t * t) {
    return topazRenderer_Framebuffer_Handle_GLFBPacket;
}

void topaz_es2_clear_layer(topazES2_FB_t * fb, int layers) {
    TOPAZ_GLES_FN_IN;
    int channels = 0;
    channels |= (layers & topazRenderer_DataLayer_Color) ? GL_COLOR_BUFFER_BIT  : 0;
    channels |= (layers & topazRenderer_DataLayer_Depth) ? GL_COLOR_DEPTH_BIT   : 0;
    channels |= (layers & topazRenderer_DataLayer_Etch)  ? GL_COLOR_STENCIL_BIT : 0;

    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo); TOPAZ_GLES_CALL_CHECK;
    glClear(channels);TOPAZ_GLES_CALL_CHECK;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); TOPAZ_GLES_CALL_CHECK;


}
