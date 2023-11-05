#include <topaz/components/text2d.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(text2d_api__create) {
    topazComponent_t * component = topaz_text2d_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__TEXT2D, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}

TSO_SCRIPT_API_FN(text2d_api__get_text) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_string(script, topaz_text2d_get_text(native));
}

TSO_SCRIPT_API_FN(text2d_api__set_font) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
    topaz_text2d_set_font(
        native, 
        native1,
        topaz_script_object_as_int(arg2)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(text2d_api__set_text) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topaz_text2d_set_text(
        native, 
        topaz_script_object_as_string(arg1)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(text2d_api__get_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);

    return topaz_script_object_from_number(
        script,
        topaz_text2d_get_attribute(
            native,
            topaz_script_object_as_number(arg1)            
        )
    );
}

TSO_SCRIPT_API_FN(text2d_api__set_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   

    topaz_text2d_set_attribute(native,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(text2d_api__get_extent_width) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_extent_width(native));
}

TSO_SCRIPT_API_FN(text2d_api__get_extent_height) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_extent_height(native));
}

TSO_SCRIPT_API_FN(text2d_api__get_char_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_char_x(native, topaz_script_object_as_int(arg1)));
}

TSO_SCRIPT_API_FN(text2d_api__get_char_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_char_y(native, topaz_script_object_as_int(arg1)));
}

TSO_SCRIPT_API_FN(text2d_api__set_color) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);

    topazColor_t c = {
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4)
    };
    topaz_text2d_set_color(native, c);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(text2d_api__set_color_section) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;
    TSO_ARG_6;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);

    topazColor_t c = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5),
        topaz_script_object_as_number(arg6)
    };


    topaz_text2d_set_color_section(
        native, 
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2),
        c
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(text2d_api__get_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topazVector_t a = *topaz_transform_get_rotation(topaz_text2d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(text2d_api__get_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topazVector_t a = *topaz_transform_get_position(topaz_text2d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(text2d_api__get_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topazVector_t a = *topaz_transform_get_scale(topaz_text2d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(text2d_api__set_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   

    topazVector_t * a = topaz_transform_rotation(topaz_text2d_get_node(native));
    a->x = topaz_script_object_as_number(arg1);
    a->y = topaz_script_object_as_number(arg2);
    a->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(text2d_api__set_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   

    topazVector_t * a = topaz_transform_position(topaz_text2d_get_node(native));
    a->x = topaz_script_object_as_number(arg1);
    a->y = topaz_script_object_as_number(arg2);
    a->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(text2d_api__set_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   

    topazVector_t * a = topaz_transform_scale(topaz_text2d_get_node(native));
    a->x = topaz_script_object_as_number(arg1);
    a->y = topaz_script_object_as_number(arg2);
    a->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}



static void add_refs__text2d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_text2d__create", text2d_api__create, 0);
    TS_MAP_NATIVE_FN("topaz_text2d__get_text", text2d_api__get_text, 1);
    TS_MAP_NATIVE_FN("topaz_text2d__set_text", text2d_api__set_text, 2);
    TS_MAP_NATIVE_FN("topaz_text2d__set_font", text2d_api__set_font, 3);

    TS_MAP_NATIVE_FN("topaz_text2d__get_attribute", text2d_api__get_attribute, 2);
    TS_MAP_NATIVE_FN("topaz_text2d__set_attribute", text2d_api__set_attribute, 3);


    TS_MAP_NATIVE_FN("topaz_text2d__get_extent_width", text2d_api__get_extent_width, 1);
    TS_MAP_NATIVE_FN("topaz_text2d__get_extent_height", text2d_api__get_extent_height, 1);
    TS_MAP_NATIVE_FN("topaz_text2d__get_char_x", text2d_api__get_char_x, 2);
    TS_MAP_NATIVE_FN("topaz_text2d__get_char_y", text2d_api__get_char_y, 2);

    TS_MAP_NATIVE_FN("topaz_text2d__set_color", text2d_api__set_color, 5);
    TS_MAP_NATIVE_FN("topaz_text2d__set_color_section", text2d_api__set_color_section, 7);


    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_text2d__get_rotation", text2d_api__get_rotation, 2);
    TS_MAP_NATIVE_FN("topaz_text2d__get_position", text2d_api__get_position, 2);
    TS_MAP_NATIVE_FN("topaz_text2d__get_scale", text2d_api__get_scale, 2);

    TS_MAP_NATIVE_FN("topaz_text2d__set_rotation", text2d_api__set_rotation, 4);
    TS_MAP_NATIVE_FN("topaz_text2d__set_position", text2d_api__set_position, 4);
    TS_MAP_NATIVE_FN("topaz_text2d__set_scale", text2d_api__set_scale, 4);

}
