#ifndef H_TOPAZGL32D_INCLUDED
#define H_TOPAZGL32D_INCLUDED



// allows for controlled 2d operations with 
// a built-in shader that doesnt do any fancy stuff for 
// custom shaders
typedef struct topazGL3_2D_t topazGL3_2D_t;

#include "context.h"
#include "buffer.h"
#include "texture.h"
#include <topaz/backends/renderer.h>



// creates a 2d context. will use the texture manager and stuff from the context
topazGL3_2D_t * topaz_gl3_2d_create();

// destroys a 2d context
void topaz_gl3_2d_destroy(topazGL3_2D_t *);

// adds objects
int topaz_gl3_2d_add_objects(topazGL3_2D_t *, uint32_t * output, uint32_t count);

// removes objects
void topaz_gl3_2d_remove_objects(topazGL3_2D_t *, uint32_t * output, uint32_t count);



// prepares objects for drawing
void topaz_gl3_2d_queue_objects(topazGL3_2D_t *, const uint32_t * objects, uint32_t count);

// clears the prepared objects
void topaz_gl3_2d_clear_queue(topazGL3_2D_t *);


// sets the vertices
void topaz_gl3_2d_set_object_vertices(
    topazGL3_2D_t *,
    uint32_t object,
    topazGL3_Buffer_t *
);

//
void topaz_gl3_2d_set_object_texture(
    topazGL3_2D_t *,
    uint32_t object,
    topazGL3_Texture_t *
);

void topaz_gl3_2d_set_object_transform(
    topazGL3_2D_t *,
    uint32_t object,
    const topazMatrix_t *
);


// renders all queued objects
void topaz_gl3_2d_render(
    topazGL3_2D_t *,
    const topazRenderer_2D_Context_t *,
    const topazRenderer_Attributes_t *
);


#endif
