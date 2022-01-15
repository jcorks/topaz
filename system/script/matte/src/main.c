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
#include <topaz/modules/resources.h>
#include <topaz/topaz.h>
#include <topaz/asset.h>
#include <topaz/assets/data.h>


#include <stdio.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif
#include "./matte/src/matte.h"
#include "./matte/src/matte_heap.h"
#include "./matte/src/matte_vm.h"
#include "./matte/src/matte_bytecode_stub.h"
#include "./matte/src/matte_string.h"
#include "./matte/src/matte_array.h"
#include "./matte/src/matte_compiler.h"

#include <stdlib.h>
#include <string.h>

#define PRESET_ARGS 5


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
    // all TOPAZMATTE * instances active.
    topazArray_t * instances;

} TOPAZMATTEBackend;


typedef struct {
    matte_t * matte;
    matteVM_t * vm;
    matteHeap_t * heap;
    topazScript_t * script;
    // used for object reference creation
    int lastObjectReference;
    topaz_t * ctx;
    topazArray_t * nativeFuncs;

    matteValue_t argNamesRaw[PRESET_ARGS];
    matteValue_t argsRaw[PRESET_ARGS];

    //// debugging

    // whether debugging has started
    int isDebugging;

    // waiting for a pause request to be processed.
    int paused;

    int pendingCommand;
    int pauseUp;
    int init;
    int lastLine;
    uint32_t lastFile;
    int lastStacksize;



    // the level of stack for the current debug context
    int debugLevel;

    // state of the defbug
    topazScript_DebugState_t debugState;

    // queue of breakpoints requested
    // Maps the index to the matte id
    topazArray_t * debugBreakpoints;

    // queue of breakpoints requested to be removed.
    topazArray_t * debugRemoveBreakpoint;

    // queue of notifications to send to the script instance.
    // theyre queued until ALL intermediate steps are finished, allowing 
    // the script context to rely on the debug state properly. 
    topazArray_t * debugQueuedNotifications;

    int debugOneOff;
} TOPAZMATTE;

typedef struct {
    TOPAZMATTE * ctx;
    void * userData;
    topaz_script_native_function userFunction;
    uint8_t argCount;
} TOPAZMATTENativeFunction;


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
    TOPAZMATTE * ctx;

    // populated if the object was created from C
    topaz_script_native_function cfinalizer;

    void * cfinalizerData;

    // used for finding the tag in the global stash
    matteValue_t selfID;
} TOPAZMATTEObjectTag;



// when notifications are prepared, they are stored intermediately 
// in in this.
typedef struct {
    // relecant command
    int command;

    // the result to deliver to the script instance
    topazString_t * result;

} DebugNotification;



