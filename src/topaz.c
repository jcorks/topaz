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

    // singleton-like backends
    topazInputManager_t * inputManager;
    topazRenderer_t * renderer;
    topazRenderer_2D_t * renderer2d;
    // modules
    topazInput_t * input;
    topazViewManager_t * viewManager;
    topazResources_t * resources;

    topazTime_t * timeRef;
    uint64_t frameEnd;
    uint64_t frameStart;
    
    int quit;
    int paused;
    int fps;


};

static intptr_t api_nothing(){return 0;}



topaz_t * topaz_context_create(const topaz_Attributes_t * a) {
    topaz_t * out = calloc(1, sizeof(topaz_t));
    out->fps = 60;
    out->paused = FALSE;
    out->quit = FALSE;
    out->api = *a;
    out->params = topaz_table_create_hash_topaz_string();
    out->timeRef = topaz_time_create(a->timeBackend, out->api.timeAPI);

    // defaultParams
    topaz_table_insert(out->params, TOPAZ_STR_CAST("framerate"),     topaz_string_create_from_c_str("%d", 60));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-micro"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MICRO));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-minor"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MINOR));
    topaz_table_insert(out->params, TOPAZ_STR_CAST("version-major"), topaz_string_create_from_c_str("%d", TOPAZ__VERSION__MAJOR));
    

    out->universe = topaz_entity_null();
    out->managers = topaz_entity_create(out);
    out->managersNP = topaz_entity_create(out);
    out->modules  = topaz_array_create(sizeof(topazEntity_t*));
    

    // backend init if any
    out->inputManager = topaz_input_manager_create(a->inputManagerBackend, a->inputManagerAPI);
    out->renderer     = topaz_renderer_create(a->rendererBackend, a->rendererAPI);
    out->renderer2d   = topaz_renderer_2d_create(out->renderer);

    /// create modules    
    //out->graphics = topaz_graphics_create(a->rendererBackend, &a->rendererAPI);
    out->input = topaz_input_create(out, out->inputManager);
    out->viewManager = topaz_view_manager_create(out);
    out->resources = topaz_resources_create(out);


    // initialize all modules throught the attach signals
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(out->modules); ++i) {
        topazEntity_t * e = topaz_array_at(out->modules, topazEntity_t *, i);
        const topazEntity_Attributes_t * api = topaz_entity_get_attributes(e);
        if (api->on_attach) api->on_attach(e, api->userData);
    }
    return out;
}

const topaz_Attributes_t * topaz_context_get_attributes(const topaz_t * t) {
    return &t->api;
}


void topaz_context_destroy(topaz_t * t) {
    topaz_time_destroy(t->timeRef);
    


    topaz_entity_remove(t->universe);
    topaz_entity_remove(t->managers);
    topaz_entity_remove(t->managersNP);
    topaz_array_destroy(t->modules);

    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, t->params);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {

        topaz_string_destroy(topaz_table_iter_get_value(iter));
    }
    topaz_table_iter_destroy(iter);
    topaz_table_destroy(t->params);
    free(t);
        
}


