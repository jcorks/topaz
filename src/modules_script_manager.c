#include <topaz/modules/script_manager.h>
#include <topaz/backends/script.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <topaz/containers/table.h>
#include <topaz/system.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct topazScriptManager_t {
    topaz_t * ctx;
    topazArray_t * scripts;
    topazTable_t * lookupRefs;
};


static void add_all_refs(topazScriptManager_t * s, topazScript_t *, int permissions);


topazScriptManager_t * topaz_script_manager_create(
    topaz_t * t) {

    topazScriptManager_t * out = calloc(1, sizeof(topazScriptManager_t));
    out->ctx = t;
    out->scripts = topaz_array_create(sizeof(topazScript_t*));
    out->lookupRefs = topaz_table_create_hash_pointer();
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
    topazSystem_Backend_t * backend = topaz_system_create_backend(
        topaz_context_get_system(s->ctx), 
        TOPAZ_STR_CAST("script"), 
        &api
    );

    topazScript_t * out = topaz_script_create(s->ctx, backend, &api);

    
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
#include "script_native__entity.h"
#include "script_native__asset.h"
#include "script_native__topaz.h"
#include "script_native__component.h"
#include "script_native__shape2d.h"
#include "script_native__object2d.h"
#include "script_native__text2d.h"
#include "script_native__scheduler.h"
#include "script_native__image.h"
#include "script_native__data.h"
#include "script_native__input.h"
#include "script_native__resources.h"
#include "script_native__script.h"
////////////////////////
//////////////////////// script_native__
////////////////////////


static void add_all_refs(topazScriptManager_t * s, topazScript_t * script, int permissions) {
    add_refs__vector_api(script, s);
    add_refs__color_api(script, s);
    add_refs__entity_api(script, s);
    add_refs__asset_api(script, s);
    add_refs__topaz_api(script, s);
    add_refs__component_api(script, s);
    add_refs__shape2d_api(script, s);
    add_refs__object2d_api(script, s);
    add_refs__text2d_api(script, s);
    add_refs__scheduler_api(script, s);
    add_refs__image_api(script, s);
    add_refs__data_api(script, s);
    add_refs__input_api(script, s);
    add_refs__resources_api(script, s);
    add_refs__script_api(script, s);

}
 
