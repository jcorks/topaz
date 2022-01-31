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

TSO_SCRIPT_API_FN(vector_api__xyz_set) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    native->x = topaz_script_object_as_number(arg1);
    native->y = topaz_script_object_as_number(arg2);
    native->z = topaz_script_object_as_number(arg3);

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


static topazScript_Object_t * vector_api__cleanup(
    topazScript_t * script, 
    topazScript_Object_t ** args,
    void * userData
) {
    free(userData);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(vector_api__create) {
    topazVector_t * ptr = calloc(1, sizeof(topazVector_t));

    if (args == NULL) {
        // creates new object and sets native pointer
        TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__VECTOR, vector_api__cleanup, ptr);


        return object;
    } else {


        TSO_ARG_0;
        TSO_ARG_1;
        TSO_ARG_2;
        ptr->x = topaz_script_object_as_number(arg0); 
        ptr->y = topaz_script_object_as_number(arg1); 
        ptr->z = topaz_script_object_as_number(arg2); 

        // creates new object and sets native pointer
        TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__VECTOR, vector_api__cleanup, ptr);


        return object;
    }
}









TSO_SCRIPT_API_FN(vector_api__get_length) {
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
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    topaz_vector_normalize(native);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(vector_api__cross) {
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

TSO_SCRIPT_API_FN(vector_api__dot) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    float r = topaz_vector_dot(
        native,
        native1
    );


    return topaz_script_object_from_number(script, r);
}

TSO_SCRIPT_API_FN(vector_api__floor) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazVector_t *, TSO_OBJECT_ID__VECTOR);

    topaz_vector_floor(native);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(vector_api__rotation_x_diff) {
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
    TS_MAP_NATIVE_FN("topaz_vector__create", vector_api__create, 3);

    // member functions
    TS_MAP_NATIVE_FN("topaz_vector__get_length", vector_api__get_length, 1);
    TS_MAP_NATIVE_FN("topaz_vector__get_distance", vector_api__get_distance, 2);
    TS_MAP_NATIVE_FN("topaz_vector__normalize", vector_api__normalize, 1);
    TS_MAP_NATIVE_FN("topaz_vector__cross", vector_api__cross, 2);
    TS_MAP_NATIVE_FN("topaz_vector__dot", vector_api__dot, 2);
    TS_MAP_NATIVE_FN("topaz_vector__floor", vector_api__floor, 1);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_x_diff", vector_api__rotation_x_diff, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_x_diff_relative", vector_api__rotation_x_diff_relative, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_x", vector_api__rotation_x, 1);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_y_diff", vector_api__rotation_y_diff, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_y_diff_relative", vector_api__rotation_y_diff_relative, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_y", vector_api__rotation_y, 1);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_z_diff", vector_api__rotation_z_diff, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_z_diff_relative", vector_api__rotation_z_diff_relative, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotation_z", vector_api__rotation_z, 1);
    
    // these modify the existing vector
    TS_MAP_NATIVE_FN("topaz_vector__rotate_x", vector_api__rotate_x, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotate_y", vector_api__rotate_y, 2);
    TS_MAP_NATIVE_FN("topaz_vector__rotate_z", vector_api__rotate_z, 2);
    
    TS_MAP_NATIVE_FN("topaz_vector__set_x", vector_api__x_set, 2);
    TS_MAP_NATIVE_FN("topaz_vector__get_x", vector_api__x_get, 1);
    TS_MAP_NATIVE_FN("topaz_vector__set_y", vector_api__y_set, 2);
    TS_MAP_NATIVE_FN("topaz_vector__set_xyz", vector_api__xyz_set, 4);
    TS_MAP_NATIVE_FN("topaz_vector__get_y", vector_api__y_get, 1);
    TS_MAP_NATIVE_FN("topaz_vector__set_z", vector_api__z_set, 2);
    TS_MAP_NATIVE_FN("topaz_vector__get_z", vector_api__z_get, 1);
    
 
    

}
