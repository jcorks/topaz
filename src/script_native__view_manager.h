#include <topaz/modules/view_manager.h>
#include <topaz/backends/display.h>
#include <topaz/camera.h>

TSO_SCRIPT_API_FN(view_manager_api__set_main) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    
    topaz_view_manager_set_main(
        vm,
        native
    );    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(view_manager_api__get_main) {
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    topazDisplay_t * d = topaz_view_manager_get_main(vm);
    TSO_OBJECT_NEW_VALUE(d, TSO_OBJECT_ID__DISPLAY, NULL, NULL);
    return object;
}

TSO_SCRIPT_API_FN(view_manager_api__get_clipboard_as_string) {
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
 
    return topaz_script_object_from_string(
        script,
        topaz_view_manager_get_clipboard_as_string(
            vm
        )
    );
}

TSO_SCRIPT_API_FN(view_manager_api__set_clipboard_from_string) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    

    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);

    
    topaz_view_manager_set_clipboard_from_string(
        vm,
        topaz_script_object_as_string(arg0)
    );
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(display_api__destroy) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topaz_script_object_reference_set_native_data(arg0, NULL, 0);    
    topaz_display_destroy(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(view_manager_api__create_display) {
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    topazDisplay_t * ptr = topaz_view_manager_create_display(vm, TOPAZ_STR_CAST(""), 640, 480);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__DISPLAY, NULL, NULL);
    return object;
}

TSO_SCRIPT_API_FN(display_api__get_width) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    return topaz_script_object_from_int(
        script, 
        topaz_display_get_width(native)
    );
}

TSO_SCRIPT_API_FN(display_api__get_height) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    return topaz_script_object_from_int(
        script, 
        topaz_display_get_height(native)
    );
}

TSO_SCRIPT_API_FN(display_api__resize) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    topaz_display_resize(
        native,
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2)
    );
    TSO_NO_RETURN;
}



typedef struct {
    topazScript_Object_t * func;
    topazScript_Object_t * display;
} DisplayEventCallback;


static void script_resize_callback(topazDisplay_t * d, void * data) {
    DisplayEventCallback * ev = data;
    topaz_script_object_reference_call(
        ev->func,
        TOPAZ_ARRAY_CAST(&ev->display, topazScript_Object_t *, 1)        
    );
}

TSO_SCRIPT_API_FN(display_api__add_resize_callback) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    if (topaz_script_object_reference_get_feature_mask(arg1) | topazScript_Object_Feature_Callable) {
        DisplayEventCallback * ev = calloc(1, sizeof(DisplayEventCallback));
        ev->func = topaz_script_object_from_object(script, arg1); 
        ev->display = topaz_script_object_from_object(script, arg0);

        return topaz_script_object_from_int(
            script,
            topaz_display_add_resize_callback(
                native,
                script_resize_callback,
                ev
            )
        );
    } else {
        script_error(script, "Required object must be callable.");
    }        
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__add_close_callback) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    if (topaz_script_object_reference_get_feature_mask(arg1) | topazScript_Object_Feature_Callable) {
        DisplayEventCallback * ev = calloc(1, sizeof(DisplayEventCallback));
        ev->func = topaz_script_object_from_object(script, arg1); 
        ev->display = topaz_script_object_from_object(script, arg0);

        return topaz_script_object_from_int(
            script,
            topaz_display_add_close_callback(
                native,
                script_resize_callback,
                ev
            )
        );
    } else {
        script_error(script, "Required object must be callable.");
    }        
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(display_api__remove_callback) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    int id = topaz_script_object_as_int(arg1);
    topaz_display_remove_callback(
        native,
        id
    );
    TSO_NO_RETURN;

}

TSO_SCRIPT_API_FN(display_api__fullscreen) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    
    topaz_display_fullscreen(
        native,
        topaz_script_object_as_int(arg1)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__set_view_policy) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    
    topaz_display_set_view_policy(
        native,
        topaz_script_object_as_int(arg1)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__set_render_resolution) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    topazEntity_t * cam = topaz_display_get_render_camera(native);
    topaz_camera_set_render_resolution(
        cam,
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__get_render_width) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazEntity_t * cam = topaz_display_get_render_camera(native);

    return topaz_script_object_from_int(
        script, 
        topaz_camera_get_render_width(cam)
    );
}


TSO_SCRIPT_API_FN(display_api__get_render_height) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazEntity_t * cam = topaz_display_get_render_camera(native);

    return topaz_script_object_from_int(
        script, 
        topaz_camera_get_render_height(cam)
    );
}


TSO_SCRIPT_API_FN(display_api__get_camera_2d) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazEntity_t * out = topaz_display_get_2d_camera(native);
    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    return object;    
}

TSO_SCRIPT_API_FN(display_api__get_camera_3d) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazEntity_t * out = topaz_display_get_3d_camera(native);
    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    return object;    
}




static void add_refs__view_manager_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_view_manager__set_main", view_manager_api__set_main);
    TS_MAP_NATIVE_FN("topaz_view_manager__get_main", view_manager_api__get_main);
    TS_MAP_NATIVE_FN("topaz_view_manager__get_clipboard_as_string", view_manager_api__get_clipboard_as_string);
    TS_MAP_NATIVE_FN("topaz_view_manager__set_clipboard_from_string", view_manager_api__set_clipboard_from_string);
    TS_MAP_NATIVE_FN("topaz_view_manager__create_display", view_manager_api__create_display);


    TS_MAP_NATIVE_FN("topaz_display__destroy", display_api__destroy);
    TS_MAP_NATIVE_FN("topaz_display__get_width", display_api__get_width);
    TS_MAP_NATIVE_FN("topaz_display__get_height", display_api__get_height);
    TS_MAP_NATIVE_FN("topaz_display__resize", display_api__resize);
    TS_MAP_NATIVE_FN("topaz_display__add_resize_callback", display_api__add_resize_callback);
    TS_MAP_NATIVE_FN("topaz_display__add_close_callback", display_api__add_close_callback);
    TS_MAP_NATIVE_FN("topaz_display__remove_callback", display_api__remove_callback);
    TS_MAP_NATIVE_FN("topaz_display__fullscreen", display_api__fullscreen);
    TS_MAP_NATIVE_FN("topaz_display__set_view_policy", display_api__set_view_policy);
    TS_MAP_NATIVE_FN("topaz_display__set_render_resolution", display_api__set_render_resolution);
    TS_MAP_NATIVE_FN("topaz_display__get_render_width", display_api__get_render_width);
    TS_MAP_NATIVE_FN("topaz_display__get_render_height", display_api__get_render_height);
    TS_MAP_NATIVE_FN("topaz_display__get_camera_2d", display_api__get_camera_2d);
    TS_MAP_NATIVE_FN("topaz_display__get_camera_3d", display_api__get_camera_3d);
   
}
