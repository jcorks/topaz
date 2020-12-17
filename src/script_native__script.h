#include <topaz/backends/script.h>

TSO_SCRIPT_API_FN(script_api__import) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    
    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * src = topaz_resources_load_asset(
        r,
        TOPAZ_STR_CAST("txt"),
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_string(arg0)
    );


    

    if (src) {
        topazString_t * srcStr = topaz_data_get_as_string(src);
        topaz_script_run_once(
            script,
            topaz_script_object_as_string(arg0),
            srcStr
        );
        topaz_string_destroy(srcStr);
    } else {
        script_error(script, "No such asset could be loaded");
    }

    TSO_NO_RETURN;
}


static void add_refs__script_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_script__import", script_api__import);
}
