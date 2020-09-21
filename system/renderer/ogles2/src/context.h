#ifndef H_TOPAZES2_CONTEXT_INCLUDED
#define H_TOPAZES2_CONTEXT_INCLUDED

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>


// macros for throwing errors on glGetError() for debugging.
#ifdef TOPAZDC_DEBUG
#include <stdio.h>
#include <assert.h>
#define TOPAZ_GLES_FN_IN       int __er = glGetError(); if (__er != GL_NO_ERROR) {printf("Encountered OpenGLES2 error on function enter (%d). Please check the program.\n", __er); assert(!"OpenGL Error. Please check STDOUT for details.");}
#define TOPAZ_GLES_CALL_CHECK      __er = glGetError(); if (__er != GL_NO_ERROR) {printf("Encountered OpenGLES2 error on function enter (%d). Please check the program.\n", __er); assert(!"OpenGL Error. Please check STDOUT for details.");}
#else
#define TOPAZ_GLES_FN_IN;
#define TOPAZ_GLES_CALL_CHECK;
#endif



#endif
