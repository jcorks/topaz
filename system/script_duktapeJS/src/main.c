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
#include <topaz/containers/array.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif
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
    // used for object reference creation
    int lastObjectReference;
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
} TOPAZDUKObjectTag;


// Gets the object tag from the top object
static TOPAZDUKObjectTag * topaz_duk_object_get_tag_from_top(duk_context * js) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(js);
    #endif

    // uhh?? unsafe if someone puts a __tz prop by hand in a js context.
    // think of a different way before release, thanks
    duk_push_string(js, "__tz");
    duk_get_prop(js, -2);
    TOPAZDUKObjectTag * tag = NULL;
    sscanf(duk_get_string(js, -1), "%p", &tag);
    duk_pop(js);

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

    TOPAZDUKObjectTag * out = calloc(1, sizeof(TOPAZDUKObjectTag));
    duk_push_string(ctx->js, "___tz");
    duk_push_sprintf(ctx->js, "%p", out);
    duk_def_prop(
        ctx->js,
        -3,
        DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_CONFIGURABLE
    );

    duk_pop(ctx->js);


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
    duk_push_sprintf(ctx->js, "p%s", tag);
    duk_dup(ctx->js, -3);
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
    duk_push_sprintf(ctx->js, "p%s", tag);
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
    duk_push_sprintf(tag->ctx->js, "p%s", tag);
    duk_get_prop(tag->ctx->js, -2);
    #ifdef TOPAZDC_DEBUG
        if (duk_is_undefined(tag->ctx->js, -1)) {
            printf("topaz_duk_object_push_to_top_from_tag() resulted in UNDEFINED. This shouldn't happen!\n");
        }
    #endif
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
            printf("WARNING: duktapeJS returned DUK_TYPE_NONE, this is likely indicative of programmer error!");
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
        o = topaz_script_object_from_reference(ctx->script, NULL);
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
      }
      default:
        duk_push_undefined(ctx->js);
        break;        
    }
}

















static void * topaz_duk_create(topazScript_t * scr) {
    TOPAZDUK * out = calloc(1, sizeof(TOPAZDUK));
    out->js = duk_create_heap_default();
    out->script = scr;
    return out;
}

static topazScript_Object_t * topaz_duk_expression(
    topazScript_t * script, 
    void * data,
    const topazString_t * expr
) {
    TOPAZDUK * ctx = data;
    duk_eval_string(ctx->js, topaz_string_get_c_str(expr));
    topazScript_Object_t * out = topaz_duk_stack_object_to_tso(
        ctx,
        -1
    );
    duk_pop(ctx->js);
    return out;
}

static duk_ret_t topaz_duk_get_internal(duk_context * js) {
    duk_push_current_function(js);
    duk_push_string(js, "__tz");
    duk_get_prop(js, -2);
    TOPAZDUKObjectTag * tag = NULL;
    sscanf(duk_get_string(js, -1), "%p", &tag);

    topazScript_Object_t * src = topaz_duk_stack_object_to_tso(
    
}

static duk_ret_t topaz_duk_set_internal(duk_context * js) {
    
}









// assumes the object is at the top
static void * topaz_duk_object_reference_create(topazScript_Object_t * o, void * ctxSrc, void * notused) {
    TOPAZDUK * ctx = ctxSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(ctx->js);
    #endif

    TOPAZDUKObjectTag * tag = topaz_duk_object_get_tag_from_top(ctx->js);

    if (!tag) {
        // create a new tag and bind that tag to this object using a hidden prop
        tag = topaz_duk_object_set_tag(ctx);   

        tag->ctx = ctx;


        // make sure that a reference to this object exists outside of the pure 
        // js context. This allows the C context to keep the object around.
        topaz_duk_object_keep_reference(ctxSrc, tag);

        // Sets the standard finalizer function.
        duk_push_c_function(ctx->js, topaz_duk_object_finalizer, 0);
        duk_set_finalizer(ctx->js, -2);
    }



    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(ctx->js) == stackSize);
    #endif

    return tag;
}


