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
///    Render2D
///    -----
///    Base object to render a 2D object.
///    
///
///
typedef struct topazRender2D_t topazRender2D_t;



/// Creates a new 2d object. If a collection of 
/// render2d objects are used, you may pass a pre-existing 
/// spatial object, which will be used to generate the local transform 
/// for the object. Else, passing NULL will prompt the render2d 
/// instance to create its own.
///
topazRender2D_t * topaz_render2d_create(topazRenderer_2D_t *, topazSpatial_t *);

/// Destroys the 2d object.
///
void topaz_render2d_destroy(topazRender2D_t *);



typedef enum topazRender2D_Parameter topazRender2D_Parameter;

/// The parameters control more fine operations when rendering the 
/// the objects.
///
enum topazRender2D_Parameter {
    /// The transparency rule for the 2D object.
    /// Default is topazRenderer_AlphaRule_Allow
    ///
    topazRender2D_Parameter_AlphaRule,

    /// The transparency rule for the 2D object.
    /// Default is topazRenderer_AlphaRule_Allow
    ///
    topazRender2D_Parameter_DepthTest,

    /// The currently set etch rule, which determines how the 
    /// etch rules is applied when rendering this 2d object.
    /// Default is topazRenderer_EtchRule_Out
    ///
    topazRender2D_Parameter_EtchRule,

    /// The texturing hint for the 2D object.
    /// Default is topazRenderer_TextureFilterHint_Linear
    ///
    topazRender2D_Parameter_TextureFilterHint
};



/// Sets the parameter value. The value accepted is one of the appropriate 
/// enum values within renderer.h
///
void topaz_render2d_set_parameter(topazRender2D_t *, topazRender2D_Parameter, int);

/// Gets the parameter value.
///
int topaz_render2d_get_parameter(topazRender2D_t *, topazRender2D_Parameter);


/// Gets whether this 2D object is renderered in abolute coordinates
///
int topaz_render2d_get_absolute(const topazRender2D_t *);

/// Sets whether drawing calculations from the node should be interpreted as absolute 
/// pixel values on the display or should be within the hierarchy. 
/// The default is false.
///
void topaz_render2d_set_absolute(topazRender2D_t *, int);

/// Sets the texture to use with this 2D object.
///
void topaz_render2d_set_texture(topazRender2D_t *, topazRenderer_Texture_t *);

/// Gets the primitive for this 2D object. The default is 
/// topazRenderer_Primitive_Triangle
///
topazRenderer_Primitive topaz_render2d_get_primitive(const topazRender2D_t *);

/// Sets the primitive for the 2D object
///
void topaz_render2d_set_primitive(topazRender2D_t *, topazRenderer_Primitive);

/// Gets the spatial object for this 2D instance
///
topazSpatial_t * topaz_render2d_get_spatial(topazRender2D_t *);


/// Gets the vertices for this 2d object.
///
const topazArray_t * topaz_render2d_get_vertices(const topazRender2D_t *);

/// Sets the vertices 
///
void topaz_render2d_set_vertices(topazRender2D_t *, const topazArray_t *);


/// Retrieves information needed to render this object with the low-level
/// renderer
///
uint32_t topaz_render2d_get_render_data(topazRender2D_t *, topazRenderer_ProcessAttribs_t *);

#endif
