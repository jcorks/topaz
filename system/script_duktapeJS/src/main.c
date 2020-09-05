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
#include <topaz/backends/backend.h>
#include <topaz/version.h>
#include <stdio.h>
#include "duktape.h"






topazBackend_t * topaz_system_script_duktapeJS__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("DuktapeJS"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("https://duktape.org/"),

        // desc 
        TOPAZ_STR_CAST("Bindings to use duktape!"),




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

typedef struct {
    duk_context * js;
    topazScript_t * script;
} TOPAZDTJS;

// creates a new topaz script object from a value on the duk stack
static topazScript_Object_t * topaz_dtjs_stack_object_to_tso(TOPAZDTJS * ctx, int index) {
    switch(duk_get_type(ctx->js, index)) {
      case DUK_TYPE_NONE:
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: duktapeJS returned DUK_TYPE_NONE, this is likely indicative of programmer error!");
        #endif
        break;
      case DUK_TYPE_BOOLEAN:
        return topaz_script_object_from_int(ctx->script, duk_get_boolean(ctx->js, index));

      case DUK_TYPE_NUMBER:    
        return topaz_script_object_from_number(ctx->script, duk_get_number(ctx->js, index));

      case DUK_TYPE_STRING:
        return topaz_script_object_from_string(ctx->script, TOPAZ_STR_CAST(duk_get_string(ctx->js, index)));

      case DUK_TYPE_BUFFER:
        return topaz_script_object_from_string(ctx->script, TOPAZ_STR_CAST(duk_get_string(ctx->js, index)));

      default:;
    }
    return topaz_script_object_undefined(ctx->script);

}


static void * topaz_dtjs_create(topazScript_t * scr) {
    TOPAZDTJS * out = calloc(1, sizeof(TOPAZDTJS));
    out->js = duk_create_heap_default();
    out->script = scr;
    return out;
}

static topazScript_Object_t * topaz_dtjs_expression(
    topazScript_t * script, 
    void * data,
    const topazString_t * expr
) {
    TOPAZDTJS * out = data;
    duk_eval_string(out->js, topaz_string_get_c_str(expr));
    return topaz_dtjs_stack_object_to_tso(
        out,
        -1
    );
}




static intptr_t api_nothing(){return 0;}
void topaz_system_script_duktapeJS__api(topazScriptAPI_t * api) {
    api->script_create = topaz_dtjs_create;
    api->script_destroy = (void (*)(topazScript_t *, void *)) api_nothing;
    api->script_map_native_function = (int (*)(topazScript_t *, void *, const topazString_t *, topaz_script_native_function, void*)) api_nothing;
    api->script_run = (void (*)(topazScript_t *, void *, const topazString_t * sourceName, const topazString_t * scriptData)) api_nothing;
    api->script_expression = topaz_dtjs_expression;
    api->script_bootstrap = (void (*)(topazScript_t *, void *)) api_nothing;

}





