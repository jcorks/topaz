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
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_2(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   


    topaz_shape3d_set_texture(native, topaz_script_object_as_int(arg1), native2);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape3d_api__set_sample_framebuffer) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_1(topazRenderer_Framebuffer_t *, TSO_OBJECT_ID__FRAMEBUFFER);   

    topaz_shape3d_set_sample_framebuffer(native, native1);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(shape3d_api__set_mesh) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_ID__MESH);   

    topaz_shape3d_set_mesh(native, native1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape3d_api__set_material) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_ID__MATERIAL);   

    topaz_shape3d_set_material(native, native1);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(shape3d_api__get_attribute) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);

    return topaz_script_object_from_number(
        script,
        topaz_renderer_attributes_get_attribute(
            topaz_shape3d_get_attributes(
                native
            ),
            topaz_script_object_as_number(arg1)            
        )
    );
}

TSO_SCRIPT_API_FN(shape3d_api__set_attribute) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE3D);   

    topazRenderer_Attributes_t att = *topaz_shape3d_get_attributes(native);
    topaz_renderer_attributes_set_attribute(
        &att,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    topaz_shape3d_set_attributes(native, &att);
    TSO_NO_RETURN;
}







TSO_SCRIPT_API_FN(shape3d_api__get_rotation) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_rotation(topaz_shape3d_get_node(native));
    return out;
}

TSO_SCRIPT_API_FN(shape3d_api__get_position) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_position(topaz_shape3d_get_node(native));
    return out;
}

TSO_SCRIPT_API_FN(shape3d_api__get_scale) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_transform_get_scale(topaz_shape3d_get_node(native));
    return out;
}




TSO_SCRIPT_API_FN(shape3d_api__set_rotation) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_rotation(topaz_shape3d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape3d_api__set_position) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_position(topaz_shape3d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(shape3d_api__set_scale) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SHAPE2D);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_transform_scale(topaz_shape3d_get_node(native)) = *native1;
    TSO_NO_RETURN;
}




static void add_refs__shape3d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_shape3d__create", shape3d_api__create);

    TS_MAP_NATIVE_FN("topaz_shape3d__set_texture", shape3d_api__set_texture);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_sample_framebuffer", shape3d_api__set_sample_framebuffer);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_mesh", shape3d_api__set_mesh);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_material", shape3d_api__set_material);

    TS_MAP_NATIVE_FN("topaz_shape3d__set_attribute", shape3d_api__set_attribute);
    TS_MAP_NATIVE_FN("topaz_shape3d__get_attribute", shape3d_api__get_attribute);

    // replacement of transform with rotation / position / scale
    TS_MAP_NATIVE_FN("topaz_shape3d__get_position", shape3d_api__get_position);
    TS_MAP_NATIVE_FN("topaz_shape3d__get_rotation", shape3d_api__get_rotation);
    TS_MAP_NATIVE_FN("topaz_shape3d__get_scale",    shape3d_api__get_scale);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_position", shape3d_api__set_position);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_rotation", shape3d_api__set_rotation);
    TS_MAP_NATIVE_FN("topaz_shape3d__set_scale",    shape3d_api__set_scale);

    
}
