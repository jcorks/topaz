
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


#ifndef H_TOPAZDC__RENDERER_3D__INCLUDED
#define H_TOPAZDC__RENDERER_3D__INCLUDED

#include <topaz/containers/array.h>



/// A 3D rendering object 
///
typedef struct topazRenderer_3D_t topazRenderer_3D_t;


struct topazRenderer_3D_t {
    
    /// Vertices points to a renderbuffer containing all the vertex dat  a pertinent to the RenderObject.
    /// Each vertex consists of:
    ///      3-components for position,
    ///      3-components for the normal.
    ///      2-copmonents for UVs (texture coordinates)
    ///      4-components for user-defined data. Meant to be utilized with custom programs.
    ///
    topazRenderer_Buffer_t * vertices;
    
    /// Specifies how to render the vertices
    topazRenderer_Program_t * program;

    /// material matrix , 44-components containing
    /// lighting material and arbitrary data setup.
    /// The layout is as follows:
    /// 4-floats for ambient (standardly)
    /// 4-floats for diffuse (standardly)
    /// 4-floats for specular (standardly);
    /// 32-floats of user data.
    ///
    topazRenderer_Buffer_t * materialData;  

    /// local transform matrix (scale + rotation), 32-component
    /// Normal matrix (inverse transpose of modelData)
    ///
    topazRenderer_Buffer_t * modelData;  

    /// the source framebuffer optionally accessible 
    /// during rendering. If the samplebuffer is null,
    /// the no source framebuffer will be made available
    ///
    topazFramebuffer_t * samplebuffer;


    /// specifies the textures to be used. Each pair is a slot referred to by 
    /// each program and the ID of the texture.
    ///
    topazArray_t * textureSlots;
    topazArray_t * textureObjects;


    /* Static index list */
    // Index lists are used to build the geometry of the StaticState.
    // Each index refer to the index of the vertex to be used to build the triangle.
    // every 3 vertices, a triangle is formed. If a multiple of three is not used,
    // the behavior is implementation defined.
    topazArray_t * indices;    


};



/// Vertex for 3D rendering.
///
typedef struct topazRenderer_3D_Vertex_t topazRenderer_3D_Vertex_t;


struct topazRenderer_3D_Vertex_t {
    /// Positional 3D coordinates
    ///
    float x, y, z;

    /// Normal vector, normalized
    ///
    float normalX, normalY, normalZ;

    /// Texture UVs. [0, 1]
    ///
    float texX, texY;

    /// User-define data (for programs)
    ///
    float userDefinedData[4];

};


#include <topaz/backends/renderer.h>


#endif
