/// Since vectors dont have a "managed life" on their own, we wont set 
#include <topaz/vector.h>


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

    return topaz_script_object_from_number(script, native->y);    
}
TSO_SCRIPT_API_FN(vector_api__z_get) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    return topaz_script_object_from_number(script, native->z);    
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
    TSO_OBJECT_NEW(ptr, TSO_OBJECT_ID__VECTOR, NULL);

    // uses implicit names to add ref
    TSO_PROP_ADD("x", vector_api__x_set, vector_api__x_get);
    TSO_PROP_ADD("y", vector_api__y_set, vector_api__y_get);
    TSO_PROP_ADD("z", vector_api__z_set, vector_api__z_get);

    TSO_OBJECT_KEEP;
    return object;
}

TSO_SCRIPT_API_FN(vector_api__destroy) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    free(native);

    TSO_OBJECT_UNKEEP;
    TSO_NO_RETURN;
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



static void add_refs__vector_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_vector__create", vector_api__create);
    TS_MAP_NATIVE_FN("topaz_vector__destroy", vector_api__destroy);

    // member functions
    TS_MAP_NATIVE_FN("topaz_vector__get_length", vector_api__get_length);
    TS_MAP_NATIVE_FN("topaz_vector__get_distance", vector_api__get_distance);
    TS_MAP_NATIVE_FN("topaz_vector__normalize", vector_api__normalize);
    TS_MAP_NATIVE_FN("topaz_vector__cross", vector_api__cross);
    TS_MAP_NATIVE_FN("topaz_vector__floor", vector_api__floor);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_x_diff", vector_api__rotation_x_diff);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_x_diff_relative", vector_api__rotation_x_diff_relative);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_x", vector_api__rotation_x);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_y_diff", vector_api__rotation_y_diff);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_y_diff_relative", vector_api__rotation_y_diff_relative);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_y", vector_api__rotation_y);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_z_diff", vector_api__rotation_z_diff);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_z_diff_relative", vector_api__rotation_z_diff_relative);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_z", vector_api__rotation_z);
    
    // these modify the existing vector
    TS_MAP_NATIVE_FN("topaz_vector__rotate_x", vector_api__rotate_x);
    TS_MAP_NATIVE_FN("topaz_vector__rotate_y", vector_api__rotate_y);
    TS_MAP_NATIVE_FN("topaz_vector__rotate_z", vector_api__rotate_z);

}
