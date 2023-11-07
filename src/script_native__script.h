#include <topaz/backends/script.h>


TSO_SCRIPT_API_FN(script_api__run) {
    TSO_ARG_0;
    TSO_ARG_1;


    topaz_script_run(script, 
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_string(arg1)
    );

    TSO_NO_RETURN;
}


static void add_refs__script_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_script__run", script_api__run, 2);
}
