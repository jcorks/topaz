#include <topaz/color.h>
#define TSO_OBJECT_ID__COLOR 100



static void topaz_script_return_color(
    topazScript_t * script, 
    topazScript_Object_t * fn,
    float r,
    float g,
    float b,
    float a
) {

    topazScript_Object_t * args[] = {
        topaz_script_object_from_number(script, r),
        topaz_script_object_from_number(script, g),
        topaz_script_object_from_number(script, b),
        topaz_script_object_from_number(script, a)
    };


    topaz_script_object_reference_call(
        fn,
        TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 4)
    );

    topaz_script_object_destroy(args[0]);
    topaz_script_object_destroy(args[1]);
    topaz_script_object_destroy(args[2]);
    topaz_script_object_destroy(args[3]);

}




TSO_SCRIPT_API_FN(color_api__set_from_string) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazColor_t c = topaz_color_from_string(
        topaz_script_object_as_string(arg0)
    );
    topaz_script_return_color(
        script,
        arg1,
        c.r,
        c.g,
        c.b,
        c.a
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(color_api__to_hex_string) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    topazColor_t c = {
        topaz_script_object_as_number(arg0),        
        topaz_script_object_as_number(arg1),        
        topaz_script_object_as_number(arg2),        
        topaz_script_object_as_number(arg3)    
    };
    topazString_t * str = topaz_color_to_string(c);
    topazScript_Object_t * o = topaz_script_object_from_string(
        script,
        str
    );

    topaz_string_destroy(str);
    return o;
}





static void add_refs__color_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_color__to_hex_string", color_api__to_hex_string, 4);
    TS_MAP_NATIVE_FN("topaz_color__set_from_string", color_api__set_from_string, 2);
}
