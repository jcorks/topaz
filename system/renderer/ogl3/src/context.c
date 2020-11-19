#include "context.h"
#include <stdlib.h>

struct topazGL3_t {
    topazGL3_FB_t * fb;
    topazGL3_TexMan_t * texm;
};

topazGL3_t * topaz_gl3_create() {
    topazGL3_t * out = calloc(1, sizeof(topazGL3_t));
    assert(glewInit() == GLEW_OK);
    out->fb = 0;
    out->texm = topaz_gl3_texman_create();
    return out;
}

void topaz_gl3_destroy(topazGL3_t * e) {
    topaz_gl3_texman_destroy(e->texm);
    free(e);
}

topazGL3_TexMan_t * topaz_gl3_get_texture_manager(topazGL3_t * e) {
    return e->texm;
}

topazGL3_2D_t * topaz_gl3_create_2d(topazGL3_t * e) {
    return topaz_gl3_2d_create(e->texm);
}

topazGL3_FB_t * topaz_gl3_get_target(topazGL3_t * e) {
    return e->fb;
}

void topaz_gl3_set_target(topazGL3_t * e, topazGL3_FB_t * fb) {
    // flush?
    e->fb = fb;
}

void topaz_gl3_start(topazGL3_t * e) {
    if (!e->fb) return;
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, topaz_gl3_fb_get_handle(e->fb)[0]);TOPAZ_GLES_CALL_CHECK;
}

// releases the framebuffer (unbind)
void topaz_gl3_end(topazGL3_t * e) {
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;

}

void topaz_gl3_sync(topazGL3_t * e) {
    glFinish();
}

const char * topaz_gles_error_to_string(int e) {
    switch(e) {
      case 0x0500: return "GL_INVALID_ENUM";
      case 0x0501: return "GL_INVALID_VALUE";
      case 0x0502: return "GL_INVALID_OPERATION";
      case 0x0503: return "GL_STACK_OVERFLOW";
      case 0x0504: return "GL_STACK_UNDERFLOW";
      case 0x0505: return "GL_INVALID_ENUM";
    }
    return "UNKNOWN ERROR";
}
