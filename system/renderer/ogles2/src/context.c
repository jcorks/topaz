#include "context.h"

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

topazES2_TexMan_t * topaz_es2_get_texture_manager(topazES2_t * e) {
    return e->texm;
}

topazES2_2D_t * topaz_es2_create_2d(topazES2_t * e) {
    return topaz_es2_2d_create(e->texm);
}

topazES2_FB_t * topaz_es2_get_target(topazES2_t * fb) {
    return e->fb;
}

void topaz_es2_set_target(topazES2_t * e, topazES2_FB_t * fb) {
    // flush?
    e->fb = fb;
}

void topaz_es2_start(topazES2_t * e) {
    if (!e->fb) return;
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, topaz_es2_fb_get_handle(fb)[0]);TOPAZ_GLES_CALL_CHECK;
}

// releases the framebuffer (unbind)
void topaz_es2_end(topazES2_t *) {
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;

}

void topaz_es2_sync(topazES2_t *) {
    glFinish();
}
