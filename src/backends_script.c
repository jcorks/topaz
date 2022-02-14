#include <topaz/backends/script.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <topaz/containers/table.h>
#include <topaz/containers/bin.h>
#include <topaz/backends/filesystem.h>
#include <topaz/modules/resources.h>
#include <topaz/assets/data.h>
#include <topaz/topaz.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef TOPAZDC_DEBUG 
    #include <assert.h>
#endif

typedef struct {
    topaz_script_event_function fn;
    void * fndata;

} EventHandler;

/// script context
struct topazScript_t {
    topaz_t * ctx;
    topazScriptAPI_t api;

    void * implementationData;

    topazSystem_Backend_t * backend;

    // contains all inactive objects ready to be reused.
    topazArray_t * objectPool;

    // an array of pointers that must be freed individually.
    // these are raw memory blocks.
    topazArray_t * objectAllocRefs;

    // functiosn called for object events.
    EventHandler handlers[topaz_Script_Event_OnError+1];

    // whether debugging is enabled. If not enabled, all debug functions 
    // are ignored.
    int debugEnabled;

    // whether the debugger is paused. Only certain commands can 
    // be sent if the debugger is paused.
    int debugPaused;

    // a bin of all the pause notification CBs. These are called when the 
    // pause state is confirmed by the debugging implementation.
    topazBin_t * debugCBs;

    // [source name] -> GArray[line] -> topazString_t *
    topazTable_t * sources;

    topazArray_t * importPathStack;
    
};


//// object 

struct topazScript_Object_t {

    // always populated type.
    topazScript_Object_Type_t type;

    // object value as an int
    int dataInt;

    // object as a double
    double dataNumber;

    // object as a string
    topazString_t * dataString;

    // NULL unless is external 
    topazScript_Object_ReferenceAPI_t * api;

    // passed to all external api calls.
    void * apiData;
    // the context (and object pool) that currently owns this
    topazScript_t * context;

    /// Native pointer associated with this object reference.
    void * nativeData;

};



topazScript_t * topaz_script_create(topaz_t * t, topazSystem_Backend_t * b, const topazScriptAPI_t * api) {
    topazScript_t * out = calloc(1, sizeof(topazScript_t));
    out->api = *api;
    out->backend = b;
    out->objectPool = topaz_array_create(sizeof(topazScript_Object_t *));
    out->objectAllocRefs = topaz_array_create(sizeof(void *));
    out->debugCBs = topaz_bin_create();
    out->ctx = t;
    out->sources = topaz_table_create_hash_topaz_string();
    out->importPathStack = topaz_array_create(sizeof(topazArray_t *));
    #ifdef TOPAZDC_DEBUG
        assert(api->script_create);
        assert(api->script_destroy);
        assert(api->script_map_native_function);
        assert(api->script_run);
        assert(api->script_expression);
        assert(api->script_bootstrap);
    #endif

    out->implementationData = out->api.script_create(out, t);
    return out;
}

/// Destroys and frees a topaz input instance.
///
void topaz_script_destroy(topazScript_t * s) {
    s->api.script_destroy(s, s->implementationData);
    
    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, s->sources);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        topazArray_t * value = topaz_table_iter_get_value(iter);
        uint32_t i;
        uint32_t len = topaz_array_get_size(value);
        for(i = 0; i < len; ++i) {
            topaz_string_destroy(topaz_array_at(value, topazString_t *, i));
        }
        topaz_array_destroy(value);
    }
    topaz_table_iter_destroy(iter);
    topaz_table_destroy(s->sources);
    

    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->objectAllocRefs); ++i) {
        free(topaz_array_at(s->objectAllocRefs, void*, i));
    }   
    topaz_array_destroy(s->objectAllocRefs);
    topaz_array_destroy(s->objectPool);
    free(s);
}


topazSystem_Backend_t * topaz_script_get_backend(topazScript_t * s) {
    return s->backend;
}

topazScriptAPI_t topaz_script_get_api(topazScript_t * s) {
    return s->api;
}



