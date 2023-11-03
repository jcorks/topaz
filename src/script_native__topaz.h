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

TSO_SCRIPT_API_FN(topaz_api__frame_start) {
    TSO_ARG_0;
    topazScriptManager_t * mgr = context;
    topaz_context_frame_start(mgr->ctx, topaz_script_object_as_number(arg0));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__frame_end) {
    topazScriptManager_t * mgr = context;
    topaz_context_frame_end(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__get_time) {
    topazScriptManager_t * mgr = context;
    return topaz_script_object_from_number(script, topaz_context_get_time(mgr->ctx));
}

TSO_SCRIPT_API_FN(topaz_api__get_delta_time) {
    topazScriptManager_t * mgr = context;
    return topaz_script_object_from_number(script, topaz_context_get_delta_time(mgr->ctx));
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
    TS_MAP_NATIVE_FN("topaz__frame_start", topaz_api__frame_start, 1);
    TS_MAP_NATIVE_FN("topaz__frame_end", topaz_api__frame_end, 0);
    TS_MAP_NATIVE_FN("topaz__get_time", topaz_api__get_time, 0);
    TS_MAP_NATIVE_FN("topaz__get_delta_time", topaz_api__get_time, 0);
    TS_MAP_NATIVE_FN("topaz__get_version_micro", topaz_api__get_version_micro, 0);
    TS_MAP_NATIVE_FN("topaz__get_version_minor", topaz_api__get_version_minor, 0);
    TS_MAP_NATIVE_FN("topaz__get_version_major", topaz_api__get_version_major, 0);
    TS_MAP_NATIVE_FN("topaz__debug", topaz_api__debug, 0);
}