static void topaz_duk_object_reference_destroy(topazScript_Object_t * o, void * tagSrc) {
    // the finalizer handlers destruction for us, 
    // so there isnt much to do here.
    // (topaz will unref for us)
}



static int topaz_duk_object_reference_get_features(topazScript_Object_t * o, void * tagSrc) {
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

static void topaz_duk_object_reference_set_native_data(topazScript_Object_t * o, int tagID, void * nativeData, void * tagSrc) {
    TOPAZDUKObjectTag * t = tagSrc;
    t->tagID = tagID;
    t->nativeData = t->nativeData;
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

    duk_pcall(tag->ctx->js, len);
    topazScript_Object_t * out = topaz_duk_stack_object_to_tso(
        tag->ctx,
        -1
    );

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

const topazString_t * topaz_duk_object_reference_to_string(
    topazScript_Object_t * o, 
    void * tagSrc) {
    
    TOPAZDUKObjectTag * tag = tagSrc;

    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(tag->ctx->js);
    #endif

    topaz_duk_object_push_to_top_from_tag(tag);
    const char * str = duk_to_string(tag->ctx->js, -1);
    duk_pop(tag->ctx->js);


    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(tag->ctx->js) == stackSize);
    #endif


    // is this REALLY a good idea?
    return TOPAZ_STR_CAST(str);
}

void (*object_reference_extendable_add_property)(
    topazScript_Object_t * object, 
    const topazString_t * propName,
    topaz_script_native_function onSet,
    topaz_script_native_function onGet,
    void * data
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
        #ifdef TOPAZDC_DEBUG
            int stackSize2 = duk_get_top(tag->ctx->js);
        #endif

        duk_push_c_function(tag->ctx->js, topaz_duk_get_internal);
        // we need to set a prop to this tag 
        duk_push_string(tag->ctx->js, "___tz");
        duk_push_sprintf(tag->ctx->js, "%p", tag);
        duk_def_prop(
            tag->ctx->js,
            -3,
            DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_CONFIGURABLE
        );

        duk_pop(tag->ctx->js);
        hasWhech |= DUK_DEFPROP_HAVE_GETTER;
        
        #ifdef TOPAZDC_DEBUG 
            assert(duk_get_top(tag->ctx->js) == stackSize2);
        #endif

    }


    if (onSet) {
        #ifdef TOPAZDC_DEBUG
            int stackSize2 = duk_get_top(tag->ctx->js);
        #endif

        duk_push_c_function(tag->ctx->js, topaz_duk_set_internal);
        // we need to set a prop to this tag 
        duk_push_string(tag->ctx->js, "___tz");
        duk_push_sprintf(tag->ctx->js, "%p", tag);
        duk_def_prop(
            tag->ctx->js,
            -3,
            DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_CONFIGURABLE
        );

        duk_pop(tag->ctx->js);
        hasWhech |= DUK_DEFPROP_HAVE_SETTER;
        
        #ifdef TOPAZDC_DEBUG 
            assert(duk_get_top(tag->ctx->js) == stackSize2);
        #endif

    }

    
    duk_def_prop(
        tag->ctx->js, 
        -4,
        hasWHich
    );
    
    duk_pop(tag->ctx->js);
}


void











static intptr_t api_nothing(){return 0;}
void topaz_system_script_duktapeJS__api(topazScriptAPI_t * api) {
    api->objectAPI.object_reference_create = topaz_duk_object_reference_create;

    api->script_create = topaz_duk_create;
    api->script_destroy = (void (*)(topazScript_t *, void *)) api_nothing;
    api->script_map_native_function = (int (*)(topazScript_t *, void *, const topazString_t *, topaz_script_native_function, void*)) api_nothing;
    api->script_run = (void (*)(topazScript_t *, void *, const topazString_t * sourceName, const topazString_t * scriptData)) api_nothing;
    api->script_expression = topaz_duk_expression;
    api->script_bootstrap = (void (*)(topazScript_t *, void *)) api_nothing;

}





