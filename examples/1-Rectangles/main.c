#include "rectangle.h"
#include <topaz/topaz.h>
#include <topaz/modules/view_manager.h>

#include <topaz/all.h>
#include <stdio.h>

int main() {
    topaz_t * ctx = topaz_context_create();
    topaz_view_manager_create_main_default(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST("Example"));

    ///// test js
    /*
    topazScript_t * script = topaz_script_manager_create_context(
        topaz_context_get_script_manager(ctx),
        topazScriptManager_Permission_All
    );

    const topazString_t * str = topaz_script_object_as_string(
        topaz_script_expression(
            script,
            TOPAZ_STR_CAST(
                "(function(){\n"
                "   return 'Version: ' + topaz.versionMajor + '.' + topaz.versionMicro;"
                "})()"
            
            )
        )
    );

    printf("\n\nRESULT IS: %s", topaz_string_get_c_str(str));
    fflush(stdout);
    */
    /////////



    topazEntity_t * e = rectangle_create(ctx);
    topaz_context_set_root(ctx, e);
    topaz_context_run(ctx);
}
