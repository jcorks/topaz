#include <topaz/modules/view_manager.h>
#include <topaz/modules/graphics.h>
#include <topaz/backends/display.h>
#include <topaz/camera.h>




TSO_SCRIPT_API_FN(view_manager_api__get_default) {
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    topazDisplay_t * d = topaz_view_manager_get_default(vm);

    topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(d);
    if (a) return topaz_script_object_from_object(script, a);

    TSO_OBJECT_NEW_VALUE(d, TSO_OBJECT_ID__DISPLAY, NULL, NULL);
    TSO_OBJECT_KEEP_REF(d);
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
    TSO_ARG_0;
    

    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);

    
    topaz_view_manager_set_clipboard_from_string(
        vm,
        topaz_script_object_as_string(arg0)
    );
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(view_manager_api__create_display) {
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    topazDisplay_t * ptr = topaz_view_manager_create_display(vm, TOPAZ_STR_CAST(""));
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__DISPLAY, NULL, NULL);
    TSO_OBJECT_KEEP_REF(ptr);
    return object;
}


TSO_SCRIPT_API_FN(view_manager_api__destroy_display) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    TSO_OBJECT_UNKEEP_REF(arg0, native);
    topaz_script_object_reference_set_native_data(arg0, NULL, 0);    
    topaz_view_manager_destroy_display(vm, native);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(view_manager_api__get_display_count) {
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_int(
        script, 
        topaz_array_get_size(topaz_view_manager_get_all(vm))
    );
}
TSO_SCRIPT_API_FN(view_manager_api__get_display) {
    TSO_ARG_0;
    topazViewManager_t * vm = topaz_context_get_view_manager(((topazScriptManager_t*)context)->ctx);
    uint32_t index = topaz_script_object_as_int(arg0);
    const topazArray_t * displays = topaz_view_manager_get_all(vm);
    if (index >= topaz_array_get_size(displays)) TSO_NO_RETURN;

    topazDisplay_t * d = topaz_array_at(displays, topazDisplay_t *, index);
    topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(d);
    if (a) return topaz_script_object_from_object(script, a);

    TSO_OBJECT_NEW_VALUE(d, TSO_OBJECT_ID__DISPLAY, NULL, NULL);
    return object;
}



TSO_SCRIPT_API_FN(display_api__get_parameter) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    return topaz_script_object_from_number(
        script, 
        topaz_display_get_parameter(
            native,
            topaz_script_object_as_int(arg1)
        )
    );
}

TSO_SCRIPT_API_FN(display_api__is_parameter_modifiable) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);


    return topaz_script_object_from_int(
        script, 
        topaz_display_is_parameter_modifiable(
            native,
            topaz_script_object_as_int(arg1)
        )
    );
}

TSO_SCRIPT_API_FN(display_api__get_root) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    topazEntity_t * e = topaz_display_get_root(native);
    if (e && e != topaz_entity_null()) {
        topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(e);
        if (a) return topaz_script_object_from_object(script, a);
        TSO_OBJECT_NEW_VALUE(e, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
        return object;
    }
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__set_root) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    TSO_NATIVIZE_1(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_display_set_root(native, native1);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__auto_clear_framebuffer) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topaz_display_auto_clear_framebuffer(native, topaz_script_object_as_int(arg1));    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__update) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topaz_display_update(native);    
    TSO_NO_RETURN;

}

TSO_SCRIPT_API_FN(display_api__set_name) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topaz_display_set_name(native, topaz_script_object_as_string(arg1));    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(display_api__set_parameter) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    topaz_display_set_parameter(
        native,
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}








typedef struct {
    topazScript_Object_t * func;
    topazScript_Object_t * display;
    topazScript_t * script;
} DisplayEventCallback;


static void script_parameter_callback(topazDisplay_t * d, topazDisplay_Parameter p, void * data) {
    DisplayEventCallback * ev = data;
    topazScript_Object_t * args[2];
    args[0] = ev->display;
    args[1] = topaz_script_object_from_int(ev->script, p);
    topaz_script_object_reference_call(
        ev->func,
        TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2)
    );
    // TODO; free int?
}


