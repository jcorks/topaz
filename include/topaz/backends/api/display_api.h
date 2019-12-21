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
#define H_TOPAZDC__TIME_API__INCLUDED

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



typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;

/// Each function is an implementation-facing copy of 
/// the user-side API for topazDisplay_t. See <topaz/backends/Display.h>
///
struct topazDisplayAPI_t {
    void                    (*display_create)           (topazDisplayAPI_t *);
    void                    (*display_destroy)          (topazDisplayAPI_t *);





    void    (*display_resize)(topazDisplayAPI_t *, int w, int h);


    void (*display_set_position)        (topazDisplayAPI_t *, int x, int y);
    void (*display_fullscreen)          (topazDisplayAPI_t *, int);
    void (*display_hide)                (topazDisplayAPI_t *, int);    
    int  (*display_has_input_focus)     (topazDisplayAPI_t *); 
    void (*display_lock_client_resize)  (topazDisplayAPI_t *, int);
    void (*display_lock_client_position)(topazDisplayAPI_t *, int);
    void (*display_set_view_policy)     (topazDisplayAPI_t *, topazDisplay_ViewPolicy);
    int  (*display_get_width)           (topazDisplayAPI_t *);
    int  (*display_get_height)          (topazDisplayAPI_t *);
    int  (*display_get_x)               (topazDisplayAPI_t *);
    int  (*display_get_y)               (topazDisplayAPI_t *);
    void (*display_set_name)            (topazDisplayAPI_t *, const topazString_t *);
    void (*display_add_resize_callback)     (topazDisplayAPI_t *, void(*)(int w, int h, void *), void *);
    void (*display_remove_resize_callback)  (topazDisplayAPI_t *, void(*)(int w, int h, void *));
    void (*display_add_close_callback)      (topazDisplayAPI_t *, void(*)(void *), void *);
    void (*display_remove_close_callback)   (topazDisplayAPI_t *, void(*)(void *));
    int  (*display_is_capable)              (topazDisplayAPI_t *, topazDisplay_Capability); 
    void (*display_update)                  (topazDisplayAPI_t *, topazRenderer_Framebuffer_t *);    
    const topazArray_t * (*display_supported_framebuffers)  (topazDisplayAPI_t *);
    topazDisplay_Handle (*display_get_system_handle_type)   (topazDisplayAPI_t *);
    void * (*display_get_system_handle)                     (topazDisplayAPI_t *);
    topazDisplay_Event (*display_get_system_event_type)     (topazDisplayAPI_t *);
    void * (*display_get_last_system_event)                 (topazDisplayAPI_t *);
    topazArray_t * (*display_get_current_clipboard)         (topazDisplayAPI_t *);
    void (*display_set_current_clipboard)                   (topazDisplayAPI_t *, const topazArray_t *);




    /// User-given data. This is expected to data needed to persist
    /// throughout the lifedisplay of the Display
    ///
    void * implementationData;


};



#endif
