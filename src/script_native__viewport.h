#include <topaz/topaz.h>
#include <topaz/viewport.h>

static topazScript_Object_t * viewport_api__cleanup(
    topazScript_t * script, 
    topazScript_Object_t ** args,
    void * userData
) {
    topaz_entity_remove(userData);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(viewport_api__create) {
    topazEntity_t * ptr = topaz_viewport_create(
        ((topazScriptManager_t*)context)->ctx
    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__VIEWPORT| TSO_OBJECT_TYPE__ENTITY, viewport_api__cleanup, ptr);
    
    TSO_OBJECT_KEEP_REF(ptr);
    return object;
}


TSO_SCRIPT_API_FN(viewport_api__get_image) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    topazAsset_t * out = topaz_viewport_get_image(native);
    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_ID__IMAGE | TSO_OBJECT_TYPE__ASSET, NULL, NULL);
    return object;    
}

TSO_SCRIPT_API_FN(viewport_api__resize) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    
    topaz_viewport_resize(
        native,
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2)
    );
    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(viewport_api__get_width) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    return topaz_script_object_from_int(
        script,
        topaz_viewport_get_width(native)
    );
}

TSO_SCRIPT_API_FN(viewport_api__get_height) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    return topaz_script_object_from_int(
        script,
        topaz_viewport_get_height(native)
    );
}

TSO_SCRIPT_API_FN(viewport_api__get_projection_3d) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    
    topaz_script_return_matrix(script, arg1, topaz_viewport_get_projection_3d(native));
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(viewport_api__set_filtered) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    
    topaz_viewport_set_filtered(native, topaz_script_object_as_int(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(viewport_api__set_projection_3d_auto_mode) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topaz_viewport_set_projection_3d_auto_mode(native, topaz_script_object_as_int(arg1));
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(viewport_api__set_projection_3d) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);
    topazMatrix_t a = topaz_script_object_to_matrix(script, arg1);
    topaz_viewport_set_projection_3d(native, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(viewport_api__world_3d_to_screen) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topazVector_t x = {
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)
    };
    topazVector_t c = topaz_viewport_world_3d_to_screen(
        native,
        &x
    );
    topaz_script_return_vector(script, arg4, c.x, c.y, c.z);
    TSO_NO_RETURN;

}


TSO_SCRIPT_API_FN(viewport_api__screen_to_world_3d) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topazVector_t x = {
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)
    };
    topazVector_t c = topaz_viewport_screen_to_world_3d(
        native,
        &x
    );
    topaz_script_return_vector(script, arg4, c.x, c.y, c.z);
    TSO_NO_RETURN;

}

TSO_SCRIPT_API_FN(viewport_api__clear) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topaz_viewport_clear(native, topaz_script_object_as_int(arg1));
    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(viewport_api__set_auto_clear) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topaz_viewport_set_auto_clear(native, topaz_script_object_as_int(arg1));
    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(viewport_api__swap_buffers) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topaz_viewport_swap_buffers(native);
    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(viewport_api__sync) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__VIEWPORT);

    topaz_viewport_sync(native);
    
    TSO_NO_RETURN;
}


static void add_refs__viewport_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_viewport__create", viewport_api__create, 0);

    // member functions
    TS_MAP_NATIVE_FN("topaz_viewport__get_image", viewport_api__get_image, 1);
    TS_MAP_NATIVE_FN("topaz_viewport__resize", viewport_api__resize, 3);
    TS_MAP_NATIVE_FN("topaz_viewport__get_width", viewport_api__get_width, 1);
    TS_MAP_NATIVE_FN("topaz_viewport__get_height", viewport_api__get_height, 1);
    TS_MAP_NATIVE_FN("topaz_viewport__get_projection_3d", viewport_api__get_projection_3d, 2);
    TS_MAP_NATIVE_FN("topaz_viewport__set_filtered", viewport_api__set_filtered, 2);
    TS_MAP_NATIVE_FN("topaz_viewport__set_projection_3d_auto_mode", viewport_api__set_projection_3d_auto_mode, 2);
    TS_MAP_NATIVE_FN("topaz_viewport__set_projection_3d", viewport_api__set_projection_3d, 2);
    TS_MAP_NATIVE_FN("topaz_viewport__world_3d_to_screen", viewport_api__world_3d_to_screen, 5);
    TS_MAP_NATIVE_FN("topaz_viewport__screen_to_world_3d", viewport_api__screen_to_world_3d, 5);
    TS_MAP_NATIVE_FN("topaz_viewport__clear", viewport_api__clear, 1);
    TS_MAP_NATIVE_FN("topaz_viewport__set_auto_clear", viewport_api__set_auto_clear, 2);
    TS_MAP_NATIVE_FN("topaz_viewport__swap_buffers", viewport_api__swap_buffers, 1);
    TS_MAP_NATIVE_FN("topaz_viewport__sync", viewport_api__sync, 1);

    

    
}