topaz_t * topaz_context_create_empty() {
    // TODO
    topaz_Attributes_t attr;
    memset(&attr, 0xff, sizeof(topaz_Attributes_t));

    attr.audioManagerAPI.audio_manager_create          = (void (*)(topazAudioManagerAPI_t *)) api_nothing;
    attr.audioManagerAPI.audio_manager_destroy         = (void (*)(topazAudioManagerAPI_t *)) api_nothing;
    attr.audioManagerAPI.audio_manager_connect         = (int  (*)(topazAudioManagerAPI_t *, void (*audioStreamHandler)(topazAudioManager_t *, uint32_t, float *, void *), void *)) api_nothing;
    attr.audioManagerAPI.audio_manager_set_sample_rate = (void (*)(topazAudioManagerAPI_t *, uint32_t)) api_nothing;
    attr.audioManagerAPI.audio_manager_get_sample_rate = (uint32_t (*)(topazAudioManagerAPI_t *)) api_nothing;
    attr.audioManagerAPI.audio_manager_is_underrun     = (int (*)(topazAudioManagerAPI_t *)) api_nothing;
    attr.audioManagerAPI.audio_manager_enable_output   = (void (*)(topazAudioManagerAPI_t *, int)) api_nothing;
    attr.audioManagerAPI.audio_manager_set_volume_multiplier = (void (*)(topazAudioManagerAPI_t *, float)) api_nothing;
    attr.audioManagerAPI.audio_manager_get_volume_multiplier = (float (*)(topazAudioManagerAPI_t *)) api_nothing;
    attr.audioManagerAPI.audio_manager_get_current_output_sample = (float (*)(topazAudioManagerAPI_t *)) api_nothing;


    attr.displayAPI.display_create = (void (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_destroy = (void (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_resize = (void (*)(topazDisplayAPI_t *, int, int)) api_nothing;
    attr.displayAPI.display_set_position = (void (*)(topazDisplayAPI_t *, int, int)) api_nothing;
    attr.displayAPI.display_hide = (void (*)(topazDisplayAPI_t *, int)) api_nothing;
    attr.displayAPI.display_has_input_focus = (int (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_lock_client_resize = (void (*)(topazDisplayAPI_t *, int)) api_nothing;
    attr.displayAPI.display_lock_client_position = (void (*)(topazDisplayAPI_t *, int)) api_nothing;
    attr.displayAPI.display_set_view_policy = (void (*)(topazDisplayAPI_t *, topazDisplay_ViewPolicy)) api_nothing;
    attr.displayAPI.display_get_height = (int (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_width = (int (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_x = (int (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_y = (int (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_set_name = (void (*)(topazDisplayAPI_t *, const topazString_t *)) api_nothing;
    attr.displayAPI.display_add_resize_callback = (void (*)(topazDisplayAPI_t *, void(*)(int w, int h, void *), void *)) api_nothing;
    attr.displayAPI.display_remove_resize_callback = (void (*)(topazDisplayAPI_t *, void(*)(int w, int h, void *))) api_nothing;
    attr.displayAPI.display_add_close_callback = (void (*)(topazDisplayAPI_t *, void(*)(void *), void *)) api_nothing;
    attr.displayAPI.display_remove_close_callback = (void (*)(topazDisplayAPI_t *, void(*)(void *))) api_nothing;
    attr.displayAPI.display_is_capable = (int (*)(topazDisplayAPI_t *, topazDisplay_Capability)) api_nothing;
    attr.displayAPI.display_update = (void (*)(topazDisplayAPI_t *, topazRenderer_Framebuffer_t *)) api_nothing;
    attr.displayAPI.display_supported_framebuffers = (const topazArray_t * (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_system_handle_type = (topazDisplay_Handle (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_system_handle = (void * (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_system_event_type = (topazDisplay_Event (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_last_system_event = (void * (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_get_current_clipboard = (topazArray_t * (*)(topazDisplayAPI_t *)) api_nothing;
    attr.displayAPI.display_set_current_clipboard = (void (*)(topazDisplayAPI_t *, const topazArray_t *)) api_nothing;
    attr.filesysAPI.filesys_create = (void (*)(topazFilesysAPI_t *))api_nothing;
    attr.filesysAPI.filesys_destroy = (void (*)(topazFilesysAPI_t *))api_nothing;
    attr.filesysAPI.filesys_query = (const topazArray_t * (*)(topazFilesysAPI_t *))api_nothing;
    attr.filesysAPI.filesys_set_path = (int (*)(topazFilesysAPI_t *, const topazString_t *))api_nothing;
    attr.filesysAPI.filesys_get_path = (const topazString_t * (*)(topazFilesysAPI_t *))api_nothing;
    attr.filesysAPI.filesys_go_to_child = (int (*)(topazFilesysAPI_t *, const topazString_t *))api_nothing;
    attr.filesysAPI.filesys_go_to_parent = (int (*)(topazFilesysAPI_t *))api_nothing;
    attr.filesysAPI.filesys_create_node = (int (*)(topazFilesysAPI_t *, const topazString_t *))api_nothing;
    attr.filesysAPI.filesys_read = (topazRbuffer_t * (*)(topazFilesysAPI_t *, const topazString_t *))api_nothing;
    attr.filesysAPI.filesys_write = (int (*)(topazFilesysAPI_t *, const topazString_t *, const topazWbuffer_t *))api_nothing;
    attr.filesysAPI.filesys_is_node = (int (*)(topazFilesysAPI_t *, const topazString_t *))api_nothing;
    attr.filesysAPI.filesys_is_child = (int (*)(topazFilesysAPI_t *, const topazString_t *))api_nothing;
    attr.inputManagerAPI.input_manager_create = (void (*)(topazInputManagerAPI_t *)) api_nothing;
    attr.inputManagerAPI.input_manager_destroy = (void (*)(topazInputManagerAPI_t *)) api_nothing;
    attr.inputManagerAPI.input_manager_id_to_string = (const char * (*)(int)) api_nothing;
    attr.inputManagerAPI.input_manager_handle_events = (int (*)(topazInputManagerAPI_t *)) api_nothing;
    attr.inputManagerAPI.input_manager_query_device = (topazInputDevice_t * (*)(topazInputManagerAPI_t *, int)) api_nothing;
    attr.inputManagerAPI.input_manager_query_auxiliary_devices = (int (*)(topazInputManagerAPI_t *, int *)) api_nothing;
    attr.inputManagerAPI.input_manager_max_devices = (int (*)(topazInputManagerAPI_t *)) api_nothing;
    attr.inputManagerAPI.input_manager_set_focus = (void (*)(topazInputManagerAPI_t *, topazDisplay_t *)) api_nothing;
    attr.inputManagerAPI.input_manager_get_focus = (topazDisplay_t * (*)(topazInputManagerAPI_t *)) api_nothing;
    attr.inputManagerAPI.input_manager_show_virtual_keyboard = (void (*)(topazInputManagerAPI_t *, int)) api_nothing;
    attr.timeAPI.time_create = (void (*)(topazTimeAPI_t *)) api_nothing;
    attr.timeAPI.time_destroy = (void (*)(topazTimeAPI_t *)) api_nothing;
    attr.timeAPI.time_sleep_ms = (void (*)(topazTimeAPI_t *, uint64_t)) api_nothing;
    attr.timeAPI.time_ms_since_startup = (uint64_t (*)(topazTimeAPI_t *)) api_nothing;
    attr.rendererAPI.core.renderer_create = (void (*)(topazRenderer_CoreAPI_t*))api_nothing;
    attr.rendererAPI.core.renderer_destroy = (void (*)(topazRenderer_CoreAPI_t*))api_nothing;
    attr.rendererAPI.core.renderer_draw_2d = (void (*)(topazRenderer_CoreAPI_t*, topazRenderer_2DAPI_t *, const topazRenderer_2D_Context_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    attr.rendererAPI.core.renderer_draw_3d = (void (*)(topazRenderer_CoreAPI_t *, topazRenderer_3D_t *, const topazRenderer_ProcessAttribs_t *))api_nothing;
    attr.rendererAPI.core.renderer_set_3d_viewing_matrix = (void (*)(topazRenderer_CoreAPI_t *, const topazRenderer_Buffer_t *))api_nothing;
    attr.rendererAPI.core.renderer_set_3d_projection_matrix = (void (*)(topazRenderer_CoreAPI_t *, const topazRenderer_Buffer_t *))api_nothing;
    attr.rendererAPI.core.renderer_clear_layer = (void (*)(topazRenderer_CoreAPI_t*, topazRenderer_DataLayer))api_nothing;
    attr.rendererAPI.core.renderer_get_parameters = (topazRenderer_Parameters_t (*)(topazRenderer_CoreAPI_t*)) api_nothing;
    attr.rendererAPI.core.renderer_sync = (void (*)(topazRenderer_CoreAPI_t*)) api_nothing;
    attr.rendererAPI.core.renderer_attach_target = (void (*)(topazRenderer_CoreAPI_t*, topazRenderer_Framebuffer_t *)) api_nothing;
    attr.rendererAPI.core.renderer_get_supported_framebuffers = (const topazArray_t *  (*)(topazRenderer_CoreAPI_t*)) api_nothing;
    attr.rendererAPI.buffer.renderer_buffer_create = (void (*)(topazRendererAPI_t *, topazRenderer_BufferAPI_t *, float * data, int numElements)) api_nothing;
    attr.rendererAPI.buffer.renderer_buffer_destroy = (void (*)(topazRenderer_BufferAPI_t *)) api_nothing;
    attr.rendererAPI.buffer.renderer_buffer_update = (void (*)(topazRenderer_BufferAPI_t *, const float * newData, int offset, int numElements)) api_nothing;
    attr.rendererAPI.buffer.renderer_buffer_read = (void (*)(topazRenderer_BufferAPI_t *, float * ouputData, int offset, int numELements)) api_nothing;
    attr.rendererAPI.program.renderer_program_create = (topazRenderer_Program_t * (*)(topazRendererAPI_t *,
                                                                        topazRenderer_ProgramAPI_t *,
                                                                        const topazString_t *, 
                                                                        const topazString_t *, 
                                                                        topazString_t *)) api_nothing;
    attr.rendererAPI.program.renderer_program_get_preset = (topazRenderer_Program_t * (*)(topazRendererAPI_t *,
                                                                        topazRenderer_ProgramAPI_t *,
                                                                        topazRenderer_PresetProgram)) api_nothing;
    attr.rendererAPI.program.renderer_program_destroy = (void (*)(topazRenderer_ProgramAPI_t *)) api_nothing;
    attr.rendererAPI.texture.renderer_texture_create = (void (*)(topazRendererAPI_t *, topazRenderer_TextureAPI_t *,  int w, int h, const uint8_t * rgbaTextureData)) api_nothing;
    attr.rendererAPI.texture.renderer_texture_destroy = (void (*)(topazRenderer_TextureAPI_t *)) api_nothing;
    attr.rendererAPI.texture.renderer_texture_update = (void (*)(topazRenderer_TextureAPI_t *, const uint8_t * newData)) api_nothing;
    attr.rendererAPI.texture.renderer_texture_get = (void (*)(topazRenderer_TextureAPI_t *, uint8_t *)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_create = (void (*)(topazRendererAPI_t *, topazRenderer_2DAPI_t *)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_destroy = (void (*)(topazRenderer_2DAPI_t *)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_add_objects = (int (*)(topazRenderer_2DAPI_t *, uint32_t * output, uint32_t count)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_remove_objects = (void (*)(topazRenderer_2DAPI_t *, uint32_t * ids, uint32_t count)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_queue_vertices = (void (*)(topazRenderer_2DAPI_t *,
        const uint32_t * objects,
        uint32_t count
    )) api_nothing;
    attr.rendererAPI.twod.renderer_2d_clear_queue = (void (*)(topazRenderer_2DAPI_t *)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_add_vertices = (int (*)(topazRenderer_2DAPI_t *, uint32_t * output, uint32_t count)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_remove_vertices = (void (*)(topazRenderer_2DAPI_t *, uint32_t * objects, uint32_t count)) api_nothing;
    attr.rendererAPI.twod.renderer_2d_set_vertices = (void (*)(
        topazRenderer_2DAPI_t *, 
        uint32_t * vertices, 
        const topazRenderer_2D_Vertex_t *, 
        uint32_t count
    )) api_nothing;
    attr.rendererAPI.twod.renderer_2d_get_vertices = (void (*)(
        topazRenderer_2DAPI_t *, 
        const uint32_t * vertexIDs, 
        topazRenderer_2D_Vertex_t * output,
        uint32_t count
    )) api_nothing;
    attr.rendererAPI.twod.renderer_2d_set_object_params = (void (*)(
        topazRenderer_2DAPI_t *, 
        uint32_t object, 
        const topazRenderer_2D_ObjectParams_t *
    )) api_nothing;
    attr.rendererAPI.light.renderer_light_create = (void (*)(topazRendererAPI_t *, topazRenderer_LightAPI_t *, topazRenderer_LightType)) api_nothing;
    attr.rendererAPI.light.renderer_light_destroy = (void (*)(topazRenderer_LightAPI_t *)) api_nothing;
    attr.rendererAPI.light.renderer_light_update_attribs = (void (*)(topazRenderer_LightAPI_t *, float *)) api_nothing;
    attr.rendererAPI.light.renderer_light_enable = (void (*)(topazRenderer_LightAPI_t *, int doIt)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_create = (void (*)(topazRendererAPI_t *, topazRenderer_FramebufferAPI_t *)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_destroy = (void (*)(topazRenderer_FramebufferAPI_t *)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_resize = (int (*)(topazRenderer_FramebufferAPI_t *, int w, int h)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_get_handle = (void * (*)(topazRenderer_FramebufferAPI_t *)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_get_raw_data = (int (*)(topazRenderer_FramebufferAPI_t *, uint8_t *)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_set_filtered_hint = (void (*)(topazRenderer_FramebufferAPI_t *, int)) api_nothing;
    attr.rendererAPI.fb.renderer_framebuffer_get_handle_type = (topazRenderer_Framebuffer_Handle (*)(topazRenderer_FramebufferAPI_t *)) api_nothing;



    return topaz_context_create(&attr);
}


int topaz_context_run(topaz_t * t) {
    if (t->quit == TRUE) {
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
    
    
    if (!t->paused) {
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
    
    
    if (!t->paused) {
        topaz_entity_draw(t->managers);
    }    
    topaz_entity_draw(t->managersNP);
    
    
    
    
    /// throttle 
    if (t->fps >= 0) {
        topaz_context_wait(t, t->fps);
        
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


void topaz_context_attach_manager(topaz_t * t, topazEntity_t * id) {
    topaz_entity_attach(t->managers, id);
}

void topaz_context_attach_manager_unpausable(topaz_t * t, topazEntity_t * id) {
    topaz_entity_attach(t->managersNP, id);

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
    return topaz_filesys_create(t->api.filesysBackend, t->api.filesysAPI);
}


topazInput_t * topaz_context_get_input(const topaz_t * t) {
    return ((topaz_t*)t)->input;
}

topazViewManager_t * topaz_context_get_view_manager(const topaz_t * t) {
    return ((topaz_t*)t)->viewManager;
}

topazResources_t * topaz_context_get_resources(const topaz_t * t) {
    return ((topaz_t*)t)->resources;
}




topazRenderer_t * topaz_context_get_backend_renderer(topaz_t * t) {
    return t->renderer;
}

topazRenderer_2D_t * topaz_context_get_backend_renderer_2d(topaz_t * t) {
    return t->renderer2d;
}