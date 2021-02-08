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
#include <topaz/modules/resources.h>
#include <topaz/assets/data.h>
#include "backend.h"

#include <stdio.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif


typedef struct {
    topazScript_t * script;
    topaz_t * topaz;
    gravity_delegate_t   delegate;
    gravity_vm       * vm;
    gravity_class_t    * topazClass;
    gravity_class_t    * topazMeta;

    // holds references in the VM so that instances 
    // do not get sweeped
    gravity_map_t * topazReftable_inst;
    gravity_value_t topazReftable;

    // maps gravity_instance_t -> TopazGravityObject
    topazTable_t * ownedRefs;

    topazTable_t * files;
    uint32_t fileIDPool;
    topazString_t * bootstrap;
    int paused;
    topazScript_DebugState_t debugState;
} TopazGravity;




typedef struct {
    // reference to the instance in question.
    // 
    gravity_value_t value;

    // referring object;
    topazScript_Object_t * object;

    // any set native data.
    void * nativeData;
    int nativeTag;

    
    // the object's reference count
    int refCount;
    
    // source script reference
    TopazGravity * ctx;

    // whether this object originated from a 
    // topaz_script_create_empty_object call 
    // Cleanup function for that.
    topaz_script_native_function cleanupFunc;
    
    void * cleanupData;

} TopazGravityObject;

// holds a reference to the object in question 
static void topaz_gravity_lock_ref(TopazGravityObject * o) {
    gravity_closure_t * storeat = gravity_class_lookup_closure(gravity_class_map, VALUE_FROM_CSTRING(o->ctx->vm, "storeat"));
    char ploc[64];
    sprintf(ploc, "%p", o);
    gravity_value_t args[] = {
        // key
        VALUE_FROM_CSTRING(o->ctx->vm, ploc),

        // value
        o->value
    };
    gravity_vm_runclosure(
        o->ctx->vm, 
        storeat, 
        o->ctx->topazReftable,
        args,
        2
    );
}

// releases the object from the VM
static void topaz_gravity_unlock_ref(TopazGravityObject * o) {
    gravity_closure_t * remove = gravity_class_lookup_closure(gravity_class_map, VALUE_FROM_CSTRING(o->ctx->vm, "remove"));
    char ploc[64];
    sprintf(ploc, "%p", o);
    gravity_value_t args[] = {
        // key
        VALUE_FROM_CSTRING(o->ctx->vm, ploc)
    };
    gravity_vm_runclosure(
        o->ctx->vm, 
        remove, 
        o->ctx->topazReftable,
        args,
        1
    );
}



static uint32_t topaz_gravity_object_gc(gravity_vm * vm, gravity_object_t * obj) {
    TopazGravity * ctx = gravity_vm_getdata(vm);
    TopazGravityObject * g = topaz_table_find(ctx->ownedRefs, obj);        
    if (g->cleanupFunc)
        g->cleanupFunc(ctx->script, NULL, g->cleanupData);

    topaz_table_remove(ctx->ownedRefs, obj);
    free(g);   
    return 0;
}

// Asserts that the given value has a TopazGravityObject 
// associated with it. If there is none, will one be assigned. If 
// assigning one is nor possible, NULL is returned
static TopazGravityObject * topaz_gravity_object_get_tag(TopazGravity * g, gravity_value_t v) {
    // not instanted, so object cannot be tracked
    if (!OBJECT_IS_VALID(VALUE_AS_OBJECT(v))) return NULL;

    TopazGravityObject * o = topaz_table_find(g->ownedRefs, VALUE_AS_OBJECT(v));
    if (!o) {
        o = calloc(1, sizeof(TopazGravityObject));
        o->value = v;
        o->ctx = g;
        topaz_table_insert(g->ownedRefs, VALUE_AS_OBJECT(v), o);        
        VALUE_AS_INSTANCE(v)->gc.free = topaz_gravity_object_gc;
        // notice that the refcount is 0 and that the value has not 
        // be locked to prevent GC.
    }
    return o;
}


