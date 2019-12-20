/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/


#ifndef H_TOPAZDC__RENDERER_2D__INCLUDED
#define H_TOPAZDC__RENDERER_2D__INCLUDED




#include <topaz/backends/renderer.h>


/*
    Renderer2D
    ----
    // The general use case for 2D geometry is we will draw many 
    // small objects with unique tranform properties. Thus, a static rendering 
    // approach is less favorable as that would lead to more frequent draws.
    // Render2DVertices, using user-maintained global vertices, is a more 
    // performant option for drawing specifically 2D vertices.

*/

typedef struct topazRenderer_2D_t topazRenderer_2D_t;




/// Struct representing a dynamic (2D) vertex state.
/// Dynamic vertices do not support lighting or 
/// custom graphics programs.
///
typedef struct topazRenderer_2D_Vertex_t topazRenderer_2D_Vertex_t;

struct topazRenderer_2D_Vertex_t{
    /// vertex position
    ///
    float x, y;       
    
    /// color, scale from 0.f to 1.f (red, green, blue, and alpha)             
    ///
    float r, g, b, a; 
    
    /// texture coordinates (0, 0 is topleft)
    ///
    float texX, texY;                     
    
    /// if not used, set to -1, else float form of texture id
    ///
    float useTex;                         
    
    /// the transform reference object
    ///
    float object;                         
};



/// Context parameters used to give additional information 
/// when drawing a collection of 2D vertices.
///
typedef struct topazRenderer_2D_Context_t topazRenderer_2D_Context_t;

struct topazRenderer_2D_Context_t {
    /// Output display width.
    ///
    float width;

    /// Output display height.
    ///
    float height;

    /// Etch rule when drawing. 
    ///
    topazRenderer_EtchRule etchRule;        

    /// Matrix transform. Even though the input vertices are 2D, it 
    /// is possible to get a3D produced visual if the transform calls 
    /// for the transformation.
    ///
    float * transform;
};


typedef struct topazRenderer_2D_ObjectParams_t topazRenderer_2D_ObjectParams_t;

struct topazRenderer_2D_ObjectParams_t {
    // transform
    float data[16];
    
};








/// Creates a 2D render object.
///
topazRenderer_2D_t * topaz_renderer_2d_create(topazRenderer_t *);

/// Destroys a 2D render object.
///
void topaz_renderer_2d_destroy(topazRenderer_2D_t *);

/// Adds a new 2D objects and returns its IDs.
/// count should be the number of objects to request 
/// output is a buffer of sizeof(uint32_t)*count length.
/// Success is returned.
///
int topaz_renderer_2d_add_objects(topazRenderer_2D_t *, uint32_t * output, uint32_t count);

/// Removes a collection of objects.
///
void ropaz_renderer_2d_remove_objects(topazRenderer_2D_t *, uint32_t * ids, uint32_t count);


/// Requests that an additional set of objects 
/// be drawn when using topaz_renderer_draw_2d
///
void topaz_renderer_2d_queue_vertices(
    topazRenderer_2D_t *,
    const uint32_t * objects,
    uint32_t count
);

/// Clears all requests queued
///
void topaz_renderer_2d_clear_queue(topazRenderer_2D_t *);




/// Adds a collection of vertices
/// count is the number of objects
/// output is the produced ids and should be a buffer of size sizeof(uint32_t)*count
/// Success is returned.
///
int topaz_renderer_2d_add_vertices(topazRenderer_2D_t *, uint32_t * output, uint32_t count);


/// Removes a set of 2d vertices.
///
void topaz_renderer_2d_remove_vertices(topazRenderer_2D_t *, uint32_t * objects, uint32_t count);

/// Sets vertices 
///
void topaz_renderer_2d_set_vertices(
    topazRenderer_2D_t *, 
    uint32_t * vertices, 
    const topazRenderer_2D_Vertex_t *, 
    uint32_t count
);

/// Gets the data for the specified vertices.
///
void topaz_renderer_2d_get_vertices(
    topazRenderer_2D_t *, 
    const uint32_t * vertexIDs, 
    topazRenderer_2D_Vertex_t * output,
    uint32_t count
);


/// Sets parameters for a specific object
///
void topaz_renderer_2d_set_object_params(
    topazRenderer_2D_t *, 
    uint32_t object, 
    const topazRenderer_2D_ObjectParams_t *
);


typedef struct topazRenderer_t topazRenderer_t;









#endif
