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


#ifndef H_TOPAZDC__CONSOLE_DISPLAY__INCLUDED
#define H_TOPAZDC__CONSOLE_DISPLAY__INCLUDED

#include <topaz/backends/api/console_display_api.h>
#include <topaz/backends/backend.h>



/*

    Console Display
    -----
    Abstracts an interface for interacting with a console.

*/

typedef struct topazConsoleDisplay_t topazConsoleDisplay_t;






/// Creates a new console display instance.
///
topazConsoleDisplay_t * topaz_console_display_create(topaz_t *, topazConsoleDisplayAPI_t);


/// Destroys and cleans up a console display instance.
///
void topaz_console_display_destroy(topazTime_t *);



/// Gets the backend for this console display.
///
topazBackend_t * topaz_console_display_get_backend(topazConsoleDisplay_t *);


/// Returns the API for this console display.
///
topazConsoleDisplayAPI_t topaz_time_get_api(topazConsoleDisplay_t *);





/// Clears all lines from the console display. 
/// After this call, the line history is also cleared.
/// This means topaz_console_display_get_line_count() will return 0 and 
/// all topaz_console_display_get_line() calls will return the empty string.
/// 
void topaz_console_display_clear(topazConsoleDisplay_t * t);

/// Adds a line to the console display. This line becomes the next line to 
/// be display. This increases the line count by 1. The line count prior 
/// to this call becomes this new line's index. This index can be used with 
/// topaz_console_display_get_line().
///
void topaz_console_display_add_line(topazConsoleDisplay_t * t, const topazString_t *);

/// Gets the count of lines that were added to the console display.
///
uint32_t topaz_console_display_get_line_count(const topazConsoleDisplay_t *);

/// Gets a read-only copy of the line for the given index.
///
const topazString_t * topaz_console_display_get_line(const topazConsoleDisplay_t *, uint32_t);


/// Sets inp
void topaz_console_display_send_input(topazConsoleDisplay_t *, const topazString_t *);


/// Callback called by the console display if the user sends 
/// input to the console display. A full line of input is 
/// will be returned. The input isn't printed back to 
/// the console display, so it is up to the callback itself 
/// to print it to the display when applicable.
///
typedef void (*topaz_console_display_input_callback)(topazConsoleDisplay_t *, const topazString_t * line, void * userData);


/// Adds an input callback to be called when input from the user is 
/// received. The ID of the callback is returned. This callback can
/// be used to remove the callback.
///
uint32_t topaz_console_display_add_input_callback(
    topazConsoleDisplay_t *, 
    topaz_console_display_input_callback,
    void * data
);

/// Removes an added callback. If the ID doesnt refer to a 
/// input callback, then no action is taken.
///
void topaz_console_display_remove_input_callback(
    topazConsoleDisplay_t *,
    uint32_t id;
)

#endif