static void clear_debug_state(topazScript_DebugState_t * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->callstack);
    for(i = 0; i < len; ++i) {
        topazScript_CallstackFrame_t * frame = &topaz_array_at(s->callstack, topazScript_CallstackFrame_t, i);

        topaz_string_destroy((topazString_t*)frame->filename);
        topaz_string_destroy((topazString_t*)frame->functionName);
        topaz_string_destroy((topazString_t*)frame->entityName);
        frame->filename = NULL;
        frame->functionName = NULL;
        frame->entityName = NULL;
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

static void debug_populate_state(TOPAZMATTE * ctx) {
    matteVM_t *vm = ctx->vm;
    topazScript_DebugState_t * s = &ctx->debugState;
    
    
    clear_debug_state(s);
    uint32_t i;
    uint32_t len = matte_vm_get_stackframe_size(vm);
    for(i = 0; i < len; ++i) {
        uint32_t ct;
        matteVMStackFrame_t frame = matte_vm_get_stackframe(vm, i);
        matteBytecodeStubInstruction_t inst = matte_bytecode_stub_get_instructions(frame.stub, &ct)[frame.pc];
        debug_state_add_frame(
            s,
            inst.lineNumber,
            matte_string_get_c_str(
                matte_vm_get_script_name_by_id(
                    vm, 
                    matte_bytecode_stub_get_file_id(
                        frame.stub
                    )
                )
            ),

            matte_string_get_c_str(frame.prettyName)
        );
        if (i == 0) {
            ctx->lastLine = inst.lineNumber;
            ctx->lastFile = matte_bytecode_stub_get_file_id(
                frame.stub
            );
            ctx->lastStacksize = len;
        }

    }
}

static topazString_t * topaz_matte_stack_where(TOPAZMATTE * ctx) {
    topazString_t * str = topaz_string_create();
    uint32_t i;
    uint32_t len = matte_vm_get_stackframe_size(ctx->vm);
    for(i = 0; i < len; ++i) {
        matteVMStackFrame_t frame = matte_vm_get_stackframe(ctx->vm, i);
        uint32_t numinst;
        const matteBytecodeStubInstruction_t * inst = matte_bytecode_stub_get_instructions(frame.stub, &numinst);

        uint32_t fileid = matte_bytecode_stub_get_file_id(frame.stub);
        const matteString_t * fileName = matte_vm_get_script_name_by_id(ctx->vm, fileid);

        if (i == ctx->debugLevel) {
            topaz_string_concat_printf(str, " -> @%d:%s <%s>, line %d\n", i, matte_string_get_c_str(frame.prettyName), fileName ? matte_string_get_c_str(fileName) : "???", inst[frame.pc].lineNumber);
        } else {
            topaz_string_concat_printf(str, "    @%d:%s <%s>, line %d\n", i, matte_string_get_c_str(frame.prettyName), fileName ? matte_string_get_c_str(fileName) : "???", inst[frame.pc].lineNumber);
        }
    }
    return str;
}


// Gets the object tag from the top object
static TOPAZMATTEObjectTag * topaz_matte_object_get_tag_from_value(TOPAZMATTE * ctx, matteValue_t val) {

    // uhh?? unsafe if someone puts a ___tz prop by hand in a js context.
    // think of a different way before release, thanks
    return matte_value_object_get_userdata(ctx->heap, val);
}


// assigns the pointer to the tag as a read-only property string 
// for the object itself.
static TOPAZMATTEObjectTag * topaz_matte_object_set_tag(TOPAZMATTE * ctx, matteValue_t val) {

    TOPAZMATTEObjectTag * out = calloc(1, sizeof(TOPAZMATTEObjectTag));
    out->selfID = val;
    matte_value_object_set_userdata(ctx->heap, val, out);



    return out;
}

static void topaz_matte_object_finalizer(void * objectUserdata, void * functionUserdata) {
    TOPAZMATTEObjectTag * tag = objectUserdata;

    #ifdef TOPAZDC_DEBUG
        if (!tag) {
            printf("SOMETHIGN HAPPENED. TAG WAS NULL..\n");
            return;
        }
    #endif

    // if this is even happening, the global stash entry was already removed.

    // call c finalizer, which is why this is here mostly
    if (tag->cfinalizer)
        tag->cfinalizer(
                tag->ctx->script, 
                NULL, 
                tag->cfinalizerData
        );
    free(tag);
}



static matteValue_t topaz_matte_object_value_from_tag(TOPAZMATTEObjectTag * tag) {
    return tag->selfID;
}





// fetches/creates an ObjectTag for a given object
static void * topaz_matte_object_wrap(void * ctxSrc, matteValue_t val) {
    TOPAZMATTE * ctx = ctxSrc;
    TOPAZMATTEObjectTag * tag = topaz_matte_object_get_tag_from_value(ctx, val);

    if (!tag) {
        // create a new tag and bind that tag to this object using a hidden prop
        tag = topaz_matte_object_set_tag(ctx, val);   
        tag->ctx = ctx;

 

        // Sets the standard finalizer function.
        matte_value_object_set_native_finalizer(
            ctx->heap,
            val,
            topaz_matte_object_finalizer,
            NULL
        );
    }

    if (tag->refCount == 0) { 
        // make sure that a reference to this object exists outside of the pure 
        // js context. This allows the C context to keep the object around.
        // in the case that the C tag has persisted when vulnerable to garbage collection,
        // the ref count might be 0 despite the tag existing before. THats why this check 
        // is for tag->ref == 0 rather than just creation

        matte_value_object_push_lock(ctx->heap, val);
    }


    return tag;
} 



// creates a new topaz script object from a value on the duk stack
static topazScript_Object_t * topaz_matte_value_to_tso(TOPAZMATTE * ctx, matteValue_t val) {    
    topazScript_Object_t * o = NULL;
    switch(val.binID) {
      case MATTE_VALUE_TYPE_BOOLEAN:
        o = topaz_script_object_from_int(ctx->script, val.value.boolean);
        break;
      case MATTE_VALUE_TYPE_NUMBER:    
        o = topaz_script_object_from_number(ctx->script, val.value.number);
        break;

      case MATTE_VALUE_TYPE_STRING:
        o = topaz_script_object_from_string(ctx->script, TOPAZ_STR_CAST(matte_string_get_c_str(matte_value_string_get_string_unsafe(matte_vm_get_heap(matte_get_vm(ctx->matte)), val))));
        break;

      case MATTE_VALUE_TYPE_OBJECT:
      case MATTE_VALUE_TYPE_FUNCTION: {
        o = topaz_script_object_wrapper(ctx->script, topaz_matte_object_wrap(ctx, val));        
        break;
      }

      default:;
        o = topaz_script_object_undefined(ctx->script);
        break;
    }

    return o;
}


// pushes the given object as a duktape object
// on the top of the stack.
static matteValue_t topaz_matte_tso_to_value(TOPAZMATTE * ctx, topazScript_Object_t * o) {
    matteValue_t out = matte_heap_new_value(ctx->heap);
    switch(topaz_script_object_get_type(o)) {

      case topazScript_Object_Type_Integer:
        matte_value_into_number(ctx->heap, &out, topaz_script_object_as_int(o));
        break;

      case topazScript_Object_Type_Number:
        matte_value_into_number(ctx->heap, &out, topaz_script_object_as_number(o));
        break;

      case topazScript_Object_Type_String: {
        matteString_t * str = matte_string_create_from_c_str(topaz_string_get_c_str(topaz_script_object_as_string(o)));
        matte_value_into_string(ctx->heap, &out, str);
        matte_string_destroy(str);
        break;
      }
      case topazScript_Object_Type_Reference: {
        TOPAZMATTEObjectTag * tag = topaz_script_object_get_api_data(o);
        matte_value_into_copy(ctx->heap, &out, tag->selfID);
        break;
      }
      default:;
    }
    return out;
}
/*
static duk_ret_t topaz_matte_get_internal(duk_context * js) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(js);
    #endif

    // retrieve tag and actual getter
    duk_push_current_function(js);
    TOPAZMATTEObjectTag * tag = topaz_matte_get_private_prop(js, "___tz");
    topaz_script_native_function getReal = (topaz_script_native_function)topaz_matte_get_private_prop(js, "___tzfn");
    duk_pop(js);

    // We need a representative object for the 'this' to which the 
    // getter belongs
    topaz_matte_object_push_to_top_from_tag(tag);
    topazScript_Object_t * self = topaz_matte_value_to_tso(tag->ctx, -1);
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
        topaz_matte_object_push_tso(tag->ctx, res);
    } else {
        duk_push_undefined(tag->ctx->js);
    }

    topaz_script_object_destroy(self);
    topaz_script_object_destroy(res);
    return 1;
}

static duk_ret_t topaz_matte_set_internal(duk_context * js) {
    // we assume that get_top == 2 since its a setter
    duk_push_current_function(js);
    TOPAZMATTEObjectTag * tag = topaz_matte_get_private_prop(js, "___tz");
    topaz_script_native_function setReal = (topaz_script_native_function)topaz_matte_get_private_prop(js, "___tzfn");
    duk_pop(js);

    topazScript_Object_t * args[2];


    // We need a representative object for the 'this' to which the 
    // getter belongs
    topaz_matte_object_push_to_top_from_tag(tag);
    args[0] = topaz_matte_value_to_tso(tag->ctx, -1);
    duk_pop(js);

    // arg 0 is always the value in question
    args[1] = topaz_matte_value_to_tso(tag->ctx, 0);


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




*/
static matteValue_t topaz_matte_native_function_internal(matteVM_t * vm, matteValue_t fn, const matteValue_t * args, void * userData) {
    TOPAZMATTENativeFunction * src = userData;
    TOPAZMATTE * ctx = src->ctx;    
    topaz_script_native_function fnReal = src->userFunction;
    void * fnData = src->userData;


    // convert all args to tso
    uint32_t i;
    topazScript_Object_t * argsD[src->argCount];
    for(i = 0; i < src->argCount; ++i) {
        
        argsD[i] = topaz_matte_value_to_tso(ctx, args[i]);
    }

    // actually call the native fn
    topazScript_Object_t * res = fnReal(
        ctx->script,
        argsD,
        fnData
    );

    matteValue_t out = matte_heap_new_value(ctx->heap);
    // finally, push the tso result as a duk object 
    if (res) {
        out = topaz_matte_tso_to_value(ctx, res);
        topaz_script_object_destroy(res);
    }
    
    for(i = 0; i < src->argCount; ++i) {
        topaz_script_object_destroy(argsD[i]);
    }
    return out;
}




static void topaz_matte_fatal(matteVM_t * vm, uint32_t file, int lineNumber, matteValue_t value, void * udata) {
    TOPAZMATTE * ctx = udata;
    matteHeap_t * heap = matte_vm_get_heap(vm);
    {
        const matteString_t * rep = matte_value_string_get_string_unsafe(heap, matte_value_object_access_string(heap, value, MATTE_VM_STR_CAST(vm, "summary")));

        PERROR(ctx->ctx, topaz_string_create_from_c_str("Topaz Scripting error: (%s, line %d):\n%s\n%s", 
            matte_string_get_c_str(matte_vm_get_script_name_by_id(vm, file)),
            lineNumber,
            matte_string_get_c_str(rep)));
    }
    topazString_t * str = topaz_matte_stack_where(ctx);
    if (topaz_string_get_length(str)) {
        PERROR(ctx->ctx, str);
    } else {
        topaz_string_destroy(str);
    }
    
}







static uint8_t * topaz_matte_import(
    matteVM_t * vm,
    const matteString_t * importPath,
    uint32_t * fileid,
    uint32_t * dataLength,
    void * usrdata
) {
    TOPAZMATTE * ctx = usrdata;
    topaz_t * t = ctx->ctx;
    
    topazResources_t * res = topaz_context_get_resources(t);
    topazAsset_t * asset = topaz_resources_fetch_asset(
        res,
        topazAsset_Type_Data,
        TOPAZ_STR_CAST(matte_string_get_c_str(importPath))
    );
    if (!asset) {
        PERROR(ctx->ctx, topaz_string_create_from_c_str("Could not find source asset %s", matte_string_get_c_str(importPath)));        
        *dataLength = 0;
        return NULL;
    }
    const topazArray_t * data = topaz_data_get_as_bytes(asset);
    uint8_t * dataBytes = malloc(topaz_array_get_size(data));
    memcpy(dataBytes, topaz_array_get_data(data), topaz_array_get_size(data));
    *dataLength = topaz_array_get_size(data);
    *fileid = matte_vm_get_new_file_id(vm, importPath);
    topazString_t * srcstr = topaz_data_get_as_string(asset);
    topaz_script_register_source(ctx->script, TOPAZ_STR_CAST(matte_string_get_c_str(importPath)), srcstr);
    matte_string_destroy(srcstr);
    return dataBytes;
    
    /*
    uint32_t * id = matte_table_find(vm->defaultImport_table, importPath);
    if (id) {
        *dataLength = 0;
        *fileid = *id;
        return NULL;
    }


    FILE * f = fopen(matte_string_get_c_str(importPath), "rb");
    if (!f) {
        *dataLength = 0;
        return NULL;
    }

    #define DEFAULT_DUMP_SIZE 1024
    char * msg = malloc(DEFAULT_DUMP_SIZE);
    uint32_t totalLen = 0;
    uint32_t len;
    while(len = fread(msg, 1, DEFAULT_DUMP_SIZE, f)) {
        totalLen += len;
    }
    fseek(f, SEEK_SET, 0);
    uint8_t * outBuffer = malloc(totalLen);
    uint8_t * iter = outBuffer;
    while(len = fread(msg, 1, DEFAULT_DUMP_SIZE, f)) {
        memcpy(iter, msg, len);
        iter += len;
    }
    free(msg);
    fclose(f);
    id = malloc(sizeof(uint32_t));
    *id = matte_vm_get_new_file_id(vm, importPath);
    matte_table_insert(vm->defaultImport_table, importPath, id);

    *fileid = *id;
    *dataLength = totalLen;
    return outBuffer;
    */
}








static void * topaz_matte_create(topazScript_t * scr, topaz_t * ctx) {
    TOPAZMATTE * out = calloc(1, sizeof(TOPAZMATTE));
    out->debugState.callstack = topaz_array_create(sizeof(topazScript_CallstackFrame_t));
    out->ctx = ctx;
    out->debugBreakpoints = topaz_array_create(sizeof(int));
    out->debugRemoveBreakpoint = topaz_array_create(sizeof(int));
    out->debugQueuedNotifications = topaz_array_create(sizeof(DebugNotification));
    out->pendingCommand = -1;

    out->matte = matte_create();
    out->vm = matte_get_vm(out->matte);
    out->heap = matte_vm_get_heap(out->vm);
    out->nativeFuncs = topaz_array_create(sizeof(void*));
    matte_vm_set_unhandled_callback(
        matte_get_vm(out->matte),
        topaz_matte_fatal,
        out
    );
    matte_vm_set_import(
        out->vm,
        topaz_matte_import,
        out
    );
    out->script = scr;

    TOPAZMATTEBackend * backend = topaz_system_backend_get_user_data(topaz_script_get_backend(scr));
    topaz_array_push(backend->instances, out);

    matteValue_t t = matte_heap_new_value(out->heap);
    matte_value_into_string(out->heap, &t, MATTE_VM_STR_CAST(out->vm, "a")); out->argNamesRaw[0] = t;
    matte_value_into_string(out->heap, &t, MATTE_VM_STR_CAST(out->vm, "b")); out->argNamesRaw[1] = t;
    matte_value_into_string(out->heap, &t, MATTE_VM_STR_CAST(out->vm, "c")); out->argNamesRaw[2] = t;
    matte_value_into_string(out->heap, &t, MATTE_VM_STR_CAST(out->vm, "d")); out->argNamesRaw[3] = t;
    matte_value_into_string(out->heap, &t, MATTE_VM_STR_CAST(out->vm, "e")); out->argNamesRaw[4] = t;


    return out;
} 


static void topaz_matte_destroy(topazScript_t * scr, void * data) {
    TOPAZMATTEBackend * backend = topaz_system_backend_get_user_data(topaz_script_get_backend(scr));
    uint32_t i;
    uint32_t len = topaz_array_get_size(backend->instances);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(backend->instances, TOPAZMATTE*, i) == data) {
            topaz_array_remove(backend->instances, i);
            break;
        }
    }


    TOPAZMATTE * ctx = data;
    // TODO: destroy ALL objects??
    matte_destroy(ctx->matte);
}


