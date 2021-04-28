#ifndef H_TOPAZES2PROGRAM2D_INCLUDED
#define H_TOPAZES2PROGRAM2D_INCLUDED

#include "texture.h"
#include "context.h"
#include <topaz/matrix.h>
#include <topaz/backends/renderer.h>


typedef struct topazES2_Program2D_t topazES2_Program2D_t;

// Creates a program meant for 2d rendering
topazES2_Program2D_t * topaz_es2_p2d_create();

// Represents a discrete object to be rendered
// it is assumed that:
// 1) vbo is an openGL GL_ARRAY_BUFFER tightly packed with topazRenderer_2D_Vertex_t members
// 2) texture is an openGL texture object that the vbo references exclusively
// 3) localMatrix is a matrix representing the local transform for the renderable
typedef struct {
    GLuint vbo;
    GLuint texture;
    uint32_t vboVertexCount;
    topazMatrix_t localMatrix;
} topazES2_Program2D_Renderable_t;

// Renders a set of objects at once.
void topaz_es2_p2d_render(
    topazES2_Program2D_t *, 
    const topazRenderer_2D_Context_t * ctx,
    const topazRenderer_Attributes_t * attribs,
    const topazES2_Program2D_Renderable_t **,
    uint32_t count
);


#endif