topazScriptAPI_t topaz_script_get_api(topazScript_t *);

int topaz_script_map_native_function(
    topazScript_t * s,
    const topazString_t * localSymbolName,
    uint8_t argumentCount,
    topaz_script_native_function fn,
    void * userData
) {

    return s->api.script_map_native_function(
        s, s->implementationData,
        localSymbolName,
        argumentCount,
        fn,
        userData
    );
}


void topaz_script_run_once(
    topazScript_t * s, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
) {
    if (!topaz_table_find(s->sources, sourceName)) {
        topaz_script_run(s, sourceName, scriptData);
    }
}


void topaz_script_register_source(
    topazScript_t * s, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
) {
    topazArray_t * source = topaz_table_find(s->sources, sourceName);
    if (source) {
        uint32_t i;
        uint32_t len = topaz_array_get_size(source);
        for(i = 0; i < len; ++i) {
            topaz_string_destroy(topaz_array_at(source, topazString_t *, i));
        }
        topaz_array_set_size(source, 0);
    } else {
        source = topaz_array_create(sizeof(topazString_t *));

        // line breakdown
        uint32_t len = topaz_string_get_length(scriptData);
        uint32_t i = 0;
        topazString_t * line = topaz_string_create();
        while(i < len) {
            int c  = topaz_string_get_char(scriptData, i);
            int cN = i+1 >= len ? 0 : topaz_string_get_char(scriptData, i);

            if (c == '\r' && cN == '\n') {
                topazString_t * lineComplete = topaz_string_clone(line);
                topaz_array_push(source, lineComplete);
                topaz_string_clear(line);
                i+=2;
            } else if (c == '\n') {
                topazString_t * lineComplete = topaz_string_clone(line);
                topaz_array_push(source, lineComplete);
                topaz_string_clear(line);
                i++;
            } else {
                topaz_string_append_char(line, c);
                i++;
            }
        }
        topaz_string_destroy(line);
        topaz_table_insert(s->sources, sourceName, source);

    }
}


void topaz_script_run(
    topazScript_t * s, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
) {
    topaz_script_register_source(s, sourceName, scriptData);

    s->api.script_run(
        s, s->implementationData,
        sourceName,
        scriptData
    );
}



int topaz_script_get_line_count(
    topazScript_t * s,
    const topazString_t * sourceName    
) {
    topazArray_t * source = topaz_table_find(s->sources, sourceName);
    if (!source) return 0;
    return (int)topaz_array_get_size(source);
}



const topazString_t * topaz_script_get_line(
    topazScript_t * s,
    const topazString_t * sourceName,
    int lineNumber
) {
    topazArray_t * source = topaz_table_find(s->sources, sourceName);
    if (!source) return TOPAZ_STR_CAST("");
    int lineCount = (int)topaz_array_get_size(source);
    if (lineNumber < 0 || lineNumber >= lineCount) return TOPAZ_STR_CAST("");
    return topaz_array_at(source, topazString_t *, lineNumber);
}

topazScript_Object_t * topaz_script_expression(
    topazScript_t * s, 
    const topazString_t * expr) {

    return s->api.script_expression(
        s, s->implementationData,
        expr
    );
}

void topaz_script_bootstrap(
    topazScript_t * s) {

    s->api.script_bootstrap(
        s, s->implementationData
    );
}

topazScript_Object_t * topaz_script_create_empty_object(
    topazScript_t * s,
    topaz_script_native_function fn,
    void * data) {

    return s->api.script_create_empty_object(
        s, s->implementationData,
        fn,
        data
    );
}



void topaz_script_set_handler(
    topazScript_t * s, 
    topazScript_Event_t ev, 
    topaz_script_event_function fn, 
    void * data) {
    
    s->handlers[ev].fn = fn;
    s->handlers[ev].fndata = data;
}

