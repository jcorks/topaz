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
#include <topaz/system.h>
#include <topaz/version.h>
#include <topaz/containers/array.h>
#include <topaz/modules/console.h>
#include <topaz/topaz.h>

#include <stdio.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif
#include "duktape.h"
#include "duk_trans_dvalue.h"


void PLOG(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Warning
    );
    topaz_string_destroy(str);
}


void PWARN(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Warning
    );
    topaz_string_destroy(str);
}


void PERROR(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Error
    );
    topaz_string_destroy(str);
}

typedef struct {
    // all TOPAZDUK * instances active.
    topazArray_t * instances;

} TOPAZDUKBackend;


typedef struct {
    duk_context * js;
    topazScript_t * script;
    // used for object reference creation
    int lastObjectReference;
    topaz_t * ctx;

    //// debugging

    // whether debugging has started
    int isDebugging;

    // facilitates debug messages from the duktape context
    duk_trans_dvalue_ctx * trans_ctx;

    // debug messages left to process
    topazArray_t * pendingMessages;

    // Whether the debug state is waiting for the initial cooperation message 
    // to be processed.
    int debugReachedInit;

    // sent commands t
    topazArray_t * lastCommand;

    // the level of stack for the current debug context
    int debugLevel;

    // state of the defbug
    topazScript_DebugState_t debugState;

    // queue of breakpoints requested
    // Maps the "duktape" index to the duktapeJS id
    topazArray_t * debugBreakpoints;

    // queue of breakpoints requested to be removed.
    topazArray_t * debugRemoveBreakpoint;

    // queue of notifications to send to the script instance.
    // theyre queued until ALL intermediate steps are finished, allowing 
    // the script context to rely on the debug state properly. 
    topazArray_t * debugQueuedNotifications;
} TOPAZDUK;


// The tag is the data that "follows" a script object and is unique 
// to that object. Since you can have multiple object references 
// refer to the same object, the tag for that object should always be
// the same for all those references.
//
typedef struct {
    
    // used for set/get native
    void * nativeData;

    // also used for set/get native
    int tagID;

    // reference counting for when to lock the object to prevent garbage collection
    int refCount;

    // the source context
    TOPAZDUK * ctx;

    // populated if the object was created from C
    topaz_script_native_function cfinalizer;

    void * cfinalizerData;

    // used for finding the tag in the global stash
    uint32_t stashID;
} TOPAZDUKObjectTag;



// when notifications are prepared, they are stored intermediately 
// in in this.
typedef struct {
    // relecant command
    int command;

    // the result to deliver to the script instance
    topazString_t * result;

} DebugNotification;


// assumes object to receive prop is already at the top of the stack
// ideally sets the property as a read-only value
static void topaz_duk_set_private_prop(duk_context * js, const char * pName, void * val) {
    duk_push_string(js, pName);
    duk_push_sprintf(js, "p%p", val);
    duk_def_prop(
        js,
        -3,
        DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_CONFIGURABLE
    );
}

// Retrieves the data placed within the js obejct as a pointer.
// Null if none. Assumes the source object is the top.
static void * topaz_duk_get_private_prop(duk_context * js, const char * pName) {
    duk_get_prop_string(js, -1, pName);
    void * out = NULL;
    const char * target = duk_get_string(js, -1);
    if (target)
        sscanf(target, "p%p", &out);
    duk_pop(js);
    return out;
}


// Gets the object tag from the top object
static TOPAZDUKObjectTag * topaz_duk_object_get_tag_from_top(duk_context * js) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(js);
    #endif

    // uhh?? unsafe if someone puts a ___tz prop by hand in a js context.
    // think of a different way before release, thanks
    TOPAZDUKObjectTag * tag = topaz_duk_get_private_prop(js, "___tz");

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(js) == stackSize);
    #endif
    return tag;
}

// assigns the pointer to the tag as a read-only property string 
// for the object itself.
static TOPAZDUKObjectTag * topaz_duk_object_set_tag(TOPAZDUK * ctx) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif

    static uint32_t stashPool = 1;

    TOPAZDUKObjectTag * out = calloc(1, sizeof(TOPAZDUKObjectTag));
    out->stashID = stashPool++;
    topaz_duk_set_private_prop(ctx->js, "___tz", out);


    #ifdef TOPAZDC_DEBUG  
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

    return out;
}


// prevents garbage collection by assigning a property to the global stash object.
//
static void topaz_duk_object_keep_reference(TOPAZDUK * ctx, TOPAZDUKObjectTag * tag) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif
    
    duk_push_global_stash(ctx->js);
    duk_push_sprintf(ctx->js, "p%d", tag->stashID);
    duk_dup(ctx->js, -3);
    
    assert(duk_is_object(ctx->js, -1));
    assert(duk_put_prop(ctx->js, -3));
    duk_pop(ctx->js);


    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif
}


// removes the key in the global stash, which leaves the object 
// open to garbage collection sweeps if it is no longer used / accessible.
static void topaz_duk_object_unkeep_reference(TOPAZDUK * ctx, TOPAZDUKObjectTag * tag) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif

    duk_push_global_stash(ctx->js);
    duk_push_sprintf(ctx->js, "p%d", tag->stashID);

    duk_del_prop(ctx->js, -2);
    duk_pop(ctx->js);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

}


static duk_ret_t topaz_duk_object_finalizer(duk_context * js) {
    TOPAZDUKObjectTag * tag = topaz_duk_object_get_tag_from_top(js);
    #ifdef TOPAZDC_DEBUG
        assert(tag);
    #endif

    // if this is even happening, the global stash entry was already removed.

    // call c finalizer, which is why this is here mostly
    if (tag->cfinalizer)
        tag->cfinalizer(
                tag->ctx->script, 
                topaz_array_empty(), 
                tag->cfinalizerData
        );

    free(tag);
    return 0;
}