static int topaz_matte_map_native_function(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * pname, 
    uint8_t argCount,
    topaz_script_native_function fn, 
    void * userData

) {
    TOPAZMATTE * ctx = data;
    matteString_t * name = matte_string_create_from_c_str(topaz_string_get_c_str(pname));

    TOPAZMATTENativeFunction * native = calloc(1, sizeof(TOPAZMATTENativeFunction));
    native->ctx = ctx;
    native->userFunction = fn;
    native->userData = userData;
    native->argCount = argCount;

    topaz_array_push(ctx->nativeFuncs, native);

    matte_vm_set_external_function_autoname(
        matte_get_vm(ctx->matte),
        name,
        argCount,
        topaz_matte_native_function_internal,
        native
    );
    return 1;
}
static void topaz_matte_run__error(const matteString_t * s, uint32_t line, uint32_t ch, void * userdata) {
    TOPAZMATTE * ctx = userdata;
    topazString_t * str = topaz_string_create();
    topaz_string_concat_printf(str, "ERROR @ line %d, %d:\n%s\n", line, ch, matte_string_get_c_str(s));
    topaz_string_concat(str, topaz_matte_stack_where(ctx));
        matteString_t * errm = matte_string_create_from_c_str("%s", topaz_string_get_c_str(str));
        matte_vm_raise_error_string(ctx->vm, errm);
        matte_string_destroy(errm);
        PERROR(ctx->ctx, str);
    
}

