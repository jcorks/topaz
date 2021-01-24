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
#include <topaz/system.h>
typedef struct topaz_t topaz_t;



/// Defines a visual interface for interacting with a console.
typedef struct topazConsoleDisplay_t topazConsoleDisplay_t;






/// Creates a new console display instance.
///
topazConsoleDisplay_t * topaz_console_display_create(
    /// The topaz context
    topaz_t * context, 

    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazConsoleDisplayAPI_t api
);


/// Destroys and cleans up a console display instance.
///
void topaz_console_display_destroy(
    /// The console display to destroy.
    topazConsoleDisplay_t * cDisplay
);



/// Gets the backend for this console display.
///
topazSystem_Backend_t * topaz_console_display_get_backend(
    /// The console display to query.
    topazConsoleDisplay_t * cDisplay
);


/// Returns the API for this console display.
///
const topazConsoleDisplayAPI_t * topaz_console_display_get_api(
    /// The console display to query.
    topazConsoleDisplay_t * cDisplay
);





/// Clears all lines from the console display. 
/// After this call, the line history is also cleared.
/// This means topaz_console_display_get_line_count() will return 0 and 
/// all topaz_console_display_get_line() calls will return the empty string.
/// 
void topaz_console_display_clear(
    /// The console display to clear.
    topazConsoleDisplay_t * cDisplay
);


/// Adds text to the current line in the given color.
///
void topaz_console_display_add_text(
    /// The console display to modify.
    topazConsoleDisplay_t * cDisplay, 

    /// The text to add to the current line.
    const topazString_t * text, 

    /// The color of the text to add.
    const topazColor_t * color
);

/// Adds a line to the console display. This line becomes the next line to 
/// be display. This increases the line count by 1. The line count prior 
/// to this call becomes this new line's index. This index can be used with 
/// topaz_console_display_get_line().
///
void topaz_console_display_new_line(
    /// The console display to add a new line to.
    topazConsoleDisplay_t * cDisplay
);


/// Gets the count of lines that were added to the console display.
///
uint32_t topaz_console_display_get_line_count(
    /// The console display to query.
    const topazConsoleDisplay_t * cDisplay
);

/// Gets a read-only copy of the line for the given index.
///
const topazString_t * topaz_console_display_get_line(
    /// The console display to query.
    const topazConsoleDisplay_t * cDisplay, 

    /// The line to read.
    uint32_t line
);


/// Sends input as if it came from the console. This is indistinguishable from 
/// externally generated input.
///
void topaz_console_display_send_input(
    /// The console display to send input to.
    topazConsoleDisplay_t * cDisplay, 

    /// The text to send.
    const topazString_t * input
);


/// Enables or disables the console display.
/// 
void topaz_console_display_enable(
    /// The console display to toggle.
    topazConsoleDisplay_t * cDisplay, 

    /// Whether to enable.
    int enabled
);


/// Callback called by the console display if the user sends 
/// input to the console display. A full line of input is 
/// will be returned. The input isn't printed back to 
/// the console display, so it is up to the callback itself 
/// to print it to the display when applicable.
///
typedef void (*topaz_console_display_input_callback)(
    /// The console display that received the event.
    topazConsoleDisplay_t * cDisplay,

    /// The text received. 
    const topazString_t * line,

    /// Data bound to the console display. 
    void * userData
);


/// Adds an input callback to be called when input from the user is 
/// received. The ID of the callback is returned. This callback can
/// be used to remove the callback.
///
uint32_t topaz_console_display_add_input_callback(
    /// The console display to add an input hook to.
    topazConsoleDisplay_t * cDisplay, 

    /// The callback to add.
    topaz_console_display_input_callback callback,

    /// The data to bind to the callback.
    void * data
);

/// Removes an added callback. If the ID doesnt refer to a 
/// input callback, then no action is taken.
///
void topaz_console_display_remove_input_callback(
    /// The console display to remove a callback from.
    topazConsoleDisplay_t * cDisplay,

    /// The id of the callback to remove.
    uint32_t id
);




#endif
