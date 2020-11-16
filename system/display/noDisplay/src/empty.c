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




#include "backend.h"
#include <topaz/version.h>
#include <topaz/system.h>






static intptr_t api_nothing(){return 0;}

void topaz_system_display_noDisplay__backend(
    topazSystem_t *         system, 
    topazSystem_Backend_t * backend, 
    topazDisplayAPI_t *     api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("NoDisplay"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Placeholder for a display. Does nothing!"),




        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );

    api->display_create = (void * (*)(topazDisplay_t *, topaz_t *)) api_nothing;
    api->display_fullscreen = (void (*)(topazDisplay_t *, void *, int)) api_nothing;
    api->display_destroy = (void (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_resize = (void (*)(topazDisplay_t *, void *, int, int)) api_nothing;
    api->display_set_position = (void (*)(topazDisplay_t *, void *, int, int)) api_nothing;
    api->display_hide = (void (*)(topazDisplay_t *, void *, int)) api_nothing;
    api->display_has_input_focus = (int (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_lock_client_resize = (void (*)(topazDisplay_t *, void *, int)) api_nothing;
    api->display_lock_client_position = (void (*)(topazDisplay_t *, void *, int)) api_nothing;
    api->display_get_height = (int (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_width = (int (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_x = (int (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_y = (int (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_set_name = (void (*)(topazDisplay_t *, void *, const topazString_t *)) api_nothing;
    api->display_add_resize_callback = (void (*)(topazDisplay_t *, void *, void(*)(int w, int h, void *), void *)) api_nothing;
    api->display_remove_resize_callback = (void (*)(topazDisplay_t *, void *, void(*)(int w, int h, void *))) api_nothing;
    api->display_add_close_callback = (void (*)(topazDisplay_t *, void *, void(*)(void *), void *)) api_nothing;
    api->display_remove_close_callback = (void (*)(topazDisplay_t *, void *, void(*)(void *))) api_nothing;
    api->display_is_capable = (int (*)(topazDisplay_t *, void *, topazDisplay_Capability)) api_nothing;
    api->display_update = (void (*)(topazDisplay_t *, void *, topazRenderer_Framebuffer_t *)) api_nothing;
    api->display_supported_framebuffers = (const topazArray_t * (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_system_handle_type = (topazDisplay_Handle (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_system_handle = (void * (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_system_event_type = (topazDisplay_Event (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_last_system_event = (void * (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_get_current_clipboard = (topazArray_t * (*)(topazDisplay_t *, void *)) api_nothing;
    api->display_set_current_clipboard = (void (*)(topazDisplay_t *, void *, const topazArray_t *)) api_nothing;

}