static void topaz_duk_object_push_to_top_from_tag(TOPAZDUKObjectTag * tag) {
    duk_push_global_stash(tag->ctx->js);
    duk_push_sprintf(tag->ctx->js, "p%d", tag->stashID);
    if (!duk_get_prop(tag->ctx->js, -2)) {
        #ifdef TOPAZDC_DEBUG
            if (duk_is_undefined(tag->ctx->js, -1)) {
                assert(!"topaz_duk_object_push_to_top_from_tag() resulted in UNDEFINED. This shouldn't happen!\n");
            }
        #endif
    }
    duk_remove(tag->ctx->js, -2);
}








// creates a new topaz script object from a value on the duk stack
static topazScript_Object_t * topaz_duk_stack_object_to_tso(TOPAZDUK * ctx, int index) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif
    
    duk_dup(ctx->js, index);
    topazScript_Object_t * o = NULL;
    switch(duk_get_type(ctx->js, -1)) {
      case DUK_TYPE_NONE:
        #ifdef TOPAZDC_DEBUG
            
            PWARN(ctx->ctx, topaz_string_create_from_c_str("duktapeJS returned DUK_TYPE_NONE, this is likely indicative of programmer error!"));
        #endif
        break;
      case DUK_TYPE_BOOLEAN:
        o = topaz_script_object_from_int(ctx->script, duk_get_boolean(ctx->js, -1));
        break;
      case DUK_TYPE_NUMBER:    
        o = topaz_script_object_from_number(ctx->script, duk_get_number(ctx->js, -1));
        break;

      case DUK_TYPE_STRING:
        o = topaz_script_object_from_string(ctx->script, TOPAZ_STR_CAST(duk_get_string(ctx->js, -1)));
        break;

      case DUK_TYPE_OBJECT: {
        o = topaz_script_object_from_reference(ctx->script, ctx);
        break;
      }

      default:;
        o = topaz_script_object_undefined(ctx->script);
        break;
    }

    duk_pop(ctx->js);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif
    return o;
}


// pushes the given object as a duktape object
// on the top of the stack.
static void topaz_duk_object_push_tso(TOPAZDUK * ctx, topazScript_Object_t * o) {
    switch(topaz_script_object_get_type(o)) {

      case topazScript_Object_Type_Integer:
        duk_push_int(ctx->js, topaz_script_object_as_int(o));
        break;

      case topazScript_Object_Type_Number:
        duk_push_number(ctx->js, topaz_script_object_as_number(o));
        break;

      case topazScript_Object_Type_String:
        duk_push_string(ctx->js, topaz_string_get_c_str(topaz_script_object_as_string(o)));
        break;

      case topazScript_Object_Type_Reference: {
        TOPAZDUKObjectTag * tag = topaz_script_object_get_api_data(o);
        topaz_duk_object_push_to_top_from_tag(tag);
        break;
      }
      default:
        duk_push_undefined(ctx->js);
        break;        
    }
}

static duk_ret_t topaz_duk_get_internal(duk_context * js) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(js);
    #endif

    // retrieve tag and actual getter
    duk_push_current_function(js);
    TOPAZDUKObjectTag * tag = topaz_duk_get_private_prop(js, "___tz");
    topaz_script_native_function getReal = (topaz_script_native_function)topaz_duk_get_private_prop(js, "___tzfn");
    duk_pop(js);

    // We need a representative object for the 'this' to which the 
    // getter belongs
    topaz_duk_object_push_to_top_from_tag(tag);
    topazScript_Object_t * self = topaz_duk_stack_object_to_tso(tag->ctx, -1);
    duk_pop(js);

    // actually call the native getter
    topazScript_Object_t * res = getReal(
        tag->ctx->script,
        TOPAZ_ARRAY_CAST(&self, topazScript_Object_t *, 1),
        NULL //????
    );

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(js) == stackSize);
    #endif

    // finally, push the tso result as a duk object
    if (res) {
        topaz_duk_object_push_tso(tag->ctx, res);
    } else {
        duk_push_undefined(tag->ctx->js);
    }

    topaz_script_object_destroy(self);
    topaz_script_object_destroy(res);
    return 1;
}

static duk_ret_t topaz_duk_set_internal(duk_context * js) {
    // we assume that get_top == 2 since its a setter
    duk_push_current_function(js);
    TOPAZDUKObjectTag * tag = topaz_duk_get_private_prop(js, "___tz");
    topaz_script_native_function setReal = (topaz_script_native_function)topaz_duk_get_private_prop(js, "___tzfn");
    duk_pop(js);

    topazScript_Object_t * args[2];


    // We need a representative object for the 'this' to which the 
    // getter belongs
    topaz_duk_object_push_to_top_from_tag(tag);
    args[0] = topaz_duk_stack_object_to_tso(tag->ctx, -1);
    duk_pop(js);

    // arg 0 is always the value in question
    args[1] = topaz_duk_stack_object_to_tso(tag->ctx, 0);


    topazScript_Object_t * result = setReal(
        tag->ctx->script,
        TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2),
        NULL //???????????
    );

    #ifdef TOPAZDC_DEBUG
        assert(!result);
    #endif

    
    topaz_script_object_destroy(args[0]);
    topaz_script_object_destroy(args[1]);

    return 0;
}





static duk_ret_t topaz_duk_native_function_internal(duk_context * js) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(js);
    #endif

    // retrieve tag and actual getter
    duk_push_current_function(js);
    TOPAZDUK * ctx = topaz_duk_get_private_prop(js, "___tzcx");
    topaz_script_native_function fnReal = topaz_duk_get_private_prop(js, "___tzfn");
    void * fnData = topaz_duk_get_private_prop(js, "___tzdt");
    duk_pop(js);


    // convert all args to tso
    uint32_t i;
    uint32_t nargs = duk_get_top(js);
    topazScript_Object_t * args[nargs];
    for(i = 0; i < nargs; ++i) {
        args[i] = topaz_duk_stack_object_to_tso(ctx, i);
    }

    // actually call the native fn
    topazScript_Object_t * res = fnReal(
        ctx->script,
        TOPAZ_ARRAY_CAST(&args, topazScript_Object_t *, nargs),
        fnData
    );

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(js) == stackSize);
    #endif

    // finally, push the tso result as a duk object 
    if (res) {
        topaz_duk_object_push_tso(ctx, res);
        topaz_script_object_destroy(res);
    } else {
        duk_push_undefined(ctx->js);
    }

    for(i = 0; i < nargs; ++i) {
        topaz_script_object_destroy(args[i]);
    }
    return 1;
}