static void topaz_matte_expression__error(matteVM_t * vm, matteVMDebugEvent_t event, uint32_t file, int lineNumber, matteValue_t value, void * userdata) {
    if (event == MATTE_VM_DEBUG_EVENT__ERROR_RAISED) {
        TOPAZMATTE * ctx = userdata;    
        topazString_t * str = topaz_string_create();
        topaz_string_concat_printf(str, "Error in expression: %s\n", matte_string_get_c_str(matte_value_string_get_string_unsafe(ctx->heap, matte_value_as_string(ctx->heap, value))));
        topaz_string_concat(str, topaz_matte_stack_where(ctx));
        if (topaz_string_get_length(str)) {
            PERROR(ctx->ctx, str);
        }
    }   
}
static topazScript_Object_t * topaz_matte_expression(
    topazScript_t * script, 
    void * data,
    const topazString_t * expr
) {

    TOPAZMATTE * ctx = data;

    if (!matte_vm_get_stackframe_size(ctx->vm)) {
        topazString_t * str = topaz_string_create_from_c_str("Eval error: Cannot evaluate expression if there is no callstack currently active.");
        PERROR(ctx->ctx, str);
        return topaz_script_object_undefined(ctx->script);   
    }
    matteString_t * exprM = matte_string_create_from_c_str(topaz_string_get_c_str(expr));
    topazScript_Object_t * out = topaz_matte_value_to_tso(
        ctx,
        matte_vm_run_scoped_debug_source(
            ctx->vm,
            exprM,
            0,
            topaz_matte_expression__error,
            ctx
        )
    );

    matte_string_destroy(exprM);
    return out;
}

