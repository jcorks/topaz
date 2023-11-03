/*

Copyright (c) 2018, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the Dynacoe project (https://github.com/jcorks/Dynacoe)
Dynacoe was released under the MIT License, as detailed below.



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


#ifndef H_TOPAZDC__TEXT2D__INCLUDED
#define H_TOPAZDC__TEXT2D__INCLUDED
#include <topaz/color.h>
#include <topaz/render2d.h>
typedef struct topazComponent_t topazComponent_t;
typedef struct topazString_t topazString_t;
typedef struct topaz_t topaz_t;
typedef struct topazTransform_t topazTransform_t;
typedef struct topazAsset_t topazAsset_t;
    
/// Component that, when attached, produces a simple 2D text visual.
typedef struct topazText2D_t topazText2D_t;

/// Creates a new text component with no text.
///
topazComponent_t * topaz_text2d_create(
    /// The topaz context.
    topaz_t * context
);


/// Sets the text content for the text2d object.
///
void topaz_text2d_set_text(
    /// The text2d to modify.
    topazComponent_t * text2d,

    /// The text to copy as source into the text2d.
    const topazString_t * text

);


/// Sets the font for the text 
void topaz_text2d_set_font(
    /// The text2d to modify
    topazComponent_t * c,
    
    /// The font to use. Should be a registered font asset.
    /// See topaz_font_manager_register_font().
    const topazAsset_t * font,
    
    /// The pixel size of the text to appear.
    int pixelSize
);


/// Gets the text being displayed by the component.
///
const topazString_t * topaz_text2d_get_text(
    /// The text2d to query.
    topazComponent_t * text2d
);

/// Gets the width of the text without transformation.
///
float topaz_text2d_get_extent_width(
    /// The text2d to query.
    topazComponent_t * text2d
);

/// Gets the height of the text without transformation.
///
float topaz_text2d_get_extent_height(
    /// The text2d to query.
    topazComponent_t * text2d
);

/// Gets the x position of the character at the given index.
/// This refers to its top-left.
///
float topaz_text2d_get_char_x(
    /// The text2d to query.
    topazComponent_t * text2d,

    /// The index of the character to query.
    int index
);

/// Gets the x position of the character at the given index.
/// This refers to its top-left.
///
float topaz_text2d_get_char_y(
    /// The text2d to query.
    topazComponent_t * text2d,

    /// The index of the character to query.
    int index
);


/// Gest the node transform for the text
///
/// This function has no script mapping; however,
/// the position, scale, and rotation for this 
/// node are settable and gettable via 
/// setPosition, setScale, setRotation, 
/// getPosition, getScale, and getRotation as part 
/// of the text2d instance.
///
topazTransform_t * topaz_text2d_get_node(
    /// The text2d to query.
    topazComponent_t * text2d
);

/// Sets the color for certain characters.
///
void topaz_text2d_set_color_section(
    /// The text2d to modify.
    topazComponent_t * text2d, 

    /// The start character index to set the color.
    int fromIndex,

    /// The end character index to set the color.
    int toIndex,

    /// The color to set.
    topazColor_t color
);

/// Sets the color for all characters.
///
void topaz_text2d_set_color(
    /// The text2d to modify.
    topazComponent_t * text2d,

    /// The color to set.
    topazColor_t color
);


/// Sets the rendering attributes.
///
void topaz_text2d_set_attribute(
    /// The text to modify
    topazComponent_t * text2d, 

    /// The attribute to modify
    topazRenderer_Attribute attribute,

    /// The new attribute value
    int value
);

/// Gets the rendering attributes.
///
int topaz_text2d_get_attribute(
    /// The text to query.
    topazComponent_t * text2d,
    
    /// The attribute to query
    topazRenderer_Attribute attribute    
);







#endif


