#include "rectangle.h"
#include <topaz/topaz.h>
#include <topaz/modules/view_manager.h>

#include <topaz/all.h>
#include <stdio.h>

int main() {
    topaz_t * ctx = topaz_context_create();
    topaz_view_manager_create_main_default(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST("Example"));

    topazEntity_t * r = topaz_entity_create(ctx);

    topazEntity_t * e0 = rectangle_create(ctx);
    topazEntity_t * e1 = rectangle_create(ctx);

    topaz_entity_position(e0)->y = 50;

    topaz_entity_attach(r, e0);
    topaz_entity_attach(r, e1);

    topaz_context_set_root(ctx, r);


    /*{
        topazResources_t * res = topaz_context_get_resources(ctx);
        topazAsset_t * sound = topaz_resources_load_asset(
            res, 
            TOPAZ_STR_CAST("ogg"), 
            TOPAZ_STR_CAST("plion.ogg"),
            TOPAZ_STR_CAST("test")
        );
            
        
        topaz_audio_play_sound(topaz_context_get_audio(ctx), sound, 0, 0.7, 0.8);

    }*/


    topaz_context_run(ctx);
}
