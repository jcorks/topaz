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
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_string(script, topaz_text2d_get_text(native));
}

TSO_SCRIPT_API_FN(text2d_api__set_text) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topaz_text2d_set_text(
        native, 
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_int(arg2)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(text2d_api__set_text_monospace) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topaz_text2d_set_text_monospace(
        native, 
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_int(arg2)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(text2d_api__get_parameter) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);

    return topaz_script_object_from_number(
        script,
        topaz_text2d_get_parameter(
            native,
            topaz_script_object_as_number(arg1)
        )
    );
}

TSO_SCRIPT_API_FN(text2d_api__set_parameter) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   


    topaz_text2d_set_parameter(
        native,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );  
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(text2d_api__get_extent_width) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_extent_width(native));
}

TSO_SCRIPT_API_FN(text2d_api__get_extent_height) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_extent_height(native));
}

TSO_SCRIPT_API_FN(text2d_api__get_char_x) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_char_x(native, topaz_script_object_as_int(arg1)));
}

TSO_SCRIPT_API_FN(text2d_api__get_char_y) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    return topaz_script_object_from_number(script, topaz_text2d_get_char_y(native, topaz_script_object_as_int(arg1)));
}

TSO_SCRIPT_API_FN(text2d_api__set_color) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    TSO_NATIVIZE_1(topazColor_t *, TSO_OBJECT_ID__COLOR);   
    topaz_text2d_set_color(native, *native1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(text2d_api__set_color_section) {
    TSO_ASSERT_ARG_COUNT(4);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);
    TSO_NATIVIZE_3(topazColor_t *, TSO_OBJECT_ID__COLOR);   
    topaz_text2d_set_color_section(
        native, 
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2),
        *native3
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(text2d_api__get_rotation) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_rotation(topaz_text2d_get_node(native));
    return out;
}

TSO_SCRIPT_API_FN(text2d_api__get_position) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_position(topaz_text2d_get_node(native));
    return out;
}

TSO_SCRIPT_API_FN(text2d_api__get_scale) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_scale(topaz_text2d_get_node(native));
    return out;
}




TSO_SCRIPT_API_FN(text2d_api__set_rotation) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_rotation(topaz_text2d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(text2d_api__set_position) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_position(topaz_text2d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(text2d_api__set_scale) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__TEXT2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_scale(topaz_text2d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}


static void add_refs__text2d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_text2d__create", text2d_api__create);
    TS_MAP_NATIVE_FN("topaz_text2d__get_text", text2d_api__get_text);
    TS_MAP_NATIVE_FN("topaz_text2d__set_text", text2d_api__set_text);
    TS_MAP_NATIVE_FN("topaz_text2d__set_text_monospace", text2d_api__set_text_monospace);

    TS_MAP_NATIVE_FN("topaz_text2d__get_parameter", text2d_api__get_parameter);
    TS_MAP_NATIVE_FN("topaz_text2d__set_parameter", text2d_api__set_parameter);


    TS_MAP_NATIVE_FN("topaz_text2d__get_extent_width", text2d_api__get_extent_width);
    TS_MAP_NATIVE_FN("topaz_text2d__get_extent_height", text2d_api__get_extent_height);
    TS_MAP_NATIVE_FN("topaz_text2d__get_char_x", text2d_api__get_char_x);
    TS_MAP_NATIVE_FN("topaz_text2d__get_char_y", text2d_api__get_char_y);

    TS_MAP_NATIVE_FN("topaz_text2d__set_color", text2d_api__set_color);
    TS_MAP_NATIVE_FN("topaz_text2d__set_color_section", text2d_api__set_color_section);


    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_text2d__get_position", text2d_api__get_position);
    TS_MAP_NATIVE_FN("topaz_text2d__get_rotation", text2d_api__get_rotation);
    TS_MAP_NATIVE_FN("topaz_text2d__get_scale",    text2d_api__get_scale);
    TS_MAP_NATIVE_FN("topaz_text2d__set_position", text2d_api__set_position);
    TS_MAP_NATIVE_FN("topaz_text2d__set_rotation", text2d_api__set_rotation);
    TS_MAP_NATIVE_FN("topaz_text2d__set_scale",    text2d_api__set_scale);

}
