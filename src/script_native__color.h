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









TSO_SCRIPT_API_FN(color_api__create) {
    topazColor_t * colorPtr = calloc(1, sizeof(topazColor_t));

    // creates new object and sets native pointer
    TSO_OBJECT_NEW(colorPtr, TSO_OBJECT_ID__COLOR, NULL);

    // uses implicit names to add ref
    TSO_PROP_ADD("r", color_api__r_set, color_api__r_get);
    TSO_PROP_ADD("g", color_api__g_set, color_api__g_get);
    TSO_PROP_ADD("b", color_api__b_set, color_api__b_get);
    TSO_PROP_ADD("a", color_api__a_set, color_api__a_get);

    TSO_PROP_ADD("rInt", color_api__r_int_set, color_api__r_int_get);
    TSO_PROP_ADD("gInt", color_api__g_int_set, color_api__g_int_get);
    TSO_PROP_ADD("bInt", color_api__b_int_set, color_api__b_int_get);
    TSO_PROP_ADD("aInt", color_api__a_int_set, color_api__a_int_get);

    TSO_OBJECT_KEEP;
    return object;
}

TSO_SCRIPT_API_FN(color_api__destroy) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazColor_t *, TSO_OBJECT_ID__COLOR);
    free(native);
    TSO_OBJECT_UNKEEP;
    TSO_NO_RETURN;
}   

static void add_refs__color_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_color__create", color_api__create);
    TS_MAP_NATIVE_FN("topaz_color__", color_api__destroy);

    // member functions
    TS_MAP_NATIVE_FN("topaz_color__to_hex_string", color_api__to_hex_string);
    TS_MAP_NATIVE_FN("topaz_color__set_from_string", color_api__set_from_string);
}
