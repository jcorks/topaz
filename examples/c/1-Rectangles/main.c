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


    {
        topazResources_t * res = topaz_context_get_resources(ctx);
        topazAsset_t * sound = topaz_resources_fetch_asset(res, topazAsset_Type_Sound, TOPAZ_STR_CAST("test"));
        
        topazSound_Sample_t samples[] = {
            { 0, 0},
            { 0, 1},
            { 0, 2},
            { 0, 4},
            { 0, 8},
            { 0, 16},
            { 1, 32},
            { 2, 64},
            { 4, 128},
            { 8, 0xff},
            { 16, 0xff},
            { 32, 0xff},
            { 64, 0xff},
            { 64, 0xff}

        };      
        topaz_sound_set_samples(sound, samples, sizeof(samples)/sizeof(topazSound_Sample_t));
        
        
        topaz_audio_play_sound(topaz_context_get_audio(ctx), sound, 0, 0.7, 0.8);

    }


    topaz_context_run(ctx);
}
