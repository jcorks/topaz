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


#ifndef H_TOPAZDC__RENDER2D__INCLUDED
#define H_TOPAZDC__RENDER2D__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/backends/renderer.h>
#include <topaz/spatial.h>

///
/// Base object to render a 2D object.
/// 
/// In topaz, 2D objects are rendered and treated as a subset of 
/// 3D object functionality: all 2D renderable objects are expressed
/// in triangles made of vertices, each editable. But unlike
/// 3D, the z component is ignored, and there is no interface to 
/// a backend programmable pipeline. Because of this, 2D operations are 
///, in general, easier to work with but somewhat limited. In theory 
/// most operations would be recreatable using topaz' 3D features.
///
typedef struct topazRender2D_t topazRender2D_t;



/// Creates a new 2d object. If a collection of 
/// render2d objects are used, you may pass a pre-existing 
/// spatial object, which will be used to generate the local transform 
/// for the object. Else, passing NULL will prompt the render2d 
/// instance to create its own.
///
topazRender2D_t * topaz_render2d_create(
    /// The 2D renderer to bind to this render2d instance.
    topazRenderer_2D_t * renderer, 

    /// The spatial reference to associate with this instance. If NULL,
    /// a new one will be created.
    topazSpatial_t * spatial
);

/// Destroys the 2d object.
///
void topaz_render2d_destroy(
    /// The render2d instance to destroy.
    topazRender2D_t * r2d
);




/// Sets the rendering attributes
///
void topaz_render2d_set_attributes(
    /// The render2d to modify.
    topazRender2D_t * r2d,

    /// The attributes to set.
    const topazRenderer_Attributes_t * attribs
);

/// Gets the rendering attributes.
/// Default is topazRenderer_TextureFilterHint_Linear
///            topazRenderer_EtchRule_Out
///            topazRenderer_DepthTest_None
///            topazRenderer_AlphaRule_Allow
///
const topazRenderer_Attributes_t * topaz_render2d_get_attributes(
    /// The render2d to query.
    topazRender2D_t * r2d
);


/// Gets whether this 2D object is renderered in abolute coordinates
///
int topaz_render2d_get_absolute(
    /// The render2d to query.
    const topazRender2D_t * r2d
);

/// Sets whether drawing calculations from the node should be interpreted as absolute 
/// pixel values on the display or should be within the hierarchy. 
/// The default is false.
///
void topaz_render2d_set_absolute(
    /// The render2d to modify.
    topazRender2D_t * r2d, 

    /// Whether to be absolute.    
    int state
);

/// Sets the texture to use with this 2D object.
///
void topaz_render2d_set_texture(
    /// The render2d to modify.
    topazRender2D_t * r2d, 

    /// The texture to use with this render2d instance.
    topazRenderer_Texture_t * tex
);


/// Gets the spatial object for this 2D instance
///
topazSpatial_t * topaz_render2d_get_spatial(
    /// The render2d to query.
    topazRender2D_t * r2d
);


/// Gets the vertices for this 2d object.
///
const topazArray_t * topaz_render2d_get_vertices(
    /// The render2d to query.
    const topazRender2D_t * r2d
);

/// Sets the vertices 
///
void topaz_render2d_set_vertices(
    /// The render2d to modify
    topazRender2D_t * r2d, 

    /// The vertices to be copied into the render2d, replacing the old vertices.
    const topazArray_t * vertices
);


/// Retrieves information needed to render this object with the low-level
/// renderer
///
uint32_t topaz_render2d_get_render_data(
    /// The render2d to pull from.
    topazRender2D_t * r2d,

    /// The process attributes to populate. 
    topazRenderer_Attributes_t * attribs
);

#endif
