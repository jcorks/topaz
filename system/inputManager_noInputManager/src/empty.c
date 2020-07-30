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







topazBackend_t * topaz_system_inputManager_noInputManager__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("NoInputManager"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Placeholder for an input manager. Does nothing!"),




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
void topaz_system_inputManager_noInputManager__api(topazDisplayAPI_t * api);
    api->input_manager_create = (void (*)(topazInputManagerAPI_t *)) api_nothing;
    api->input_manager_destroy = (void (*)(topazInputManagerAPI_t *)) api_nothing;
    api->input_manager_id_to_string = (const char * (*)(int)) api_nothing;
    api->input_manager_handle_events = (int (*)(topazInputManagerAPI_t *)) api_nothing;
    api->input_manager_query_device = (topazInputDevice_t * (*)(topazInputManagerAPI_t *, int)) api_nothing;
    api->input_manager_query_auxiliary_devices = (int (*)(topazInputManagerAPI_t *, int *)) api_nothing;
    api->input_manager_max_devices = (int (*)(topazInputManagerAPI_t *)) api_nothing;
    api->input_manager_set_focus = (void (*)(topazInputManagerAPI_t *, topazDisplay_t *)) api_nothing;
    api->input_manager_get_focus = (topazDisplay_t * (*)(topazInputManagerAPI_t *)) api_nothing;
    api->input_manager_show_virtual_keyboard = (void (*)(topazInputManagerAPI_t *, int)) api_nothing;

}





