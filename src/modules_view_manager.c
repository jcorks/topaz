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

#include <topaz/compat.h>
#include <topaz/modules/view_manager.h>
#include <topaz/modules/graphics.h>
#include <topaz/backends/display.h>
#include <topaz/backends/renderer.h>
#include <topaz/containers/array.h>
#include <topaz/system.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

struct topazViewManager_t {
    topazDisplay_t * defaultDisplay;
    topaz_t * t;
    topazString_t * str;
    topazArray_t * views;
};



topazViewManager_t * topaz_view_manager_create(topaz_t * t) {
    topazViewManager_t * out = calloc(1, sizeof(topazViewManager_t));
    out->t = t;
    out->views = topaz_array_create(sizeof(topazDisplay_t*));
    return out;
}

void topaz_view_manager_destroy(topazViewManager_t * t) {
    if (t->str) {
        topaz_string_destroy(t->str);
    }
    topaz_array_destroy(t->views);
    free(t);
}



topazDisplay_t * topaz_view_manager_create_display(topazViewManager_t * v, const topazString_t * name) {
    int w = 640;
    int h = 480;
    topazDisplayAPI_t api = {};
    topazSystem_Backend_t * backend = topaz_system_create_backend(
        topaz_context_get_system(v->t),
        TOPAZ_STR_CAST("display"),
        &api
    );
    topazDisplay_t * d = topaz_display_create(
        v->t,
        backend,
        api
    );
    topaz_display_set_name(d, name);
    topaz_display_set_parameter(d, topazDisplay_Parameter_Width,  w);
    topaz_display_set_parameter(d, topazDisplay_Parameter_Height, h);    
    topaz_array_push(v->views, d);
    if (!v->defaultDisplay) v->defaultDisplay = d;
    return d;
}




void topaz_view_manager_destroy_display(topazViewManager_t * v, topazDisplay_t * d) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(v->views);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(v->views, topazDisplay_t *, i) == d) {
            topaz_display_destroy(d);
            topaz_array_remove(v->views, i);
            if (v->defaultDisplay == d) {
                v->defaultDisplay = NULL;
            }
            return;
        }
    }

}



topazDisplay_t * topaz_view_manager_get_default(topazViewManager_t * v) {
    return v->defaultDisplay;
}



const topazString_t * topaz_view_manager_get_clipboard_as_string(topazViewManager_t * v) {
    if (!v->str) {
        v->str = topaz_string_create();
    }

    topazArray_t * arr = topaz_display_get_current_clipboard(v->defaultDisplay);
    uint32_t len = topaz_array_get_size(arr);
    char * cstring =  calloc(sizeof(char), len+1);
    memcpy(cstring, topaz_array_get_data(arr), len);
    
    topaz_string_set(v->str, TOPAZ_STR_CAST(cstring));
    free(cstring);
    return v->str;
}


void topaz_view_manager_set_clipboard_from_string(topazViewManager_t * v, const topazString_t * str) {
    if (!v->defaultDisplay) return;
    topazArray_t * arr = topaz_array_create(sizeof(uint8_t));
    topaz_array_set_size(arr, topaz_string_get_length(str)+1);
    memcpy(
        topaz_array_get_data(arr), 
        topaz_string_get_c_str(str), 
        topaz_string_get_length(str)+1
    );
    topaz_display_set_current_clipboard(v->defaultDisplay, arr);
    topaz_array_destroy(arr);
}




const topazArray_t * topaz_view_manager_get_all(const topazViewManager_t * v) {
    return v->views;
}

topazArray_t * topaz_view_manager_get_all_active(const topazViewManager_t * v) {
    topazArray_t * out = topaz_array_create(sizeof(topazDisplay_t *));
    uint32_t i;
    uint32_t len = topaz_array_get_size(v->views);
    for(i = 0; i < len; ++i) {
        topazDisplay_t * d = topaz_array_at(v->views, topazDisplay_t *, i);
        if (topaz_display_get_parameter(d, topazDisplay_Parameter_Active) > 0.5) {
            topaz_array_push(out, d);
        }
    }
    return out;
}