static topazString_t * topaz_duk_stack_where(TOPAZDUK * ctx) {
    topazString_t * str = topaz_string_create();

    if (duk_is_error(ctx->js, -2)) {
        duk_get_prop_string(ctx->js, -2, "lineNumber");
        topaz_string_concat_printf(str, "ERROR @ line %d", duk_to_int(ctx->js, -1));
        duk_pop(ctx->js);

        duk_get_prop_string(ctx->js, -2, "fileName");
        topaz_string_concat_printf(str, " in \"%s\"\n", duk_to_string(ctx->js, -1));
        duk_pop(ctx->js);

    }



    int level = -1;    

    for(;;) {
        #ifdef TOPAZDC_DEBUG
            int stackSize = duk_get_top(ctx->js);
        #endif


        duk_inspect_callstack_entry(ctx->js, level);
        if (duk_is_undefined(ctx->js, -1)) 
            break;

        if (level == -1)
            topaz_string_concat_printf(str, "@ innermost :");
        else 
            topaz_string_concat_printf(str, "@ L%d :", (level*-1)-1);

            


        duk_get_prop_string(ctx->js, -1, "function");
        duk_get_prop_string(ctx->js, -1, "name");
        topaz_string_concat_printf(str, "In call %s()", duk_to_string(ctx->js, -1));
        duk_pop(ctx->js);
        duk_pop(ctx->js);

        duk_get_prop_string(ctx->js, -1, "lineNumber");
        int lineNum = duk_to_int(ctx->js, -1);
        if (lineNum)
            topaz_string_concat_printf(str, ", line [%d]", lineNum);

        duk_pop(ctx->js);
        duk_pop(ctx->js);

        topaz_string_concat_printf(str, "\n");
        level--;
 
        #ifdef TOPAZDC_DEBUG 
            assert(duk_get_top(ctx->js) == stackSize);
        #endif

    }
    return str;
}


static void topaz_duk_fatal(void * udata, const char *msg) {
    TOPAZDUK * ctx = udata;
    PERROR(ctx->ctx, topaz_string_create_from_c_str("TOPAZ-DUKTAPE FATAL ERROR: %s\n", msg));

    topazString_t * str = topaz_duk_stack_where(ctx);
    PERROR(ctx->ctx, str);
    fflush(stdout);
}













static void * topaz_duk_create(topazScript_t * scr, topaz_t * ctx) {
    TOPAZDUK * out = calloc(1, sizeof(TOPAZDUK));
    out->debugState.callstack = topaz_array_create(sizeof(topazScript_CallstackFrame_t));
    out->ctx = ctx;
    out->debugBreakpoints = topaz_array_create(sizeof(int));
    out->debugRemoveBreakpoint = topaz_array_create(sizeof(int));
    out->debugQueuedNotifications = topaz_array_create(sizeof(DebugNotification));

    out->js = duk_create_heap(
        NULL,
        NULL,
        NULL, 
        out,
        topaz_duk_fatal
    );
    out->script = scr;

    TOPAZDUKBackend * backend = topaz_system_backend_get_user_data(topaz_script_get_backend(scr));
    topaz_array_push(backend->instances, out);
    return out;
} 

static void topaz_duk_destroy(topazScript_t * scr, void * data) {
    TOPAZDUKBackend * backend = topaz_system_backend_get_user_data(topaz_script_get_backend(scr));
    uint32_t i;
    uint32_t len = topaz_array_get_size(backend->instances);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(backend->instances, TOPAZDUK*, i) == data) {
            topaz_array_remove(backend->instances, i);
            break;
        }
    }


    TOPAZDUK * ctx = data;
    // TODO: destroy ALL objects??
    duk_destroy_heap(ctx->js);
}

static int topaz_duk_map_native_function(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * pname, 
    topaz_script_native_function fn, 
    void * userData

) {
    TOPAZDUK * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif

    duk_push_global_object(ctx->js);
    duk_push_c_function(ctx->js, topaz_duk_native_function_internal, DUK_VARARGS);

    topaz_duk_set_private_prop(ctx->js, "___tzcx", ctx);
    topaz_duk_set_private_prop(ctx->js, "___tzfn", (void*) fn);
    topaz_duk_set_private_prop(ctx->js, "___tzdt", userData);

    duk_put_prop_string(ctx->js, -2, topaz_string_get_c_str(pname));
    duk_pop(ctx->js);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

    return 1;
}


static topazScript_Object_t * topaz_duk_expression(
    topazScript_t * script, 
    void * data,
    const topazString_t * expr
) {
    TOPAZDUK * ctx = data;
    topazScript_Object_t * out;
    if (duk_peval_lstring(ctx->js, topaz_string_get_c_str(expr), topaz_string_get_length(expr))) {
        topazString_t * str = topaz_string_create_from_c_str("Eval error: ");

        if (duk_is_error(ctx->js, -1)) {
            duk_get_prop_string(ctx->js, -1, "lineNumber");
            topaz_string_concat_printf(str, "ERROR @ line %d", duk_to_int(ctx->js, -1));
            duk_pop(ctx->js);

            duk_get_prop_string(ctx->js, -1, "fileName");
            topaz_string_concat_printf(str, " in \"%s\"\n", duk_to_string(ctx->js, -1));
            duk_pop(ctx->js);

            duk_get_prop_string(ctx->js, -1, "stack");
            topaz_string_concat_printf(str, " in \"%s\"\n", duk_to_string(ctx->js, -1));
            duk_pop(ctx->js);

        }

        PERROR(ctx->ctx, str);
        fflush(stdout);






        out = topaz_script_object_undefined(ctx->script);    
    } else {
        out = topaz_duk_stack_object_to_tso(
            ctx,
            -1
        );
    }

    duk_pop(ctx->js);
    return out;
}

