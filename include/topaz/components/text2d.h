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


/*

    Text2D
    -----
    
    Produces simple 2D Text


*/


/// Creates a new text component with no text.
///
topazComponent_t * topaz_text2d_create(topaz_t*);


/// Sets the text content and font size to use.
///
void topaz_shape2d_set_text(topazComponent_t *,
    const topazString_t *,
    int pixelSize,
);

/// Sets the text content and font size to use,
/// but uses the same spacing between all characters
///
void topaz_shape2d_set_text_monospace(
    topazComponent_t *,
    const topazString_t *,
    int pixelSize,
);

/// Gets the text being displayed by the component.
///
const topazString_t * topaz_shape2d_get_text(topazComponent_t *);

/// Gets the width of the text without transformation.
///
float topaz_text2d_get_extent_width(topazComponent_t *);

/// Gets the height of the text without transformation.
///
float topaz_text2d_get_extent_height(topazComponent_t *);

/// Gets the x position of the character at the given index.
/// This refers to its top-left.
///
float topaz_text2d_get_char_x(topazComponent_t *, int charIndex);

/// Gets the x position of the character at the given index.
/// This refers to its top-left.
///
float topaz_text2d_get_char_y(topazComponent_t *, int charIndex);


/// Gest the node transform for the text
///
topazTransform_t * topaz_text2d_get_node(topazComponent_t *);

/// Sets the color for certain characters.
///
void topaz_text2d_set_color_section(
    topazComponent_t *, 
    int fromIndex,
    int toIndex,
    const topazColor_t *
);

/// Sets the color for all characters.
///
void topaz_text2d_set_color(
    topazComponent_t *,
    const topazColor_t *
)





#endif


