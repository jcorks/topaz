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


#ifndef H_TOPAZDC__FONT_RENDERER_API__INCLUDED
#define H_TOPAZDC__FONT_RENDERER_API__INCLUDED

#include <stdint.h>

typedef struct topaz_t topaz_t;
typedef struct topazFontRenderer_t topazFontRenderer_t;
typedef struct topazFontRenderer_Spacing_t topazFontRenderer_Spacing_t;

/*

    FontRendererAPI
    -----
    
    The set of functions that define how the font renderer abstraction should 
    behave.


*/
typedef struct topazFontRendererAPI_t topazFontRendererAPI_t;

// Each function is an implementation-facing copy of 
// the user-side API for topazFontRenderer_t. See <topaz/backends/FontRenderer.h>
//
struct topazFontRendererAPI_t {
    void * (*font_renderer_create) (topazFontRenderer_t *, topaz_t *);
    void   (*font_renderer_destroy) (topazFontRenderer_t *, void *);


    // Returns a new RGBA pixel buffer. It should be free()d after done
    //
    uint8_t * (*font_renderer_render)(
        // renderer instance 
        //
        topazFontRenderer_t *, 

        // Renderer userdata
        //
        void *,

        // Character
        //
        int charcode, 

        // Size request for the character in pixels.
        //
        int sizeRequest,

        // The output size of the RGBA pixel buffer.
        uint32_t * w,

        // The output 
        uint32_t * h
    );



    void (*font_renderer_query_spacing) (
        // renderer instance 
        //
        topazFontRenderer_t *, 

        // Renderer userdata
        //
        void *,

        // The spacing information to populate.
        // The xNextOrigin and yNextOrigin should be 
        // zero'd when starting a new string of characters,
        // as the output xNextOrigin and yNextOrigin are relative
        // to the initial character.
        //
        topazFontRenderer_Spacing_t *,

        // Requested size of the font in pixels.
        //
        int sizePixels, 

        // The previous character in the string. If none,
        // should be 0.
        //
        int prevchar,

        // The current character to request spacing info for.
        //
        int thischar,

        // The previous character in the string. If none,
        // should be 0.
        //
        int nextchar

    );
};






#endif
