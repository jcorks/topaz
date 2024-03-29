#ifndef H_TOPAZGL3_CONTEXT_INCLUDED
#define H_TOPAZGL3_CONTEXT_INCLUDED

#include "glad.h"
#include <GLFW/glfw3.h>


// macros for throwing errors on glGetError() for debugging.
#ifdef TOPAZDC_DEBUG
#include <stdio.h>
#include <assert.h>


const char * topaz_gles_error_to_string(int);
#define TOPAZ_GLES_FN_IN       int __er = glGetError(); if (__er != GL_NO_ERROR) {printf("Encountered OpenGLGL3 error on function enter (%s). Please check the program.\n", topaz_gles_error_to_string(__er)); exit(999);}
#define TOPAZ_GLES_CALL_CHECK      __er = glGetError(); if (__er != GL_NO_ERROR) {printf("Encountered OpenGLGL3 error after previous call (%s). Please check the program.\n", topaz_gles_error_to_string(__er)); exit(999);}
#else
#define TOPAZ_GLES_FN_IN ;;
#define TOPAZ_GLES_CALL_CHECK ;;
#endif


#include "texture.h"
#include "framebuffer.h"
#include "2d.h"
#include "program.h"


typedef struct topazGL3_t topazGL3_t;
typedef struct topazGL3_Program_t topazGL3_Program_t;


// creates a new es2 context
topazGL3_t * topaz_gl3_create();

// Destroys the instance
void topaz_gl3_destroy(topazGL3_t *);

// creates a 2d instance from this one
topazGL3_2D_t * topaz_gl3_create_2d(topazGL3_t *);

// Gets the current fb for this instances
topazGL3_FB_t * topaz_gl3_get_target(topazGL3_t *);

// sets the current fb. CANNOT be called between 
// topaz_gl3_start and topaz_gl3_end else undefined behavior.
void topaz_gl3_set_target(topazGL3_t *, topazGL3_FB_t *);

// attaches framebuffer for drawing operations
int topaz_gl3_start(topazGL3_t *);

// releases the framebuffer (unbind)
void topaz_gl3_end(topazGL3_t *);

// Ensures that following rendering options adhere to the process 
// attribs specified here.
void topaz_gl3_commit_process_attribs(topazGL3_t *, const topazRenderer_Attributes_t *);

// glFinish()?
void topaz_gl3_sync(topazGL3_t *);

// gets the default dynamic program.
topazGL3_Program_t * topaz_gl3_get_default_program(topazGL3_t *);

#endif
