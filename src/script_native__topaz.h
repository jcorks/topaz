#include <topaz/topaz.h>
#include <topaz/modules/console.h>
#include <topaz/modules/resources.h>
#include <topaz/containers/array.h>
#include <topaz/assets/data.h>
#include <topaz/compat.h>


TSO_SCRIPT_API_FN(topaz_api__run) {
    topazScriptManager_t * mgr = context;
    topaz_context_run(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__pause) {
    topazScriptManager_t * mgr = context;
    topaz_context_pause(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__pause_now) {
    topazScriptManager_t * mgr = context;
    topaz_context_pause_now(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__resume) {
    topazScriptManager_t * mgr = context;
    topaz_context_resume(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__iterate) {
    topazScriptManager_t * mgr = context;
    topaz_context_iterate(mgr->ctx);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(topaz_api__is_paused) {
    topazScriptManager_t * mgr = context;
    return topaz_script_object_from_int(script, topaz_context_is_paused(mgr->ctx));
}




TSO_SCRIPT_API_FN(topaz_api__attach_pre_manager) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_pre_manager(mgr->ctx, native);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__attach_pre_manager_unpausable) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_pre_manager_unpausable(mgr->ctx, native);    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(topaz_api__attach_post_manager) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_post_manager(mgr->ctx, native);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__attach_post_manager_unpausable) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_post_manager_unpausable(mgr->ctx, native);    
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(topaz_api__quit) {
    topazScriptManager_t * mgr = context;
    topaz_context_quit(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__wait) {
    TSO_ARG_0;
    topazScriptManager_t * mgr = context;
    topaz_context_wait(mgr->ctx, topaz_script_object_as_number(arg0));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__get_time) {
    topazScriptManager_t * mgr = context;
    return topaz_script_object_from_number(script, topaz_context_get_time(mgr->ctx));
}

TSO_SCRIPT_API_FN(topaz_api__get_version_micro) {
    return topaz_script_object_from_int(script, TOPAZ__VERSION__MICRO);
}

TSO_SCRIPT_API_FN(topaz_api__get_version_major) {
    return topaz_script_object_from_int(script, TOPAZ__VERSION__MAJOR);
}

TSO_SCRIPT_API_FN(topaz_api__get_version_minor) {
    return topaz_script_object_from_int(script, TOPAZ__VERSION__MINOR);
}


TSO_SCRIPT_API_FN(topaz_api__log) {
    int newline = TRUE;
    TSO_ARG_1;
    newline = topaz_script_object_as_int(arg1);
    TSO_ARG_0;
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);
    if (newline) 
        topaz_console_print(c, topaz_script_object_as_string(arg0));
    else {
        topazColor_t col;
        col.r = col.g = col.a = col.b = 255;
        topaz_console_add_text_color(c, topaz_script_object_as_string(arg0), &col);        
    }
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__to_base64) {
    TSO_ARG_0;

    if (topaz_script_object_reference_get_feature_mask(arg0) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg0);
        uint32_t i;

        uint8_t * v = malloc(len);
        for(i = 0; i < len; ++i) {
            v[i] = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg0, i));
        }
        topazString_t * out = topaz_string_base64_from_bytes(v, len);
        free(v);
        topazScript_Object_t * obj = topaz_script_object_from_string(script, out);
        topaz_string_destroy(out);
        return obj;
    }
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(topaz_api__from_base64) {
    TSO_ARG_0;

    uint32_t size;
    uint8_t * data = topaz_string_base64_to_bytes(topaz_script_object_as_string(arg0), &size);
    if (!data) {
        // not correctly encoded.
        TSO_NO_RETURN;
    }
    topazScriptManager_t * mgr = context;
    topazResources_t * resources = topaz_context_get_resources(mgr->ctx);
    topazAsset_t * asset = topaz_resources_fetch_asset(resources, TOPAZ_STR_CAST("topaz_api__from_base64"));
    if (!asset) {
        asset = topaz_resources_create_asset(resources, TOPAZ_STR_CAST("topaz_api__from_base64"));
    }

    topaz_data_set(
        asset,
        TOPAZ_ARRAY_CAST(data, uint8_t, size)
    );

    TSO_OBJECT_NEW_VALUE(asset, TSO_OBJECT_ID__DATA | TSO_OBJECT_TYPE__ASSET, NULL, NULL);
    return object;
}

TSO_SCRIPT_API_FN(topaz_api__debug) {
    topazScriptManager_t * mgr = context;

    topazConsole_t * console = topaz_context_get_console(mgr->ctx);
    topaz_console_attach_script(console, script);
    topaz_console_enable(console, TRUE);

    topaz_script_enable_debugging(script);
    topaz_script_debug_send_command(
        script,
        topazScript_DebugCommand_Pause,
        TOPAZ_STR_CAST("script")
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__enable_console) {
    topazScriptManager_t * mgr = context;

    topazConsole_t * console = topaz_context_get_console(mgr->ctx);
    topaz_console_attach_script(console, script);
    topaz_console_enable(console, TRUE);

    TSO_NO_RETURN;
}


static void add_refs__topaz_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz__run", topaz_api__run, 0);
    TS_MAP_NATIVE_FN("topaz__pause", topaz_api__pause, 0);
    TS_MAP_NATIVE_FN("topaz__pause_now", topaz_api__pause_now, 0);
    TS_MAP_NATIVE_FN("topaz__resume", topaz_api__resume, 0);
    TS_MAP_NATIVE_FN("topaz__iterate", topaz_api__iterate, 0);
    TS_MAP_NATIVE_FN("topaz__is_paused", topaz_api__is_paused, 0);
    TS_MAP_NATIVE_FN("topaz__attach_pre_manager", topaz_api__attach_pre_manager, 1);
    TS_MAP_NATIVE_FN("topaz__attach_pre_manager_unpausable", topaz_api__attach_pre_manager_unpausable, 1);
    TS_MAP_NATIVE_FN("topaz__attach_post_manager", topaz_api__attach_post_manager, 1);
    TS_MAP_NATIVE_FN("topaz__attach_post_manager_unpausable", topaz_api__attach_post_manager_unpausable, 1);
    TS_MAP_NATIVE_FN("topaz__quit", topaz_api__quit, 0);
    TS_MAP_NATIVE_FN("topaz__wait", topaz_api__wait, 1);
    TS_MAP_NATIVE_FN("topaz__get_time", topaz_api__get_time, 0);
    TS_MAP_NATIVE_FN("topaz__get_version_micro", topaz_api__get_version_micro, 0);
    TS_MAP_NATIVE_FN("topaz__get_version_minor", topaz_api__get_version_minor, 0);
    TS_MAP_NATIVE_FN("topaz__get_version_major", topaz_api__get_version_major, 0);
    TS_MAP_NATIVE_FN("topaz__log", topaz_api__log, 2);
    TS_MAP_NATIVE_FN("topaz__to_base64", topaz_api__to_base64, 1);
    TS_MAP_NATIVE_FN("topaz__from_base64", topaz_api__from_base64, 1);
    TS_MAP_NATIVE_FN("topaz__debug", topaz_api__debug, 0);
    TS_MAP_NATIVE_FN("topaz__enable_console", topaz_api__enable_console, 0);
}
