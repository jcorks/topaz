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


#include "gravity/gravity_compiler.h"
#include "gravity/gravity_macros.h"
#include "gravity/gravity_core.h"
#include "gravity/gravity_vm.h"

#include <topaz/system.h>
#include <topaz/version.h>
#include <topaz/containers/array.h>
#include <topaz/modules/console.h>
#include <topaz/topaz.h>
#include <topaz/containers/table.h>
#include <topaz/backends/script.h>
#include "backend.h"

#include <stdio.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif


typedef struct {
    topazScript_t * script;
    topaz_t * topaz;
    gravity_compiler_t * compiler;
    gravity_delegate_t   delegate;
    gravity_vm       * vm;
    gravity_class_t    * topazClass;
    gravity_class_t    * topazMeta;

    topazTable_t * files;
    uint32_t fileIDPool;
} TopazGravity;

static void PLOG(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Warning
    );
    topaz_string_destroy(str);
}


static void report_error(
    gravity_vm *  vm,
    error_type_t  error,
    const char *  desc,
    error_desc_t  error_desc,
    void *        data
) {
    TopazGravity * g  = data;
    PLOG(g->topaz, topaz_string_create_from_c_str("Gravity VM error: %s\n", desc));
}

void * topaz_gravity_create(topazScript_t * script, topaz_t * topaz) {
    TopazGravity * g = calloc(1, sizeof(TopazGravity));
    g->script = script;
    g->topaz  = topaz;
    g->delegate.error_callback = report_error;
    g->delegate.xdata = g;
    g->compiler = gravity_compiler_create(&g->delegate);
    g->vm = gravity_vm_new(&g->delegate);

    g->topazClass = gravity_class_new_pair(NULL, "topaz", NULL, 0, 0);
    g->topazMeta = gravity_class_get_meta(g->topazClass);
    g->files = topaz_table_create_hash_topaz_string();
    g->fileIDPool = 1;
    return g;
}



void topaz_gravity_destroy(topazScript_t * s, void * data) {
    TopazGravity * g = data;
    gravity_compiler_free(g->compiler);
    gravity_vm_free(g->vm);
    gravity_core_free();
}



typedef struct {
    topaz_script_native_function func;
    void * userData;
} TopazGravityFunctionData;



static topazScript_Object_t * topaz_gravity_value_to_script_object(
    topazScript_t * script,
    gravity_value_t val
) {
    if (val.isa == gravity_class_null) {
        return topaz_script_object_undefined(script);
    } else if (val.isa == gravity_class_int ||
               val.isa == gravity_class_bool) {
        return topaz_script_object_from_int(script, VALUE_AS_INT(val));
    } else if (val.isa == gravity_class_float) {
        return topaz_script_object_from_number(script, VALUE_AS_FLOAT(val));
    } else if (val.isa == gravity_class_string) {
        return topaz_script_object_from_string(script, topaz_string_create_from_c_str("%s", VALUE_AS_STRING(val)));
    } else {
        assert("!under construction");
    }      
    return topaz_script_object_undefined(script);
}



static gravity_value_t topaz_script_object_to_gravity_value(
    gravity_vm * vm,
    topazScript_Object_t * obj
) {
    switch(topaz_script_object_get_type(obj)) {
      case topazScript_Object_Type_Undefined:
        return VALUE_FROM_NULL;

      case topazScript_Object_Type_Integer:
        return VALUE_FROM_INT(topaz_script_object_as_int(obj));

      case topazScript_Object_Type_Number:
        return VALUE_FROM_FLOAT(topaz_script_object_as_number(obj));

      case topazScript_Object_Type_String:
        return VALUE_FROM_CSTRING(vm, topaz_string_get_c_str(topaz_script_object_as_string(obj)));

      default:
        assert("!under construction");
    }

    return VALUE_FROM_NULL;
}







static bool topaz_gravity_native_function(
    gravity_vm * vm,
    gravity_value_t * args,
    uint16_t nargs,
    uint32_t rindex,
    void * userData
) {
    TopazGravityFunctionData * fdata = userData;
    TopazGravity * g  = fdata->userData;
    topazArray_t * argsT = topaz_array_create(sizeof(topazScript_Object_t *));
    uint32_t i;
    uint32_t len = nargs;
    for(i = 1; i < len; ++i) {
        topazScript_Object_t * o = topaz_gravity_value_to_script_object(g->script, args[i]);
        topaz_array_push(argsT, o);
    }
    topazScript_Object_t * retval = fdata->func(
        g->script,
        argsT,
        fdata->userData
    );
    len = topaz_array_get_size(argsT);
    for(i = 0; i < len; ++i) {
        topaz_script_object_destroy(topaz_array_at(argsT, topazScript_Object_t *, i));
    }
    topaz_array_destroy(argsT);
    gravity_vm_setslot(
        vm,
        topaz_script_object_to_gravity_value(g->vm, retval),
        rindex
    );
    topaz_script_object_destroy(retval);
    return TRUE;
}

