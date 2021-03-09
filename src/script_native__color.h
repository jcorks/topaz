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

    native->g = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__b_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->b = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__a_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);

    native->a = (uint8_t)(int)(topaz_script_object_as_number(arg1) * 255);
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



static topazScript_Object_t * color_api__cleanup(
    topazScript_t * script, 
    const topazArray_t * args, 
    void * userData
) {
    free(userData);
    TSO_NO_RETURN;
}






TSO_SCRIPT_API_FN(color_api__create) {
    topazColor_t * colorPtr = calloc(1, sizeof(topazColor_t));

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(colorPtr, TSO_OBJECT_ID__COLOR, color_api__cleanup, colorPtr);

    return object;
}


static void add_refs__color_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_color__create", color_api__create);

    // member functions
    TS_MAP_NATIVE_FN("topaz_color__to_hex_string", color_api__to_hex_string);
    TS_MAP_NATIVE_FN("topaz_color__set_from_string", color_api__set_from_string);
    
    
    TS_MAP_NATIVE_FN("topaz_color__set_r", color_api__r_set);
    TS_MAP_NATIVE_FN("topaz_color__set_g", color_api__g_set);
    TS_MAP_NATIVE_FN("topaz_color__set_b", color_api__b_set);
    TS_MAP_NATIVE_FN("topaz_color__set_a", color_api__a_set);

    TS_MAP_NATIVE_FN("topaz_color__get_r", color_api__r_get);
    TS_MAP_NATIVE_FN("topaz_color__get_g", color_api__g_get);
    TS_MAP_NATIVE_FN("topaz_color__get_b", color_api__b_get);
    TS_MAP_NATIVE_FN("topaz_color__get_a", color_api__a_get);


    TS_MAP_NATIVE_FN("topaz_color__set_r_int", color_api__r_int_set);
    TS_MAP_NATIVE_FN("topaz_color__set_g_int", color_api__g_int_set);
    TS_MAP_NATIVE_FN("topaz_color__set_b_int", color_api__b_int_set);
    TS_MAP_NATIVE_FN("topaz_color__set_a_int", color_api__a_int_set);
    
    TS_MAP_NATIVE_FN("topaz_color__get_r_int", color_api__r_int_get);
    TS_MAP_NATIVE_FN("topaz_color__get_g_int", color_api__g_int_get);
    TS_MAP_NATIVE_FN("topaz_color__get_b_int", color_api__b_int_get);
    TS_MAP_NATIVE_FN("topaz_color__get_a_int", color_api__a_int_get);

    
    
    
}
