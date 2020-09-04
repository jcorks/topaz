#include <topaz/modules/script_manager.h>
#include <topaz/backends/script.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <topaz/system.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>
struct topazScriptManager_t {
    topaz_t * ctx;
    topazArray_t * scripts;
};


static void add_all_refs(topazScriptManager_t * s, topazScript_t *, int permissions);


topazScriptManager_t * topaz_script_manager_create(
    topaz_t * t) {

    topazScriptManager_t * out = calloc(1, sizeof(topazScriptManager_t));
    out->ctx = t;
    out->scripts = topaz_array_create(sizeof(topazScript_t*));
    return out;
}




void script_manager_destroy(topazScriptManager_t * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->scripts);
    for(i = 0; i < len; ++i) {
        topaz_script_destroy(
            topaz_array_at(
                s->scripts, 
                topazScript_t *,
                i
            )
        );
    }

    free(s);
}


topazScript_t * topaz_script_manager_create_context(
    topazScriptManager_t * s,
    topazScriptManager_Permission_t permissions
) {
    topazScriptAPI_t api;
    topazBackend_t * backend = topaz_system_create_backend(
        topaz_context_get_system(s->ctx), 
        TOPAZ_STR_CAST("script"), 
        &api
    );

    topazScript_t * out = topaz_script_create(backend, &api);

    
    add_all_refs(
        s,
        out,
        permissions
    );

    topaz_script_bootstrap(out);
    return out;
}


static void add_refs__color_api(topazScript_t * script);
static void add_refs__vector_api(topazScript_t * script);

static void add_all_refs(topazScriptManager_t * s, topazScript_t * script, int permissions) {
    add_refs__vector_api(script);
    add_refs__color_api(script);
}










///////////////////////
/////////////////////// Linking utilities
///////////////////////



// utility function for script errors
static void script_error(topazScript_t * src, const char * message) {
    topazScript_Object_t * messageObj = topaz_script_object_from_string(src, TOPAZ_STR_CAST(message));
    topaz_script_emit_event(
        src,
        topaz_Script_Event_OnError,
        TOPAZ_ARRAY_CAST(&messageObj, topazScript_Object_t *, 1)
    );
    topaz_script_object_destroy(messageObj);
}


static void * script_create_native_object(topazScript_t * script, topaz_script_native_function create, void ** nativeRef) {
    topazScript_Object_t * obj = create(script, TOPAZ_ARRAY_CAST(NULL, void*, 0), NULL);
    int unused;
    *nativeRef = topaz_script_object_reference_get_native_data(obj, &unused);
    return obj;
}



// main macro for defining functions
#define TSO_SCRIPT_API_FN(__name__) static topazScript_Object_t * __name__(topazScript_t * script, const topazArray_t * args, void * userData)


// macro for adding a property.
// requires:
// - TSO_OBJECT_NEW to be called within the same scope
#define TSO_PROP_ADD(__s__, __os__, __og__) topaz_script_object_reference_extendable_add_property(object, TOPAZ_STR_CAST(__s__), __os__, __og__)

// Creates a new object to be modified in the current scope.
// __native__ is the variable to that contains the native object 
// __tag__ is the unique ID tag for this type of object. this is varified unpon functions calls.
// __remover__ is the native function to call when the object reference is destroyed in the script context.
#define TSO_OBJECT_NEW(__native__, __tag__, __remover__) topazScript_Object_t * object = topaz_script_create_empty_object(script, __remover__, NULL); topaz_script_object_reference_set_native_data(object, __native__, __tag__); 

// Creates an object from a different context and returns its object and native ref.
#define TSO_OBJECT_INSTANTIATE(__nativecreate__, __native__) script_create_native_object(script, __nativecreate__, (void**)&__native__);

// Tells the scripting engine to prevent garbage collection on this newly created objectobject.
#define TSO_OBJECT_KEEP topaz_script_object_reference_ref(object);

// Tells the scripting engine that it can garbage collect arg0 if desired.
#define TSO_OBJECT_UNKEEP() topaz_script_object_reference_unref(arg0);


// Ensures that, past this point, AT LEAST the given number of args is 
// passed to this function. If not, an error is "thrown"
#define TSO_ASSERT_ARG_COUNT(__count__) if (topaz_array_get_size(args) < __count__){script_error(script, "Insufficient arguments given to native function."); return topaz_script_object_undefined(script);}

// Retrieves and stores the given args
#define TSO_ARG_0 topazScript_Object_t * arg0 = topaz_array_at(args, topazScript_Object_t *, 0);
#define TSO_ARG_1 topazScript_Object_t * arg1 = topaz_array_at(args, topazScript_Object_t *, 1);
#define TSO_ARG_2 topazScript_Object_t * arg2 = topaz_array_at(args, topazScript_Object_t *, 2);
#define TSO_ARG_3 topazScript_Object_t * arg3 = topaz_array_at(args, topazScript_Object_t *, 3);

