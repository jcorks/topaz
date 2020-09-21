#include "texture.h"
#include <stdlib.h>

#define ATLAS_START_LEN 1024
struct topazES2_Atlas_t {
    GLuint object;
    int width;
    int height;
    
    int maxWidth;
    int maxHeight;
    
};



topazES2_Atlas_t * topaz_es2_atlas_create() {
    TOPAZ_GLES_FN_IN;
    topazES2_Atlas_t * out = calloc(1, sizeof(topazES2_Atlas_t));
    glGenTextures(1, &out->object); TOPAZ_GLES_CALL_CHECK;
    glBindTexture(GL_TEXTURE_2D, out->object); TOPAZ_GLES_CALL_CHECK;

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


    out->width  = ATLAS_START_LEN;
    out->height = ATLAS_START_LEN;



    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        out->width,
        out->height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    ); TOPAZ_GLES_CALL_CHECK;



    return out;
}

GLuint topaz_es2_atlas_gl_texture_id(const topazES2_Atlas_t * a) {
    return a->object;
}


