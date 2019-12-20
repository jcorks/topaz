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


#ifndef H_TOPAZDC__RENDERER_LIGHT__INCLUDED
#define H_TOPAZDC__RENDERER_LIGHT__INCLUDED




#include <topaz/backends/renderer.h>


/*

    RendererLight
    -----
    TODO: change to RendererProgramData as a generic 
    buffer, which is more accurate and usable.

*/

typedef struct topazRenderer_Light_t topazRenderer_Light_t;




/// Types of rendering lights.
typedef enum {
    topazRenderer_LightType_Point,
    topazRenderer_LightType_Directional,
    topazRenderer_LightType_Spot
} topazRenderer_LightType;



// Creates a new light. When the light is first created, the state of the light
// is undefined. (It is expected that you will update the lights attributes
// before drawing);
topazRenderer_Light_t * topaz_renderer_light_create(topazRenderer_t *, topazRenderer_LightType);

// Removes all resources associated with the light ID.
void topaz_renderer_light_destroy(topazRenderer_Light_t *);

// As the default, the attributes requred are
//  Position (3-components)
//  Color (3-components)
//  Intensity (1 component)
// Thus, the renderer expects the array passed to have at least 7 components.

void topaz_renderer_light_update_attribs(topazRenderer_Light_t *, float *);

// Enables or diables the light by default, once added, the light is enabled by default
void topaz_renderer_light_enable(topazRenderer_Light_t *, int doIt);






#endif
