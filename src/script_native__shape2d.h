#include <topaz/components/shape2d.h>
#include <topaz/transform.h>


TSO_SCRIPT_API_FN(shape2d_api__create) {
    topazComponent_t * component = topaz_shape2d_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__SHAPE2D, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    component_script_object_bind_destroy(component, script, context);
    return object;
}

TSO_SCRIPT_API_FN(shape2d_api__get_color) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);

    topazColor_t c =  topaz_shape2d_get_color(native);
    topaz_script_return_color(script, arg1, c.r, c.g, c.b, c.a);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(shape2d_api__set_color) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topazColor_t c = {};
    c.r = topaz_script_object_as_number(arg1);
    c.g = topaz_script_object_as_number(arg2);
    c.b = topaz_script_object_as_number(arg3);
    c.a = topaz_script_object_as_number(arg4);
    topaz_shape2d_set_color(native, c);
    TSO_NO_RETURN;
}







TSO_SCRIPT_API_FN(shape2d_api__get_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);

    return topaz_script_object_from_number(
        script,
        topaz_shape2d_get_attribute(
            native,
            topaz_script_object_as_number(arg1)            
        )
    );
}

TSO_SCRIPT_API_FN(shape2d_api__set_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   

    topaz_shape2d_set_attribute(
        native,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
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


TSO_SCRIPT_API_FN(shape2d_api__get_center) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);
    topazVector_t a = *topaz_shape2d_get_center(native);
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape2d_api__set_center) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = {
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)
    };
    
    topaz_shape2d_set_center(native, &a);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(shape2d_api__get_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = *topaz_transform_get_rotation(topaz_shape2d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;    
}



TSO_SCRIPT_API_FN(shape2d_api__get_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = *topaz_transform_get_position(topaz_shape2d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;    
}





TSO_SCRIPT_API_FN(shape2d_api__get_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = *topaz_transform_get_scale(topaz_shape2d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;    
}



TSO_SCRIPT_API_FN(shape2d_api__set_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * a = topaz_transform_rotation(topaz_shape2d_get_node(native));
    a->x = topaz_script_object_as_number(arg1);
    a->y = topaz_script_object_as_number(arg2);
    a->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(shape2d_api__set_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * a = topaz_transform_position(topaz_shape2d_get_node(native));
    a->x = topaz_script_object_as_number(arg1);
    a->y = topaz_script_object_as_number(arg2);
    a->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(shape2d_api__set_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * a = topaz_transform_scale(topaz_shape2d_get_node(native));
    a->x = topaz_script_object_as_number(arg1);
    a->y = topaz_script_object_as_number(arg2);
    a->z = topaz_script_object_as_number(arg3);
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

TSO_SCRIPT_API_FN(shape2d_api__form_image_frame_scaled) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   


    if (topaz_script_object_get_type(arg2) == topazScript_Object_Type_Undefined) {
        topaz_shape2d_form_image_frame_scaled(
            native,
            NULL,
            topaz_script_object_as_number(arg2),
            topaz_script_object_as_number(arg3),
            topaz_script_object_as_number(arg4)
        );
    } else {
        TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
        topaz_shape2d_form_image_frame_scaled(
            native,
            native1,
            topaz_script_object_as_number(arg2),
            topaz_script_object_as_number(arg3),
            topaz_script_object_as_number(arg4)
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
            topazScript_Object_t * x = topaz_script_object_reference_array_get_nth(arg1, i);
            topazScript_Object_t * y = topaz_script_object_reference_array_get_nth(arg1, i+1);
            v[i/2].x = topaz_script_object_as_number(x);
            v[i/2].y = topaz_script_object_as_number(y);
            topaz_script_object_destroy(x);
            topaz_script_object_destroy(y);
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
            topazScript_Object_t * x = topaz_script_object_reference_array_get_nth(arg1, i);
            topazScript_Object_t * y = topaz_script_object_reference_array_get_nth(arg1, i+1);
            v[i/2].x = topaz_script_object_as_number(x);
            v[i/2].y = topaz_script_object_as_number(y);
            topaz_script_object_destroy(x);
            topaz_script_object_destroy(y);
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
    TS_MAP_NATIVE_FN("topaz_shape2d__get_color", shape2d_api__get_color, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_color", shape2d_api__set_color, 5);


    TS_MAP_NATIVE_FN("topaz_shape2d__get_attribute", shape2d_api__get_attribute, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_attribute", shape2d_api__set_attribute, 3);



    TS_MAP_NATIVE_FN("topaz_shape2d__get_anim_speed", shape2d_api__get_anim_speed, 1);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_anim_speed", shape2d_api__set_anim_speed, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_center", shape2d_api__get_center, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_center", shape2d_api__set_center, 4);


    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_shape2d__get_rotation", shape2d_api__get_rotation, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_position", shape2d_api__get_position, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__get_scale", shape2d_api__get_scale, 2);

    TS_MAP_NATIVE_FN("topaz_shape2d__set_rotation", shape2d_api__set_rotation, 4);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_position", shape2d_api__set_position, 4);
    TS_MAP_NATIVE_FN("topaz_shape2d__set_scale", shape2d_api__set_scale, 4);


    TS_MAP_NATIVE_FN("topaz_shape2d__form_rectangle", shape2d_api__form_rectangle, 3);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image", shape2d_api__form_image, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image_frame", shape2d_api__form_image_frame, 3);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image_scaled", shape2d_api__form_image_scaled, 4);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_image_frame_scaled", shape2d_api__form_image_frame_scaled, 5);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_radial", shape2d_api__form_radial, 3);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_triangles", shape2d_api__form_triangles, 2);
    TS_MAP_NATIVE_FN("topaz_shape2d__form_lines", shape2d_api__form_lines, 2);
    
}
