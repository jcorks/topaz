#include <topaz/components/shape2d.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(shape2d_api__create) {
    topazComponent_t * component = topaz_shape2d_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__SHAPE2D, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}

TSO_SCRIPT_API_FN(shape2d_api__get_color_r) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(script, topaz_shape2d_get_color(native).r);
}

TSO_SCRIPT_API_FN(shape2d_api__get_color_g) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(script, topaz_shape2d_get_color(native).g);
}

TSO_SCRIPT_API_FN(shape2d_api__get_color_b) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(script, topaz_shape2d_get_color(native).b);
}

TSO_SCRIPT_API_FN(shape2d_api__get_color_a) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(script, topaz_shape2d_get_color(native).a);
}


TSO_SCRIPT_API_FN(shape2d_api__set_color_r) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazColor_t c = topaz_shape2d_get_color(native);
    c.r = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_color(native, c);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_color_g) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazColor_t c = topaz_shape2d_get_color(native);
    c.g = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_color(native, c);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_color_b) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazColor_t c = topaz_shape2d_get_color(native);
    c.b = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_color(native, c);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_color_a) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazColor_t c = topaz_shape2d_get_color(native);
    c.a = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_color(native, c);
    TSO_NO_RETURN;
}







TSO_SCRIPT_API_FN(shape2d_api__get_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);

    return topaz_script_object_from_number(
        script,
        topaz_renderer_attributes_get_attribute(
            topaz_shape2d_get_attributes(
                native
            ),
            topaz_script_object_as_number(arg1)            
        )
    );
}

TSO_SCRIPT_API_FN(shape2d_api__set_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazRenderer_Attributes_t att = *topaz_shape2d_get_attributes(native);
    topaz_renderer_attributes_set_attribute(
        &att,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    topaz_shape2d_set_attributes(native, &att);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__get_anim_speed) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(script, topaz_shape2d_get_anim_speed(native));
}

TSO_SCRIPT_API_FN(shape2d_api__set_anim_speed) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    topaz_shape2d_set_anim_speed(
        native,
        topaz_script_object_as_number(arg1)
    );
    TSO_NO_RETURN;

}


TSO_SCRIPT_API_FN(shape2d_api__get_center_x) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(
        script,
        topaz_shape2d_get_center(native)->x
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_center_y) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(
        script,
        topaz_shape2d_get_center(native)->y
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_center_z) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    return topaz_script_object_from_number(
        script,
        topaz_shape2d_get_center(native)->z
    );
}


TSO_SCRIPT_API_FN(shape2d_api__set_center_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazVector_t v = *topaz_shape2d_get_center(native);
    v.x = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_center(native, &v);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__set_center_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazVector_t v = *topaz_shape2d_get_center(native);
    v.y = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_center(native, &v);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__set_center_z) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazVector_t v = *topaz_shape2d_get_center(native);
    v.z = topaz_script_object_as_number(arg1);
    topaz_shape2d_set_center(native, &v);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(shape2d_api__get_rotation_x) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_rotation(topaz_shape2d_get_node(native))->x
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_rotation_y) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_rotation(topaz_shape2d_get_node(native))->y
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_rotation_z) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_rotation(topaz_shape2d_get_node(native))->z
    );
}


TSO_SCRIPT_API_FN(shape2d_api__get_position_x) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_position(topaz_shape2d_get_node(native))->x
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_position_y) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_position(topaz_shape2d_get_node(native))->y
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_position_z) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_position(topaz_shape2d_get_node(native))->z
    );
}



TSO_SCRIPT_API_FN(shape2d_api__get_scale_x) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_scale(topaz_shape2d_get_node(native))->x
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_scale_y) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_scale(topaz_shape2d_get_node(native))->y
    );
}

TSO_SCRIPT_API_FN(shape2d_api__get_scale_z) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    return topaz_script_object_from_number(
        script,
        topaz_transform_get_scale(topaz_shape2d_get_node(native))->z
    );
}