int topaz_gravity_map_native_function(
    topazScript_t * s, 
    void * data, 
    const topazString_t * name, 
    topaz_script_native_function func,
    void * userData    
) {


    TopazGravityFunctionData * fdata = calloc(1, sizeof(TopazGravityFunctionData));
    fdata->func = func;
    fdata->userData = userData;

    gravity_function_t *  gfunc = gravity_function_new_internal(
        NULL, 
        NULL, 
        topaz_gravity_native_function,
        0
    );
    gfunc->internalData = fdata;



    return TRUE;
}

void topaz_gravity_run(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * sourceName, 
    const topazString_t * scriptData
) {
    TopazGravity * g = data;
    uint32_t fileid = (uint32_t)topaz_table_find(g->files, sourceName);
    if (!fileid) {
        fileid = g->fileIDPool++;
        topaz_table_insert(g->files, sourceName, (void*)fileid);
    }
    gravity_closure_t * cl = gravity_compiler_run(
        g->compiler,
        topaz_string_get_c_str(scriptData),
        topaz_string_get_length(scriptData),
        fileid,
        1,
        1
    );

    if (!cl) {
        return;
    }

    gravity_vm_runmain(g->vm, cl);
}


topazScript_Object_t * topaz_gravity_expression(
    topazScript_t * script, 
    void * data,     
    const topazString_t * scriptData
) {
    TopazGravity * g = data;
    gravity_closure_t * cl = gravity_compiler_run(
        g->compiler,
        topaz_string_get_c_str(scriptData),
        topaz_string_get_length(scriptData),
        0,
        1,
        1
    );

    if (!cl) {
        return topaz_script_object_undefined(g->script);
    }

    gravity_vm_runclosure(
        g->vm, 
        cl,
        VALUE_FROM_OBJECT(cl),
        NULL,
        0
    );

    gravity_value_t v = gravity_vm_result(g->vm);
    topazScript_Object_t * out = topaz_gravity_value_to_script_object(g->script, v);
    gravity_value_dump(g->vm, v, NULL, 0);
    return out;
}

void topaz_gravity_bootstrap(
    topazScript_t * script, 
    void * data
) {
    TopazGravity * g = data;
    
}




void topaz_system_script_gravity__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazScriptAPI_t      *  api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Gravity"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("https://marcobambini.github.io/gravity/"),

        // desc 
        TOPAZ_STR_CAST("Bindings to use the gravity scripting language."),



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

    /*
    api->objectAPI.object_reference_create = topaz_gravity_object_reference_create;
    api->objectAPI.object_reference_create_from_reference = topaz_gravity_object_reference_create_from_reference;
    api->objectAPI.object_reference_destroy = topaz_gravity_object_reference_destroy;
    api->objectAPI.object_reference_get_feature_mask = topaz_gravity_object_reference_get_feature_mask;
    api->objectAPI.object_reference_get_native_data = topaz_gravity_object_reference_get_native_data;
    api->objectAPI.object_reference_set_native_data = topaz_gravity_object_reference_set_native_data;
    api->objectAPI.object_reference_ref = topaz_gravity_object_reference_ref;
    api->objectAPI.object_reference_unref = topaz_gravity_object_reference_unref;
    api->objectAPI.object_reference_call = topaz_gravity_object_reference_call;
    api->objectAPI.object_reference_array_get_nth = topaz_gravity_object_reference_array_get_nth;
    api->objectAPI.object_reference_array_get_count = topaz_gravity_object_reference_array_get_count;
    api->objectAPI.object_reference_map_get_property = topaz_gravity_object_reference_map_get_property;
    api->objectAPI.object_reference_to_string = topaz_gravity_object_reference_to_string;
    api->objectAPI.object_reference_extendable_add_property = topaz_gravity_object_reference_extendable_add_property;
    */

    api->script_create = topaz_gravity_create;
    api->script_destroy = topaz_gravity_destroy;
    api->script_map_native_function = topaz_gravity_map_native_function;
    api->script_run = topaz_gravity_run;
    api->script_expression = topaz_gravity_expression;
    //api->script_create_empty_object = topaz_gravity_create_empty_object;
    //api->script_throw_error = topaz_gravity_throw_error;
    api->script_bootstrap = topaz_gravity_bootstrap;


    //api->script_debug_start = topaz_gravity_debug_start;
    //api->script_debug_send_command = topaz_gravity_debug_send_command;
    //api->script_debug_get_state = topaz_gravity_debug_get_state;
}





