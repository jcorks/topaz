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


#ifndef H_TOPAZDC__DISPLAY_API__INCLUDED
#define H_TOPAZDC__DISPLAY_API__INCLUDED

#include <stdint.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>


typedef struct topazDisplay_t topazDisplay_t;

/*

    DisplayAPI
    -----
    
    The set of functions that define how the display abstraction should 
    behave. Creating filsystem abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/display.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazDisplayAPI_t topazDisplayAPI_t;




// function for backend to signal a resize event.
// Both user requested resizes AND external resizes should end up calling 
// this from the backend.
void topaz_display_signal_parameter_change(topazDisplay_t *, topazDisplay_Parameter, int value);

// function for backend to signal a close event.
// Both user requested resizes AND external close events should end up calling 
// this from the backend.
void topaz_display_signal_close(topazDisplay_t *);

typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;

// Each function is an implementation-facing copy of 
// the user-side API for topazDisplay_t. See <topaz/backends/Display.h>
//
struct topazDisplayAPI_t {
    void *                  (*display_create)           (topazDisplay_t *, topaz_t *);
    void                    (*display_destroy)          (topazDisplay_t *, void *);



    void (*display_request_parameter_change)   (topazDisplay_t *, void *, topazDisplay_Parameter, int value);
    int (*display_is_parameter_modifiable)   (topazDisplay_t *, void *, topazDisplay_Parameter);

    void (*display_set_name)            (topazDisplay_t *, void *, const topazString_t *);
    void (*display_update)                  (topazDisplay_t *, void *, topazRenderer_Framebuffer_t *);    
    const topazArray_t * (*display_supported_framebuffers)  (topazDisplay_t *, void *);
    topazDisplay_Handle (*display_get_system_handle_type)   (topazDisplay_t *, void *);
    void * (*display_get_system_handle)                     (topazDisplay_t *, void *);
    topazDisplay_Event (*display_get_system_event_type)     (topazDisplay_t *, void *);
    void * (*display_get_last_system_event)                 (topazDisplay_t *, void *);
    topazArray_t * (*display_get_current_clipboard)         (topazDisplay_t *, void *);
    void (*display_set_current_clipboard)                   (topazDisplay_t *, void *, const topazArray_t *);




    // User-given data. This is expected to data needed to persist
    // throughout the lifedisplay of the Display
    //
    void * implementationData;


};



#endif
