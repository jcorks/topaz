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


#ifndef H_TOPAZDC__MATERIAL__INCLUDED
#define H_TOPAZDC__MATERIAL__INCLUDED


#include <stdint.h>
typedef struct topaz_t topaz_t;
typedef struct topazRenderer_Program_t topazRenderer_Program_t;
typedef struct topazImage_Frame_t topazImage_Frame_t;
typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;
typedef struct topazRenderer_3D_t topazRenderer_3D_t;

/*

    Material
    -----

    Defines how 3D objects visually express their vertex data. This
    includes things such as textures, shader programs, and color definitions.

*/


typedef struct topazMaterial_t topazMaterial_t;
typedef struct topazMaterial_ProgramData_t topazMaterial_ProgramData_t;




/// The editable data of the material. These are the raw values that are 
/// accessible to material shaders.
/// The default is ambient = {1, 0, 1, 1};
///                diffuse = {1, 0, 1, 1}
///                specular = {1, 0, 1, 1}
///                userData = {0..0}
///
struct topazMaterial_ProgramData_t {
    /// The ambient color of the material.
    ///
    float ambient[4];

    /// The diffuse color of the material.
    ///
    float diffuse[4];

    /// The specular color of the material.
    ///
    float specular[4];

    /// Customly-defined data for the material. This is interpreted 
    /// based on the currently set program.
    float userData[32];
};


/// Creates a new material. These can then be used with mesh instances to render 
/// 3D object.s
///
topazMaterial_t * topaz_material_create(topaz_t *);

/// Creates a copy of the material with the same attributes as the source.
///
topazMaterial_t * topaz_material_clone(const topazMaterial_t *);

/// Frees a material
///
void topaz_material_destroy(topazMaterial_t *);



/// Gets a read-only reference to the program data that material programs 
/// use.
///
const topazMaterial_ProgramData_t * topaz_material_get_program_data(const topazMaterial_t *);

/// Sets the program data that material programs use.
///
void topaz_material_set_program_data(topazMaterial_t *, const topazMaterial_ProgramData_t *);

/// Sets the program that the material should use. See renderer.h
///
void topaz_material_set_program(topazMaterial_t *, topazRenderer_Program_t *);


/// Sets the given topazImage_Frame_t * to be a texture source 
/// at the given index slot.
///
void topaz_material_set_texture(topazMaterial_t *, int, const topazImage_Frame_t *);

/// Removes all index-texture associations within this material.
/// 
void topaz_material_clear_textures(topazMaterial_t *);

/// Sets the camera reference to use a framebuffer source.
///
void topaz_material_set_framebuffer_source(topazMaterial_t *, topazRenderer_Framebuffer_t *);


/// Populates a 3D renderer state with data from the material, 
/// allowing the 3D render state to use this material as specified.
///
void topaz_material_update_3d(topazMaterial_t *, topazRenderer_3D_t *);



#endif