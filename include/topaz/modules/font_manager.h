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


#ifndef H_TOPAZDC__FONTMANAGER__INCLUDED
#define H_TOPAZDC__FONTMANAGER__INCLUDED


typedef struct topazFontManager_t topazFontManager_t;





/// Creates a new graphics instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_graphics();
///
topazFontManager_t * topaz_font_manager_create(
    /// The context.
    topaz_t * context
);

/// Destroys and frees a topaz input instance.
///
void topaz_font_manager_destroy(
    /// The graphics instance.
    topazFontManager_t * graphics
);



void topaz_font_manager_register(
    topazFontManager_t * f,
    const topazString_t * fontName,
    topazAsset_t * dataAsset
);

void

topazFontRenderer_t * topaz_font_manager_get_renderer(
    topazFontManager_t * f,
    const topazString_t * fontName
);

topazFontRenderer_t * topaz_font_manager_get_renderer_any(
    topazFontManager_t * f
);

topazFontRenderer_t * topaz_font_manager_preload_glyphs(
    topazFontManager_t * f,
    topazString_t * fontName,
    const topaz
);








#endif
