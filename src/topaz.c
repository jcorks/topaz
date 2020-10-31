/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/

#include <topaz/compat.h>
#include <topaz/topaz.h>
#include <topaz/backends/time.h>
#include <topaz/backends/filesys.h>
#include <topaz/modules/graphics.h>
#include <topaz/modules/script_manager.h>
#include <topaz/modules/input.h>
#include <topaz/modules/view_manager.h>
#include <topaz/modules/resources.h>
#include <topaz/modules/console.h>
#include <topaz/containers/table.h>
#include <topaz/system.h>
#include <topaz/entity.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



struct topaz_t {
    topazSystem_t * system;
    
    topazTable_t * params;
    topazEntity_t * universe;
    topazEntity_t * managersPre;
    topazEntity_t * managersPreNP;
    topazEntity_t * managersPost;
    topazEntity_t * managersPostNP;

    topazArray_t * modules; // moduels are entities


    // modules
    topazInput_t * input;
    topazViewManager_t * viewManager;
    topazResources_t * resources;
    topazGraphics_t * graphics;
    topazScriptManager_t * script;
    topazConsole_t * console;

    topazTime_t * timeRef;
    uint64_t frameEnd;
    uint64_t frameStart;
    
    int quit;
    int paused;
    int fps;


};
static int configured = 0;

topaz_t * topaz_context_create() {
    if (!configured)
        topaz_system_configure();

    return topaz_context_create_from_system( topaz_system_create_default());
}
topaz_t * topaz_context_create_from_system(topazSystem_t * a) {
    if (!configured)
        topaz_system_configure();

    topaz_t * out = calloc(1, sizeof(topaz_t));
    out->fps = 60;
    out->paused = FALSE;
    out->quit = FALSE;
    out->system = a;
    out->params = topaz_table_create_hash_topaz_string();

    {
        topazTimeAPI_t api;
        topazSystem_Backend_t * ref = topaz_system_create_backend(out->system, TOPAZ_STR_CAST("time"), &api);
        out->timeRef = topaz_time_create(ref, api);
    }


    // defaultParams
    topaz_table_insert(out->params, TOPAZ_STR_CAST("framerate"),     topaz_string_create_from_c_str("%d", 60));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-micro"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MICRO));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-minor"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MINOR));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-major"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MAJOR));
    

    out->universe = topaz_entity_null();
    out->managersPre = topaz_entity_create(out);
    out->managersPreNP = topaz_entity_create(out);
    out->managersPost = topaz_entity_create(out);
    out->managersPostNP = topaz_entity_create(out);
    out->modules  = topaz_array_create(sizeof(topazEntity_t*));
    




    /// create modules    
    out->input = topaz_input_create(out);
    out->viewManager = topaz_view_manager_create(out);
    out->resources = topaz_resources_create(out);
    out->script = topaz_script_manager_create(out);
    out->graphics = topaz_graphics_create(out);
    out->console = topaz_console_create(out);

    // creating



    // initialize all modules throught the attach signals
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(out->modules); ++i) {
        topazEntity_t * e = topaz_array_at(out->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_attach) api->on_attach(e, api->userData);
    }

    return out;
}

const topazSystem_t * topaz_context_get_system(const topaz_t * t) {
    return t->system;
}


void topaz_context_destroy(topaz_t * t) {
    topaz_time_destroy(t->timeRef);
    


    topaz_entity_remove(t->universe);
    topaz_entity_remove(t->managersPre);
    topaz_entity_remove(t->managersPreNP);
    topaz_entity_remove(t->managersPost);
    topaz_entity_remove(t->managersPostNP);
    topaz_array_destroy(t->modules);

    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, t->params);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {

        topaz_string_destroy(topaz_table_iter_get_value(iter));
    }
    topaz_table_iter_destroy(iter);
    topaz_table_destroy(t->params);

    topaz_input_destroy(t->input);
    topaz_view_manager_destroy(t->viewManager);
    topaz_resources_destroy(t->resources);
    topaz_graphics_destroy(t->graphics);

    free(t);
        
}


