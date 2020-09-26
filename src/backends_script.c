#include <topaz/backends/script.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef TOPAZDC_DEBUG 
    #include <assert.h>
#endif

typedef struct {
    topaz_script_native_function fn;
    void * fndata;

} EventHandler;

/// script context
struct topazScript_t {
    topazScriptAPI_t api;

    void * implementationData;

    topazBackend_t * backend;

    // contains all inactive objects ready to be reused.
    topazArray_t * objectPool;

    // an array of pointers that must be freed individually.
    // these are raw memory blocks.
    topazArray_t * objectAllocRefs;

    // functiosn called for object events.
    EventHandler handlers[topaz_Script_Event_OnError+1];
    
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

    // user data argument from reference creation function
    void * sourceAPIData;

    // the context (and object pool) that currently owns this
    topazScript_t * context;

    /// Native pointer associated with this object reference.
    void * nativeData;

};



topazScript_t * topaz_script_create(topazBackend_t * b, const topazScriptAPI_t * api) {
    topazScript_t * out = calloc(1, sizeof(topazScript_t));
    out->api = *api;
    out->backend = b;
    out->objectPool = topaz_array_create(sizeof(topazScript_Object_t *));
    out->objectAllocRefs = topaz_array_create(sizeof(void *));
    #ifdef TOPAZDC_DEBUG
        assert(api->script_create);
        assert(api->script_destroy);
        assert(api->script_map_native_function);
        assert(api->script_run);
        assert(api->script_expression);
        assert(api->script_bootstrap);
    #endif

    out->implementationData = out->api.script_create(out);
    return out;
}

/// Destroys and frees a topaz input instance.
///
void topaz_script_destroy(topazScript_t * s) {
    s->api.script_destroy(s, s->implementationData);
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->objectAllocRefs); ++i) {
        free(topaz_array_at(s->objectAllocRefs, void*, i));
    }   
    topaz_array_destroy(s->objectAllocRefs);
    topaz_array_destroy(s->objectPool);
    free(s);
}


int topaz_script_map_native_function(
    topazScript_t * s,
    const topazString_t * localSymbolName,
    topaz_script_native_function fn,
    void * userData
) {

    return s->api.script_map_native_function(
        s, s->implementationData,
        localSymbolName,
        fn,
        userData
    );
}

void topaz_script_run(
    topazScript_t * s, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
) {
    s->api.script_run(
        s, s->implementationData,
        sourceName,
        scriptData
    );
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
    topaz_script_native_function fn, 
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
    out->sourceAPIData = o->sourceAPIData;

    if (out->api->object_reference_create)
        out->apiData = out->api->object_reference_create_from_reference(out, out->sourceAPIData, o, o->apiData);

    if (out->api->object_reference_ref)
        out->api->object_reference_ref(out, out->apiData);
    return out;

}

topazScript_Object_t * topaz_script_object_from_reference(
    topazScript_t * s,
    void * userData
) {
    topazScript_Object_t * o = object_pool_fetch(s);
    o->type = topazScript_Object_Type_Reference;
    o->api = &s->api.objectAPI;
    o->sourceAPIData = userData;
    if (o->api->object_reference_create)
        o->apiData = o->api->object_reference_create(o, userData);

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


void topaz_script_object_reference_extendable_add_property(
    topazScript_Object_t * o,
    const topazString_t * propName,
    topaz_script_native_function onSet,
    topaz_script_native_function onGet
) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_extendable_add_property)
            o->api->object_reference_extendable_add_property(o, propName, onSet, onGet, o->apiData);
    }       
}

void * topaz_script_object_get_api_data(topazScript_Object_t * o) {
    return o->apiData;
}









