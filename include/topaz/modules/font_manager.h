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

typedef struct topazString_t topazString_t;
typedef struct topaz_t topaz_t;
typedef struct topazAsset_t topazAsset_t;
typedef struct topazFontRenderer_t topazFontRenderer_t;



/// Font managers are responsible for 
/// controlling interactions with topaz and multiple, distinct fonts.
///
typedef struct topazFontManager_t topazFontManager_t;





/// Creates a new font manager. 
///
/// (No script mapping)
///
topazFontManager_t * topaz_font_manager_create(
    /// The context.
    topaz_t * context
);

/// Destroys and frees a topaz font manager instance.
///
/// (No script mapping)
///
void topaz_font_manager_destroy(
    /// The font manager to destroy.
    topazFontManager_t * fontManager
);


/// Adds a new font to be registered with the font manager 
/// by name. The name for the font is the unique name of the 
/// asset loaded within the topaz instance's resource instance. 
/// In most circumstances, this name is one used 
/// to refer to the font from topaz's point of view.
///
/// Returns success.
///
int topaz_font_manager_register_font(
    /// The font manager to register the new font with
    topazFontManager_t * fontManager,
    
    /// The name of the asset that contains the raw font data, specific 
    /// to the backend. The asset type needs to be 
    /// a raw data asset.
    const topazAsset_t * asset
);


/// Returns the font renderer for the specific font requested.
/// The font name should be the original font data asset's name.
/// If no such renderer exists, NULL is returned.
///
/// (No script mapping)
///
topazFontRenderer_t * topaz_font_manager_get_renderer(
    /// The font manager to query.
    topazFontManager_t * fontManager,
    
    /// The name of the font to get the renderer for.
    const topazAsset_t * asset
);


/// Returns the first available font renderer 
/// that the manager is aware of.
///
/// (No script mapping)
///
topazFontRenderer_t * topaz_font_manager_get_renderer_any(
    /// The font manager to query.
    topazFontManager_t * fontManager
);


/// Convenience function to get tell the font renderer 
/// to transfer characters glyphs into textures ahead of time
/// for the requested font, size, and characters.
/// This can speed up processing time when first displaying characters.
void topaz_font_manager_preload_glyphs(
    /// The font manager to work with.
    topazFontManager_t * f,
    
    /// The name of the font to preload.
    const topazAsset_t * font,
    
    /// The size of the font to preload.
    int sizeRequest,
    
    /// A string of characters to preload.
    const topazString_t * characters
);

/// Removes the font from the manager by name.
void topaz_font_manager_unregister_font(
    /// The font manager to modify.
    topazFontManager_t * fontManager,
    
    /// The name of the font to remove.
    const topazAsset_t * font
);







#endif