// Retrieves argN as a native object of the given type.
// If it is not the given type, an error is "thrown".
#define TSO_NATIVIZE(__type__, __assertTag__) int tag__;__type__ native = topaz_script_object_reference_get_native_data(arg0, &tag__); if (tag__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}
#define TSO_NATIVIZE_1(__type__, __assertTag__) int tag1__;__type__ native1 = topaz_script_object_reference_get_native_data(arg1, &tag1__); if (tag1__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}
#define TSO_NATIVIZE_2(__type__, __assertTag__) int tag2__;__type__ native2 = topaz_script_object_reference_get_native_data(arg2, &tag2__); if (tag2__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}
#define TSO_NATIVIZE_3(__type__, __assertTag__) int tag3__;__type__ native3 = topaz_script_object_reference_get_native_data(arg3, &tag3__); if (tag3__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}

// Default return value.
#define TSO_NO_RETURN return topaz_script_object_undefined(script);

// macro for assigning a native function
#define TS_MAP_NATIVE_FN(__name__, __fn__) topaz_script_map_native_function(script, TOPAZ_STR_CAST(__name__), __fn__, NULL)







////////////////////
//////////////////// Vector
////////////////////
/// Since vectors dont have a "managed life" on their own, we wont set 
#include <topaz/vector.h>
#define TSO_OBJECT_ID__VECTOR 101


TSO_SCRIPT_API_FN(vector_api__x_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    native->x = topaz_script_object_as_number(arg1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(vector_api__y_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    native->y = topaz_script_object_as_number(arg1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(vector_api__z_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    native->z = topaz_script_object_as_number(arg1);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(vector_api__x_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(script, native->x);    
}
TSO_SCRIPT_API_FN(vector_api__y_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(script, native->x);    
}
TSO_SCRIPT_API_FN(vector_api__z_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(script, native->x);    
}


TSO_SCRIPT_API_FN(vector_api__destroy) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    free(native);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(vector_api__create) {
    topazVector_t * ptr = calloc(1, sizeof(topazVector_t));

    if (topaz_array_get_size(args) == 1) { // from string
        TSO_ARG_0;
        *ptr = topaz_vector_from_string(topaz_script_object_as_string(arg0));
    } else if (topaz_array_get_size(args) == 3) {// from xyz
        TSO_ARG_0;
        TSO_ARG_1;
        TSO_ARG_2;
        ptr->x = topaz_script_object_as_number(arg0); 
        ptr->y = topaz_script_object_as_number(arg1); 
        ptr->z = topaz_script_object_as_number(arg2); 
    }

    // creates new object and sets native pointer
    TSO_OBJECT_NEW(ptr, TSO_OBJECT_ID__VECTOR, vector_api__destroy);

    // uses implicit names to add ref
    TSO_PROP_ADD("x", vector_api__x_set, vector_api__x_get);
    TSO_PROP_ADD("y", vector_api__y_set, vector_api__y_get);
    TSO_PROP_ADD("z", vector_api__z_set, vector_api__z_get);

    return object;
}








TSO_SCRIPT_API_FN(vector_api__get_length) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_get_length(
            native
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__get_distance) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_get_distance(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__normalize) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    topaz_vector_normalize(native);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(vector_api__cross) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    topazVector_t v = topaz_vector_cross(
        native,
        native1
    );

    topazVector_t * outNative;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, outNative);

    *outNative = v;
    return out;
}

TSO_SCRIPT_API_FN(vector_api__floor) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    topaz_vector_floor(native);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(vector_api__rotation_x_diff) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_x_diff(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_x_diff_relative) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_x_diff_relative(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_x) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_x(
            native
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_y_diff) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_y_diff(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_y_diff_relative) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_y_diff_relative(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_y) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_y(
            native
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_z_diff) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_z_diff(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_z_diff_relative) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_z_diff_relative(
            native,
            native1
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__rotation_z) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(
        script,
        topaz_vector_rotation_z(
            native
        )
    );    
}


TSO_SCRIPT_API_FN(vector_api__rotate_x) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);


    topaz_vector_rotate_x(
        native,
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(vector_api__rotate_y) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);


    topaz_vector_rotate_y(
        native,
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(vector_api__rotate_z) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);


    topaz_vector_rotate_z(
        native,
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;
}



static void add_refs__vector_api(topazScript_t * script) {
    TS_MAP_NATIVE_FN("_topaz_vector__create", vector_api__create);

    // member functions
    TS_MAP_NATIVE_FN("_topaz_vector__get_length", vector_api__get_length);
    TS_MAP_NATIVE_FN("_topaz_vector__get_distance", vector_api__get_distance);
    TS_MAP_NATIVE_FN("_topaz_vector__normalize", vector_api__normalize);
    TS_MAP_NATIVE_FN("_topaz_vector__cross", vector_api__cross);
    TS_MAP_NATIVE_FN("_topaz_vector__floor", vector_api__floor);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_x_diff", vector_api__rotation_x_diff);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_x_diff_relative", vector_api__rotation_x_diff_relative);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_x", vector_api__rotation_x);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_y_diff", vector_api__rotation_y_diff);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_y_diff_relative", vector_api__rotation_y_diff_relative);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_y", vector_api__rotation_y);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_z_diff", vector_api__rotation_z_diff);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_z_diff_relative", vector_api__rotation_z_diff_relative);
    TS_MAP_NATIVE_FN("_topaz_vector__rotation_z", vector_api__rotation_z);
    
    // these modify the existing vector
    TS_MAP_NATIVE_FN("_topaz_vector__rotate_x", vector_api__rotate_x);
    TS_MAP_NATIVE_FN("_topaz_vector__rotate_y", vector_api__rotate_y);
    TS_MAP_NATIVE_FN("_topaz_vector__rotate_z", vector_api__rotate_z);

}
////////////////////
//////////////////// Vector
////////////////////









