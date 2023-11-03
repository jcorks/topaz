/// Since vectors dont have a "managed life" on their own, we wont set 
#include <topaz/vector.h>








static void topaz_script_return_vector(
    topazScript_t * script, 
    topazScript_Object_t * fn,
    float x,
    float y,
    float z
) {

    topazScript_Object_t * args[] = {
        topaz_script_object_from_number(script, x),
        topaz_script_object_from_number(script, y),
        topaz_script_object_from_number(script, z)
    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn,
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 3)
        )
    );
    topaz_script_object_destroy(args[0]);
    topaz_script_object_destroy(args[1]);
    topaz_script_object_destroy(args[2]);

}


TSO_SCRIPT_API_FN(vector_api__from_string) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazVector_t a = topaz_vector_from_string(topaz_script_object_as_string(arg0));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(vector_api__reset) {
    TSO_ARG_0;

    topaz_script_return_vector(script, arg0, 0, 0, 0);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(vector_api__get_length) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    
    topazVector_t v = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    return topaz_script_object_from_number(
        script,
        topaz_vector_get_length(
            &v
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__get_distance) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;


    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    topazVector_t b = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5)
    };


    return topaz_script_object_from_number(
        script,
        topaz_vector_get_distance(
            &a,
            &b
        )
    );    
}

TSO_SCRIPT_API_FN(vector_api__normalize) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };

    topaz_vector_normalize(&a);
    topaz_script_return_vector(script, arg3, a.x, a.y, a.z);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(vector_api__cross) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;


    TSO_ARG_6;


    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    topazVector_t b = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5)
    };


    topazVector_t v = topaz_vector_cross(
        &a,
        &b
    );

    topaz_script_return_vector(script, arg6, v.x, v.y, v.z);
    TSO_NO_RETURN;
}
TSO_SCRIPT_API_FN(vector_api__reflect_2d) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;


    TSO_ARG_6;


    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    topazVector_t b = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5)
    };


    topazVector_t v = topaz_vector_reflect_2d(
        &a,
        &b
    );
    topaz_script_return_vector(script, arg6, v.x, v.y, v.z);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(vector_api__point_at_2d) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;



    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    topazVector_t b = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5)
    };

    float r = topaz_vector_point_at_2d(
        &a,
        &b
    );

    return topaz_script_object_from_number(script, r);
}

TSO_SCRIPT_API_FN(vector_api__look_at_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;

    TSO_ARG_6;
    TSO_ARG_7;
    TSO_ARG_8;

    TSO_ARG_9;


    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    topazVector_t b = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5)
    };
    topazVector_t c = {
        topaz_script_object_as_number(arg6),
        topaz_script_object_as_number(arg7),
        topaz_script_object_as_number(arg8)
    };

    topazVector_t v = topaz_vector_look_at_rotation(
        &a,
        &b,
        &c
    );
    topaz_script_return_vector(script, arg6, v.x, v.y, v.z);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(vector_api__dot) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;



    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };
    topazVector_t b = {
        topaz_script_object_as_number(arg3),
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5)
    };

    float r = topaz_vector_dot(
        &a,
        &b
    );


    return topaz_script_object_from_number(script, r);
}

TSO_SCRIPT_API_FN(vector_api__floor) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    topazVector_t a = {
        topaz_script_object_as_number(arg0),
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    };


    topaz_vector_floor(&a);
    topaz_script_return_vector(script, arg3, a.x, a.y, a.z);
    TSO_NO_RETURN;
}






static void add_refs__vector_api(topazScript_t * script, topazScriptManager_t * context) {

    // member functions
    TS_MAP_NATIVE_FN("topaz_vector__from_string", vector_api__from_string, 2);
    TS_MAP_NATIVE_FN("topaz_vector__reset", vector_api__reset, 1);
    TS_MAP_NATIVE_FN("topaz_vector__get_length", vector_api__get_length, 3);
    TS_MAP_NATIVE_FN("topaz_vector__get_length", vector_api__get_length, 3);
    TS_MAP_NATIVE_FN("topaz_vector__get_distance", vector_api__get_distance, 6);
    TS_MAP_NATIVE_FN("topaz_vector__normalize", vector_api__normalize, 4);
    TS_MAP_NATIVE_FN("topaz_vector__cross", vector_api__cross, 7);
    TS_MAP_NATIVE_FN("topaz_vector__reflect_2d", vector_api__reflect_2d, 7);
    TS_MAP_NATIVE_FN("topaz_vector__point_at_2d", vector_api__point_at_2d, 6);
    TS_MAP_NATIVE_FN("topaz_vector__look_at_rotation", vector_api__look_at_rotation, 10);
    TS_MAP_NATIVE_FN("topaz_vector__dot", vector_api__dot, 6);
    TS_MAP_NATIVE_FN("topaz_vector__floor", vector_api__floor, 4);

    
 
    

}