topaz_t * topaz_context_create_empty() {
    if (!configured)
        topaz_system_configure();

    topazSystem_t * sys = topaz_system_create_default();
    topaz_system_set_backend(sys, TOPAZ_STR_CAST("renderer"),     TOPAZ_STR_CAST("noRenderer"));
    topaz_system_set_backend(sys, TOPAZ_STR_CAST("audioManager"), TOPAZ_STR_CAST("noAudioManager"));
    topaz_system_set_backend(sys, TOPAZ_STR_CAST("inputManager"), TOPAZ_STR_CAST("noInputManager"));
    topaz_system_set_backend(sys, TOPAZ_STR_CAST("display"),      TOPAZ_STR_CAST("noDisplay"));
    topaz_system_set_backend(sys, TOPAZ_STR_CAST("time"),         TOPAZ_STR_CAST("noTime"));
    topaz_system_set_backend(sys, TOPAZ_STR_CAST("filesys"),      TOPAZ_STR_CAST("noFilesys"));
    return topaz_context_create(sys);
}


int topaz_context_run(topaz_t * t) {
    if (t->quit == TRUE) {
        // requested early termination?
        return -2;
    }

    while(!t->quit) {
        topaz_context_iterate(t);
        /// throttle 
        if (t->fps >= 0) {
            topaz_context_wait(t, t->fps);
            
        }
    }

    return 0;
}
    
    
void topaz_context_pause(topaz_t * t) {
    t->paused = TRUE;
}

    
void topaz_context_break(topaz_t * t) {
    t->paused = TRUE;
    while(t->paused)
        topaz_context_iterate(t);
}



void topaz_context_resume(topaz_t * t) {
    t->paused = TRUE;
}
    
void topaz_context_step(topaz_t * t) {
    /////// step 
    // Order:
    /*
        - pre backends
        - pre managers step() 
        - non-pausable pre managers step
        - modules prestep fn
        - user entity step() 
        - modules step fn
        - post managers step() 
        - non-pausable post managers step
        - post backends step
    
    */
    topazEntity_t * e;
    uint32_t i;
    uint32_t len;
    
    topaz_system_pre_step(t->system);
    
    if (!t->paused) {
        topaz_entity_step(t->managersPre);
    }    
    topaz_entity_step(t->managersPreNP);
    
    len = topaz_array_get_size(t->modules);
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        if (!topaz_entity_is_valid(e)) continue;

        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_pre_step) api->on_pre_step(e, api->userData);
    }

    if (!t->paused) {
        if (topaz_entity_is_valid(t->universe))
            topaz_entity_step(t->universe);
    }
    
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        if (!topaz_entity_is_valid(e)) continue;
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_step) api->on_step(e, api->userData);
    }
    
    
    if (!t->paused) {
        topaz_entity_step(t->managersPost);
    }    
    topaz_entity_step(t->managersPostNP);

    topaz_system_post_step(t->system);

}


void topaz_context_draw(topaz_t * t) {
    
    
    /////// render 
    // Order:
    /*
        - backends pre draw
        - modules predraw fn
        - entity draw()
        - modules draw fn
        - managers draw()
        - np Managers draw()
        - backends post draw        
        - commit if applicable (needed?)
    
    
    */
    topazEntity_t * e;
    uint32_t i;
    uint32_t len;

    topaz_system_pre_draw(t->system);


    if (!t->paused) {
        topaz_entity_draw(t->managersPre);
    }    
    topaz_entity_draw(t->managersPreNP);

    len = topaz_array_get_size(t->modules);
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        if (!topaz_entity_is_valid(e)) continue;

        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_pre_draw) api->on_pre_draw(e, api->userData);
    }

    if (!t->paused) {
        if (topaz_entity_is_valid(t->universe))
            topaz_entity_draw(t->universe);
    }
    
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        if (!topaz_entity_is_valid(e)) continue;

        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_draw) api->on_draw(e, api->userData);
    }
    
    
    if (!t->paused) {
        topaz_entity_draw(t->managersPost);
    }    
    topaz_entity_draw(t->managersPostNP);
    topaz_system_post_draw(t->system);

    topaz_graphics_sync(t->graphics);
}

