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


#ifndef H_TOPAZDC__FONT_RENDERER__INCLUDED
#define H_TOPAZDC__FONT_RENDERER__INCLUDED

#include <topaz/backends/api/font_renderer_api.h>
#include <topaz/system.h>



/*

    FontRenderer
    -----
    An abstraction for generating image data 
    for text characters.

*/

typedef struct topazFontRenderer_t topazFontRenderer_t;






/// Creates a new time object
///
topazFontRenderer_t * topaz_font_renderer_create(topaz_t *, topazSystem_Backend_t *, topazTimeAPI_t);


/// Destroys and cleans up a time API
///
void topaz_font_renderer_destroy(topazFontRenderer_t *);





/// Retrieves the backend for this time object.
///
topazSystem_Backend_t * topaz_font_renderer_get_backend(topazFontRenderer_t *);


/// Returns the API for this time.
///
topazFontRendererAPI_t topaz_font_renderer_get_api(topazFontRenderer_t *);




/// Returns a pixel buffer containing the glyph 
/// in question. The pixel buffer is owned by the renderer.
/// Each call to topaz_font_renderer_glyph_ref() incremenebts 
/// a reference count to the pixel buffer representing the 
/// glyph. Once the user is done with the buffer, they may 
/// call topaz_font_renderer_glyph_unref to decrement the 
/// reference count. Once the reference count reaches 0, 
/// the topazImage_t * will be freed.
///
const topazImage_t * topaz_font_renderer_glyph_ref(
    topazFontRenderer_t *,

    /// The codepoint for the character.
    ///
    int charcode,

    /// The requested size in pixels.
    ///
    int sizePixels
);

void topaz_font_renderer_glyph_unref(
    topazFontRenderer_t *,
    
    int charcode
);



typedef struct topazFontRenderer_Spacing_t topazFontRenderer_Spacing_t;



struct topazFontRenderer_Spacing_t {
    /// The physical width of this characters visual. 
    /// It may differ from the generated glyph, which may need to 
    /// be texture mapped to fit this width.
    ///
    float width;

    /// The physical height of this characters visual. 
    /// It may differ from the generated glyph, which may need to 
    /// be texture mapped to fit this height.
    ///
    float height;

    /// Offset x position this character should be from the origin
    ///
    float xOffset;

    /// Offset y position this character should be from the origin 
    ///
    float yOffset;

    /// The x position of the origin to use for the next character.
    ///
    float xNextOrigin;

    /// The y position of the origin to use for the next character.
    ///
    float yNextOrigin;
};


const topazFontRenderer_Spacing_t * topaz_font_renderer_query_spacing(
    topazFontRenderer_t *,
    int 
    /// The current character to request spacing info for.
    ///
    int thischar,

    /// The next character 
    int nextchar
);





#endif
