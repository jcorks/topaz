#include <topaz/matrix.h>








static void topaz_script_return_matrix(
    topazScript_t * script, 
    topazScript_Object_t * fn,
    topazMatrix_t * a
) {

    topazScript_Object_t * args[] = {
        topaz_script_object_from_number(script, a->data[0]),
        topaz_script_object_from_number(script, a->data[1]),
        topaz_script_object_from_number(script, a->data[2]),
        topaz_script_object_from_number(script, a->data[3]),

        topaz_script_object_from_number(script, a->data[4]),
        topaz_script_object_from_number(script, a->data[5]),
        topaz_script_object_from_number(script, a->data[6]),
        topaz_script_object_from_number(script, a->data[7]),
        
        topaz_script_object_from_number(script, a->data[8]),
        topaz_script_object_from_number(script, a->data[9]),
        topaz_script_object_from_number(script, a->data[10]),
        topaz_script_object_from_number(script, a->data[11]),

        topaz_script_object_from_number(script, a->data[12]),
        topaz_script_object_from_number(script, a->data[13]),
        topaz_script_object_from_number(script, a->data[14]),
        topaz_script_object_from_number(script, a->data[15])

    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn,
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 16)
        )
    );
    topaz_script_object_destroy(args[0]);
    topaz_script_object_destroy(args[1]);
    topaz_script_object_destroy(args[2]);
    topaz_script_object_destroy(args[3]);

    topaz_script_object_destroy(args[4]);
    topaz_script_object_destroy(args[5]);
    topaz_script_object_destroy(args[6]);
    topaz_script_object_destroy(args[7]);

    topaz_script_object_destroy(args[8]);
    topaz_script_object_destroy(args[9]);
    topaz_script_object_destroy(args[10]);
    topaz_script_object_destroy(args[11]);

    topaz_script_object_destroy(args[12]);
    topaz_script_object_destroy(args[13]);
    topaz_script_object_destroy(args[14]);
    topaz_script_object_destroy(args[15]);

}

topazMatrix_t topaz_script_object_to_matrix(topazScript_t * script, topazScript_Object_t * arg) {
    topazMatrix_t a = {};
    int i;
    for(i = 0; i < 16; ++i) {
        a.data[i] = topaz_script_object_as_number(
            topaz_script_object_reference_array_get_nth(arg, i)
        );
    }
    return a;
}


TSO_SCRIPT_API_FN(matrix_api__set_identity) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazMatrix_t a;
    topaz_matrix_set_identity(&a);
    topaz_script_return_matrix(script, arg1, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__transform) {
    TSO_ARG_0;

    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    TSO_ARG_4;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topazVector_t b = {
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)
    };    
    
    topazVector_t c = topaz_matrix_transform(&a, &b);
    
    topaz_matrix_set_identity(&a);
    topaz_script_return_vector(script, arg1, c.x, c.y, c.z);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__to_string) {
    TSO_ARG_0;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topazString_t * str = topaz_matrix_to_string(&a);
    topazScript_Object_t * o = topaz_script_object_from_string(script, str);
    topaz_string_destroy(str);
    return o;
}

TSO_SCRIPT_API_FN(matrix_api__transpose) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_transpose(&a);
    topaz_script_return_matrix(script, arg1, &a);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(matrix_api__invert) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_invert(&a);
    topaz_script_return_matrix(script, arg1, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__reverse_majority) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_reverse_majority(&a);
    topaz_script_return_matrix(script, arg1, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__multiply) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topazMatrix_t b = topaz_script_object_to_matrix(script, arg1);
    topazMatrix_t c = topaz_matrix_multiply(&a, &b);
    topaz_script_return_matrix(script, arg3, &c);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__rotate_by_angles) {
    TSO_ARG_0;

    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    TSO_ARG_4;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_rotate_by_angles(
        &a,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)        
    );
    topaz_script_return_matrix(script, arg4, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__rotate_by_axis) {
    TSO_ARG_0;

    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;

    TSO_ARG_5;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_rotate_by_axis(
        &a,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3),        
        topaz_script_object_as_number(arg4)        
    );
    topaz_script_return_matrix(script, arg5, &a);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(matrix_api__translate) {
    TSO_ARG_0;

    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    TSO_ARG_4;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_translate(
        &a,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)        
    );
    topaz_script_return_matrix(script, arg4, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(matrix_api__scale) {
    TSO_ARG_0;

    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;

    TSO_ARG_4;

    topazMatrix_t a = topaz_script_object_to_matrix(script, arg0);
    topaz_matrix_scale(
        &a,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)        
    );
    topaz_script_return_matrix(script, arg4, &a);
    TSO_NO_RETURN;
}

static void add_refs__matrix_api(topazScript_t * script, topazScriptManager_t * context) {

    // member functions
    TS_MAP_NATIVE_FN("topaz_matrix__set_identity", matrix_api__set_identity, 2);
    TS_MAP_NATIVE_FN("topaz_matrix__transform", matrix_api__transform, 5);
    TS_MAP_NATIVE_FN("topaz_matrix__to_string", matrix_api__to_string, 1);
    TS_MAP_NATIVE_FN("topaz_matrix__transpose", matrix_api__transpose, 2);
    TS_MAP_NATIVE_FN("topaz_matrix__invert", matrix_api__invert, 2);
    TS_MAP_NATIVE_FN("topaz_matrix__reverse_majority", matrix_api__reverse_majority, 2);
    TS_MAP_NATIVE_FN("topaz_matrix__multiply", matrix_api__multiply, 4);
    TS_MAP_NATIVE_FN("topaz_matrix__rotate_by_angles", matrix_api__rotate_by_angles, 5);
    TS_MAP_NATIVE_FN("topaz_matrix__rotate_by_angles", matrix_api__rotate_by_axis, 6);
    TS_MAP_NATIVE_FN("topaz_matrix__translate", matrix_api__translate, 5);
    TS_MAP_NATIVE_FN("topaz_matrix__scale", matrix_api__scale, 5);


    
 
    

}