static void topaz_matte_throw_error(
    topazScript_t * script, 
    void * data,
    const topazArray_t * args
) {
    TOPAZMATTE * ctx = data;
    matteString_t * str = matte_string_create();
    matte_string_concat_printf(
        str,
        "%s",
        topaz_array_get_size(args) ?
            topaz_string_get_c_str(topaz_script_object_as_string(topaz_array_at(args, topazScript_Object_t *, 0)))
        :
            "Internal native error."
    );
    matte_vm_raise_error_string(ctx->vm, str);
    matte_string_destroy(str);
}


static void topaz_matte_run__first_time(
    topazScript_t * script
) {
    #include "debug_bytes"

    topaz_script_run(
        script,
        TOPAZ_STR_CAST("Topaz.DebugPrinter"),
        TOPAZ_STR_CAST((char*)debug_bytes)
    );

    #include "constants_bytes"

    topaz_script_run(
        script,
        TOPAZ_STR_CAST("Topaz.Constants"),
        TOPAZ_STR_CAST((char*)constants_bytes)
    );
    #include "vector_bytes"

    topaz_script_run(
        script,
        TOPAZ_STR_CAST("Topaz.Vector"),
        TOPAZ_STR_CAST((char*)vector_bytes)
    );
    #include "color_bytes"

    topaz_script_run(
        script,
        TOPAZ_STR_CAST("Topaz.Color"),
        TOPAZ_STR_CAST((char*)color_bytes)
    );

    #include "bootstrap_bytes"

    topaz_script_run(
        script,
        TOPAZ_STR_CAST("Topaz.Core"),

        TOPAZ_STR_CAST((char*)bootstrap_bytes)
    );

}

static void topaz_matte_run(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * sourceName, 
    const topazString_t * sourceDataD) {

    TOPAZMATTE * ctx = data;
    
    if (!ctx->init) {
        ctx->init = 1;
        topaz_matte_run__first_time(script);
    }
    

    uint32_t bytelen = 0;
    uint8_t * bytecode = matte_compiler_run(
        (const uint8_t*)topaz_string_get_c_str(sourceDataD),
        topaz_string_get_length(sourceDataD),
        &bytelen,
        topaz_matte_run__error,
        ctx
    );


    if (!bytecode || !bytelen) {
        topazString_t * str = topaz_string_create_from_c_str("Could not compile source \"%s\"", topaz_string_get_c_str(sourceName));
        PERROR(ctx->ctx, str);
        return;
    }
    
    matteString_t * name = matte_string_create_from_c_str("%s", topaz_string_get_c_str(sourceName));
    uint32_t id = matte_vm_get_new_file_id(ctx->vm, name);

    matteArray_t * stubs = matte_bytecode_stubs_from_bytecode(
        ctx->heap,
        id,
        bytecode, bytelen
    );


    matte_vm_add_stubs(ctx->vm, stubs);
    matte_heap_recycle(
        ctx->heap,
        matte_vm_run_fileid(
            ctx->vm,
            id,
            matte_heap_new_value(ctx->heap)
        )
    );
}


topazScript_Object_t * topaz_matte_create_empty_object(
    topazScript_t * script,
    void * data,

    topaz_script_native_function cleanup,
    void * cleanupData

) {
    TOPAZMATTE * ctx = data;
    matteValue_t outp = matte_heap_new_value(ctx->heap);
    matte_value_into_new_object_ref(ctx->heap, &outp);

    // create raw tso
    topazScript_Object_t * out = topaz_matte_value_to_tso(ctx, outp);

    // get the tag so we can add the finalizer
    TOPAZMATTEObjectTag * tag = topaz_matte_object_get_tag_from_value(ctx, outp);
    tag->cfinalizer = cleanup;
    tag->cfinalizerData = cleanupData;

    return out;
}


void topaz_matte_bootstrap(topazScript_t * script, void * n) {

}
























 
 
static void * topaz_matte_object_reference_create_from_reference(topazScript_Object_t * o, void * ctxSrc, topazScript_Object_t * from, void * fromData) {
    TOPAZMATTE * ctx = ctxSrc;
    matteValue_t val = topaz_matte_tso_to_value(ctx, from);
    return topaz_matte_object_wrap(ctx, val);
}




static void topaz_matte_object_reference_destroy(topazScript_Object_t * o, void * tagSrc) {
    // the finalizer handlers destruction for us, 
    // so there isnt much to do here.
    // (topaz will unref for us)
}



static int topaz_matte_object_reference_get_feature_mask(topazScript_Object_t * o, void * tagSrc) {
    TOPAZMATTEObjectTag * t = tagSrc;
    int out = 0;
    if (t->selfID.binID == MATTE_VALUE_TYPE_FUNCTION) {
        out |= topazScript_Object_Feature_Callable;
    } else if (t->selfID.binID == MATTE_VALUE_TYPE_OBJECT) {
        out |= topazScript_Object_Feature_Map;
        out |= topazScript_Object_Feature_Array;
    }
    return out;
}

static void * topaz_matte_object_reference_get_native_data(topazScript_Object_t * o, int * tagID, void * tagSrc) {
    TOPAZMATTEObjectTag * t = tagSrc;
    *tagID = t->tagID;
    return t->nativeData;
}

static void topaz_matte_object_reference_set_native_data(topazScript_Object_t * o, void * nativeData, int tagID, void * tagSrc) {
    TOPAZMATTEObjectTag * t = tagSrc;
    t->tagID = tagID;
    t->nativeData = nativeData;
}