static void topaz_duk_throw_error(
    topazScript_t * script, 
    void * data,
    const topazArray_t * args
) {
    TOPAZDUK * ctx = data;
    
    topazString_t * str = topaz_string_create();
    topaz_string_concat_printf(
        str,
        "throw new Error('%s');",
        topaz_array_get_size(args) ?
            topaz_string_get_c_str(topaz_script_object_as_string(topaz_array_at(args, topazScript_Object_t *, 0)))
        :
            "Internal native error."
    );
    duk_eval_string(ctx->js, topaz_string_get_c_str(str));
    duk_pop(ctx->js);
    topaz_string_destroy(str);
}

void topaz_duk_run(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * sourceName, 
    const topazString_t * sourceData) {

    TOPAZDUK * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif

    duk_push_string(ctx->js, topaz_string_get_c_str(sourceData));
    duk_push_string(ctx->js, topaz_string_get_c_str(sourceName));
    duk_compile(ctx->js, 0);
    if (duk_pcall(ctx->js, 0)) {
        topazString_t * str = topaz_string_create();
        if (duk_is_error(ctx->js, -1)) {
            duk_get_prop_string(ctx->js, -1, "lineNumber");
            topaz_string_concat_printf(str, "ERROR @ line %d", duk_to_int(ctx->js, -1));
            duk_pop(ctx->js);

            duk_get_prop_string(ctx->js, -1, "fileName");
            topaz_string_concat_printf(str, " in \"%s\"\n", duk_to_string(ctx->js, -1));
            duk_pop(ctx->js);

            duk_get_prop_string(ctx->js, -1, "stack");
            topaz_string_concat_printf(str, " in \"%s\"\n", duk_to_string(ctx->js, -1));
            duk_pop(ctx->js);
        } else {
            topaz_string_concat_printf(str, "%s", "Unknown error occurred within scripting context (DUKTAPE did not throw error object?)\n");
        }
        PERROR(ctx->ctx, str);
        fflush(stdout);
    }
    duk_pop(ctx->js); // throw away result of last statement.

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif
}

topazScript_Object_t * topaz_duk_create_empty_object(
    topazScript_t * script,
    void * data,

    topaz_script_native_function cleanup,
    void * cleanupData

) {
    TOPAZDUK * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif


    // create the new object
    duk_push_object(ctx->js);

    // create raw tso
    topazScript_Object_t * out = topaz_duk_stack_object_to_tso(ctx, -1);

    // get the tag so we can add the finalizer
    TOPAZDUKObjectTag * tag = topaz_duk_object_get_tag_from_top(ctx->js);

    tag->cfinalizer = cleanup;
    tag->cfinalizerData = cleanupData;

    duk_pop(ctx->js);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

    return out;
}


void topaz_duk_bootstrap(topazScript_t * script, void * n) {
    #include "bootstrap_bytes"

    #ifdef TOPAZDC_DEBUG
        topaz_script_run(
            script,
            TOPAZ_STR_CAST("TOPAZ_BOOTSTRAP.js"),
            TOPAZ_STR_CAST((char*)bootstrap_bytes)
        );
    #else
        topaz_duk_run(
            script, n,
            TOPAZ_STR_CAST("[internal]"),
            TOPAZ_STR_CAST((char*)bootstrap_bytes)
        );
    #endif
}
























// assumes the object is at the top
static void * topaz_duk_object_reference_create(topazScript_Object_t * o, void * ctxSrc) {
    TOPAZDUK * ctx = ctxSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif

    TOPAZDUKObjectTag * tag = topaz_duk_object_get_tag_from_top(ctx->js);

    if (!tag) {
        // create a new tag and bind that tag to this object using a hidden prop
        tag = topaz_duk_object_set_tag(ctx);   

        tag->ctx = ctx;

 

        // Sets the standard finalizer function.
        duk_push_c_function(ctx->js, topaz_duk_object_finalizer, 0);
        duk_set_finalizer(ctx->js, -2);
    }

    if (tag->refCount == 0) { 
        // make sure that a reference to this object exists outside of the pure 
        // js context. This allows the C context to keep the object around.
        // in the case that the C tag has persisted when vulnerable to garbage collection,
        // the ref count might be 0 despite the tag existing before. THats why this check 
        // is for tag->ref == 0 rather than just creation
        topaz_duk_object_keep_reference(ctxSrc, tag);
    }



    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

    return tag;
} 
 
 
static void * topaz_duk_object_reference_create_from_reference(topazScript_Object_t * o, void * ctxSrc, topazScript_Object_t * from, void * fromData) {
    TOPAZDUK * ctx = ctxSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif
    topaz_duk_object_push_to_top_from_tag(fromData);
    void * outTag = topaz_duk_object_reference_create(o, ctxSrc);

    duk_pop(ctx->js);
    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif
    return outTag;
}




static void topaz_duk_object_reference_destroy(topazScript_Object_t * o, void * tagSrc) {
    // the finalizer handlers destruction for us, 
    // so there isnt much to do here.
    // (topaz will unref for us)
}



