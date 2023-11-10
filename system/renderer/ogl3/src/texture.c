#include "texture.h"
#include "charpixel.h"
#include <stdlib.h>
#include <stdio.h>
#include <topaz/containers/array.h>


struct topazGL3_Texture_t {
    GLint object;
    int w; // width
    int h; // height
};


topazGL3_Texture_t * topaz_gl3_texture_create(
    int w, 
    int h, 
    const uint8_t * data
) {

    TOPAZ_GLES_FN_IN;

    topazGL3_Texture_t * t = calloc(1, sizeof(topazGL3_Texture_t));
    t->w = w;
    t->h = h;
    
    glGenTextures(&t->object, 1);
    topaz_gl3_texture_update(t, data);
    return t;
}

// creates a new texture from an existing GL texture object.
topazGL3_Texture_t * topaz_gl3_texture_from_object(GLint object) {
    TOPAZ_GLES_FN_IN;

    topazGL3_Texture_t * t = calloc(1, sizeof(topazGL3_Texture_t));
    glBindTexture(GL_TEXTURE_2D, object); TOPAZ_GLES_CALL_CHECK;
    
    glGetTexLevelParameter(
        GL_TEXTURE_2D, 
        0,
        GL_TEXTURE_WIDTH,
        &(t->w)
    ); TOPAZ_GLES_CALL_CHECK;

    glGetTexLevelParameter(
        GL_TEXTURE_2D, 
        0,
        GL_TEXTURE_WIDTH,
        &(t->h)
    ); TOPAZ_GLES_CALL_CHECK;

    t->object = object;
    return t;
}

// forces a resize. No public API does this.
void topaz_gl3_texture_force_resize(topazGL3_Texture_t * t) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, t->object); TOPAZ_GLES_CALL_CHECK;
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        t->w,
        t->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        NULL
    ); TOPAZ_GLES_CALL_CHECK;
}

// Destroys a texture according to the topaz rendering api. No wrapper needed.
void topaz_gl3_texture_destroy(topazGL3_Texture_t * t) {
    TOPAZ_GLES_FN_IN;
    glDeleteTextures(&t->object, 1); TOPAZ_GLES_CALL_CHECK;
    free(t);
}


// Updates a texture according tot he topaz rendering api. No wrapper needed.
void topaz_gl3_texture_update(topazGL3_Texture_t * t, const uint8_t * data) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, t->object); TOPAZ_GLES_CALL_CHECK;
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        t->w,
        t->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    ); TOPAZ_GLES_CALL_CHECK;
}

// Fetches a texture's texels RGBA according to the topaz rendering API. no wrapper needed.
void topaz_gl3_texture_get(topazGL3_Texture_t *, uint8_t * data) {
    TOPAZ_GLES_FN_IN;
    glReadPixels(
        0, 0,
        t->w, t->h,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        target
    ); TOPAZ_GLES_CALL_CHECK;
}

// gets the index of the atlas that owns this texture
GLint topaz_gl3_texture_get_handle(const topazGL3_Texture_t * t) {
    return t->object;
}

// sets whether to use bilinear filtering on this texture.
void topaz_gl3_texture_set_filter(topazGL3_Texture_t * t, int filter) {
    glBindTexture(GL_TEXTURE_2D, t->object); TOPAZ_GLES_CALL_CHECK;

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        filter ? GL_LINEAR : GL_NEAREST
    ); TOPAZ_GLES_CALL_CHECK;


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        filter ? GL_LINEAR : GL_NEAREST
    ); TOPAZ_GLES_CALL_CHECK;
}