static void topaz_matte_object_reference_ref(topazScript_Object_t * o, void * data) {
    TOPAZMATTEObjectTag * tag = data;
    // when 0, will unkeep
    tag->refCount++;
}

static void topaz_matte_object_reference_unref(topazScript_Object_t * o, void * data) {
    TOPAZMATTEObjectTag * tag = data;
    // when 0, will unkeep
    tag->refCount--;

    if (tag->refCount == 0) {
        matte_value_object_pop_lock(tag->ctx->heap, topaz_matte_object_value_from_tag(tag));
    }

}




static topazScript_Object_t * topaz_matte_object_reference_call(topazScript_Object_t * o, const topazArray_t * argsIn, void * data) {
    TOPAZMATTEObjectTag * tag = data;
    TOPAZMATTE * ctx = tag->ctx;

    // assumes callable
    matteValue_t v = topaz_matte_object_value_from_tag(tag);



    uint32_t i = 0;
    uint32_t len = topaz_array_get_size(argsIn);
    for(; i < len; ++i) {
        topazScript_Object_t * o = topaz_array_at(argsIn, topazScript_Object_t *, i);
        ctx->argsRaw[i] = topaz_matte_tso_to_value(
            tag->ctx, 
            o
        );
    }
    matteArray_t argNames = MATTE_ARRAY_CAST(ctx->argNamesRaw, sizeof(matteValue_t), len);
    matteArray_t args = MATTE_ARRAY_CAST(ctx->argsRaw, sizeof(matteValue_t), len);

    topazScript_Object_t * out = topaz_matte_value_to_tso(
        tag->ctx,
        matte_vm_call(
            ctx->vm,
            v,
            &args,
            &argNames,
            MATTE_VM_STR_CAST(ctx->vm, "<from native>")
        )
    );
 
    return out;
}


static topazScript_Object_t * topaz_matte_object_reference_array_get_nth(
    topazScript_Object_t * oSrc, 
    int index,  
    void * tagSrc
) {

    TOPAZMATTEObjectTag * tag = tagSrc;
    topazScript_Object_t * o = topaz_matte_value_to_tso(
        tag->ctx,
        matte_value_object_access_index(
            tag->ctx->heap,
            tag->selfID,
            index
        )
    );
    return o;
}


static int topaz_matte_object_reference_array_get_count(topazScript_Object_t * o, void * tagSrc) {
    TOPAZMATTEObjectTag * tag = tagSrc;
    int len = matte_value_object_get_key_count(tag->ctx->heap, tag->selfID);

    return len;
}

static topazScript_Object_t * topaz_matte_object_reference_map_get_property(
    topazScript_Object_t * o, 
    const topazString_t * prop, 
    void * tagSrc
) {
    TOPAZMATTEObjectTag * tag = tagSrc;
    matteString_t * key = matte_string_create_from_c_str("%s", topaz_string_get_c_str(prop));
    topazScript_Object_t * out = topaz_matte_value_to_tso(tag->ctx, matte_value_object_access_string(tag->ctx->heap, tag->selfID, key));
    matte_string_destroy(key);
    return out;
}

void topaz_matte_object_reference_to_string(
    topazScript_Object_t * o, 
    topazString_t * str,
    void * tagSrc) {
    
    TOPAZMATTEObjectTag * tag = tagSrc;
    matteValue_t strc = matte_value_as_string(tag->ctx->heap, tag->selfID);
    topaz_string_concat_printf(
        str,
        matte_string_get_c_str(
            matte_value_string_get_string_unsafe(
                tag->ctx->heap,
                strc
            )
        )
    );

}


// SUPPORT TRANS WRITES
// 
// NOTE: this was more clever when writing duktape JS bindings because of the 
// nomenclature of duktape debugging code...




// commands

#define DUKTAPE_COMMAND__GET_CALLSTACK 256

static void topaz_matte_trans_command__pause(TOPAZMATTE * ctx) {
    ctx->pendingCommand = topazScript_DebugCommand_Pause;
}



static void topaz_matte_trans_command__resume(TOPAZMATTE * ctx) {
    ctx->pendingCommand = topazScript_DebugCommand_Resume;
}

static void topaz_matte_trans_command__step_into(TOPAZMATTE * ctx) {

    ctx->pendingCommand = topazScript_DebugCommand_StepInto;                 
}


static void topaz_matte_trans_command__step_over(TOPAZMATTE * ctx) {

    ctx->pendingCommand = topazScript_DebugCommand_StepOver;                 
}







