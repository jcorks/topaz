#ifndef H_TOPAZES2_CONTEXT_INCLUDED
#define H_TOPAZES2_CONTEXT_INCLUDED

#define GLFW_INCLUDE_ES2
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>


// macros for throwing errors on glGetError() for debugging.
#ifdef TOPAZDC_DEBUG
#include <stdio.h>
#include <assert.h>


const char * topaz_gles_error_to_string(int);
#define TOPAZ_GLES_FN_IN       int __er = glGetError(); if (__er != GL_NO_ERROR) {printf("Encountered OpenGLES2 error on function enter (%s). Please check the program.\n", topaz_gles_error_to_string(__er)); exit(999);}
#define TOPAZ_GLES_CALL_CHECK      __er = glGetError(); if (__er != GL_NO_ERROR) {printf("Encountered OpenGLES2 error after previous call (%s). Please check the program.\n", topaz_gles_error_to_string(__er)); exit(999);}
#else
#define TOPAZ_GLES_FN_IN ;;
#define TOPAZ_GLES_CALL_CHECK ;;
#endif


#include "texture.h"
#include "framebuffer.h"
#include "2d.h"
#include "program.h"

typedef struct topazES2_t topazES2_t;
typedef struct topazES2_Program_t topazES2_Program_t;

// creates a new es2 context
topazES2_t * topaz_es2_create();

// Destroys the instance
void topaz_es2_destroy(topazES2_t *);

// gets the es2 instance's texture manager
topazES2_TexMan_t * topaz_es2_get_texture_manager(topazES2_t *);

// creates a 2d instance from this one
topazES2_2D_t * topaz_es2_create_2d(topazES2_t *);

// Gets the current fb for this instances
topazES2_FB_t * topaz_es2_get_target(topazES2_t *);

// sets the current fb. CANNOT be called between 
// topaz_es2_start and topaz_es2_end else undefined behavior.
void topaz_es2_set_target(topazES2_t *, topazES2_FB_t *);

// attaches framebuffer for drawing operations
void topaz_es2_start(topazES2_t *);

// releases the framebuffer (unbind)
void topaz_es2_end(topazES2_t *);

// Ensures that following rendering options adhere to the process 
// attribs specified here.
void topaz_es2_commit_process_attribs(topazES2_t *, const topazRenderer_Attributes_t *);

// glFinish()?
void topaz_es2_sync(topazES2_t *);

// gets the default dynamic program.
topazES2_Program_t * topaz_es2_get_default_program(topazES2_t *);

#endif
