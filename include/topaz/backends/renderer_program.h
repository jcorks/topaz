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


#ifndef H_TOPAZDC__RENDERER_PROGRAM__INCLUDED
#define H_TOPAZDC__RENDERER_PROGRAM__INCLUDED




#include <topaz/backends/renderer.h>



/* 

    RendererProgram
    -----

*/

typedef struct topazRenderer_Program_t topazRenderer_Program_t;




/// Preset programs are default contexts 
///
typedef enum {
    /// MaterialIDs for render objects should be only 4 floats
    /// Each float represents a color, all 4 corresponding to RGBA.
    /// No lighting is enabled
    ///
    topazRenderer_PresetProgram_Basic,

    /// The objects output color is determined by
    /// a phong shading-like algorithm that utilizes any Light objects
    /// and set material for the object
    ///
    topazRenderer_PresetProgram_Light,


} topazRenderer_PresetProgram;


// Compiles and enacts a shader for use with Static Rendering. If the shader fails
// to compile or is invalid in some way, the id returned is invalid. See StaticState.
// In any case, log will be populated with information on the building process status.
topazRenderer_Program_t * topaz_renderer_program_create(
    topazRenderer_t *,
    const topazString_t *   vertexSrc, 
    const topazString_t *   fragSrc, 
    topazString_t *         log
);


// Returns a new renderer program from built-in params.
topazRenderer_Program_t * topaz_renderer_program_create_preset(
    topazRenderer_t *,
    topazRenderer_PresetProgram
);


void topaz_renderer_program_destroy(topazRenderer_Program_t *);






#endif