static int topaz_duk_object_reference_get_feature_mask(topazScript_Object_t * o, void * tagSrc) {
    TOPAZDUKObjectTag * t = tagSrc;
    duk_context * ctx = t->ctx->js;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx);
    #endif

    topaz_duk_object_push_to_top_from_tag(t);
    int out = topazScript_Object_Feature_Extendable; // all objects in js can have properties
    if (duk_is_callable(ctx, -1)) {
        out |= topazScript_Object_Feature_Callable;
    }

    if (duk_is_array(ctx, -1)) {
        out |= topazScript_Object_Feature_Array;
    }

    // /should/ always be true...
    if (duk_is_object(ctx, -1)) {
        out |= topazScript_Object_Feature_Map;
    }

    duk_pop(ctx);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx) == stackSize);
    #endif
    return out;
}

static void * topaz_duk_object_reference_get_native_data(topazScript_Object_t * o, int * tagID, void * tagSrc) {
    TOPAZDUKObjectTag * t = tagSrc;
    *tagID = t->tagID;
    return t->nativeData;
}

static void topaz_duk_object_reference_set_native_data(topazScript_Object_t * o, void * nativeData, int tagID, void * tagSrc) {
    TOPAZDUKObjectTag * t = tagSrc;
    t->tagID = tagID;
    t->nativeData = nativeData;
}



static void topaz_duk_object_reference_ref(topazScript_Object_t * o, void * data) {
    TOPAZDUKObjectTag * tag = data;
    // when 0, will unkeep
    tag->refCount++;
}

static void topaz_duk_object_reference_unref(topazScript_Object_t * o, void * data) {
    TOPAZDUKObjectTag * tag = data;
    // when 0, will unkeep
    tag->refCount--;

    if (tag->refCount == 0) {
        #ifdef TOPAZDC_DEBUG
            int stackSize = duk_get_top(tag->ctx->js);
        #endif

        topaz_duk_object_push_to_top_from_tag(tag);
        topaz_duk_object_unkeep_reference(tag->ctx, tag);
        duk_pop(tag->ctx->js); 

        #ifdef TOPAZDC_DEBUG 
            assert(duk_get_top(tag->ctx->js) == stackSize);
        #endif
    }

}




static topazScript_Object_t * topaz_duk_object_reference_call(topazScript_Object_t * o, const topazArray_t * args, void * data) {
    TOPAZDUKObjectTag * tag = data;

    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    // assumes callable
    topaz_duk_object_push_to_top_from_tag(tag);


    // duk calling
    uint32_t i = 0;
    uint32_t len = topaz_array_get_size(args);
    for(; i < len; ++i) {
        topazScript_Object_t * o = topaz_array_at(args, topazScript_Object_t *, i);
        topaz_duk_object_push_tso(
            tag->ctx, 
            o
        );
    }

    duk_call(tag->ctx->js, len);
    topazScript_Object_t * out = topaz_duk_stack_object_to_tso(
        tag->ctx,
        -1
    );
    duk_pop(tag->ctx->js);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif
 
    return out;
}


static topazScript_Object_t * topaz_duk_object_reference_array_get_nth(
    topazScript_Object_t * oSrc, 
    int index,  
    void * tagSrc
) {

    TOPAZDUKObjectTag * tag = tagSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    topaz_duk_object_push_to_top_from_tag(tag);
    duk_get_prop_index(tag->ctx->js, -1, index);
    topazScript_Object_t * o = topaz_duk_stack_object_to_tso(tag->ctx, index);
    duk_pop_2(tag->ctx->js);

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif

    return o;
}


static int topaz_duk_object_reference_array_get_count(topazScript_Object_t * o, void * tagSrc) {
    TOPAZDUKObjectTag * tag = tagSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    topaz_duk_object_push_to_top_from_tag(tag);
    int len = (int)duk_get_length(tag->ctx->js, -1);
    duk_pop(tag->ctx->js); 

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif
    return len;
}

static topazScript_Object_t * topaz_duk_object_reference_map_get_property(
    topazScript_Object_t * o, 
    const topazString_t * prop, 
    void * tagSrc
) {
    TOPAZDUKObjectTag * tag = tagSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    topaz_duk_object_push_to_top_from_tag(tag);
    duk_push_string(tag->ctx->js, topaz_string_get_c_str(prop));
    duk_get_prop(tag->ctx->js, -2);
    topazScript_Object_t * out = topaz_duk_stack_object_to_tso(tag->ctx, -1);
    duk_pop_2(tag->ctx->js);


    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif

    return out;
}

void topaz_duk_object_reference_to_string(
    topazScript_Object_t * o, 
    topazString_t * str,
    void * tagSrc) {
    
    TOPAZDUKObjectTag * tag = tagSrc;

    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    topaz_duk_object_push_to_top_from_tag(tag);
    const char * strC = duk_to_string(tag->ctx->js, -1);
    topaz_string_concat_printf(str, "%s", strC);
    duk_pop(tag->ctx->js);


    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif
}

void topaz_duk_object_reference_extendable_add_property(
    topazScript_Object_t * object, 
    const topazString_t * propName,
    topaz_script_native_function onSet,
    topaz_script_native_function onGet,
    void * tagSrc
) {
    TOPAZDUKObjectTag * tag = tagSrc;
    int hasWhich = 0;

    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    #ifdef DEBUG
        assert(onSet || onGet);
    #endif

    topaz_duk_object_push_to_top_from_tag(tag);

    duk_push_string(tag->ctx->js, topaz_string_get_c_str(propName));

    if (onGet) {

        duk_push_c_function(tag->ctx->js, topaz_duk_get_internal, 0);
        topaz_duk_set_private_prop(tag->ctx->js, "___tz",   tag);
        topaz_duk_set_private_prop(tag->ctx->js, "___tzfn", (void*) onGet);
        hasWhich |= DUK_DEFPROP_HAVE_GETTER;

    }


    if (onSet) {
        duk_push_c_function(tag->ctx->js, topaz_duk_set_internal, 1);
        // we need to set a prop to this tag 
        topaz_duk_set_private_prop(tag->ctx->js, "___tz",   tag);
        topaz_duk_set_private_prop(tag->ctx->js, "___tzfn", (void*) onSet);
        hasWhich |= DUK_DEFPROP_HAVE_SETTER;
    }

    
    duk_def_prop(
        tag->ctx->js, 
        -4,
        hasWhich
    );
    
    duk_pop(tag->ctx->js);
    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif
}



