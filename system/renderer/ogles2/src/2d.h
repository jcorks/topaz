#ifndef H_TOPAZES22D_INCLUDED
#define H_TOPAZES22D_INCLUDED



// allows for controlled 2d operations with 
// a built-in shader that doesnt do any fancy stuff for 
// custom shaders
typedef struct topazES2_2D_t topazES2_2D_t;

#include "context.h"
#include "buffer.h"
#include "texture.h"
#include <topaz/backends/renderer.h>



// creates a 2d context. will use the texture manager and stuff from the context
topazES2_2D_t * topaz_es2_2d_create(topazES2_TexMan_t *);

// destroys a 2d context
void topaz_es2_2d_destroy(topazES2_2D_t *);

// adds objects
int topaz_es2_2d_add_objects(topazES2_2D_t *, uint32_t * output, uint32_t count);

// removes objects
void topaz_es2_2d_remove_objects(topazES2_2D_t *, uint32_t * output, uint32_t count);



// prepares objects for drawing
void topaz_es2_2d_queue_objects(topazES2_2D_t *, const uint32_t * objects, uint32_t count);

// clears the prepared objects
void topaz_es2_2d_clear_queue(topazES2_2D_t *);


// sets the vertices
void topaz_es2_2d_set_object_vertices(
    topazES2_2D_t *,
    uint32_t object,
    topazES2_Buffer_t *
);

//
void topaz_es2_2d_set_object_texture(
    topazES2_2D_t *,
    uint32_t object,
    topazES2_Texture_t *
);

void topaz_es2_2d_set_object_transform(
    topazES2_2D_t *,
    uint32_t object,
    const topazMatrix_t *
);


// renders all queued objects
void topaz_es2_2d_render(
    topazES2_2D_t *,
    const topazRenderer_2D_Context_t *,
    const topazRenderer_Attributes_t *
);


#endif
