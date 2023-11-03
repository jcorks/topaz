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
///    A standard object representing an RGBA, float-based color.
///    Each component is a saturation amount from 0 to 1.
///
///
typedef struct topazColor_t topazColor_t;
struct topazColor_t {
    /// The red component of the color.
    ///
    float r;

    /// The green component of the color.
    ///
    float g;

    /// The blue component of the color.
    ///
    float b;

    /// The alpha/opacity component of the color.
    ///
    float a;
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




/// Returns a color based on integers.
///
/// (No script mapping)
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


/// Returns a string representation of the color.
/// This can be used with topaz_color_from_string() to retrieve the 
/// color once more.
/// This function is not thread-safe
topazString_t * topaz_color_to_string(
    /// The color option to convert.    
    topazColor_t color
);



#endif


