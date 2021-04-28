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
typedef struct topazRenderer_3D_t topazRenderer_3D_t;
typedef struct topazAsset_t topazAsset_t;
typedef struct topazTable_t topazTable_t;


///
///    Defines how 3D objects visually express their vertex data. This
///    includes things such as textures, shader programs, and color definitions.
///
///
typedef struct topazMaterial_t topazMaterial_t;




typedef struct topazMaterial_ProgramData_t topazMaterial_ProgramData_t;

/// The editable data of the material. These are the raw values that are 
/// accessible to material shaders.
/// The default is ambient =  {1, 0, 1, 1}
///                diffuse =  {1, 0, 1, 1}
///                specular = {1, 0, 1, 1}
///                userData = {0..0} (16 floats)
///
struct topazMaterial_ProgramData_t {
    /// Customly-defined data for the material. This is interpreted 
    /// based on the currently set program.
    float userData[28];
};


// Creates a new material. These can then be used with mesh instances to render 
// 3D object.s
//
topazAsset_t * topaz_material_create(
    topaz_t *, 
    const topazString_t *
);

// Creates a new data asset object, but without 
// any loading behavior. 
//
topazAsset_t * topaz_material_empty(
    topaz_t *
);





/// Gets a read-only reference to the program data that material programs 
/// use.
///
const topazMaterial_ProgramData_t * topaz_material_get_program_data(
    /// The material to query.
    topazAsset_t * mat
);

/// Sets the program data that material programs use.
///
void topaz_material_set_program_data(
    /// The material to modify
    topazAsset_t * mat, 

    /// The program data to set.
    const topazMaterial_ProgramData_t * pd
);



/// Sets the source for the program to use.
/// If the source changes, and the language matches that 
/// of the backend renderer, the shader program will 
/// attempt to be compiled on-the-fly. 
/// If compilation fails, a default, error shader will be 
/// be utilized.
/// Materials can store vertex and fragment source for 
/// a variety of languages, that way materials can be kept 
/// and maintained for better portability.
void topaz_material_set_program_source(
    /// The material to modify
    topazAsset_t * mat, 

    /// The language that this source pertains to.
    const topazString_t * language,

    /// The vertex shader source.    
    const topazString_t * vertexSource,
    
    /// The fragment shader source.
    const topazString_t * fragmentSource
);


/// Returns a table of shader languages with mapped vertex
/// source string.
const topazTable_t * topaz_material_get_vertex_source_table(
    /// The material to query.
    topazAsset_t * mat
);

/// Returns a table of shader languages with mapped vertex
/// source string.
const topazTable_t * topaz_material_get_fragment_source_table(
    /// The material to query.
    topazAsset_t * mat
);

/// Removes all shader program sources from the material.
void topaz_material_clear_sources(
    /// The material to modify.
    topazAsset_t * mat
);


void topaz_material_update_3d(topazAsset_t * m, topazRenderer_3D_t * d3);





#endif
