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


#ifndef H_TOPAZDC__COLOR__INCLUDED
#define H_TOPAZDC__COLOR__INCLUDED

#include <topaz/containers/string.h>



///
///    A standard object representing an RGBA, 32-bit color
///
///
typedef struct topazColor_t topazColor_t;
struct topazColor_t {
    /// The red component of the color.
    ///
    uint8_t r;

    /// The green component of the color.
    ///
    uint8_t g;

    /// The blue component of the color.
    ///
    uint8_t b;

    /// The alpha/opacity component of the color.
    ///
    uint8_t a;
};


/// Sets the color to the color referred to by string.
/// In the case that the string starts with a '#', the
/// remainder of the string will be read as a hexidecimal
/// integer referring to the values of the hex string. For example
/// Color("#FF00FFFF") is equivalent to Color(255, 0, 255, 255)
/// It is also valid to specify only 3 components:
/// Color("#FF00FF") is equivalent to Color(255, 0, 255, 255). If the string does not
/// begin with a '#', the string is impied to represent a string that has been
/// mapped with Define().
/// 
topazColor_t topaz_color_from_string(
    /// The color encoded into a string.
    const topazString_t * str
);

/// Returns an int representation of the color
///
int topaz_color_to_rgba_int(
    /// Color object to read.
    topazColor_t color
);

/// Returns a color based on floating point decimals  
/// each float is interpreted on a range form 0 to 1.0.
/// The value is clamped onthis range.
///
topazColor_t topaz_color_from_amt(
    /// Red amount of color
    float r, 

    /// Green amount of color
    float g, 

    /// Blue amount of color
    float b,

    /// Alpha (transparency) amount of color 
    float a
);

/// Returns a color based on integers.
///
topazColor_t topaz_color_from_int(
    /// Red component. 0-255
    uint8_t r, 
    
    /// Green component. 0-255
    uint8_t g, 

    /// Blue component. 0-255
    uint8_t b, 

    /// Alpha component. 0-255
    uint8_t a
);

/// Returns a decimal amount of the red component for the current color
/// __C__ is the color object, of type topazColor_t
///
#define topaz_color_r_amt(__C__) (__C__.r / 255.0)

/// Returns a decimal amount of the green component for the current color
/// __C__ is the color object, of type topazColor_t
///
#define topaz_color_g_amt(__C__) (__C__.g / 255.0)

/// Returns a decimal amount of the blue component for the current color
/// __C__ is the color object, of type topazColor_t
///
#define topaz_color_b_amt(__C__) (__C__.b / 255.0)

/// Returns a decimal amount of the alpha component for the current color
/// __C__ is the color object, of type topazColor_t
///
#define topaz_color_a_amt(__C__) (__C__.a / 255.0)

/// Returns whether 2 colors are equal in value.
/// __C1__ and __C2__ are the color objects, of type topazColor_t
///
#define topaz_color_cmp(__C1__,__C2__) (__C1__.r == __C2__.r && __C1__.g == __C2__.g && __C1__.b == __C2__.b && __C1__.a == __C2__.a)

/// Returns a string representation of the color.
/// This can be used with topaz_color_from_string() to retrieve the 
/// color once more.
/// This function is not thread-safe
topazString_t * topaz_color_to_string(
    /// The color option to convert.    
    topazColor_t color
);



#endif


