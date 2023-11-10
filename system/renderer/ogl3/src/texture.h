#ifndef H_OGLGL3TOPAZ_TEXTURE_INCLUDED
#define H_OGLGL3TOPAZ_TEXTURE_INCLUDED


typedef struct topazGL3_Texture_t topazGL3_Texture_t;
#include "context.h"
#include <topaz/containers/array.h>


// Creates a texture according to the topaz rendering api. 
topazGL3_Texture_t * topaz_gl3_texture_create(
    int w, 
    int h, 
    const uint8_t *
);

// creates a new texture from an existing GL texture object.
topazGL3_Texture_t * topaz_gl3_texture_from_object(GLint);


// forces a resize. No public API does this.
void topaz_gl3_texture_force_resize(topazGL3_Texture_t *);

// Destroys a texture according to the topaz rendering api. No wrapper needed.
void topaz_gl3_texture_destroy(topazGL3_Texture_t *);


// Updates a texture according tot he topaz rendering api. No wrapper needed.
void topaz_gl3_texture_update(topazGL3_Texture_t *, const uint8_t * data);

// Fetches a texture's texels RGBA according to the topaz rendering API. no wrapper needed.
void topaz_gl3_texture_get(topazGL3_Texture_t *, uint8_t * data);

// gets the index of the atlas that owns this texture
GLint topaz_gl3_texture_get_handle(const topazGL3_Texture_t *);

// sets whether to use bilinear filtering on this texture.
void topaz_gl3_texture_set_filter(topazGL3_Texture_t *, int filter);





#endif
