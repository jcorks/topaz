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


#ifndef H_TOPAZDC__CONSOLE_DISPLAY_API__INCLUDED
#define H_TOPAZDC__CONSOLE_DISPLAY_API__INCLUDED

typedef struct topazColor_t topazColor_t;
typedef struct topazString_t topazString_t;
typedef struct topaz_t topaz_t;

typedef struct topazConsoleDisplay_t topazConsoleDisplay_t;

/*

    ConsoleDisplayAPI
    -----
    
    The set of functions that define how the console display abstraction should 
    behave.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/console_display.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazConsoleDisplayAPI_t topazConsoleDisplayAPI_t;




/// Each function is an implementation-facing copy of 
/// the user-side API for topazConsoleDisplay_t. See <topaz/backends/ConsoleDisplay.h>
///
struct topazConsoleDisplayAPI_t {
    /// Creates a console display
    ///
    void *                  (*console_display_create)           (topazConsoleDisplay_t *, topaz_t *);

    /// Destroys the console display
    /// 
    void                    (*console_display_destroy)          (topazConsoleDisplay_t *, void *);

    /// Adds a new line to the console display in the requested color.
    ///
    void                    (*console_display_add_line)         (topazConsoleDisplay_t *, void *, const topazString_t *, const topazColor_t * reqColor);

    /// Requests that the console display gets clear
    ///
    void                    (*console_display_clear)            (topazConsoleDisplay_t *, void *);


};



#endif