void topaz_script_emit_event(
    topazScript_t * s, 
    topazScript_Event_t ev, 
    const topazArray_t * args) {


    if (s->handlers[ev].fn)
        s->handlers[ev].fn(s, args, s->handlers[ev].fndata);

    if (s->api.script_throw_error)
        s->api.script_throw_error(s, s->implementationData, args);
    else {
        printf("TOPAZ_SCRIPT_EMITTED_ERROR (no handler): %s\n",
            topaz_array_get_size(args) ?
                topaz_string_get_c_str(topaz_script_object_as_string(topaz_array_at(args, topazScript_Object_t *, 0)))
            :
                "(no data)"
        );
    }
}





// clears an object, making it appropriate for reuse
static void object_reset(topazScript_t * s, topazScript_Object_t * o) {
    o->type = topazScript_Object_Type_Undefined;
    o->api = NULL;
    o->nativeData = NULL;
    o->context = s;
    o->apiData = NULL;
}

#define OBJECT_POOL_CHUNK_COUNT 256
#define OBJECT_POOL_CHUNK_SIZE  (OBJECT_POOL_CHUNK_COUNT*sizeof(topazScript_Object_t))

// Gets a new script object using the "efficient" pool
static topazScript_Object_t * object_pool_fetch(topazScript_t * s) {
    uint32_t size = topaz_array_get_size(s->objectPool);
    if (!size) {
        int i = 0;
        topazScript_Object_t * pool_chunk = calloc(1, OBJECT_POOL_CHUNK_SIZE);
        topaz_array_push(s->objectAllocRefs, pool_chunk);
        for(i = 0; i < OBJECT_POOL_CHUNK_COUNT; ++i) {
            topaz_array_push(s->objectPool, pool_chunk);
            pool_chunk++;
        }
        size = OBJECT_POOL_CHUNK_COUNT;
    }  

    topazScript_Object_t * o = topaz_array_at(
        s->objectPool, 
        topazScript_Object_t *,
        size-1
    );

    topaz_array_set_size(
        s->objectPool, 
        size-1
    );

    object_reset(s, o);
    return o;
}

// puts an obejct back into the object pool
static void object_pool_recycle(topazScript_t * s, topazScript_Object_t * o) {
    topaz_array_push(s->objectPool, o);
}






topazScript_Object_t * topaz_script_object_undefined(topazScript_t * s) {
    return object_pool_fetch(s);
}

topazScript_Object_t * topaz_script_object_from_int(topazScript_t * s, int value) {
    topazScript_Object_t * o = object_pool_fetch(s);
    o->dataInt = value;
    o->type = topazScript_Object_Type_Integer;
    return o;
}


topazScript_Object_t * topaz_script_object_from_number(topazScript_t * s, double value) {
    topazScript_Object_t * o = object_pool_fetch(s);
    o->dataNumber = value;
    o->type = topazScript_Object_Type_Number;
    return o;
}

topazScript_Object_t * topaz_script_object_from_string(topazScript_t * s, const topazString_t * value) {
    topazScript_Object_t * o = object_pool_fetch(s);
    if (!o->dataString) {
        o->dataString = topaz_string_create();
    } else {
        topaz_string_clear(o->dataString);
    }
    topaz_string_set(o->dataString, value);
    o->type = topazScript_Object_Type_String;
    return o;
}


topazScript_Object_t * topaz_script_object_from_object(topazScript_t * s, topazScript_Object_t * o) {
    switch(o->type) {
      case topazScript_Object_Type_Integer: 
        return topaz_script_object_from_int(s, o->dataInt);

      case topazScript_Object_Type_Number: 
        return topaz_script_object_from_number(s, o->dataNumber);

      case topazScript_Object_Type_String:
        return topaz_script_object_from_string(s, o->dataString);

      default:;
    }

    topazScript_Object_t * out = object_pool_fetch(s);
    out->type = topazScript_Object_Type_Reference;
    out->api = &s->api.objectAPI;

    if (out->api->object_reference_create_from_reference)
        out->apiData = out->api->object_reference_create_from_reference(
            out, 
            s->implementationData, 
            o, 
            o->apiData
        );

    if (out->api->object_reference_ref)
        out->api->object_reference_ref(out, out->apiData);
    return out;

}