/*
static void topaz_matte_trans_cooperate(duk_trans_dvalue_ctx * ctxT, int block) {
    TOPAZMATTE * ctx = ctxT->userData;
    if (!ctx->debugReachedInit) {
        #ifdef TOPAZDC_DEBUG
            printf("Init response\n");
        #endif
        topaz_matte_trans_command__resume(ctx->trans_ctx);
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
                topaz_matte_trans_command__get_call_stack(ctx->trans_ctx);
                if (!strcmp(messages[2], "1")) { // paused state
                    ctx->debugLevel = 0;
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_Pause,
                        topaz_string_create()
                    );
                } else if (!strcmp(messages[2], "0")) { // resume
                    ctx->debugLevel = 0;
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_Resume,
                        topaz_string_create()
                    );
                } 
            } else if (!strcmp(messages[1], "5")) { // error?
                topaz_matte_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_Pause,
                    topaz_string_create_from_c_str("[ERROR DETECTED]: %s", messages[3])
                );
            }
        } else {

            switch((int)command) {
              case topazScript_DebugCommand_AddBreakpoint: {
                if (!strcmp(messages[0], "ERR")) {
                    // error
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_AddBreakpoint,
                        topaz_string_create()
                    );
                } else {
                    static int localID = 1000;
                    topaz_array_push(ctx->debugBreakpoints, localID);

                    // success
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_AddBreakpoint,
                        topaz_string_create_from_c_str("%d", localID++)
                    );
                }
                break;
              }
              case topazScript_DebugCommand_Pause:
                ctx->debugLevel = 0;
                topaz_matte_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_Pause,
                    topaz_string_create()
                );
                break;
                
              case topazScript_DebugCommand_Resume:
                ctx->debugLevel = 0;
                topaz_matte_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_Resume,
                    topaz_string_create()
                );

                break;
                
              case topazScript_DebugCommand_ScopedEval:

                if (!strcmp(messages[1], "0")) {
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_ScopedEval,
                        topaz_string_create_from_c_str("%s", messages[2])
                    );
                } else {
                    // Error
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_ScopedEval,
                        topaz_string_create_from_c_str("Could not evaluate expression (%s).\n", messages[2])
                    );                    
                }
                break;
              case topazScript_DebugCommand_StepInto:
                topaz_matte_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_StepInto,
                    topaz_string_create()
                );                    
                break;
              case topazScript_DebugCommand_StepOver:
                ctx->debugLevel = 0;
                topaz_matte_debug_queue_notify(
                    ctx,
                    topazScript_DebugCommand_StepOver,
                    topaz_string_create()
                );                    
                break;
                

                
              case topazScript_DebugCommand_RemoveBreakpoint:{
                int id = topaz_array_at(ctx->debugRemoveBreakpoint, int, 0);
                topaz_array_remove(ctx->debugRemoveBreakpoint, 0);

                if (!strcmp(messages[0], "ERR")) {
                    topaz_matte_debug_queue_notify(
                        ctx,
                        topazScript_DebugCommand_RemoveBreakpoint,
                        topaz_string_create()
                    );                    
                } else {
                    topaz_matte_debug_queue_notify(
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
                //topaz_matte_trans_command__get_call_stack(ctx);
                //printf("Received %d messages.\n", messagesLen);
                fflush(stdout);
            }
            
        }
        // clear strings
        int i;
        for(i = 0; i < messagesLen; ++i) {
            free(messages[i]);
        }
        topaz_array_destroy(messagesSrc);
    
    }
    if (!block) return; // still receiving messages
    topaz_context_pause(ctx->ctx);
    topaz_context_iterate(ctx->ctx);
    topaz_context_wait(ctx->ctx, 60);
}
*/


static void topaz_matte_debug_trap(TOPAZMATTE * ctx) {
    ctx->debugLevel = 0;
    debug_populate_state(ctx);
    while(ctx->pendingCommand == topazScript_DebugCommand_Pause) {
        topaz_context_pause(ctx->ctx);
        topaz_context_iterate(ctx->ctx);
        topaz_context_wait(ctx->ctx, 60);
    }

}
static void topaz_matte_debug_callback(
    matteVM_t * vm, 
    matteVMDebugEvent_t event, 
    uint32_t file, 
    int lineNumber, 
    matteValue_t value, 
    void * data
) {
    int pendingError = 0;
    TOPAZMATTE * ctx = data;
    if (event == MATTE_VM_DEBUG_EVENT__ERROR_RAISED) {
        // forward a formal pause command and continue
        topaz_script_debug_send_command(
            ctx->script, 
            topazScript_DebugCommand_Pause,            
            topaz_string_create()            
        );
        pendingError = 1;
    }
    
    switch(ctx->pendingCommand) {
      case topazScript_DebugCommand_Pause:
        ctx->debugLevel = 0;
        debug_populate_state(ctx);


        if (ctx->pauseUp || pendingError) {
            topazString_t * err = topaz_string_create_from_c_str("%dL", ctx->pauseUp ? 1 : 0);
            
            if (pendingError) {
                matteHeap_t * heap = ctx->heap;
                const matteString_t * rep = matte_value_string_get_string_unsafe(heap, value);            
                topaz_string_concat_printf(err, "Script runtime error: %s", matte_string_get_c_str(rep));
            }
            topaz_script_notify_command(
                ctx->script, 
                topazScript_DebugCommand_Pause,            
                err
            );
        
        }
        ctx->pauseUp = 0;
        topaz_matte_debug_trap(ctx);


        break;
      case topazScript_DebugCommand_Resume:
        topaz_script_notify_command(
            ctx->script, 
            topazScript_DebugCommand_Resume,            
            topaz_string_create()
        );

        break;

      case topazScript_DebugCommand_StepInto:
        //initial step into should have populated lastLine / lastFileID
        if (lineNumber != ctx->lastLine ||
            file       != ctx->lastFile) {
            debug_populate_state(ctx);
            topaz_script_notify_command(
                ctx->script,
                topazScript_DebugCommand_StepInto,
                topaz_string_create()
            );
            ctx->pendingCommand = topazScript_DebugCommand_Pause;
        }
        break;

      case topazScript_DebugCommand_StepOver:
        //initial step into should have populated lastLine / lastFileID
        if ((lineNumber != ctx->lastLine ||
             file       != ctx->lastFile) &&
            matte_vm_get_stackframe_size(ctx->vm) <= ctx->lastStacksize) {
            debug_populate_state(ctx);
            topaz_script_notify_command(
                ctx->script,
                topazScript_DebugCommand_StepOver,
                topaz_string_create()
            );
            ctx->pendingCommand = topazScript_DebugCommand_Pause;
        }
        break;
    


    
    }
    if (ctx->pendingCommand) {

    }

}
static void topaz_matte_debug_start(topazScript_t * scr, void * data) {
    TOPAZMATTE * ctx = data;
    ctx->isDebugging = TRUE;
    matte_vm_set_debug_callback(
        ctx->vm,
        topaz_matte_debug_callback,
        ctx
    );
}

