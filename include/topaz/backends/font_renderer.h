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
typedef struct topazAsset_t topazAsset_t;



/// An abstraction for generating image data 
/// for text characters.
typedef struct topazFontRenderer_t topazFontRenderer_t;






/// Creates a new font renderer object
///
topazFontRenderer_t * topaz_font_renderer_create(
    /// The topaz context.
    topaz_t * context, 


    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazFontRendererAPI_t api
);



/// Destroys and cleans up a font renderer API
///
void topaz_font_renderer_destroy(
    /// The font renderer to destroy
    topazFontRenderer_t * fRenderer
);





/// Retrieves the backend for this font renderer object.
///
topazSystem_Backend_t * topaz_font_renderer_get_backend(
    /// The font renderer to query.
    topazFontRenderer_t * fRenderer
);


/// Returns the API for this font renderer.
///
topazFontRendererAPI_t topaz_font_renderer_get_api(
    /// The font renderer to query.
    topazFontRenderer_t * fRenderer
);





/// Returns a pixel buffer containing the glyph 
/// in question. The pixel buffer is owned by the renderer.
/// Each call to topaz_font_renderer_glyph_ref() increments 
/// a reference count to the pixel buffer representing the 
/// glyph. Once the user is done with the buffer, they may 
/// call topaz_font_renderer_glyph_unref to decrement the 
/// reference count. Once the reference count reaches 0, 
/// the topazAsset_t * will be freed.
///
const topazAsset_t * topaz_font_renderer_image_ref(
    /// The font renderer to use.
    topazFontRenderer_t * fRenderer,

    /// The codepoint for the character.
    int charcode,

    /// The requested size in pixels.
    int sizePixels
);

/// Gives a hint to the font renderer to decrement 
/// the reference count for the character in question.
///
void topaz_font_renderer_image_unref(
    /// The font renderer to use.
    topazFontRenderer_t * fRenderer,

    /// The codepoint for the character.
    int charcode,

    /// The requested size in pixels.
    int sizePixels
);

/// Sets the font renderer's font data.
/// This corresponds to the data that the font renderer understands
/// which is backend-dependent. For most cases, this is 
/// raw data from an external source, i.e. a file.
void topaz_font_renderer_set_font_data(
    /// The font renderer to send data to.
    topazFontRenderer_t * fRenderer,

    /// The raw bytes to send to the font renderer.
    uint8_t * srcData,

    /// The number of raw bytes being sent to the font renderer.
    uint32_t size
);

typedef struct topazFontRenderer_Spacing_t topazFontRenderer_Spacing_t;

/// Object that holds spacing information, allowing for more complex placement of 
/// characters next to each other.
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

    /// The minimum amount of space needed to display the font in the 
    /// y direction given the size of the font.
    /// This is always populated even for characters that have 
    /// no valid glyph.
    float yMinimumSpace;
};



/// Returns sizing information for a character.
void topaz_font_renderer_query_spacing(
    /// The font renderer to use.
    topazFontRenderer_t * fRenderer,

    /// The spacing information to populate.
    /// The xNextOrigin and yNextOrigin should be 
    /// zero'd when starting a new string of characters,
    /// as the output xNextOrigin and yNextOrigin are relative
    /// to the initial character.
    ///
    topazFontRenderer_Spacing_t * spacing,

    /// Requested size of the font in pixels.
    ///
    int sizePixels, 
    
    /// The previous character in the string. If none,
    /// should be 0.
    ///
    int prevchar,

    /// The current character to request spacing info for.
    ///
    int thischar,

    /// The previous character in the string. If none,
    /// should be 0.
    ///
    int nextchar
);





#endif