// SUPPORT TRANS WRITES

static const char * dvalue_to_string(duk_dvalue * dv) {
    static char bufOut[64];

	if (!dv) {
		return "NULL";
	}

	switch (dv->tag) {
	case DUK_DVALUE_EOM:
        return "EOM";

	case DUK_DVALUE_REQ:
        return "REQ";

	case DUK_DVALUE_REP:
        return "REP";

	case DUK_DVALUE_ERR:
        return "ERR";

	case DUK_DVALUE_NFY:
        return "NFY";

	case DUK_DVALUE_INTEGER: {
		sprintf(bufOut, "%d", dv->i);
		return bufOut;
    }

	case DUK_DVALUE_STRING:
        return (const char*)dv->buf;        

	case DUK_DVALUE_BUFFER:
        return "[Buffer]";

	case DUK_DVALUE_UNUSED:
        return "undefined";

	case DUK_DVALUE_UNDEFINED:
        return "undefined";

	case DUK_DVALUE_NULL:
        return "null";

	case DUK_DVALUE_TRUE:
        return "true";

	case DUK_DVALUE_FALSE:
        return "false";

	case DUK_DVALUE_NUMBER:
		if (fpclassify(dv->d) == FP_ZERO) {
			if (signbit(dv->d)) {
                return "-0";
			} else {
                return "0";
			}
		} else {
			sprintf(bufOut, "%lg", dv->d);
            return bufOut;
		}

	case DUK_DVALUE_OBJECT:
        return "[Object]";

	case DUK_DVALUE_POINTER:
        return "[Pointer]";

	case DUK_DVALUE_LIGHTFUNC:
        return "[L-Function]";

	case DUK_DVALUE_HEAPPTR:
        return "[H-Pointer]";

	default:;
	}
    return "[Unknown]";

}

static void topaz_duk_trans_received(duk_trans_dvalue_ctx * ctxT, duk_dvalue * dv) {

    //printf("dvalue received: %s\n", &bufferSrc[0]);
    TOPAZDUK * ctx = ctxT->userData;
    char * cpy = strdup(dvalue_to_string(dv));
    topaz_array_push(ctx->pendingMessages, cpy);
    #ifdef TOPAZDC_DEBUG
        //printf("RECEIVED DEBUG MSG: %s\n", cpy);
    #endif
}

static void topaz_duk_trans_handshake(duk_trans_dvalue_ctx * ctx, const char * handshake) {
    //printf("handshake: %s\n", handshake);
    fflush(stdout);

}

static void topaz_duk_trans_detached(duk_trans_dvalue_ctx * ctx) {
    //printf("was detached!\n");
    fflush(stdout);

}


// commands

#define DUKTAPE_COMMAND__GET_CALLSTACK 256

static void topaz_duk_trans_command__pause(duk_trans_dvalue_ctx * ctxT) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x12);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent pause request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_Pause;
    topaz_array_push(ctx->lastCommand, c);    
}



static void topaz_duk_trans_command__resume(duk_trans_dvalue_ctx * ctxT) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x13);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent resume request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_Resume;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_duk_trans_command__step_into(duk_trans_dvalue_ctx * ctxT) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x14);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent step into request\n");


    topazScript_DebugCommand_t c = topazScript_DebugCommand_StepInto;
    topaz_array_push(ctx->lastCommand, c);    
}



static void topaz_duk_trans_command__step_over(duk_trans_dvalue_ctx * ctxT) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x15);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent step over request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_StepOver;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_duk_trans_command__add_breakpoint(duk_trans_dvalue_ctx * ctxT, const topazString_t * filename, int line) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x18);  
    duk_trans_dvalue_send_string(ctxT, topaz_string_get_c_str(filename));
    duk_trans_dvalue_send_integer(ctxT, line);
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent add break request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_AddBreakpoint;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_duk_trans_command__delete_breakpoint(duk_trans_dvalue_ctx * ctxT, int index) {
    TOPAZDUK * ctx = ctxT->userData;

    // assumes that we're keeping track of the debug indices correctly.
    if (index >= 0 && index < topaz_array_get_size(ctx->debugBreakpoints)) {
        int id = topaz_array_at(ctx->debugBreakpoints, int, index);
        topaz_array_remove(ctx->debugBreakpoints, index);
        topaz_array_push(ctx->debugRemoveBreakpoint, id);
    }


    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x19);  
    duk_trans_dvalue_send_integer(ctxT, index);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent breakpoint delete request %d\n", index);

    topazScript_DebugCommand_t c = topazScript_DebugCommand_RemoveBreakpoint;
    topaz_array_push(ctx->lastCommand, c);    

}

static void topaz_duk_trans_command__eval(duk_trans_dvalue_ctx * ctxT, const topazString_t * eval, int callstackLevel) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x1e);  
    duk_trans_dvalue_send_integer(ctxT, callstackLevel);
    duk_trans_dvalue_send_string(ctxT, topaz_string_get_c_str(eval));
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent eval request\n");


    topazScript_DebugCommand_t c = topazScript_DebugCommand_ScopedEval;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_duk_trans_command__get_call_stack(duk_trans_dvalue_ctx * ctxT) {
    TOPAZDUK * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x1c);  
    duk_trans_dvalue_send_eom(ctxT);

    int c = DUKTAPE_COMMAND__GET_CALLSTACK;
    topaz_array_push(ctx->lastCommand, c);    
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

// queues a notification. Ownership of the string 
static void topaz_duk_debug_queue_notify(TOPAZDUK * ctx, int cmd, topazString_t * str) {
    DebugNotification n;
    n.command = cmd;
    n.result = str;
    topaz_array_push(ctx->debugQueuedNotifications, n);
}

