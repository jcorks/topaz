#include <topaz/topaz.h>
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

TSO_SCRIPT_API_FN(topaz_api__break) {
    topazScriptManager_t * mgr = context;
    topaz_context_break(mgr->ctx);
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

TSO_SCRIPT_API_FN(topaz_api__step) {
    topazScriptManager_t * mgr = context;
    topaz_context_step(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__draw) {
    topazScriptManager_t * mgr = context;
    topaz_context_draw(mgr->ctx);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__is_paused) {
    topazScriptManager_t * mgr = context;
    return topaz_script_object_from_int(script, topaz_context_is_paused(mgr->ctx));
}


TSO_SCRIPT_API_FN(topaz_api__get_root) {
    topazScriptManager_t * mgr = context;
    topazEntity_t * e = topaz_context_get_root(mgr->ctx);
    TSO_OBJECT_NEW(e, TSO_OBJECT_ID__ENTITY, NULL);
    return object;
}

TSO_SCRIPT_API_FN(topaz_api__set_root) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_set_root(mgr->ctx, native);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__attach_pre_manager) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_pre_manager(mgr->ctx, native);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__attach_pre_manager_unpausable) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_pre_manager_unpausable(mgr->ctx, native);    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(topaz_api__attach_post_manager) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazScriptManager_t * mgr = context;
    topaz_context_attach_post_manager(mgr->ctx, native);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(topaz_api__attach_post_manager_unpausable) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
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
    TSO_ASSERT_ARG_COUNT(1);
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
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    printf("%s", topaz_string_get_c_str(topaz_script_object_as_string(arg0)));
    fflush(stdout);
    TSO_NO_RETURN;
}


static void add_refs__topaz_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz__run", topaz_api__run);
    TS_MAP_NATIVE_FN("topaz__pause", topaz_api__pause);
    TS_MAP_NATIVE_FN("topaz__break", topaz_api__break);
    TS_MAP_NATIVE_FN("topaz__resume", topaz_api__resume);
    TS_MAP_NATIVE_FN("topaz__iterate", topaz_api__iterate);
    TS_MAP_NATIVE_FN("topaz__step", topaz_api__step);
    TS_MAP_NATIVE_FN("topaz__draw", topaz_api__draw);
    TS_MAP_NATIVE_FN("topaz__is_paused", topaz_api__is_paused);
    TS_MAP_NATIVE_FN("topaz__get_root", topaz_api__get_root);
    TS_MAP_NATIVE_FN("topaz__set_root", topaz_api__set_root);
    TS_MAP_NATIVE_FN("topaz__attach_pre_manager", topaz_api__attach_pre_manager);
    TS_MAP_NATIVE_FN("topaz__attach_pre_manager_unpausable", topaz_api__attach_pre_manager_unpausable);
    TS_MAP_NATIVE_FN("topaz__attach_post_manager", topaz_api__attach_post_manager);
    TS_MAP_NATIVE_FN("topaz__attach_post_manager_unpausable", topaz_api__attach_post_manager_unpausable);
    TS_MAP_NATIVE_FN("topaz__quit", topaz_api__quit);
    TS_MAP_NATIVE_FN("topaz__wait", topaz_api__wait);
    TS_MAP_NATIVE_FN("topaz__get_time", topaz_api__get_time);
    TS_MAP_NATIVE_FN("topaz__get_version_micro", topaz_api__get_version_micro);
    TS_MAP_NATIVE_FN("topaz__get_version_minor", topaz_api__get_version_minor);
    TS_MAP_NATIVE_FN("topaz__get_version_major", topaz_api__get_version_major);
    TS_MAP_NATIVE_FN("topaz__log", topaz_api__log);
}
