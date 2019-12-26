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

#include <topaz/topaz.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



struct topaz_t {
    topaz_Attributes_t api;
    
    topazTable_t * params;
    topazEntity_t * universe;
    topazEntity_t * managers;
    topazEntity_t * managersNP;

    topazArray_t * modules; // moduels are entities
    
    
    topazEntity_t * graphics;
    
    int quit;
    int paused;
    int fps;
};




topaz_t * topaz_context_create(const topaz_Attributes_t * a) {
    topaz_t * out = malloc(sizeof(topaz_t));
    out->fps = 60;
    out->paused = FALSE;
    out->quit = FALSE;
    out->api = *a;
    out->params = topaz_table_create_hash_topaz_string();

    // defaultParams
    topaz_table_insert(out->params, TOPAZ_STR_CAST("framerate"),     topaz_string_create_from_c_str("%d", 60));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-micro"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MICRO));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-minor"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MINOR));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-major"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MAJOR));
    

    out->universe = topaz_entity_null();
    out->managers = topaz_entity_create(out);
    out->managerNP = topaz_entity_create(out);
    out->modules  = topaz_array_create(sizeof(topazEntity_t*));
    
    
    /// create modules    
    //out->graphics = topaz_graphics_create(a->rendererBackend, &a->rendererAPI);
    //topaz_array_push(out->modules, out->graphics);



    // initialize all modules throught the attach signals
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(out->modules); ++i) {
        topazEntity_t * e = topaz_array_at(out->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_enttiy_get_attributes(e);
        if (api->on_attach) api->on_attach(e, api->userData);
    }
    return out;
}

const topaz_Attributes_t * topaz_context_get_attributes(const topaz_t * t) {
    return &t->api;
}





topaz_t * topaz_context_create_empty() {
    // TODO
    return NULL;
}


int topaz_context_run(const topaz_t * t) {
    if (quit == TRUE) {
        // requested early termination?
        return -2;
    }

    while(!t->quit) {
        topaz_context_iterate(t);
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
    
void topaz_context_iterate(topaz_t * t) {
    /////// step 
    // Order:
    /*
        - modules prestep fn
        - user entity step() 
        - modules step fn
        - managers step() 
        - non-pausable managers step
    
    */
    topazEntity_t * e;
    uint32_t i;
    uint32_t len;
    
    
    
    len = topaz_array_get_size(t->modules);
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_pre_step) api->on_pre_step(e, api->userData);
    }

    if (!t->paused) {
        topaz_entity_step(t->universe);
    }
    
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_step) api->on_step(e, api->userData);
    }
    
    
    if (!t->paused);
        topaz_entity_step(t->managers);
    }    
    topaz_entity_step(t->managersNP);
    
    
    
    
    /////// render 
    // Order:
    /*
        - modules predraw fn
        - entity draw()
        - modules draw fn
        - managers draw()
        - np Managers draw()
        - commit if applicable (needed?)
        
    
    
    */
    
    len = topaz_array_get_size(t->modules);
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_pre_draw) api->on_pre_draw(e, api->userData);
    }

    if (!t->paused) {
        topaz_entity_draw(t->universe);
    }
    
    for(i = 0; i < len; ++i) {
        e = topaz_array_at(t->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_draw) api->on_draw(e, api->userData);
    }
    
    
    if (!t->paused);
        topaz_entity_draw(t->managers);
    }    
    topaz_entity_draw(t->managersNP);
    
    
    
    
    /// throttle 
    if (t->fps >= 0) {
        topaz_engine_wait(t->fps);
        
    }
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


/// Attaches a management-type entity.
///
/// If pausable is false, the manager will continue to update even when 
/// the engine is in a paused state.
void topaz_context_attach_manager(topazEntityID_t id);
void topaz_context_attach_manager_unpausable(topazEntityID_t id);



/// Ends the Engine execution loop.
///
void topaz_context_quit(topaz_t *);

/// Sleeps until the time required for the target frames-per-second is reached.
///
/// @param FPS  The target FPS; useful for loops.
///
/// The actual resolution is machine-dependent, but it tends to be millisecond resolution.
void topaz_context_wait(topaz_t *. int FPS);


/// Retrieves the specified parameter.
///
/// List of known parameters:
///
///     "fps"               - frames per second for the engine loop 
///     "version-minor"     - minor version 
///     "version-major"     - major version 
///     "git-hash"          - hash for this commit
///     "base-directory"    - start directory for this instance 
///     "draw-time-ms"      - time it took to complete a draw cycle last frame 
///     "step-time-ms"      - time it took to complete a step cycle last frame 
///     "system-time-ms"    - time it took to complete engine tasks last frame 
///     "engine-real-ms"    - time it took to complete the entire last frame
double topaz_get_parameter(const topaz_t *, const topazString_t *);

/// Sets a parameter. Some parameters are read-only. In such a case, 
/// 0 is returned.
int topaz_context_set_parameter(const topaz_t *, const topazString_t *, double);

/// Retrieves an array of all parameter names known.
///
const topazArray_t * topaz_get_parameter_names(const topaz *);