static void topaz_duk_push_notify(TOPAZDUK * ctx) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(ctx->debugQueuedNotifications);
    if (!len) return;
    for(i = 0; i < len; ++i) {
        topaz_script_notify_command(
            ctx->script, 
            topaz_array_at(ctx->debugQueuedNotifications, DebugNotification, i).command,            
            topaz_array_at(ctx->debugQueuedNotifications, DebugNotification, i).result  
        );
    }
    topaz_array_set_size(ctx->debugQueuedNotifications, 0);
    #ifdef TOPAZDC_DEBUG
        //printf("Pushed %d notifications to the script instance.\n", (int)len);
    #endif
}

static void topaz_duk_trans_cooperate(duk_trans_dvalue_ctx * ctxT, int block) {
    TOPAZDUK * ctx = ctxT->userData;
    if (!ctx->debugReachedInit) {
        #ifdef TOPAZDC_DEBUG
            printf("Init response\n");
        #endif
        topaz_duk_trans_command__resume(ctx->trans_ctx);
        ctx->debugReachedInit = 1;
        return;
    }
    


    if (topaz_array_get_size(ctx->pendingMessages) && 
        !strcmp(
            topaz_array_at(ctx->pendingMessages, char *, topaz_array_get_size(ctx->pendingMessages)-1), 
            "EOM"
        )) {

        topazArray_t * messagesSrc = topaz_array_clone(ctx->pendingMessages);
        char ** messages     = topaz_array_get_data(messagesSrc);
        uint32_t messagesLen = topaz_array_get_size(messagesSrc);

        topaz_array_set_size(ctx->pendingMessages, 0);
        
        topazScript_DebugCommand_t command = topazScript_DebugCommand_Custom;
        if (topaz_array_get_size(ctx->lastCommand)) {
            command = topaz_array_at(ctx->lastCommand, topazScript_DebugCommand_t, topaz_array_get_size(ctx->lastCommand)-1);
            topaz_array_set_size(ctx->lastCommand, topaz_array_get_size(ctx->lastCommand)-1);
        }



        // notify from debugger (status)
        if (!strcmp(messages[0], "NFY")) {
            if (!strcmp(messages[1], "1")) {
                topaz_duk_trans_command__get_call_stack(ctx->trans_ctx);
                if (!strcmp(messages[2], "1")) { // paused state
                    ctx->debugLevel = 0;
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_Pause,
                        topaz_string_create()
                    );
                } else if (!strcmp(messages[2], "0")) { // resume
                    ctx->debugLevel = 0;
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_Resume,
                        topaz_string_create()
                    );
                } 
            } 
        } else {

            switch((int)command) {
              case topazScript_DebugCommand_AddBreakpoint: {
                if (!strcmp(messages[0], "ERR")) {
                    // error
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_AddBreakpoint,
                        topaz_string_create()
                    );
                } else {
                    static int localID = 1000;
                    topaz_array_push(ctx->debugBreakpoints, localID);

                    // success
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_AddBreakpoint,
                        topaz_string_create_from_c_str("%d", localID++)
                    );
                }
                break;
              }
              case topazScript_DebugCommand_Pause:
                ctx->debugLevel = 0;
                topaz_duk_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_Pause,
                    topaz_string_create()
                );
                break;
                
              case topazScript_DebugCommand_Resume:
                ctx->debugLevel = 0;
                topaz_duk_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_Resume,
                    topaz_string_create()
                );

                break;
                
              case topazScript_DebugCommand_ScopedEval:

                if (!strcmp(messages[1], "0")) {
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_ScopedEval,
                        topaz_string_create_from_c_str("%s", messages[2])
                    );
                } else {
                    // Error
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_ScopedEval,
                        topaz_string_create_from_c_str("Could not evaluate expression (%s).\n", messages[2])
                    );                    
                }
                break;
              case topazScript_DebugCommand_StepInto:
                topaz_duk_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_StepInto,
                    topaz_string_create()
                );                    
                break;
              case topazScript_DebugCommand_StepOver:
                ctx->debugLevel = 0;
                topaz_duk_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_StepOver,
                    topaz_string_create()
                );                    
                break;
                

                
              case topazScript_DebugCommand_RemoveBreakpoint:{
                int id = topaz_array_at(ctx->debugRemoveBreakpoint, int, 0);
                topaz_array_remove(ctx->debugRemoveBreakpoint, 0);

                if (!strcmp(messages[0], "ERR")) {
                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_RemoveBreakpoint,
                        topaz_string_create()
                    );                    
                } else {

                    topaz_duk_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_RemoveBreakpoint,
                        topaz_string_create_from_c_str("%d", id)
                    );                    
                    
                }
                break;
              }

              case DUKTAPE_COMMAND__GET_CALLSTACK: {
                messages++;
                messagesLen-=1;

                clear_debug_state(&ctx->debugState);

                int i;    
                for(i = 0; i < messagesLen/4; ++i) {
                    debug_state_add_frame(&ctx->debugState,
                        atoi(messages[i*4+2]),
                        messages[i*4+0],
                        messages[i*4+1]
                    );
                }
                break;
              }

              default:
                //topaz_duk_trans_command__get_call_stack(ctx);
                //printf("Received %d messages.\n", messagesLen);
                fflush(stdout);
            }
        }
    
    }
    if (!block) return; // still receiving messages
    topaz_context_iterate(ctx->ctx);
}


