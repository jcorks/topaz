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



#include <ft2build.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H

#include "backend.h"
#include <topaz/version.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    FT_Library lib;
    FT_Face    face;

} TopazFreetype;

void * topaz_freetype_renderer_create(topazFontRenderer_t * r, topaz_t * t) {
    TopazFreetype * ft = calloc(sizeof(TopazFreetype), 1);
    if (FT_Init_FreeType(&ft->lib)) {
        free(ft);
        return NULL;
    }
    return ft;
}

void topaz_freetype_renderer_destroy(topazFontRenderer_t * r, void * ftSrc) {
    TopazFreetype * ft = ftSrc;
    free(ft);
}

void topaz_freetype_set_font_data(
    // Renderer instance
    topazFontRenderer_t * renderer, 

    // Renderer userdata
    void * ftSrc, 

    // Raw bytes to send to the font renderer,
    // usually directly from a file.
    const uint8_t * srcBytes, 

    // number of bytes
    uint32_t size
) {
    TopazFreetype * ft = ftSrc;
    FT_New_Memory_Face(ft->lib, srcBytes, size, 0, &ft->face);
}



static uint8_t * topaz_freetype_render(
    /// renderer instance 
    ///
    topazFontRenderer_t * r, 

    /// Renderer userdata
    ///
    void * ftSrc,

    /// Character
    ///
    int charcode, 

    /// Size request for the character in pixels.
    ///
    int sizeRequest,

    /// The output size of the RGBA pixel buffer.
    uint32_t * w,

    /// The output 
    uint32_t * h
) {
    TopazFreetype * ft = ftSrc;
    int status;
    if (!(status = FT_Set_Pixel_Sizes(ft->face, 0, sizeRequest)) &&
         (status = FT_Load_Char(ft->face, charcode, FT_LOAD_RENDER))) {
        *w = 0;
        *h = 0;
        return NULL;
    }

    // handle basic space cases.
    if (isspace(charcode)) {
        *w = 1;
        *h = 1;
        uint8_t * out = malloc(sizeof(uint8_t)*4);
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        return out;
    }

    FT_GlyphSlot glyphSrc = ft->face->glyph;

    *w = glyphSrc->bitmap.width;
    *h = glyphSrc->bitmap.rows;
    uint8_t * texData = malloc(4 * (*w)*(*h));


    int pitch = (glyphSrc->bitmap.pitch);
    pitch = (pitch < 0 ? -1 : 1) * pitch;
    // get the data *grumble*

    uint32_t x, y;
    uint8_t * iter = texData;
    for(y = 0; y < *h; ++y) {
        for(x = 0; x < glyphSrc->bitmap.width; ++x) {
            iter[0] = 255;
            iter[1] = 255;
            iter[2] = 255;
            switch(glyphSrc->bitmap.pixel_mode) {
                case FT_PIXEL_MODE_GRAY:
                iter[3] = *(glyphSrc->bitmap.buffer + y*pitch + x);
                break;
                case FT_PIXEL_MODE_MONO:
                iter[3] = 255 * (((*(glyphSrc->bitmap.buffer + y*pitch)) << x) & 0x1);
                break;
            }
            iter+=4;
        }
    }


    return texData;

}



static void topaz_freetype_get_spacing(
    /// renderer instance 
    ///
    topazFontRenderer_t * r, 

    /// Renderer userdata
    ///
    void * ftSrc,

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

) {
    int status;
    TopazFreetype * ft = ftSrc;

    if (!(status = FT_Set_Pixel_Sizes(ft->face, 0, sizePixels)) &&
         (status = FT_Load_Char(ft->face, thischar, FT_LOAD_RENDER))) {
        return;
    }

    FT_GlyphSlot glyphSrc = ft->face->glyph;
    spacing->yMinimumSpace = ft->face->size->metrics.height/64.f;
    if (thischar == '\n') {
        spacing->xOffset = 0;
        spacing->yOffset = 0;
        spacing->yNextOrigin -= ft->face->size->metrics.height/64.f;
        spacing->xNextOrigin = 0;
        spacing->width = 0;
        spacing->height = 0;        
        return;
    }
    

    spacing->width = glyphSrc->bitmap.width;
    spacing->height = glyphSrc->bitmap.rows;

    if (FT_HAS_KERNING((ft->face))) {
        FT_Vector kernResult;
        FT_Get_Kerning(
            ft->face,
            FT_Get_Char_Index(ft->face, prevchar),
            FT_Get_Char_Index(ft->face, thischar),
            FT_KERNING_DEFAULT,
            &kernResult
        );
        spacing->xOffset = kernResult.x;
        spacing->yOffset = -kernResult.y;
    } else {
        spacing->xOffset = 0;
        spacing->yOffset = 0;
    }
    spacing->xNextOrigin += glyphSrc->advance.x/64.f;
    spacing->yNextOrigin -= glyphSrc->advance.y/64.f;

    spacing->yOffset = spacing->height - glyphSrc->metrics.horiBearingY/64.f;

}


void topaz_system_fontRenderer_freetype__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazFontRendererAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("FreeType"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("https://www.freetype.org/"),

        // desc 
        TOPAZ_STR_CAST("Bridge for FreeType."),



        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );

    api->font_renderer_create = topaz_freetype_renderer_create;
    api->font_renderer_destroy = topaz_freetype_renderer_destroy;
    api->font_renderer_set_font_data = topaz_freetype_set_font_data;
    api->font_renderer_render = topaz_freetype_render;
    api->font_renderer_query_spacing = topaz_freetype_get_spacing;

}