static void script_close_callback(topazDisplay_t * d, void * data) {
    DisplayEventCallback * ev = data;
    topazScript_Object_t * args[1];
    args[0] = ev->display;
    topaz_script_object_reference_call(
        ev->func,
        TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 1)
    );
    // TODO; free int?
}


TSO_SCRIPT_API_FN(display_api__add_parameter_callback) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    if (topaz_script_object_reference_get_feature_mask(arg1) | topazScript_Object_Feature_Callable) {
        DisplayEventCallback * ev = calloc(1, sizeof(DisplayEventCallback));
        ev->func = topaz_script_object_from_object(script, arg1); 
        ev->display = topaz_script_object_from_object(script, arg0);
        ev->script = script;

        return topaz_script_object_from_int(
            script,
            topaz_display_add_parameter_callback(
                native,
                script_parameter_callback,
                ev
            )
        );
    } else {
        script_error(script, "Required object must be callable.");
    }        
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(display_api__add_close_callback) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    if (topaz_script_object_reference_get_feature_mask(arg1) | topazScript_Object_Feature_Callable) {
        DisplayEventCallback * ev = calloc(1, sizeof(DisplayEventCallback));
        ev->func = topaz_script_object_from_object(script, arg1); 
        ev->display = topaz_script_object_from_object(script, arg0);
        ev->script = script;

        return topaz_script_object_from_int(
            script,
            topaz_display_add_close_callback(
                native,
                script_close_callback,
                ev
            )
        );
    } else {
        script_error(script, "Required object must be callable.");
    }        
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(display_api__remove_callback) {
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



TSO_SCRIPT_API_FN(display_api__get_camera_2d) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazEntity_t * out = topaz_display_get_2d_camera(native);
    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    return object;    
}

TSO_SCRIPT_API_FN(display_api__get_camera_3d) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    topazEntity_t * out = topaz_display_get_3d_camera(native);
    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    return object;    
}

TSO_SCRIPT_API_FN(display_api__get_framebuffer) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    
    topazRenderer_Framebuffer_t * fb = topaz_display_get_framebuffer(native, 
        topaz_script_object_as_int(arg1)
    );
    TSO_OBJECT_NEW_VALUE(fb, TSO_OBJECT_ID__FRAMEBUFFER, NULL, NULL);
    return object;
}
TSO_SCRIPT_API_FN(display_api__use_framebuffer) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    
    topaz_display_use_framebuffer(
        native,
        topaz_script_object_as_int(arg1)
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__set_post_process_shader) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    
    topazString_t * err = topaz_display_set_post_process_shader(
        native,
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_string(arg2)
    );
    if (err) {
        topazScript_Object_t * obj = topaz_script_object_from_string(script, err);
        topaz_string_destroy(err);
        return obj;
    }

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__get_main_framebuffer) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    
    topazRenderer_Framebuffer_t * fb = topaz_display_get_main_framebuffer(native);
    TSO_OBJECT_NEW_VALUE(fb, TSO_OBJECT_ID__FRAMEBUFFER, NULL, NULL);
    return object;
}


TSO_SCRIPT_API_FN(display_api__clear_main_framebuffer) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);

    topaz_display_clear_main_framebuffer(
        native,
        topaz_script_object_as_int(arg1)
    );    
    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(display_api__capture_main_framebuffer) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazDisplay_t *, TSO_OBJECT_ID__DISPLAY);
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_ID__IMAGE);
    
    topaz_display_capture_main_framebuffer(
        native,
        native1
    );
    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(framebuffer_api__get_width) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazRenderer_Framebuffer_t *, TSO_OBJECT_ID__FRAMEBUFFER);

    return topaz_script_object_from_int(
        script,
        topaz_renderer_framebuffer_get_width(native)
    );
}

TSO_SCRIPT_API_FN(framebuffer_api__get_height) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazRenderer_Framebuffer_t *, TSO_OBJECT_ID__FRAMEBUFFER);

    return topaz_script_object_from_int(
        script,
        topaz_renderer_framebuffer_get_height(native)
    );
}


TSO_SCRIPT_API_FN(framebuffer_api__resize) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazRenderer_Framebuffer_t *, TSO_OBJECT_ID__FRAMEBUFFER);

    topaz_renderer_framebuffer_resize(native, 
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(framebuffer_api__get_filtered_hint) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazRenderer_Framebuffer_t *, TSO_OBJECT_ID__FRAMEBUFFER);
    
    return topaz_script_object_from_int(
        script,
        topaz_renderer_framebuffer_get_filtered_hint(native)
    );
}