void topaz_matte_debug_send_command(
    topazScript_t * scr, 
    void * data, 
    int cmd, 
    const topazString_t * arg
) {
    TOPAZMATTE * ctx = data;
    switch(cmd) {
      case topazScript_DebugCommand_Pause:
        if (topaz_string_get_length(arg)) ctx->pauseUp = 1;
        else ctx->pauseUp = 0;
        topaz_matte_trans_command__pause(ctx);
        
        break;    

      case topazScript_DebugCommand_Resume:
        topaz_matte_trans_command__resume(ctx);
        break;    

      case topazScript_DebugCommand_StepInto:
        topaz_matte_trans_command__step_into(ctx);
        break;

      case topazScript_DebugCommand_StepOver:
        topaz_matte_trans_command__step_over(ctx);
        break;

      case topazScript_DebugCommand_AddBreakpoint: {
        topazString_t * str = topaz_string_clone(arg);
        const topazString_t * iter = topaz_string_chain_start(str, TOPAZ_STR_CAST(":"));
        topazString_t * filename = topaz_string_clone(iter);
        iter = topaz_string_chain_proceed(str);        
        int lineNumber = atoi(topaz_string_get_c_str(iter));
                
        //topaz_matte_trans_command__add_breakpoint(ctx->trans_ctx, filename, lineNumber);
        break;
      }

      case topazScript_DebugCommand_RemoveBreakpoint: {
        int id = atoi(topaz_string_get_c_str(arg));
        uint32_t i;
        uint32_t len = topaz_array_get_size(ctx->debugBreakpoints);
        for(i = 0; i < len; ++i) {
            if (topaz_array_at(ctx->debugBreakpoints, int, i) == id) {
                //topaz_matte_trans_command__delete_breakpoint(ctx->trans_ctx, i);
                return;
            }
        } 
        //topaz_matte_trans_command__delete_breakpoint(ctx->trans_ctx, -1);
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
        if (!matte_vm_get_stackframe_size(ctx->vm)) {
            topazString_t * str = topaz_string_create_from_c_str("Eval error: Cannot evaluate expression if there is no callstack currently active.");
            PERROR(ctx->ctx, str);
            return;   
        }
        matteString_t * exprM = matte_string_create_from_c_str("return (import(module:'Topaz.DebugPrinter'))(o:%s);", topaz_string_get_c_str(command));
        matteValue_t result = 
            matte_vm_run_scoped_debug_source(
                ctx->vm,
                exprM,
                scope,
                topaz_matte_expression__error,
                ctx
            );
        

        topaz_script_notify_command(
            ctx->script,
            topazScript_DebugCommand_ScopedEval,
            topaz_string_create_from_c_str("%s", matte_string_get_c_str(matte_value_string_get_string_unsafe(ctx->heap, matte_value_as_string(ctx->heap, result))))
        );

        matte_string_destroy(exprM);
        topaz_string_destroy(str);
        break;
      }
      default:;
    }
}


const topazScript_DebugState_t * topaz_matte_debug_get_state(topazScript_t * scr, void * data) {
    TOPAZMATTE * ctx = data;
    return &ctx->debugState;
}




void topaz_system_script_matte__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazScriptAPI_t      *  api
) {
    TOPAZMATTEBackend * b = calloc(1, sizeof(TOPAZMATTEBackend));
    b->instances = topaz_array_create(sizeof(TOPAZMATTE*));
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Matte"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("https://jcorks.github.com/matte"),

        // desc 
        TOPAZ_STR_CAST("Bindings to use the Matte language during runtime."),



        // on step 
        NULL,
        
        // on step late 
        NULL,
        
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


    api->objectAPI.object_reference_create_from_reference = topaz_matte_object_reference_create_from_reference;
    api->objectAPI.object_reference_destroy = topaz_matte_object_reference_destroy;
    api->objectAPI.object_reference_get_feature_mask = topaz_matte_object_reference_get_feature_mask;
    api->objectAPI.object_reference_get_native_data = topaz_matte_object_reference_get_native_data;
    api->objectAPI.object_reference_set_native_data = topaz_matte_object_reference_set_native_data;
    api->objectAPI.object_reference_ref = topaz_matte_object_reference_ref;
    api->objectAPI.object_reference_unref = topaz_matte_object_reference_unref;
    api->objectAPI.object_reference_call = topaz_matte_object_reference_call;
    api->objectAPI.object_reference_array_get_nth = topaz_matte_object_reference_array_get_nth;
    api->objectAPI.object_reference_array_get_count = topaz_matte_object_reference_array_get_count;
    api->objectAPI.object_reference_map_get_property = topaz_matte_object_reference_map_get_property;
    api->objectAPI.object_reference_to_string = topaz_matte_object_reference_to_string;

    api->script_create = topaz_matte_create;
    api->script_destroy = topaz_matte_destroy;
    api->script_map_native_function = topaz_matte_map_native_function;
    api->script_run = topaz_matte_run;
    api->script_expression = topaz_matte_expression;
    api->script_create_empty_object = topaz_matte_create_empty_object;
    api->script_throw_error = topaz_matte_throw_error;
    api->script_bootstrap = topaz_matte_bootstrap;


    api->script_debug_start = topaz_matte_debug_start;
    api->script_debug_send_command = topaz_matte_debug_send_command;
    api->script_debug_get_state = topaz_matte_debug_get_state;
}