static void clear_debug_state(topazScript_DebugState_t * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->callstack);
    for(i = 0; i < len; ++i) {
        topazScript_CallstackFrame_t * frame = &topaz_array_at(s->callstack, topazScript_CallstackFrame_t, i);
        topaz_string_destroy((topazString_t*)frame->filename);
        topaz_string_destroy((topazString_t*)frame->functionName);
        topaz_string_destroy((topazString_t*)frame->entityName);
    }
    topaz_array_set_size(s->callstack, 0);
}

static void debug_state_add_frame(topazScript_DebugState_t * s,
    int lineNumber,
    const char * filename,
    const char * functionName
) {
    topazScript_CallstackFrame_t frame;
    frame.lineNumber   = lineNumber;
    frame.filename     = topaz_string_create_from_c_str("%s", filename);
    frame.functionName = topaz_string_create_from_c_str("%s", functionName);
    frame.entityName   = topaz_string_create();

    topaz_array_push(s->callstack, frame);
}


static void PLOG(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Error
    );
    topaz_string_destroy(str);
}

static const char * get_file_name_from_id(TopazGravity * g, int id) {
    return topaz_table_find_by_uint(
        g->files, 
        id
    ) ? 
        topaz_string_get_c_str(
            topaz_table_find_by_uint(
                g->files, 
                id
            )
        ) 
    : 
        "???";
}

static void report_error(
    gravity_vm *  vm,
    error_type_t  error,
    const char *  desc,
    error_desc_t  error_desc,
    void *        data
) {
    TopazGravity * g  = data;
    topazString_t * out = topaz_string_create_from_c_str(
        "Scripting error(%s, line %d, col %d):\n%s\n", 
        get_file_name_from_id(g, error_desc.fileid),
        error_desc.lineno,
        error_desc.colno,
        desc
    );
    PLOG(g->topaz, out);
}


static bool topaz_gravity_debug_pause(
    gravity_vm * vm,
    gravity_value_t * args,
    uint16_t nargs,
    uint32_t rindex,
    void * userData
) {
    TopazGravity * g = userData;
    topaz_script_debug_send_command(g->script, topazScript_DebugCommand_Pause, TOPAZ_STR_CAST(""));
    return TRUE;
}

static const char * topaz_gravity_loadfile_callback(
    const char * file, 
    size_t * len,
    uint32_t * fileid,
    void *xdata, 
    bool *is_static
) {
    TopazGravity * g = xdata;

    topazAsset_t * f = topaz_resources_load_asset(
        topaz_context_get_resources(g->topaz),
        TOPAZ_STR_CAST("txt"),
        TOPAZ_STR_CAST(file),
        TOPAZ_STR_CAST(file)
    );

    if (!f) {
        *len = 0;
        *is_static = TRUE;
        topazString_t * out = topaz_string_create_from_c_str(
            "Cant include unknown file %s", file
        );
        PLOG(g->topaz, out);

        return "";
    }

    const topazString_t * src = topaz_data_get_as_string(f);
    topaz_script_register_source(
        g->script,
        TOPAZ_STR_CAST(file),
        src
    );

    char * out = strdup(topaz_string_get_c_str(src));
    *fileid = g->fileIDPool++;
    *len = strlen(out);
    *is_static = 0;

    

    topaz_table_insert_by_uint(g->files, *fileid, topaz_string_create_from_c_str("%s", file));
    return out;
}

static void * topaz_gravity_create(topazScript_t * script, topaz_t * topaz) {
    TopazGravity * g = calloc(1, sizeof(TopazGravity));
    g->script = script;
    g->topaz  = topaz;
    g->delegate.error_callback = report_error;
    g->delegate.loadfile_callback = topaz_gravity_loadfile_callback;
    g->delegate.xdata = g;
    g->vm = gravity_vm_new(&g->delegate);
    gravity_vm_setdata(g->vm, g);

    g->topazClass = gravity_class_new_pair(NULL, "topaz_", NULL, 0, 0);
    g->topazMeta = gravity_class_get_meta(g->topazClass);
    g->topazReftable_inst = gravity_map_new(g->vm, 0);
    g->topazReftable = gravity_value_from_object(g->topazReftable_inst);
    gravity_class_bind(g->topazMeta, "t_", g->topazReftable);

    g->ownedRefs = topaz_table_create_hash_pointer();        
    g->files = topaz_table_create_hash_pointer();
    g->bootstrap = topaz_string_create();
    g->debugState.callstack = topaz_array_create(sizeof(topazScript_CallstackFrame_t));


    gravity_function_t * pausefunc = gravity_function_new_internal(
        NULL, 
        NULL, 
        topaz_gravity_debug_pause,
        0
    );
    pausefunc->internalData = g;

    gravity_closure_t * pausecl = gravity_closure_new(NULL, pausefunc);

    gravity_class_bind(
        g->topazMeta, 
        "pause", 
        VALUE_FROM_OBJECT(pausecl)
    );


    gravity_vm_setvalue(g->vm, "topaz_", VALUE_FROM_OBJECT(g->topazClass));

    g->fileIDPool = 1;
    return g;
}



