#include "context.h"
#include <stdlib.h>

struct topazES2_t {
    topazES2_FB_t * fb;
    topazES2_TexMan_t * texm;
};

topazES2_t * topaz_es2_create() {
    topazES2_t * out = calloc(1, sizeof(topazES2_t));
    out->fb = 0;
    out->texm = topaz_es2_texman_create();
    return out;
}

void topaz_es2_destroy(topazES2_t * e) {
    topaz_es2_texman_destroy(e->texm);
    free(e);
}

topazES2_TexMan_t * topaz_es2_get_texture_manager(topazES2_t * e) {
    return e->texm;
}

topazES2_2D_t * topaz_es2_create_2d(topazES2_t * e) {
    return topaz_es2_2d_create(e->texm);
}

topazES2_FB_t * topaz_es2_get_target(topazES2_t * e) {
    return e->fb;
}

void topaz_es2_set_target(topazES2_t * e, topazES2_FB_t * fb) {
    // flush?
    e->fb = fb;
}

void topaz_es2_start(topazES2_t * e) {
    if (!e->fb) return;
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, topaz_es2_fb_get_handle(e->fb)[0]);TOPAZ_GLES_CALL_CHECK;
}

// releases the framebuffer (unbind)
void topaz_es2_end(topazES2_t * e) {
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;

}

void topaz_es2_sync(topazES2_t * e) {
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
