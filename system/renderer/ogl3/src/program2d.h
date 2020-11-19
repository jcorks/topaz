#ifndef H_TOPAZGL3PROGRAM2D_INCLUDED
#define H_TOPAZGL3PROGRAM2D_INCLUDED

#include "texture.h"
#include "context.h"
#include <topaz/matrix.h>
#include <topaz/backends/renderer.h>


typedef struct topazGL3_Program2D_t topazGL3_Program2D_t;

// Creates a program meant for 2d rendering
topazGL3_Program2D_t * topaz_gl3_p2d_create();

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
} topazGL3_Program2D_Renderable_t;

// Renders a set of objects at once.
void topaz_gl3_p2d_render(
    topazGL3_Program2D_t *, 
    const topazRenderer_2D_Context_t * ctx,
    const topazRenderer_ProcessAttribs_t * attribs,
    const topazGL3_Program2D_Renderable_t **,
    uint32_t count
);


#endif