static void topaz_gravity_destroy(topazScript_t * s, void * data) {
    TopazGravity * g = data;
    gravity_vm_free(g->vm);
    gravity_core_free();
}



typedef struct {
    topaz_script_native_function func;
    void * userData;
    TopazGravity * g;
} TopazGravityFunctionData;



static topazScript_Object_t * topaz_gravity_value_to_script_object(
    TopazGravity * g,
    gravity_value_t val
) {
    if (val.isa == gravity_class_null) {
        return topaz_script_object_undefined(g->script);
    } else if (val.isa == gravity_class_int ||
               val.isa == gravity_class_bool) {
        return topaz_script_object_from_int(g->script, VALUE_AS_INT(val));
    } else if (val.isa == gravity_class_float) {
        return topaz_script_object_from_number(g->script, VALUE_AS_FLOAT(val));
    } else if (val.isa == gravity_class_string) {
        return topaz_script_object_from_string(g->script, topaz_string_create_from_c_str("%s", VALUE_AS_CSTRING(val)));
    } else {
        return topaz_script_object_wrapper(g->script, topaz_gravity_object_get_tag(g, val));
    }      
    return topaz_script_object_undefined(g->script);
}



static gravity_value_t topaz_script_object_to_gravity_value(
    gravity_vm * vm,
    topazScript_Object_t * obj
) {
    if (!obj) return VALUE_FROM_NULL;
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
        return ((TopazGravityObject*)topaz_script_object_get_api_data(obj))->value;
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
    TopazGravity * g  = fdata->g;
    topazArray_t * argsT = topaz_array_create(sizeof(topazScript_Object_t *));
    uint32_t i;
    uint32_t len = nargs;
    for(i = 1; i < len; ++i) {
        topazScript_Object_t * o = topaz_gravity_value_to_script_object(g, args[i]);
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
    if (retval)
        topaz_script_object_destroy(retval);
    return TRUE;
}



static int topaz_gravity_map_native_function(
    topazScript_t * s, 
    void * data, 
    const topazString_t * name, 
    topaz_script_native_function func,
    void * userData    
) {

    TopazGravity * g = data;
    TopazGravityFunctionData * fdata = calloc(1, sizeof(TopazGravityFunctionData));
    fdata->func = func;
    fdata->userData = userData;
    fdata->g = g;
    gravity_function_t *  gfunc = gravity_function_new_internal(
        NULL, 
        NULL, 
        topaz_gravity_native_function,
        0
    );
    
    gfunc->internalData = fdata;

    gravity_closure_t * cl = gravity_closure_new(NULL, gfunc);

    gravity_class_bind(
        g->topazMeta, 
        topaz_string_get_c_str(name), 
        VALUE_FROM_OBJECT(cl)
    );

    return TRUE;
}

static void topaz_gravity_run(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * sourceName, 
    const topazString_t * scriptData
) {
    TopazGravity * g = data;
    uint32_t fileid = g->fileIDPool++;
    topaz_table_insert_by_uint(g->files, fileid, topaz_string_clone(sourceName));

    gravity_compiler_t * c = gravity_compiler_create(&g->delegate);

    topazString_t * srcFull = topaz_string_clone(g->bootstrap);
    topaz_string_concat(srcFull, scriptData);
    gravity_closure_t * cl = gravity_compiler_run(
        c,
        topaz_string_get_c_str(srcFull),
        topaz_string_get_length(srcFull),
        fileid,
        1,
        1
    );
    gravity_compiler_transfer(c, g->vm);
    gravity_compiler_free(c);
    topaz_string_destroy(srcFull);

    if (!cl) {
        return;
    }

    gravity_vm_runmain(g->vm, cl);
}


static topazScript_Object_t * topaz_gravity_expression(
    topazScript_t * script, 
    void * data,     
    const topazString_t * scriptData
) {
    TopazGravity * g = data;
    
    topazString_t * commandPlus = topaz_string_create_from_c_str(
        "extern var topaz_; var debug = topaz_.t_['debug']; debug.expression({return("
    );
    topaz_string_concat(commandPlus, scriptData);
    topaz_string_concat(commandPlus, TOPAZ_STR_CAST(")});"));
    
    
    gravity_compiler_t * c = gravity_compiler_create(&g->delegate);
    gravity_closure_t * cl = gravity_compiler_run(
        c,
        topaz_string_get_c_str(commandPlus),
        topaz_string_get_length(commandPlus),
        0,
        1,
        1
    );
    gravity_compiler_transfer(c, g->vm);
    gravity_compiler_free(c);



    if (!cl) {
        return topaz_script_object_undefined(g->script);
    }

    gravity_vm_runclosure(
        g->vm, 
        cl,
        VALUE_FROM_NULL,
        NULL,
        0
    );

    topazScript_Object_t * out = topaz_script_object_from_string(script, TOPAZ_STR_CAST(""));
    return out;
}

static void topaz_gravity_bootstrap(
    topazScript_t * script, 
    void * data
) {
    TopazGravity * g = data;

    #include "bootstrap_bytes"
    //topaz_string_set(g->bootstrap, TOPAZ_STR_CAST((char*)bootstrap_bytes));    



    topazAsset_t * asset = topaz_resources_fetch_asset(
        topaz_context_get_resources(g->topaz),
        topazAsset_Type_Data,
        TOPAZ_STR_CAST("@topaz.core")
    );

    topaz_data_set(
        asset, 
        TOPAZ_ARRAY_CAST(
            (void*)bootstrap_bytes, 
            uint8_t, 
            strlen((char*)bootstrap_bytes)
        )
    );

}








static void * topaz_gravity_object_reference_create_from_reference(
    topazScript_Object_t * obj, 
    void * scriptData,
    topazScript_Object_t * from, 
    void * fromData 
) {
    TopazGravityObject * fromObj = fromData;
    return fromObj;
}

static void topaz_gravity_object_reference_destroy(topazScript_Object_t * obj, void * data) {
    // GC should take care of it when needed.
    TopazGravityObject * o = data;
    o->object = NULL;    
}

static int topaz_gravity_object_reference_get_feature_mask(topazScript_Object_t * obj, void * data) {
    TopazGravityObject * o = data;
    int out = 0;
    out |= (VALUE_ISA_CLOSURE(o->value)) ? topazScript_Object_Feature_Callable : 0;
    out |= (VALUE_ISA_LIST(o->value))    ? topazScript_Object_Feature_Array : 0;
    out |= (VALUE_ISA_MAP(o->value))    ? topazScript_Object_Feature_Map : 0;
    return out;
}

void * topaz_gravity_object_reference_get_native_data(
    topazScript_Object_t * o, 
    int * tag, 
    void * data
) {
    TopazGravityObject * obj = data;
    *tag = obj->nativeTag;
    return obj->nativeData;
}

void topaz_gravity_object_reference_set_native_data(
    topazScript_Object_t * nu, 
    void * native, 
    int nativeTag, 
    void * data
) {
    TopazGravityObject * obj = data;
    obj->nativeData = native;
    obj->nativeTag = nativeTag;
}

static void topaz_gravity_object_reference_ref(
    topazScript_Object_t * nu, 
    void * data
) {
    TopazGravityObject * obj = data;
    if (obj->refCount == 0) {
        topaz_gravity_lock_ref(obj);
    }
    obj->refCount++;

}


static void topaz_gravity_object_reference_unref(
    topazScript_Object_t * nu, 
    void * data
) {
    TopazGravityObject * obj = data;
    if (obj->refCount == 1) {
        topaz_gravity_unlock_ref(obj);
    }
    obj->refCount--;
    if (obj->refCount < 0) obj->refCount = 0;
}


static topazScript_Object_t * topaz_gravity_object_reference_call(
    topazScript_Object_t * obj, 
    const topazArray_t * args, 
    void * data
) {
    TopazGravityObject * o = data;
    TopazGravity * g = o->ctx;
    if (VALUE_ISA_CLOSURE(o->value)) {
        uint32_t i;
        uint32_t len = topaz_array_get_size(args);

        gravity_value_t * argsV = calloc(sizeof(gravity_value_t), len); 
        for(i = 0; i < len; ++i) {
            argsV[i] = topaz_script_object_to_gravity_value(
                g->vm, 
                topaz_array_at(args, topazScript_Object_t *, i)
            );
        }

        gravity_vm_runclosure(
            o->ctx->vm, 
            VALUE_AS_CLOSURE(o->value), 
            VALUE_AS_CLOSURE(o->value)->context ? VALUE_FROM_OBJECT(VALUE_AS_CLOSURE(o->value)->context) : VALUE_FROM_NULL,
            argsV,
            len
        );

        return topaz_gravity_value_to_script_object(o->ctx, gravity_vm_result(g->vm));

    } else {
        return topaz_script_object_undefined(g->script);
    }
    

}


static topazScript_Object_t * topaz_gravity_object_reference_array_get_nth(
    topazScript_Object_t * object, 
    int i,  
    void * data
) {
    TopazGravityObject * o = data;
    TopazGravity * g = o->ctx;
    if (VALUE_ISA_LIST(o->value)) {
        gravity_closure_t * loadat = gravity_class_lookup_closure(gravity_class_list, VALUE_FROM_CSTRING(o->ctx->vm, "loadat"));
        gravity_value_t args[] = {

            // key
            VALUE_FROM_INT(i)
        };
        gravity_vm_runclosure(
            g->vm, 
            loadat, 
            o->value,
            args,
            1
        );
        return topaz_gravity_value_to_script_object(o->ctx, gravity_vm_result(g->vm));
    } else {
        return topaz_script_object_undefined(g->script);
    }
}


static int topaz_gravity_object_reference_array_get_count(
    topazScript_Object_t * obj, 
    void * data
) {
    TopazGravityObject * o = data;
    TopazGravity * g = o->ctx;
    if (VALUE_ISA_LIST(o->value)) {
        gravity_closure_t * ct = gravity_class_lookup_closure(gravity_class_list, VALUE_FROM_CSTRING(o->ctx->vm, "count"));
        gravity_vm_runclosure(
            g->vm, 
            ct, 
            o->value,
            NULL,
            0
        );
        return VALUE_AS_INT(gravity_vm_result(g->vm));
    } else {
        return 0;
    }
}


static topazScript_Object_t * topaz_gravity_object_reference_map_get_property(
    topazScript_Object_t * obj, 
    const topazString_t * prop, 
    void * data
) {
    TopazGravityObject * o = data;
    TopazGravity * g = o->ctx;
    if (VALUE_ISA_MAP(o->value)) {
        gravity_closure_t * loadat = gravity_class_lookup_closure(gravity_class_map, VALUE_FROM_CSTRING(o->ctx->vm, "loadat"));
        gravity_value_t args[] = {
            // key
            VALUE_FROM_CSTRING(g->vm, topaz_string_get_c_str(prop)),
        };
        gravity_vm_runclosure(
            g->vm, 
            loadat, 
            o->value,
            args,
            1
        );
        return topaz_gravity_value_to_script_object(g, gravity_vm_result(g->vm));
    } else {
        return topaz_script_object_undefined(g->script);
    }
}

topazScript_Object_t * topaz_gravity_create_empty_object(
    topazScript_t * script, 
    void * data, 
    topaz_script_native_function func, 
    void * funcData
) {
    TopazGravity * g = data;
    gravity_map_t * mapOut = gravity_map_new(g->vm, 0);
    topazScript_Object_t * out = topaz_gravity_value_to_script_object(g, VALUE_FROM_OBJECT(mapOut));
    TopazGravityObject * o = topaz_script_object_get_api_data(out);

    o->cleanupFunc = func;
    o->cleanupData = funcData;
    return out;
}


static void topaz_gravity_debug_start(topazScript_t * scr, void * data) {
    // nothing needed, always.. ready...? for now.
}



void topaz_gravity_debug_send_command(
    topazScript_t * scr, 
    void * data, 
    int cmd, 
    const topazString_t * arg
) {
    TopazGravity * g = data;
    switch(cmd) {
      case topazScript_DebugCommand_Pause:
        if (g->paused) return;
        g->paused = 1;
        topaz_context_pause(g->topaz);
        topaz_script_notify_command(g->script, cmd, topaz_string_create());
        while(g->paused) {
            topaz_context_iterate(g->topaz);
        }
        break;    

      case topazScript_DebugCommand_Resume:
        topaz_context_resume(g->topaz);
        g->paused = 0;
        topaz_script_notify_command(g->script, cmd, topaz_string_create());
        break;    

      case topazScript_DebugCommand_StepInto:
        PLOG(g->topaz, topaz_string_create_from_c_str("This debugger does not currently support stepping."));
        break;

      case topazScript_DebugCommand_StepOver:
        PLOG(g->topaz, topaz_string_create_from_c_str("This debugger does not currently support stepping."));
        break;

      case topazScript_DebugCommand_AddBreakpoint: {
        PLOG(g->topaz, topaz_string_create_from_c_str("This debugger does not currently support in-source breakpoints. Use debug.stop() to halt mid-execution."));
        break;
      }

      case topazScript_DebugCommand_RemoveBreakpoint: {
        PLOG(g->topaz, topaz_string_create_from_c_str("This debugger does not currently support in-source breakpoints. Use debug.stop() to halt mid-execution."));
        break;
      }

      case topazScript_DebugCommand_ScopedEval: {
        topazString_t * str = topaz_string_clone(arg);
        const topazString_t * number = topaz_string_chain_start(str, TOPAZ_STR_CAST(":"));
        int scope = atoi(topaz_string_get_c_str(number));

        const topazString_t * command = topaz_string_get_substr(
            str, 
            topaz_string_get_length(number)+1, // skip the :
            topaz_string_get_length(str)-1
        );


        // we ignore the scope because only know the "VM Debug" scope :(

        topaz_script_object_destroy(topaz_script_expression(scr, command));
        topaz_string_destroy(str);
        topaz_script_notify_command(g->script, cmd, topaz_string_create());
        
        break;
      }
      default:;
    }
}






const topazScript_DebugState_t * topaz_gravity_debug_get_state(topazScript_t * scr, void * data) {
    TopazGravity * g = data;
    clear_debug_state(&(g->debugState));


    int * lineno;
    int * fileid;
    gravity_closure_t * closures;
    uint32_t count = gravity_vm_get_callstack(
        g->vm,
        &lineno,
        &fileid,
        &closures
    );

    if (count) {
        int i;
        for(i = count-1; i >= 0; --i) {
            debug_state_add_frame(
                &(g->debugState),
                lineno[i],
                get_file_name_from_id(g, fileid[i]),
                closures[i].f->identifier
            );
        }
        free(lineno);
        free(fileid);
        free(closures);
    }
    return &g->debugState;
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
    //api->objectAPI.object_reference_to_string = topaz_gravity_object_reference_to_string;
    

    api->script_create = topaz_gravity_create;
    api->script_destroy = topaz_gravity_destroy;
    api->script_map_native_function = topaz_gravity_map_native_function;
    api->script_run = topaz_gravity_run;
    api->script_expression = topaz_gravity_expression;
    api->script_create_empty_object = topaz_gravity_create_empty_object;
    //api->script_throw_error = topaz_gravity_throw_error;
    api->script_bootstrap = topaz_gravity_bootstrap;


    api->script_debug_start = topaz_gravity_debug_start;
    api->script_debug_send_command = topaz_gravity_debug_send_command;
    api->script_debug_get_state = topaz_gravity_debug_get_state;
}