topazScript_Object_t * topaz_script_object_wrapper(
    topazScript_t * s,
    void * userData
) {
    topazScript_Object_t * o = object_pool_fetch(s);
    o->type = topazScript_Object_Type_Reference;
    o->api = &s->api.objectAPI;
    o->apiData = userData;

    if (o->api->object_reference_ref)
        o->api->object_reference_ref(o, o->apiData);
    return o;
}


topazScript_t * topaz_script_object_get_source(topazScript_Object_t * o) {
    return o->context;
}


void topaz_script_object_destroy(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {

        if (o->api->object_reference_unref)
            o->api->object_reference_unref(o, o->apiData);

        if (o->api->object_reference_destroy)
            o->api->object_reference_destroy(o, o->apiData);
    }

    object_pool_recycle(o->context, o);
}

int topaz_script_object_as_int(const topazScript_Object_t * o) {
    switch(o->type) {
      case topazScript_Object_Type_Integer: 
        return o->dataInt;

      case topazScript_Object_Type_Number:
        return o->dataNumber;

      case topazScript_Object_Type_String:
        return atoi(topaz_string_get_c_str(o->dataString));

      default:
        return 0;
    }
}


double topaz_script_object_as_number(const topazScript_Object_t * o) {
    switch(o->type) {
      case topazScript_Object_Type_Integer: 
        return o->dataInt;

      case topazScript_Object_Type_Number:
        return o->dataNumber;

      case topazScript_Object_Type_String:
        return strtod(topaz_string_get_c_str(o->dataString), NULL);

      default:
        return 0;
    }
}

#define TOPAZ_SCRIPT_OBJECT_NUM_BUFFER_CHARS 100

const topazString_t * topaz_script_object_as_string(const topazScript_Object_t * oSrc) {
    // cacheing is a writing operation, but its EFFECTIVELY const...
    topazScript_Object_t * o = (topazScript_Object_t*)oSrc;
    char numBuffer[TOPAZ_SCRIPT_OBJECT_NUM_BUFFER_CHARS];
    

    switch(o->type) {
      case topazScript_Object_Type_Integer: 
        snprintf(numBuffer, TOPAZ_SCRIPT_OBJECT_NUM_BUFFER_CHARS-1, "%d", o->dataInt);
        if (!o->dataString) {
            o->dataString = topaz_string_create();
        } else {
            topaz_string_clear(o->dataString);
        }
        topaz_string_concat_printf(o->dataString, numBuffer);
        return o->dataString;


      case topazScript_Object_Type_Number:
        snprintf(numBuffer, TOPAZ_SCRIPT_OBJECT_NUM_BUFFER_CHARS-1, "%g", o->dataNumber);
        if (!o->dataString) {
            o->dataString = topaz_string_create();
        } else {
            topaz_string_clear(o->dataString);
        }
        topaz_string_concat_printf(o->dataString, numBuffer);
        return o->dataString;


      case topazScript_Object_Type_String:
        return o->dataString;

      case topazScript_Object_Type_Reference:
        if (o->api->object_reference_to_string) {
            if (!o->dataString) {
                o->dataString = topaz_string_create();
            } else {
                topaz_string_clear(o->dataString);
            }
            topaz_string_clear(o->dataString);
            o->api->object_reference_to_string(o, o->dataString, o->apiData);
            return o->dataString;
        }
        break;
      default:;
    }


    return TOPAZ_STR_CAST("");
}


void topaz_script_object_set(topazScript_Object_t * o, const topazScript_Object_t * other) {
    o->type = other->type;
    o->dataInt = other->dataInt;
    o->dataNumber = other->dataNumber;

    if (o->type == topazScript_Object_Type_String)
        topaz_string_set(o->dataString, other->dataString);


    o->api = other->api;
    o->apiData = other->apiData;
    o->context = other->context;
    o->nativeData = other->nativeData;
}