TSO_SCRIPT_API_FN(framebuffer_api__set_filtered_hint) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazRenderer_Framebuffer_t *, TSO_OBJECT_ID__FRAMEBUFFER);
    
    topaz_renderer_framebuffer_set_filtered_hint(native, 
        topaz_script_object_as_int(arg1)
    );

    TSO_NO_RETURN;
}






static void add_refs__view_manager_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_view_manager__get_default", view_manager_api__get_default, 0);
    TS_MAP_NATIVE_FN("topaz_view_manager__get_clipboard_as_string", view_manager_api__get_clipboard_as_string, 0);
    TS_MAP_NATIVE_FN("topaz_view_manager__set_clipboard_from_string", view_manager_api__set_clipboard_from_string, 1);
    TS_MAP_NATIVE_FN("topaz_view_manager__create_display", view_manager_api__create_display, 0);
    TS_MAP_NATIVE_FN("topaz_view_manager__get_display_count", view_manager_api__get_display_count, 0);
    TS_MAP_NATIVE_FN("topaz_view_manager__get_display", view_manager_api__get_display, 1);
    TS_MAP_NATIVE_FN("topaz_view_manager__destroy_display", view_manager_api__destroy_display, 1);


    TS_MAP_NATIVE_FN("topaz_display__get_parameter", display_api__get_parameter, 2);
    TS_MAP_NATIVE_FN("topaz_display__set_parameter", display_api__set_parameter, 3);
    TS_MAP_NATIVE_FN("topaz_display__is_parameter_modifiable", display_api__is_parameter_modifiable, 2);
    TS_MAP_NATIVE_FN("topaz_display__get_root", display_api__get_root, 1);
    TS_MAP_NATIVE_FN("topaz_display__set_root", display_api__set_root, 2);
    TS_MAP_NATIVE_FN("topaz_display__auto_clear_framebuffer", display_api__auto_clear_framebuffer, 2);
    TS_MAP_NATIVE_FN("topaz_display__set_name", display_api__set_name, 2);
    TS_MAP_NATIVE_FN("topaz_display__add_parameter_callback", display_api__add_parameter_callback, 2);
    TS_MAP_NATIVE_FN("topaz_display__add_close_callback", display_api__add_close_callback, 2);
    TS_MAP_NATIVE_FN("topaz_display__remove_callback", display_api__remove_callback, 2);
    TS_MAP_NATIVE_FN("topaz_display__get_camera_2d", display_api__get_camera_2d, 1);
    TS_MAP_NATIVE_FN("topaz_display__get_camera_3d", display_api__get_camera_3d, 1);
    TS_MAP_NATIVE_FN("topaz_display__get_framebuffer", display_api__get_framebuffer, 2);
    TS_MAP_NATIVE_FN("topaz_display__use_framebuffer", display_api__use_framebuffer, 2);
    TS_MAP_NATIVE_FN("topaz_display__get_main_framebuffer", display_api__get_main_framebuffer, 1);
    TS_MAP_NATIVE_FN("topaz_display__clear_main_framebuffer", display_api__clear_main_framebuffer, 2);
    TS_MAP_NATIVE_FN("topaz_display__capture_main_framebuffer", display_api__capture_main_framebuffer, 2);
    TS_MAP_NATIVE_FN("topaz_display__set_post_process_shader", display_api__set_post_process_shader, 3);
    TS_MAP_NATIVE_FN("topaz_display__update", display_api__update, 1);

    TS_MAP_NATIVE_FN("topaz_framebuffer__get_width", framebuffer_api__get_width, 1);
    TS_MAP_NATIVE_FN("topaz_framebuffer__get_height", framebuffer_api__get_height, 1);
    TS_MAP_NATIVE_FN("topaz_framebuffer__set_filtered_hint", framebuffer_api__set_filtered_hint, 2);
    TS_MAP_NATIVE_FN("topaz_framebuffer__get_filtered_hint", framebuffer_api__get_filtered_hint, 1);
    TS_MAP_NATIVE_FN("topaz_framebuffer__resize", framebuffer_api__resize, 3);
    

   
}
