#ifndef H_OGLES2TOPAZ_TEXTURE_INCLUDED
#define H_OGLES2TOPAZ_TEXTURE_INCLUDED

#include "context.h"

typedef struct topazES2_Texture_t topazES2_Texture_t;
typedef struct topazES2_Atlas_t topazES2_Atlas_t;


// Creates a new texture atlast. This atlas has a single GLTexture 
// id that is used with all rendering operations.
topazES2_Atlas_t * topaz_es2_atlas_create();

// Destroys an atlas
void topaz_es2_atlas_destroy(topazES2_Atlas_t *);

// Gets the GLtexture object ID for the atlas
GLuint topaz_es2_atlas_gl_texture_id(const topazES2_Atlas_t *);

void topaz_es2_atlas_print_section(
    topazES2_Atlas_t *, 
    int x,
    int y,
    int w,
    int h
);


// Sets a callback to be fired when a texture 
// has its atlas coordinates changed due to a reset event.
// good idea for object2d stuff

typedef void (*topaz_es2_atlas_reset_fn)(topazES2_Texture_t * texture, void * data);


void topaz_es2_atlas_add_reset_callback(
    topazES2_Atlas_t *,
    topaz_es2_atlas_reset_fn,
    void * cbData
);





// Creates a texture according to the topaz rendering api. Needs a wrapper to 
// pass the atlas reference.
topazES2_Texture_t * topaz_es2_texture_create(
    topazES2_Atlas_t *, 
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



// Takes local coordinates meant for the texture passed and returns the corresponding
// UVs for querying the atlas itself.
void topaz_es2_texture_local_coords_to_atlas(
    topazES2_Texture_t *,
    float localTexX,
    float localTexY,
    float * atlasX,
    float * atlasY
);





#endif
