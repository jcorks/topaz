#include <topaz/components/shape2d.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(shape2d_api__create) {
    topazComponent_t * component = topaz_shape2d_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__SHAPE2D, NULL);
    return object;
}

TSO_SCRIPT_API_FN(shape2d_api__get_color) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    topazColor_t * c;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(color_api__create, c);
    *c = topaz_shape2d_get_color(native);
    return out;
}

TSO_SCRIPT_API_FN(shape2d_api__set_color) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazColor_t *, TSO_OBJECT_ID__COLOR);   

    topaz_shape2d_set_color(native, *native1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__get_anim_speed) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(script, topaz_shape2d_get_anim_speed(native));
}

TSO_SCRIPT_API_FN(shape2d_api__set_anim_speed) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    topaz_shape2d_set_anim_speed(
        native,
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;

}


TSO_SCRIPT_API_FN(shape2d_api__get_center) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    topazVector_t * c;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, c);
    *c = *topaz_shape2d_get_center(native);
    return out;
}

TSO_SCRIPT_API_FN(shape2d_api__set_center) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    topaz_shape2d_set_center(native, native1);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(shape2d_api__get_rotation) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_rotation(topaz_shape2d_get_node(native));
    return out;
}

TSO_SCRIPT_API_FN(shape2d_api__get_position) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_position(topaz_shape2d_get_node(native));
    return out;
}

TSO_SCRIPT_API_FN(shape2d_api__get_scale) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_scale(topaz_shape2d_get_node(native));
    return out;
}




TSO_SCRIPT_API_FN(shape2d_api__set_rotation) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_rotation(topaz_shape2d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_position) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_position(topaz_shape2d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_scale) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_scale(topaz_shape2d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__form_rectangle) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_shape2d_form_rectangle(
        native,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__form_radial) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_shape2d_form_radial(
        native,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__form_triangles) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        topazVector_t * v = calloc(len, sizeof(topazVector_t));
        for(i = 0; i < len; i+=2) {
            v[i/2].x = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
            v[i/2].y = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
        }
        topaz_shape2d_form_triangles(
            native,
            TOPAZ_ARRAY_CAST(v, topazVector_t, len)
        );
        free(v);
    }

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__form_lines) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        topazVector_t * v = calloc(len, sizeof(topazVector_t));
        for(i = 0; i < len; i+=2) {
            v[i/2].x = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
            v[i/2].y = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i+1));
        }
        topaz_shape2d_form_lines(
            native,
            TOPAZ_ARRAY_CAST(v, topazVector_t, len)
        );
        free(v);
    }

    TSO_NO_RETURN;
}


static void add_refs__shape2d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_shape2d__create", shape2d_api__create);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_color", shape2d_api__get_color);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_color", shape2d_api__set_color);



    TS_MAP_NATIVE_FN("topaz_shape2d__get_anim_speed", shape2d_api__get_anim_speed);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_anim_speed", shape2d_api__set_anim_speed);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_center", shape2d_api__get_center);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_center", shape2d_api__set_center);


    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_shape2d__get_position", shape2d_api__get_position);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_rotation", shape2d_api__get_rotation);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_scale",    shape2d_api__get_scale);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_position", shape2d_api__set_position);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_rotation", shape2d_api__set_rotation);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_scale",    shape2d_api__set_scale);

    TS_MAP_NATIVE_FN("topaz_shape2d__form_rectangle", shape2d_api__form_rectangle);
    //TS_MAP_NATIVE_FN("topaz_shape2d__form_image", shape2d_api__form_image);
    //TS_MAP_NATIVE_FN("topaz_shape2d__form_image_scaled", shape2d_api__form_image_scaled);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_radial", shape2d_api__form_radial);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_triangles", shape2d_api__form_triangles);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_lines", shape2d_api__form_lines);

}
