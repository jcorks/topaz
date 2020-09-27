#ifndef H_TOPAZES2FB_INCLUDED
#define H_TOPAZES2FB_INCLUDED

typedef struct topazES2_FB_t topazES2_FB_t;

#include <topaz/backends/renderer.h>
#include "context.h"

// Creates a new framebuffer.
topazES2_FB_t * topaz_es2_fb_create();

// Destroys the framebuffer
void topaz_es2_fb_destroy(topazES2_FB_t *);

// Resizes the framebuffer with success returned.
int topaz_es2_fb_resize(topazES2_FB_t *, int w, int h);

// Gets a handle to the framebuffer. This is a 2-element array,
// [0] == framebuffer object
// [1] == texture target object
GLuint * topaz_es2_fb_get_handle(topazES2_FB_t *);

// Gets pixel data from the framebuffer
//
int topaz_es2_get_raw_data(topazES2_FB_t *, uint8_t *);

// Sets the filtering hint
void topaz_es2_set_filtered_hint(topazES2_FB_t *, int);

// Gets the type of handle.
topazRenderer_Framebuffer_Handle topaz_es2_fb_get_handle_type(topazES2_FB_t *);

// clears the channels within the framebuffer
void topaz_es2_fb_clear_layer(topazES2_FB_t *, int);



#endif