topazScript_Object_Type_t topaz_script_object_get_type(const topazScript_Object_t * o) {
    return o->type;
}

















topazScript_Object_Feature_t topaz_script_object_reference_get_feature_mask(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_get_feature_mask)
            return o->api->object_reference_get_feature_mask(o, o->apiData);        
    }
    return topazScript_Object_Feature_None;
}

topazScript_Object_t * topaz_script_object_reference_call(topazScript_Object_t * o, const topazArray_t * args) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_call)
            return o->api->object_reference_call(o, args, o->apiData);
    }
    return topaz_script_object_undefined(o->context);
}

void topaz_script_object_reference_set_native_data(topazScript_Object_t * o, void * data, int tag) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_set_native_data)
            o->api->object_reference_set_native_data(o, data, tag, o->apiData);
    }
}

void * topaz_script_object_reference_get_native_data(topazScript_Object_t * o, int * tag) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_get_native_data)
            return o->api->object_reference_get_native_data(o, tag, o->apiData);
    }
    return NULL;
}

void topaz_script_object_reference_ref(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_ref)
            o->api->object_reference_ref(o, o->apiData);
    }
}

void topaz_script_object_reference_unref(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_unref)
            o->api->object_reference_unref(o, o->apiData);
    }
}


topazScript_Object_t * topaz_script_object_reference_map_get_property(
    topazScript_Object_t * o,
    const topazString_t * prop
) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_map_get_property)
            return o->api->object_reference_map_get_property(o, prop, o->apiData);
    }
    return topaz_script_object_undefined(o->context);
}



int topaz_script_object_reference_array_get_count(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_array_get_count)
            return o->api->object_reference_array_get_count(o, o->apiData);
    }
    return -1;
}

topazScript_Object_t * topaz_script_object_reference_array_get_nth(topazScript_Object_t * o, int i) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_array_get_nth)
            return o->api->object_reference_array_get_nth(o, i, o->apiData);
    }
    return topaz_script_object_undefined(o->context);
}




void * topaz_script_object_get_api_data(topazScript_Object_t * o) {
    return o->apiData;
}









void topaz_script_enable_debugging(topazScript_t * s) {
    if (!s->debugEnabled) {
        if (s->api.script_debug_start) {
            s->debugEnabled = 1;
            s->api.script_debug_start(s, s->implementationData);
        }
    }
}

void topaz_script_debug_send_command(
    topazScript_t * s, 
    topazScript_DebugCommand_t command,
    const topazString_t * argument
) {
    if (!s->debugEnabled) return;

    if (s->debugPaused) {
        switch(command) {
          case topazScript_DebugCommand_Pause:
            break;

          case topazScript_DebugCommand_Resume:
          default:
            s->api.script_debug_send_command(s, s->implementationData, command, argument);
        }
    } else {
        switch(command) {
          case topazScript_DebugCommand_ScopedEval:
          case topazScript_DebugCommand_Pause:
            return s->api.script_debug_send_command(s, s->implementationData, command, argument);

          // ignored if not officially paused
          default:;
        }
    }
}

typedef struct {
    topaz_script_debug_command_callback cb;
    void * data;
} DebugCB;

uint32_t topaz_script_debug_add_command_callback(
    topazScript_t * s,
    topaz_script_debug_command_callback cbSrc,
    void * data
) {
    DebugCB * cb = malloc(sizeof(DebugCB));
    cb->cb = cbSrc;
    cb->data = data;

    return topaz_bin_add(s->debugCBs, cb);
}

/// Removes the callback references by ID from the list 
/// of callbacks registered to be called when the debugger 
/// excepts the pause request.
void topaz_script_debug_remove_command_callback(
    topazScript_t * s,
    uint32_t id
) {
    DebugCB * cb = topaz_bin_fetch(s->debugCBs, id);
    if (cb) {
        free(cb);
        topaz_bin_remove(s->debugCBs, id);
    }
}