static void topaz_duk_debug_start(topazScript_t * scr, void * data) {
    TOPAZDUK * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif
    ctx->isDebugging = TRUE;
    ctx->trans_ctx = duk_trans_dvalue_init();
    ctx->trans_ctx->cooperate = topaz_duk_trans_cooperate;
    ctx->trans_ctx->received  = topaz_duk_trans_received;
    ctx->trans_ctx->handshake = topaz_duk_trans_handshake;
    ctx->trans_ctx->detached  = topaz_duk_trans_detached;
    ctx->trans_ctx->userData  = ctx;

    ctx->pendingMessages = topaz_array_create(sizeof(char *));
    ctx->lastCommand     = topaz_array_create(sizeof(topazScript_DebugCommand_t));

    duk_debugger_attach(
        ctx->js,
        duk_trans_dvalue_read_cb,
        duk_trans_dvalue_write_cb,
        duk_trans_dvalue_peek_cb,
        duk_trans_dvalue_read_flush_cb,
        duk_trans_dvalue_write_flush_cb,
        NULL,  /* app request cb */
        duk_trans_dvalue_detached_cb,
        (void *)ctx->trans_ctx
    );


    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

}

void topaz_duk_debug_send_command(
    topazScript_t * scr, 
    void * data, 
    int cmd, 
    const topazString_t * arg
) {
    TOPAZDUK * ctx = data;
    switch(cmd) {
      case topazScript_DebugCommand_Pause:
        topaz_duk_trans_command__pause(ctx->trans_ctx);
        break;    

      case topazScript_DebugCommand_Resume:
        topaz_duk_trans_command__resume(ctx->trans_ctx);
        break;    

      case topazScript_DebugCommand_StepInto:
        topaz_duk_trans_command__step_into(ctx->trans_ctx);
        break;

      case topazScript_DebugCommand_StepOver:
        topaz_duk_trans_command__step_over(ctx->trans_ctx);
        break;

      case topazScript_DebugCommand_AddBreakpoint: {
        topazString_t * str = topaz_string_clone(arg);
        const topazString_t * iter = topaz_string_chain_start(str, TOPAZ_STR_CAST(":"));
        topazString_t * filename = topaz_string_clone(iter);
        iter = topaz_string_chain_proceed(str);        
        int lineNumber = atoi(topaz_string_get_c_str(iter));
                
        topaz_duk_trans_command__add_breakpoint(ctx->trans_ctx, filename, lineNumber);
        break;
      }

      case topazScript_DebugCommand_RemoveBreakpoint: {
        int id = atoi(topaz_string_get_c_str(arg));
        uint32_t i;
        uint32_t len = topaz_array_get_size(ctx->debugBreakpoints);
        for(i = 0; i < len; ++i) {
            if (topaz_array_at(ctx->debugBreakpoints, int, i) == id) {
                topaz_duk_trans_command__delete_breakpoint(ctx->trans_ctx, i);
                return;
            }
        } 
        topaz_duk_trans_command__delete_breakpoint(ctx->trans_ctx, -1);
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
        topaz_duk_trans_command__eval(ctx->trans_ctx, command, (scope+1)*-1);
        topaz_string_destroy(str);
        break;
      }
      default:
        assert(!"TODO");
    }
}


const topazScript_DebugState_t * topaz_duk_debug_get_state(topazScript_t * scr, void * data) {
    TOPAZDUK * ctx = data;
    return &ctx->debugState;
}


static void topaz_duk_cooperate_update(topazSystem_Backend_t * backend, void * data) {
    TOPAZDUKBackend * b = data;
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(b->instances); ++i) {
        TOPAZDUK * ctx = topaz_array_at(b->instances, TOPAZDUK *, i);
        if (ctx->isDebugging) {
            duk_debugger_cooperate(ctx->js);
            while(topaz_array_get_size(ctx->pendingMessages))
                duk_debugger_cooperate(ctx->js);

            topaz_duk_push_notify(ctx);
        }
    }    
}


void topaz_system_script_duktapeJS__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazScriptAPI_t      *  api
) {
    TOPAZDUKBackend * b = calloc(1, sizeof(TOPAZDUKBackend));
    b->instances = topaz_array_create(sizeof(TOPAZDUK*));
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("DuktapeJS"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("https://duktape.org/"),

        // desc 
        TOPAZ_STR_CAST("Bindings to use duktape!"),



        // on step 
        NULL,
        
        // on step late 
        topaz_duk_cooperate_update,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        b,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );


    api->objectAPI.object_reference_create = topaz_duk_object_reference_create;
    api->objectAPI.object_reference_create_from_reference = topaz_duk_object_reference_create_from_reference;
    api->objectAPI.object_reference_destroy = topaz_duk_object_reference_destroy;
    api->objectAPI.object_reference_get_feature_mask = topaz_duk_object_reference_get_feature_mask;
    api->objectAPI.object_reference_get_native_data = topaz_duk_object_reference_get_native_data;
    api->objectAPI.object_reference_set_native_data = topaz_duk_object_reference_set_native_data;
    api->objectAPI.object_reference_ref = topaz_duk_object_reference_ref;
    api->objectAPI.object_reference_unref = topaz_duk_object_reference_unref;
    api->objectAPI.object_reference_call = topaz_duk_object_reference_call;
    api->objectAPI.object_reference_array_get_nth = topaz_duk_object_reference_array_get_nth;
    api->objectAPI.object_reference_array_get_count = topaz_duk_object_reference_array_get_count;
    api->objectAPI.object_reference_map_get_property = topaz_duk_object_reference_map_get_property;
    api->objectAPI.object_reference_to_string = topaz_duk_object_reference_to_string;
    api->objectAPI.object_reference_extendable_add_property = topaz_duk_object_reference_extendable_add_property;

    api->script_create = topaz_duk_create;
    api->script_destroy = topaz_duk_destroy;
    api->script_map_native_function = topaz_duk_map_native_function;
    api->script_run = topaz_duk_run;
    api->script_expression = topaz_duk_expression;
    api->script_create_empty_object = topaz_duk_create_empty_object;
    api->script_throw_error = topaz_duk_throw_error;
    api->script_bootstrap = topaz_duk_bootstrap;


    api->script_debug_start = topaz_duk_debug_start;
    api->script_debug_send_command = topaz_duk_debug_send_command;
    api->script_debug_get_state = topaz_duk_debug_get_state;
}





