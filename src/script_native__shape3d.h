#include <topaz/components/shape3d.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(shape3d_api__create) {
    topazComponent_t * component = topaz_shape3d_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__SHAPE3D, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}


TSO_SCRIPT_API_FN(shape3d_api__set_texture) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_2(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   


    topaz_shape3d_set_texture(native, topaz_script_object_as_int(arg1), native2);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(shape3d_api__set_mesh) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_ID__MESH);   

    topaz_shape3d_set_mesh(native, native1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape3d_api__set_material) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_ID__MATERIAL);   

    topaz_shape3d_set_material(native, native1);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(shape3d_api__get_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);

    return topaz_script_object_from_number(
        script,
        topaz_shape3d_get_attribute(
            native,
            topaz_script_object_as_number(arg1)            
        )
    );
}

TSO_SCRIPT_API_FN(shape3d_api__set_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   

    topaz_shape3d_set_attribute(native, 
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}







TSO_SCRIPT_API_FN(shape3d_api__get_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = *topaz_transform_get_rotation(topaz_shape3d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(shape3d_api__get_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = *topaz_transform_get_position(topaz_shape3d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}





TSO_SCRIPT_API_FN(shape3d_api__get_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t a = *topaz_transform_get_scale(topaz_shape3d_get_node(native));
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(shape3d_api__set_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * a = topaz_transform_rotation(topaz_shape3d_get_node(native));
    a->x = 
        topaz_script_object_as_number(arg1)
    ;
    a->y = 
        topaz_script_object_as_number(arg2)
    ;
    a->z = 
        topaz_script_object_as_number(arg3)
    ;

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape3d_api__set_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * a = topaz_transform_position(topaz_shape3d_get_node(native));
    a->x = 
        topaz_script_object_as_number(arg1)
    ;
    a->y = 
        topaz_script_object_as_number(arg2)
    ;
    a->z = 
        topaz_script_object_as_number(arg3)
    ;

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape3d_api__set_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * a = topaz_transform_scale(topaz_shape3d_get_node(native));
    a->x = 
        topaz_script_object_as_number(arg1)
    ;
    a->y = 
        topaz_script_object_as_number(arg2)
    ;
    a->z = 
        topaz_script_object_as_number(arg3)
    ;

    TSO_NO_RETURN;
}



static void add_refs__shape3d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_shape3d__create", shape3d_api__create, 0);

    TS_MAP_NATIVE_FN("topaz_shape3d__set_texture", shape3d_api__set_texture, 3);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_mesh", shape3d_api__set_mesh, 2);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_material", shape3d_api__set_material, 2);

    TS_MAP_NATIVE_FN("topaz_shape3d__set_attribute", shape3d_api__set_attribute, 3);
    TS_MAP_NATIVE_FN("topaz_shape3d__get_attribute", shape3d_api__get_attribute, 2);

    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_shape3d__get_rotation", shape3d_api__get_rotation, 2);
    TS_MAP_NATIVE_FN("topaz_shape3d__get_position", shape3d_api__get_position, 2);
    TS_MAP_NATIVE_FN("topaz_shape3d__get_scale", shape3d_api__get_scale, 2);

    TS_MAP_NATIVE_FN("topaz_shape3d__set_rotation", shape3d_api__set_rotation, 4);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_position", shape3d_api__set_position, 4);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_scale", shape3d_api__set_scale, 4);

    
}