TSO_SCRIPT_API_FN(shape2d_api__set_rotation_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_rotation(topaz_shape2d_get_node(native))->x = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_rotation_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_rotation(topaz_shape2d_get_node(native))->y = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_rotation_z) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_rotation(topaz_shape2d_get_node(native))->z = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__set_position_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_position(topaz_shape2d_get_node(native))->x = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_position_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_position(topaz_shape2d_get_node(native))->y = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_position_z) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_position(topaz_shape2d_get_node(native))->z = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__set_scale_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_scale(topaz_shape2d_get_node(native))->x = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_scale_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_scale(topaz_shape2d_get_node(native))->y = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__set_scale_z) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topaz_transform_scale(topaz_shape2d_get_node(native))->z = 
        topaz_script_object_as_number(arg1)
    ;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__form_rectangle) {
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

TSO_SCRIPT_API_FN(shape2d_api__form_image) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   

    topaz_shape2d_form_image(
        native,
        native1
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__form_image_frame) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    if (topaz_script_object_get_type(arg2) == topazScript_Object_Type_Undefined) {
        topaz_shape2d_form_image_frame(
            native,
            NULL,
            topaz_script_object_as_int(arg2)
        );
    } else {
        TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
        topaz_shape2d_form_image_frame(
            native,
            native1,
            topaz_script_object_as_int(arg2)
        );

    }

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__form_image_scaled) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   


    if (topaz_script_object_get_type(arg2) == topazScript_Object_Type_Undefined) {
        topaz_shape2d_form_image_scaled(
            native,
            NULL,
            topaz_script_object_as_number(arg2),
            topaz_script_object_as_number(arg3)
        );
    } else {
        TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
        topaz_shape2d_form_image_scaled(
            native,
            native1,
            topaz_script_object_as_number(arg2),
            topaz_script_object_as_number(arg3)
        );

    }
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__form_radial) {
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
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        topazVector_t * v = calloc(len/2, sizeof(topazVector_t));
        for(i = 0; i < len; i+=2) {
            v[i/2].x = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
            v[i/2].y = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i+1));
        }
        topaz_shape2d_form_triangles(
            native,
            TOPAZ_ARRAY_CAST(v, topazVector_t, len/2)
        );
        free(v);
    }

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape2d_api__form_lines) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        topazVector_t * v = calloc(len/2, sizeof(topazVector_t));
        for(i = 0; i < len; i+=2) {
            v[i/2].x = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
            v[i/2].y = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i+1));
        }
        topaz_shape2d_form_lines(
            native,
            TOPAZ_ARRAY_CAST(v, topazVector_t, len/2)
        );
        free(v);
    }

    TSO_NO_RETURN;
}


static void add_refs__shape2d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_shape2d__create", shape2d_api__create, 0);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_color_r", shape2d_api__get_color_r, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_color_g", shape2d_api__get_color_g, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_color_b", shape2d_api__get_color_b, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_color_a", shape2d_api__get_color_a, 1);

    TS_MAP_NATIVE_FN("topaz_shape2d__set_color_r", shape2d_api__set_color_r, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_color_g", shape2d_api__set_color_g, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_color_b", shape2d_api__set_color_b, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_color_a", shape2d_api__set_color_a, 2);


    TS_MAP_NATIVE_FN("topaz_shape2d__get_attribute", shape2d_api__get_attribute, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_attribute", shape2d_api__set_attribute, 3);



    TS_MAP_NATIVE_FN("topaz_shape2d__get_anim_speed", shape2d_api__get_anim_speed, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_anim_speed", shape2d_api__set_anim_speed, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_center_x", shape2d_api__get_center_x, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_center_y", shape2d_api__get_center_y, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_center_z", shape2d_api__get_center_z, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_center_x", shape2d_api__set_center_x, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_center_y", shape2d_api__set_center_y, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_center_z", shape2d_api__set_center_z, 2);


    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_shape2d__get_rotation_x", shape2d_api__get_rotation_x, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_rotation_y", shape2d_api__get_rotation_y, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_rotation_z", shape2d_api__get_rotation_z, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_position_x", shape2d_api__get_position_x, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_position_y", shape2d_api__get_position_y, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_position_z", shape2d_api__get_position_z, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_scale_x", shape2d_api__get_scale_x, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_scale_y", shape2d_api__get_scale_y, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_scale_z", shape2d_api__get_scale_z, 1);

    TS_MAP_NATIVE_FN("topaz_shape2d__set_rotation_x", shape2d_api__set_rotation_x, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_rotation_y", shape2d_api__set_rotation_y, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_rotation_z", shape2d_api__set_rotation_z, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_position_x", shape2d_api__set_position_x, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_position_y", shape2d_api__set_position_y, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_position_z", shape2d_api__set_position_z, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_scale_x", shape2d_api__set_scale_x, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_scale_y", shape2d_api__set_scale_y, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_scale_z", shape2d_api__set_scale_z, 2);


    TS_MAP_NATIVE_FN("topaz_shape2d__form_rectangle", shape2d_api__form_rectangle, 3);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image", shape2d_api__form_image, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image_frame", shape2d_api__form_image_frame, 3);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image_scaled", shape2d_api__form_image_scaled, 4);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_radial", shape2d_api__form_radial, 3);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_triangles", shape2d_api__form_triangles, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_lines", shape2d_api__form_lines, 2);
    
}
