#include <topaz/backends/script.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    topaz_script_native_function fn;
    void * fndata;

} EventHandler;

/// script context
struct topazScript_t {
    topazScriptAPI_t api;
    topazBackend_t * backend;

    // contains all inactive objects ready to be reused.
    topazArray_t * objectPool;

    // an array of pointers that must be freed individually.
    // these are raw memory blocks.
    topazArray_t * objectAllocRefs;

    // functiosn called for object events.
    EventHandler handlers[topaz_Script_Event_OnError];
    
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
    #endif

    out->api.script_create(&out->api);
    return out;
}

/// Destroys and frees a topaz input instance.
///
void topaz_script_destroy(topazScript_t * s) {
    s->api.script_destroy(&s->api);
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
        &s->api,
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
        &s->api,
        sourceName,
        scriptData
    );
}

topazScript_Object_t * topaz_script_expression(
    topazScript_t * s, 
    const topazString_t * expr) {

    return s->api.script_expression(
        &s->api,
        expr
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
}





// clears an object, making it appropriate for reuse
static void object_reset(topazScript_t * s, topazScript_Object_t * o) {
    o->type = topazScript_Object_Type_Undefined;
    o->api = NULL;
    o->nativeData = NULL;
    o->context = s;
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
    o->type = topazScript_Object_Type_String;
    return o;
}

topazScript_Object_t * topaz_script_object_from_object(topazScript_t * s, const topazScript_Object_t * other) {
    topazScript_Object_t * o = object_pool_fetch(s);
    o->type = other->type;
    switch(o->type) {
      case topazScript_Object_Type_Undefined:
        break;

      case topazScript_Object_Type_Integer: 
        o->dataInt = other->dataInt;
        break;

      case topazScript_Object_Type_Number: 
        o->dataNumber = other->dataNumber;
        break;

      case topazScript_Object_Type_String:
        topaz_string_set(o->dataString, other->dataString);
        break;

      case topazScript_Object_Type_Reference: // all others are external
        o->api = other->api;
        o->apiData = other->apiData;
        if (o->api->object_reference_create)
            o->api->object_reference_create(o, o->apiData);

        break;      
    }

    return o;
}


topazScript_Object_t * topaz_script_object_from_reference(
    topazScript_t * s,
    void * userData
) {
    topazScript_Object_t * o = object_pool_fetch(s);
    o->type = topazScript_Object_Type_Reference;
    o->api = &s->api.objectAPI;
    o->apiData = userData;
    if (o->api->object_reference_create)
        o->api->object_reference_create(o, o->apiData);
    return o;
}


topazScript_t * topaz_script_object_get_source(topazScript_Object_t * o) {
    return o->context;
}


void topaz_script_object_destroy(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
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

      default:
        return 0;
    }
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

















topazScript_Object_Feature_t topaz_script_object_reference_get_features(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_get_features)
            return o->api->object_reference_get_features(o, o->apiData);        
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

void topaz_script_object_reference_set_native_data(topazScript_Object_t * o, void * data) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_set_native_data)
            o->api->object_reference_set_native_data(o, data, o->apiData);
    }
}

void * topaz_script_object_reference_get_native_data(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_get_native_data)
            return o->api->object_reference_get_native_data(o, o->apiData);
    }
    return NULL;
}

topazScript_Object_t * topaz_script_object_map_get_property(
    topazScript_Object_t * o,
    const topazString_t * prop
) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_map_get_property)
            return o->api->object_reference_map_get_property(o, prop, o->apiData);
    }
    return topaz_script_object_undefined(o->context);
}



int topaz_script_object_array_get_count(topazScript_Object_t * o) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_array_get_count)
            return o->api->object_reference_array_get_count(o, o->apiData);
    }
    return -1;
}

topazScript_Object_t * topaz_script_object_array_get_nth(topazScript_Object_t * o, int i) {
    if (o->type == topazScript_Object_Type_Reference) {
        if (o->api->object_reference_array_get_nth)
            return o->api->object_reference_array_get_nth(o, i, o->apiData);
    }
    return topaz_script_object_undefined(o->context);
}


















