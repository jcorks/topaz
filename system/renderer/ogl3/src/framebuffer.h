#ifndef H_TOPAZGL3FB_INCLUDED
#define H_TOPAZGL3FB_INCLUDED

typedef struct topazGL3_FB_t topazGL3_FB_t;
typedef struct topazGL3_Texture_t topazGL3_Texture_t;

#include <topaz/backends/renderer.h>
#include "context.h"

// Creates a new framebuffer.
topazGL3_FB_t * topaz_gl3_fb_create();

// Destroys the framebuffer
void topaz_gl3_fb_destroy(topazGL3_FB_t *);

// Resizes the framebuffer with success returned.
int topaz_gl3_fb_resize(topazGL3_FB_t *, int w, int h);

// Gets a handle to the framebuffer. This is a 2-element array,
// [0] == framebuffer object
// [1] == texture target object
GLuint * topaz_gl3_fb_get_handle(topazGL3_FB_t *);

// Gets the texture associated with the framebuffer for public use.
// persists with the framebuffer.
topazGL3_Texture_t * topaz_gl3_gb_get_texture(const topazGL3_FB_t *);

// Gets pixel data from the framebuffer
//
int topaz_gl3_get_raw_data(topazGL3_FB_t *, uint8_t *);

// Sets the filtering hint
void topaz_gl3_set_filtered_hint(topazGL3_FB_t *, int);

// Gets the type of handle.
topazRenderer_Framebuffer_Handle topaz_gl3_fb_get_handle_type(topazGL3_FB_t *);

// clears the channels within the framebuffer
void topaz_gl3_fb_clear_layer(topazGL3_FB_t *, int);

// The width of the framebuffer
int topaz_gl3_fb_get_width(topazGL3_FB_t *);

// The height of the framebuffer
int topaz_gl3_fb_get_height(topazGL3_FB_t *);


#endif
