#include <topaz/backends/script.h>

TSO_SCRIPT_API_FN(script_api__import) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;


    if (!topaz_script_import(script, topaz_script_object_as_string(arg0)))
        script_error(script, "No such asset could be loaded");


    TSO_NO_RETURN;
}


static void add_refs__script_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_script__import", script_api__import);
}
