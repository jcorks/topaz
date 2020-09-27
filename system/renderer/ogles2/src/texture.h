#ifndef H_OGLES2TOPAZ_TEXTURE_INCLUDED
#define H_OGLES2TOPAZ_TEXTURE_INCLUDED


typedef struct topazES2_Texture_t topazES2_Texture_t;
typedef struct topazES2_TexMan_t topazES2_TexMan_t;
#include "context.h"


// Creates a new texture atlast. This atlas has a single GLTexture 
// id that is used with all rendering operations.
topazES2_TexMan_t * topaz_es2_texman_create();

// Destroys an atlas
void topaz_es2_texman_destroy(topazES2_TexMan_t *);

// returns 16 integers, each index corresponding to the GL texture object 
// for that particular atlasIndex
const GLuint * topaz_es2_texman_gl_textures(const topazES2_TexMan_t *);

// Prints part of an atlas
void topaz_es2_texman_print_section(
    topazES2_TexMan_t *, 
    int x,
    int y,
    int w,
    int h,
    int atlasIndex
);


// Sets a callback to be fired when a texture 
// has its atlas coordinates changed due to a reset event.
// good idea for object2d stuff

typedef void (*topaz_es2_texman_reset_fn)(topazES2_Texture_t * texture, void * data);


void topaz_es2_texman_add_reset_callback(
    topazES2_TexMan_t *,
    topaz_es2_texman_reset_fn,
    void * cbData
);





// Creates a texture according to the topaz rendering api. Needs a wrapper to 
// pass the atlas reference.
topazES2_Texture_t * topaz_es2_texture_create(
    topazES2_TexMan_t *, 
    int w, 
    int h, 
    const uint8_t *
);


// Destroys a texture according to the topaz rendering api. No wrapper needed.
void topaz_es2_texture_destroy(topazES2_Texture_t *);


// Updates a texture according tot he topaz rendering api. No wrapper needed.
void topaz_es2_texture_update(topazES2_Texture_t *, const uint8_t * data);

// Fetches a texture's texels RGBA according to the topaz rendering API. no wrapper needed.
void topaz_es2_texture_get(topazES2_Texture_t *, uint8_t * data);

// gets the index of the atlas that owns this texture
int topaz_es2_texture_get_atlas_index(const topazES2_Texture_t *);


// Takes local coordinates meant for the texture passed and returns the corresponding
// UVs for querying the atlas itself.
void topaz_es2_texture_local_coords_to_atlas(
    topazES2_Texture_t *,
    float localTexX,
    float localTexY,
    float * atlasX,
    float * atlasY,
    int * atlasIndex
);





#endif
