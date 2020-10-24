#include "rectangle.h"
#include <topaz/topaz.h>
#include <topaz/modules/view_manager.h>

#include <topaz/all.h>
#include <stdio.h>

int main() {
    topaz_t * ctx = topaz_context_create();
    topaz_view_manager_create_main_default(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST("Example"));

    topazEntity_t * e = rectangle_create(ctx);
    topaz_context_set_root(ctx, e);
    topaz_context_run(ctx);
}
