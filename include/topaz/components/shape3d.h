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


#ifndef H_TOPAZDC__SHAPE3D__INCLUDED
#define H_TOPAZDC__SHAPE3D__INCLUDED

#include <topaz/component.h>
#include <topaz/color.h>
#include <topaz/vector.h>
#include <topaz/render2d.h>
typedef struct topazTransform_t topazTransform_t;
typedef struct topazAsset_t topazAsset_t;

/// Can express objects in 3D space.
typedef struct topazShape3D_t topazShape3D_t;



/// Every shape3d can have textures attached to it, which are
/// used by rendering programs.
typedef enum topazShape3D_TextureSlot topazShape3D_TextureSlot;
enum topazShape3D_TextureSlot {
    /// The first slot for texturing.
    topazShape3D_TextureSlot_0,
    /// The second slot for texturing.
    topazShape3D_TextureSlot_1,
    /// The third slot for texturing.
    topazShape3D_TextureSlot_2
};



/// Creates a new shape3d object
/// Use component_destroy to release.
///
topazComponent_t * topaz_shape3d_create(
    /// The topaz context.
    topaz_t * context
);


/// Gets the trasform for the object
///
topazTransform_t * topaz_shape3d_get_node(
    /// The shape3d to query.
    topazComponent_t * shape3d
);



/// Sets the image to use for the given texture slot.
void topaz_shape3d_set_texture(
    /// The shape to modify.
    topazComponent_t * shape3d,
    /// The slot to modify.
    topazShape3D_TextureSlot slot,
    /// The image to place at the slot. The 0th frame is used as the texture.
    topazAsset_t * image
);


/// Sets the special framebuffer source item.
void topaz_shape3d_set_sample_framebuffer(
    /// The shape to modify.
    topazComponent_t * shape3d,
    /// The framebuffer to act as a source.
    topazRenderer_Framebuffer_t * framebuffer;
);


/// Sets the mesh to use for this shape.
void topaz_shape3d_set_mesh(
    /// The shape to modify.
    topazComponent_t * shape3d,

    /// The mesh asset to use.
    topazAsset_t * mesh
);


/// Sets the material to use for this shape.
void topaz_shape3d_set_material(
    /// The shape to modify.
    topazComponent_t * shape3d,
    /// The material to use.
    topazAsset_t * material
);



/// Sets the parameter value. The value accepted is one of the appropriate 
/// enum values within renderer.h
///
void topaz_shape3d_set_attributes(
    /// The shape3d to modify
    topazComponent_t * shape3d, 

    /// The new process attributes
    const topazRenderer_Attributes_t * att
);

/// Gets the parameter value.
///
const topazRenderer_Attributes_t * topaz_shape3d_get_attributes(
    /// The shape3d to query.
    topazComponent_t * shape3d

);



#endif
