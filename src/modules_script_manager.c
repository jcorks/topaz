#include <topaz/modules/script_manager.h>
#include <topaz/backends/script.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <topaz/system.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>
struct topazScriptManager_t {
    topaz_t * ctx;
    topazArray_t * scripts;
};


static void add_all_refs(topazScriptManager_t * s, topazScript_t *, int permissions);


topazScriptManager_t * topaz_script_manager_create(
    topaz_t * t) {

    topazScriptManager_t * out = calloc(1, sizeof(topazScriptManager_t));
    out->ctx = t;
    out->scripts = topaz_array_create(sizeof(topazScript_t*));
    return out;
}




void script_manager_destroy(topazScriptManager_t * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->scripts);
    for(i = 0; i < len; ++i) {
        topaz_script_destroy(
            topaz_array_at(
                s->scripts, 
                topazScript_t *,
                i
            )
        );
    }

    free(s);
}


topazScript_t * topaz_script_manager_create_context(
    topazScriptManager_t * s,
    topazScriptManager_Permission_t permissions
) {
    topazScriptAPI_t api = {};
    topazBackend_t * backend = topaz_system_create_backend(
        topaz_context_get_system(s->ctx), 
        TOPAZ_STR_CAST("script"), 
        &api
    );

    topazScript_t * out = topaz_script_create(backend, &api);

    
    add_all_refs(
        s,
        out,
        permissions
    );

    topaz_script_bootstrap(out);
    return out;
}








////////////////////////
//////////////////////// script_native__
////////////////////////

#include "script_native__macro.h"
#include "script_native__vector.h"
#include "script_native__color.h"

////////////////////////
//////////////////////// script_native__
////////////////////////


static void add_all_refs(topazScriptManager_t * s, topazScript_t * script, int permissions) {
    add_refs__vector_api(script);
    add_refs__color_api(script);
}
