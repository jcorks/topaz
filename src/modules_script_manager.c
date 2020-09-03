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

static void add_all_refs(topazScriptManager_t * s, topazScript_t * script, int permissions) {
    add_refs__color_api(script);
}


// main macro for defining functions
#define TSO_SCRIPT_API_FN(__name__) static topazScript_Object_t * __name__(topazScript_t * script, const topazArray_t * args)

#define TSO_PROP_ADD(__s__, __dv__, __os__, __og__) topaz_script_object_reference_extendable_add_property(object, TOPAZ_STR_CAST(__s__), __dv__, __os__, __og__)

#define TSO_METHOD_ADD(__s__, __fn__) topaz_script_object_reference_extendable_add_property(object, TOPAZ_STR_CAST(__s__), __fn__)

#define TSO_OBJECT_NEW(__native__) topazScript_Object_t * object = topaz_script_create_object_new(script); topaz_script_object_reference_set_native_data(object, __native__); topaz_script_object_reference_ref(object);

#define TSO_OBJECT_REMOVE() topaz_script_object_reference_unref(object);

#define TSO_ARG(__n__) topazScript_Object_t * arg__n__ topaz_array_at(args, topazScript_Object_t *, __n__)

#define TSO_NATIVIZE(__type__) __type__ native = topaz_script_object_reference_get_native_data(object);

#define TSO_NO_RETURN() return topaz_script_object_undefined(script);

#define TS_MAP_NATIVE_FN(__name__, __fn__) topaz_script_map_native_function(script, TOPAZ_STR_CAST(__name__), __fn__, NULL)










////////////////////
//////////////////// Color 
////////////////////
#include <topaz/color.h>

TSO_SCRIPT_API_FN(color_api__r_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg0) * 255);
    TSO_NO_RETURN();
}

TSO_SCRIPT_API_FN(color_api__g_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg0) * 255);
    TSO_NO_RETURN();
}

TSO_SCRIPT_API_FN(color_api__b_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg0) * 255);
    TSO_NO_RETURN();
}

TSO_SCRIPT_API_FN(color_api__a_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->r = (uint8_t)(int)(topaz_script_object_as_number(arg0) * 255);
    TSO_NO_RETURN();
}





TSO_SCRIPT_API_FN(color_api__r_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_number(topaz_color_r_amt(*native));    
}


TSO_SCRIPT_API_FN(color_api__g_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_number(topaz_color_g_amt(*native));    
}

TSO_SCRIPT_API_FN(color_api__b_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_number(topaz_color_b_amt(*native));    
}

TSO_SCRIPT_API_FN(color_api__a_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_number(topaz_color_a_amt(*native));    
}





TSO_SCRIPT_API_FN(color_api__r_int_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->r = topaz_script_object_as_int(arg0);
    TSO_NO_RETURN();
}

TSO_SCRIPT_API_FN(color_api__g_int_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->g = topaz_script_object_as_int(arg0);
    TSO_NO_RETURN();
}

TSO_SCRIPT_API_FN(color_api__b_int_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->b = topaz_script_object_as_int(arg0);
    TSO_NO_RETURN();
}

TSO_SCRIPT_API_FN(color_api__a_int_set) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    native->a = topaz_script_object_as_int(arg0);
    TSO_NO_RETURN();
}





TSO_SCRIPT_API_FN(color_api__r_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_int(native->a);    
}


TSO_SCRIPT_API_FN(color_api__g_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_int(native->g);    
}

TSO_SCRIPT_API_FN(color_api__b_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_int(native->b);    
}

TSO_SCRIPT_API_FN(color_api__a_get) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);

    return topaz_script_object_from_int(native->a);    
}





TSO_SCRIPT_API_FN(color_api__to_hex_string) {
    TSO_NATIVIZE(topazColor_t *);    
    return topaz_script_object_from_string(topaz_color_to_string(*native));
}

TSO_SCRIPT_API_FN(color_api__set_from_string) {
    TSO_NATIVIZE(topazColor_t *);    
    TSO_ARG(0);

    *native = topaz_color_from_string(
        topaz_script_object_from_string(arg0)
    );
}







TSO_SCRIPT_API_FN(color_api__create) {
    topazColor_t * colorPtr = calloc(1, sizeof(colorPtr));

    // creates new object and sets native pointer
    TSO_OBJECT_NEW(colorPtr);

    // uses implicit names to add ref
    TSO_PROP_ADD("r", color_api__r_set, color_api__r_get);
    TSO_PROP_ADD("g", color_api__g_set, color_api__g_get);
    TSO_PROP_ADD("b", color_api__b_set, color_api__b_get);
    TSO_PROP_ADD("a", color_api__a_set, color_api__a_get);

    TSO_PROP_ADD("rInt", color_api__r_int_set, color_api__r_int_get);
    TSO_PROP_ADD("gInt", color_api__g_int_set, color_api__g_int_get);
    TSO_PROP_ADD("bInt", color_api__b_int_set, color_api__b_int_get);
    TSO_PROP_ADD("aInt", color_api__a_int_set, color_api__a_int_get);

    TSO_METHOD_ADD("toHexString",      color_api__to_hex_string);
    TSO_METHOD_ADD("setFromString", color_api__set_from_string);

    return object;
}

TSO_SCRIPT_API_FN(color_api__destroy) {
    TSO_ARG(0);
    TSO_NATIVIZE(topazColor_t *);
    TSO_OBJECT_REMOVE();
    free(native);
}




static add_refs__color_api(topazScript_t * script) {
    TS_MAP_NATIVE_FN("topaz_color_create", color_api__create);
    TS_MAP_NATIVE_FN("topaz_color_destroy", color_api__destroy);

}

////////////////////
//////////////////// Color 
////////////////////




