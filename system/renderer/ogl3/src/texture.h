#ifndef H_OGLGL3TOPAZ_TEXTURE_INCLUDED
#define H_OGLGL3TOPAZ_TEXTURE_INCLUDED


typedef struct topazGL3_Texture_t topazGL3_Texture_t;
typedef struct topazGL3_TexMan_t topazGL3_TexMan_t;
#include "context.h"
#include <topaz/containers/array.h>

// Creates a new texture atlast. This atlas has a single GLTexture 
// id that is used with all rendering operations.
topazGL3_TexMan_t * topaz_gl3_texman_create();

// Destroys an atlas
void topaz_gl3_texman_destroy(topazGL3_TexMan_t *);

// returns 16 integers, each index corresponding to the GL texture object 
// for that particular atlasIndex
const GLuint * topaz_gl3_texman_gl_textures(const topazGL3_TexMan_t *);

// sets whether to filter for all subsequent rendering operations
// that use textures from this texture manager.
void topaz_gl3_texman_set_filter(
    topazGL3_TexMan_t *,
    int filter
);

// Prints part of an atlas
void topaz_gl3_texman_print_section(
    topazGL3_TexMan_t *, 
    int x,
    int y,
    int w,
    int h,
    int atlasIndex
);


// Sets a callback to be fired when a texture 
// has its atlas coordinates changed due to a reset event.
// good idea for object2d stuff
// Each element within the array is a topazES2_Texture_t *
typedef void (*topaz_gl3_texman_change_fn)(topazArray_t * textures, void * data);


int topaz_gl3_texman_add_change_callback(
    topazGL3_TexMan_t *,
    topaz_gl3_texman_change_fn,
    void * cbData
);

void topaz_gl3_texman_remove_change_callback(
    topazGL3_TexMan_t *,
    int
);







// Creates a texture according to the topaz rendering api. Needs a wrapper to 
// pass the atlas reference.
topazGL3_Texture_t * topaz_gl3_texture_create(
    topazGL3_TexMan_t *, 
    int w, 
    int h, 
    const uint8_t *
);


// Destroys a texture according to the topaz rendering api. No wrapper needed.
void topaz_gl3_texture_destroy(topazGL3_Texture_t *);


// Updates a texture according tot he topaz rendering api. No wrapper needed.
void topaz_gl3_texture_update(topazGL3_Texture_t *, const uint8_t * data);

// Fetches a texture's texels RGBA according to the topaz rendering API. no wrapper needed.
void topaz_gl3_texture_get(topazGL3_Texture_t *, uint8_t * data);

// gets the index of the atlas that owns this texture
int topaz_gl3_texture_get_atlas_index(const topazGL3_Texture_t *);


// Takes local coordinates meant for the texture passed and returns the corresponding
// UVs for querying the atlas itself.
void topaz_gl3_texture_local_coords_to_atlas(
    topazGL3_Texture_t *,
    float localTexX,
    float localTexY,
    float * atlasX,
    float * atlasY,
    int * atlasIndex
);

// Gets all the information of a texture in regards to the atlas
void topaz_gl3_texture_get_info(
    topazGL3_Texture_t * t,
    float * local_x,    
    float * local_y,    
    float * local_w,    
    float * local_h,    
    float * atlas_w,
    float * atlas_h,
    GLuint * textureObjectID
);





#endif
