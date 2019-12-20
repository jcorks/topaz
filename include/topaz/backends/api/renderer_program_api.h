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


#ifndef H_TOPAZDC__RENDERER_PROGRAM_API__INCLUDED
#define H_TOPAZDC__RENDERER_PROGRAM_API__INCLUDED

#include <stdint.h>
#include <topaz/backends/renderer_program.h>


typedef struct topazRenderer_t topazRenderer_t;



/*

    Renderer_ProgramAPI
    -----
    
    The set of functions that define how the renderer abstraction should 
    behave. Creating renderer abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/renderer_program.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazRenderer_ProgramAPI_t    topazRenderer_ProgramAPI_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazRenderer_t. See <topaz/backends/renderer_program.h>
///
struct topazRenderer_ProgramAPI_t {


    topazRenderer_Program_t * (*renderer_program_create)              (topazRendererAPI_t *,
                                                                        topazRenderer_ProgramAPI_t *,
                                                                        const topazString_t *, 
                                                                        const topazString_t *, 
                                                                        topazString_t *);
    topazRenderer_Program_t * (*renderer_program_create_preset)        (topazRendererAPI_t *,
                                                                        topazRenderer_ProgramAPI_t *,
                                                                        topazRenderer_PresetProgram);

    void                    (*renderer_program_destroy)             (topazRenderer_ProgramAPI_t *);



    /// User-given data. This is expected to data needed to persist
    /// throughout the liferenderer of the Renderer
    ///
    void * implementationData;


};


#endif
