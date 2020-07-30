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







topazBackend_t * topaz_system_display_noDisplay__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("NoDisplay"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Placeholder for a display. Does nothing!"),




        // On init
        NULL,

        // On init late
        NULL,

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
}


static intptr_t api_nothing(){return 0;}
void topaz_system_display_noDisplay__api(topazDisplayAPI_t * api) {
    api->display_create = (void (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_destroy = (void (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_resize = (void (*)(topazDisplayAPI_t *, int, int)) api_nothing;
    api->display_set_position = (void (*)(topazDisplayAPI_t *, int, int)) api_nothing;
    api->display_hide = (void (*)(topazDisplayAPI_t *, int)) api_nothing;
    api->display_has_input_focus = (int (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_lock_client_resize = (void (*)(topazDisplayAPI_t *, int)) api_nothing;
    api->display_lock_client_position = (void (*)(topazDisplayAPI_t *, int)) api_nothing;
    api->display_set_view_policy = (void (*)(topazDisplayAPI_t *, topazDisplay_ViewPolicy)) api_nothing;
    api->display_get_height = (int (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_width = (int (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_x = (int (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_y = (int (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_set_name = (void (*)(topazDisplayAPI_t *, const topazString_t *)) api_nothing;
    api->display_add_resize_callback = (void (*)(topazDisplayAPI_t *, void(*)(int w, int h, void *), void *)) api_nothing;
    api->display_remove_resize_callback = (void (*)(topazDisplayAPI_t *, void(*)(int w, int h, void *))) api_nothing;
    api->display_add_close_callback = (void (*)(topazDisplayAPI_t *, void(*)(void *), void *)) api_nothing;
    api->display_remove_close_callback = (void (*)(topazDisplayAPI_t *, void(*)(void *))) api_nothing;
    api->display_is_capable = (int (*)(topazDisplayAPI_t *, topazDisplay_Capability)) api_nothing;
    api->display_update = (void (*)(topazDisplayAPI_t *, topazRenderer_Framebuffer_t *)) api_nothing;
    api->display_supported_framebuffers = (const topazArray_t * (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_system_handle_type = (topazDisplay_Handle (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_system_handle = (void * (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_system_event_type = (topazDisplay_Event (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_last_system_event = (void * (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_get_current_clipboard = (topazArray_t * (*)(topazDisplayAPI_t *)) api_nothing;
    api->display_set_current_clipboard = (void (*)(topazDisplayAPI_t *, const topazArray_t *)) api_nothing;

}