////////////////////
//////////////////// Color 
////////////////////
#include <topaz/color.h>
#define TSO_OBJECT_ID__COLOR 100
TSO_SCRIPT_API_FN(color_api__r_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__g_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__b_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__a_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
    TSO_NO_RETURN;
}





TSO_SCRIPT_API_FN(color_api__r_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_number(script, topaz_color_r_amt((*native)));    
}


TSO_SCRIPT_API_FN(color_api__g_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_number(script, topaz_color_g_amt((*native)));    
}

TSO_SCRIPT_API_FN(color_api__b_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_number(script, topaz_color_b_amt((*native)));    
}

TSO_SCRIPT_API_FN(color_api__a_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_number(script, topaz_color_a_amt((*native)));    
}





TSO_SCRIPT_API_FN(color_api__r_int_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->r = topaz_script_object_as_int(arg1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__g_int_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->g = topaz_script_object_as_int(arg1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__b_int_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->b = topaz_script_object_as_int(arg1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__a_int_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->a = topaz_script_object_as_int(arg1);
    TSO_NO_RETURN;
}





TSO_SCRIPT_API_FN(color_api__r_int_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_int(script, native->a);    
}


TSO_SCRIPT_API_FN(color_api__g_int_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_int(script, native->g);    
}

TSO_SCRIPT_API_FN(color_api__b_int_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_int(script, native->b);    
}

TSO_SCRIPT_API_FN(color_api__a_int_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    return topaz_script_object_from_int(script, native->a);    
}





TSO_SCRIPT_API_FN(color_api__to_hex_string) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);   
    return topaz_script_object_from_string(script, topaz_color_to_string(*native));
}

TSO_SCRIPT_API_FN(color_api__set_from_string) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);   

    *native = topaz_color_from_string(
        topaz_script_object_as_string(arg1)
    );
    TSO_NO_RETURN;
}






TSO_SCRIPT_API_FN(color_api__destroy) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    free(native);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(color_api__create) {
    topazColor_t * colorPtr = calloc(1, sizeof(topazColor_t));

    // creates new object and sets native pointer
    TSO_OBJECT_NEW(colorPtr, TSO_OBJECT_ID__COLOR, color_api__destroy);

    // uses implicit names to add ref
    TSO_PROP_ADD("r", color_api__r_set, color_api__r_get);
    TSO_PROP_ADD("g", color_api__g_set, color_api__g_get);
    TSO_PROP_ADD("b", color_api__b_set, color_api__b_get);
    TSO_PROP_ADD("a", color_api__a_set, color_api__a_get);

    TSO_PROP_ADD("rInt", color_api__r_int_set, color_api__r_int_get);
    TSO_PROP_ADD("gInt", color_api__g_int_set, color_api__g_int_get);
    TSO_PROP_ADD("bInt", color_api__b_int_set, color_api__b_int_get);
    TSO_PROP_ADD("aInt", color_api__a_int_set, color_api__a_int_get);

    return object;
}




static void add_refs__color_api(topazScript_t * script) {
    TS_MAP_NATIVE_FN("_topaz_color__create", color_api__create);

    // member functions
    TS_MAP_NATIVE_FN("_topaz_color__to_hex_string", color_api__to_hex_string);
    TS_MAP_NATIVE_FN("_topaz_color__set_from_string", color_api__set_from_string);
}

////////////////////
//////////////////// Color 
////////////////////