const topazScript_DebugState_t * topaz_script_debug_get_state(
    topazScript_t * s
) {
    return s->api.script_debug_get_state(s, s->implementationData);
}

int topaz_script_debug_is_paused(topazScript_t * s) {
    return s->debugPaused;
}


void topaz_script_notify_command(topazScript_t * s, int cmd, topazString_t * str) {
    if (cmd == topazScript_DebugCommand_Pause ||
        cmd == topazScript_DebugCommand_Resume) {
        int different = 0;
        if (cmd == topazScript_DebugCommand_Pause && !s->debugPaused) {
            s->debugPaused = 1;
            different = 1;
        } else if (cmd == topazScript_DebugCommand_Resume && s->debugPaused) {
            s->debugPaused = 0;
            different = 1;
        }

        if (!different) return;
    }


    if (cmd == topazScript_DebugCommand_Pause) {
        topaz_context_pause(s->ctx);
    } else if (cmd == topazScript_DebugCommand_Resume) {
        topaz_context_resume(s->ctx);
    }


    topazArray_t * arr = topaz_bin_get_all(s->debugCBs);
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(arr); ++i) {
        DebugCB * cb = topaz_array_at(arr, DebugCB *, i);
        cb->cb(s, cmd, str, cb->data);
    }   
    topaz_array_destroy(arr);
    topaz_string_destroy(str);
}

/*
static const topazString_t * topaz_script_apply_import_path(
    topazScript_t * s,
    const topazString_t * path
) {
    if (topaz_array_get_size(s->importPathStack)) {    
        topazFilesystem_Path_t * current = topaz_array_at(s->importPathStack, topazFilesystem_Path_t *, topaz_array_get_size(s->importPathStack)-1);
        const topazFilesystem_Path_t * p = topaz_filesystem_get_path_from_string(
            topaz_context_get_filesystem(s->ctx),
            current,
            path
        );
        
        return p ? topaz_filesystem_path_as_string(p) : path;
    } else {
        return path;
    }
}


// Given a path to a file, keeps the directory portion of the path 
static void topaz_script_push_import_path(
    topazScript_t * s,
    const topazString_t * path
) {
        
    topazFilesystem_Path_t * current = NULL;
    if (topaz_array_get_size(s->importPathStack)) {
        current = topaz_array_at(s->importPathStack, topazFilesystem_Path_t *, topaz_array_get_size(s->importPathStack)-1);
    } 
    const topazFilesystem_Path_t * p = topaz_filesystem_get_path_from_string(
        topaz_context_get_filesystem(s->ctx),
        current ? NULL : topaz_filesystem_get_path(topaz_context_get_filesystem(s->ctx), topazFilesystem_DefaultNode_Resources),
        path
    );
    
    if (!p) return;
    const topazFilesystem_Path_t * parent = topaz_filesystem_path_get_parent(p);

    if (parent) {
        printf("PUSHING %s\n", topaz_string_get_c_str(topaz_filesystem_path_as_string(parent)));
        topaz_array_push(s->importPathStack, parent);
    }
}
*/
// Pops the current directory.
static void topaz_script_pop_import_path(
    topazScript_t * s
) {
    if (topaz_array_get_size(s->importPathStack)) {
        printf("poppin\n");
        topaz_array_set_size(s->importPathStack, topaz_array_get_size(s->importPathStack)-1);
    }     
}



int topaz_script_import(
    topazScript_t * script,
    const topazString_t * path
) {

    
    topazResources_t * r = topaz_context_get_resources(script->ctx);
    // first, raw path is used. This should handle any special files 
    // and direct paths.
    topazAsset_t * src = topaz_resources_fetch_asset(
        r,
        path
    );


    if (src) {
        topazString_t * srcStr = topaz_data_get_as_string(src);
        topaz_script_run_once(
            script,
            path,
            srcStr
        );
        topaz_string_destroy(srcStr);
        topaz_script_pop_import_path(script);
        return 1;
    } else {
        return 0;
    }

}