void topaz_context_iterate(topaz_t * t) {
    topaz_context_step(t);
    topaz_context_draw(t);
    topaz_view_manager_update_view(t->viewManager);
    topaz_graphics_reset_scene(t->graphics);
}

int topaz_context_is_paused(const topaz_t * t) {
    return t->paused;
}


topazEntity_t * topaz_context_get_root(const topaz_t * t) {
    return t->universe;
}

void topaz_context_set_root(topaz_t * t, topazEntity_t * u) {
    t->universe = u;
}


void topaz_context_attach_pre_manager(topaz_t * t, topazEntity_t * id) {
    topaz_entity_attach(t->managersPre, id);
}

void topaz_context_attach_pre_manager_unpausable(topaz_t * t, topazEntity_t * id) {
    topaz_entity_attach(t->managersPreNP, id);

}

void topaz_context_attach_post_manager(topaz_t * t, topazEntity_t * id) {
    topaz_entity_attach(t->managersPost, id);
}

void topaz_context_attach_post_manager_unpausable(topaz_t * t, topazEntity_t * id) {
    topaz_entity_attach(t->managersPostNP, id);

}



void topaz_context_quit(topaz_t * t) {
    t->quit = TRUE;
}

void topaz_context_wait(topaz_t * t, int FPS) {
    t->frameEnd = topaz_context_get_time(t);
    uint64_t realEnd = t->frameEnd;

    
    // TODO: swap to just sleep for the time difference directly?
    while (realEnd - t->frameStart < 1000.0 / ((float)FPS)) {
        topaz_time_sleep_ms(t->timeRef, 1);
        realEnd = topaz_context_get_time(t);
    }

    t->frameStart = topaz_context_get_time(t);
}


const topazString_t * topaz_get_parameter(const topaz_t * t, const topazString_t * str) {
    topazString_t * out = topaz_table_find(t->params, str);
    if (!out) {
        return TOPAZ_STR_CAST("");
    }
    return out;
}


int topaz_context_set_parameter(const topaz_t * t, const topazString_t * key, const topazString_t * data) {
    topazString_t * oldRef = topaz_table_find(t->params, key);
    if (!oldRef) {
        return 0;
    }

    topaz_string_set(oldRef, data);
    return 1;
}


topazArray_t * topaz_get_parameter_names(const topaz_t * t) {
    topazArray_t * names = topaz_array_create(sizeof(topazString_t*));

    topazTableIter_t * iter = topaz_table_iter_create();
    for( topaz_table_iter_start(iter, t->params);
        !topaz_table_iter_is_end(iter);
         topaz_table_iter_proceed(iter)) {
    
        topazString_t * t = topaz_table_iter_get_value(iter);        
        topaz_array_push(names, t);
    }
    topaz_table_iter_destroy(iter);
    return names;
}


uint64_t topaz_context_get_time(topaz_t * t) {
    return topaz_time_ms_since_startup(t->timeRef);
}

topazFilesys_t * topaz_context_filesys_create(const topaz_t * t) {
    topazFilesysAPI_t api;
    topazSystem_Backend_t * backend = topaz_system_create_backend(
        t->system,
        TOPAZ_STR_CAST("filesys"),
        &api
    );
    return topaz_filesys_create(backend, api);
}


topazInput_t * topaz_context_get_input(topaz_t * t) {
    return t->input;
}

topazViewManager_t * topaz_context_get_view_manager(topaz_t * t) {
    return t->viewManager;
}

topazResources_t * topaz_context_get_resources(topaz_t * t) {
    return t->resources;
}

topazGraphics_t * topaz_context_get_graphics(topaz_t * t) {
    return t->graphics;
}

topazScriptManager_t  * topaz_context_get_script_manager(topaz_t * t) {
    return t->script;

}

topazConsole_t * topaz_context_get_console(topaz_t * t) {
    return t->console;
}
