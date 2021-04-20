#include <topaz/compat.h>
#include <topaz/backends/renderer.h>
#include <topaz/backends/display.h>
#include <topaz/camera.h>
#include <topaz/topaz.h>
#include <topaz/entity.h>
#include <topaz/modules/graphics.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazDisplay_t {
    topaz_t * ctx;
    topazDisplayAPI_t api;
    topazSystem_Backend_t * backend;
    topazEntity_t * camera2d;
    topazEntity_t * camera3d;
    topazEntity_t * cameraRender;
    int autoRefresh;
    void * apiData;

    int params[topazDisplay_Parameter_InputFocus];


    topazArray_t * resizeCBs;
    topazArray_t * closeCBs;
    int cbPool;
};


typedef struct {
    int id;
    topaz_display_callback func;
    void * data;
} DisplayCB;




topazDisplay_t * topaz_display_create(topaz_t * ctx, topazSystem_Backend_t * b, topazDisplayAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(api.display_create);
        assert(api.display_destroy);

        assert(api.display_request_parameter_change);
        assert(api.display_set_name);
        assert(api.display_update);
        assert(api.display_supported_framebuffers);
        assert(api.display_get_system_handle_type);
        assert(api.display_get_system_handle);
        assert(api.display_get_system_event_type);
        assert(api.display_get_last_system_event);
        assert(api.display_get_current_clipboard);
        assert(api.display_set_current_clipboard);
    #endif
    topazDisplay_t * out = calloc(1, sizeof(topazDisplay_t));
    out->api = api;
    out->backend = b;
    out->camera2d = topaz_camera_create(ctx);
    out->camera3d = topaz_camera_create(ctx);
    out->cameraRender = topaz_camera_create(ctx);
    out->ctx = ctx;
    out->autoRefresh = TRUE;
    topaz_context_attach_post_manager(ctx, out->camera2d);
    topaz_context_attach_post_manager(ctx, out->camera3d);
    topaz_context_attach_post_manager(ctx, out->cameraRender);
    
    out->resizeCBs = topaz_array_create(sizeof(DisplayCB));
    out->closeCBs = topaz_array_create(sizeof(DisplayCB));


    out->apiData = out->api.display_create(out, ctx);


    topaz_display_set_parameter(
        out, 
        topazDisplay_Parameter_ViewPolicy, 
        topazDisplay_ViewPolicy_MatchSize
    );
    topaz_display_set_parameter(
        out, 
        topazDisplay_Parameter_Width, 
        640
    );
    topaz_display_set_parameter(
        out, 
        topazDisplay_Parameter_Height, 
        480
    );


    return out;
}


void topaz_display_destroy(topazDisplay_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif
    t->api.display_destroy(t, t->apiData);
    topaz_entity_remove(t->camera2d);
    topaz_entity_remove(t->camera3d);
    topaz_entity_remove(t->cameraRender);
    topaz_array_destroy(t->resizeCBs);
    topaz_array_destroy(t->closeCBs);
    free(t);
}





topazSystem_Backend_t * topaz_display_get_backend(topazDisplay_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazDisplayAPI_t topaz_display_get_api(topazDisplay_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api;
}


topazEntity_t * topaz_display_get_2d_camera(topazDisplay_t * t) {
    return t->camera2d;
}

topazEntity_t * topaz_display_get_3d_camera(topazDisplay_t * t) {
    return t->camera3d;
}

topazEntity_t * topaz_display_get_render_camera(topazDisplay_t * t) {
    return t->cameraRender;
}




void topaz_display_set_parameter(
    topazDisplay_t * t,
    topazDisplay_Parameter p,
    int value
) {
    t->params[p] = value;
    t->api.display_request_parameter_change(
        t,
        t->apiData,
        p,
        value
    );
}


int topaz_display_get_parameter(
    const topazDisplay_t * display,
    topazDisplay_Parameter p
) {
    return display->params[p];
}



int topaz_display_is_parameter_modifiable(topazDisplay_t * t, topazDisplay_Parameter p) {  
    return t->api.display_is_parameter_modifiable(t, t->apiData, p);
}







void topaz_display_set_name(topazDisplay_t * t, const topazString_t * s) {
    t->api.display_set_name(t, t->apiData, s);
}

int topaz_display_add_parameter_callback(topazDisplay_t * t, topaz_display_parameter_callback func, void * cbData) {
    if (!func) return -1;
    DisplayCB cb;
    cb.id = t->cbPool++;
    cb.func = (topaz_display_callback)func;
    cb.data = cbData;    
    topaz_array_push(t->resizeCBs, cb);
    return cb.id;
}


int topaz_display_add_close_callback(topazDisplay_t * t, topaz_display_callback func, void * cbData) {
    if (!func) return -1;
    DisplayCB cb;
    cb.id = t->cbPool++;
    cb.func = func;
    cb.data = cbData;    
    topaz_array_push(t->closeCBs, cb);
    return cb.id;
}

void topaz_display_remove_callback(topazDisplay_t * t, int cb) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->closeCBs);

    for(i = 0; i < len; ++i) {
        if (topaz_array_at(t->closeCBs, DisplayCB, i).id == cb) {
            topaz_array_remove(t->closeCBs, i);
            return;
        }
    }
    len = topaz_array_get_size(t->resizeCBs);

    for(i = 0; i < len; ++i) {
        if (topaz_array_at(t->resizeCBs, DisplayCB, i).id == cb) {
            topaz_array_remove(t->resizeCBs, i);
            return;
        }
    }
}


void topaz_display_signal_parameter_change(
    topazDisplay_t * t, 
    topazDisplay_Parameter param, 
    int value
) {
    

    t->params[param] = value;
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->resizeCBs);

    for(i = 0; i < len; ++i) {
        DisplayCB * cb = &topaz_array_at(t->resizeCBs, DisplayCB, i);
        ((topaz_display_parameter_callback)cb->func)(t, param, cb->data);
    }    
    if (param == topazDisplay_Parameter_Width ||
        param == topazDisplay_Parameter_Height) {
        if (t->params[topazDisplay_Parameter_ViewPolicy] == topazDisplay_ViewPolicy_MatchSize) {
            topaz_camera_set_render_resolution(
                t->cameraRender, 
                t->params[topazDisplay_Parameter_Width], 
                t->params[topazDisplay_Parameter_Height]
            );
        }
    }
}


void topaz_display_signal_close(topazDisplay_t * t) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->closeCBs);

    for(i = 0; i < len; ++i) {
        DisplayCB * cb = &topaz_array_at(t->closeCBs, DisplayCB, i);
        cb->func(t, cb->data);
    }
}









void topaz_display_update(topazDisplay_t * t) {
    topazRenderer_Framebuffer_t * fb = topaz_camera_get_framebuffer(t->cameraRender);
    t->api.display_update(t, t->apiData, fb);

    topaz_camera_swap_buffers(t->cameraRender);

    if (t->autoRefresh) {
        topaz_camera_refresh(t->cameraRender);
    }


    // need to re-attach to renderer if previous fb was attach to it 
    // since now it has been swapped.
    if (topaz_renderer_get_target(topaz_graphics_get_renderer(topaz_context_get_graphics(t->ctx))) == fb) {
        topaz_renderer_attach_target(
            topaz_graphics_get_renderer(topaz_context_get_graphics(t->ctx)),
            topaz_camera_get_framebuffer(t->cameraRender)
        );
    }


}

const topazArray_t * topaz_display_supported_framebuffers(topazDisplay_t * t) {
    return t->api.display_supported_framebuffers(t, t->apiData);
}

topazDisplay_Handle topaz_display_get_system_handle_type(topazDisplay_t * t) {
    return t->api.display_get_system_handle_type(t, t->apiData);
}

void * topaz_display_get_system_handle(topazDisplay_t * t) {
    return t->api.display_get_system_handle(t, t->apiData);
}

topazDisplay_Event topaz_display_get_system_event_type(topazDisplay_t * t) {
    return t->api.display_get_system_event_type(t, t->apiData);
}

void * topaz_display_get_last_system_event(topazDisplay_t * t) {
    return t->api.display_get_last_system_event(t, t->apiData);
}


topazArray_t * topaz_display_get_current_clipboard(topazDisplay_t * t) {
    return t->api.display_get_current_clipboard(t, t->apiData);
}

void topaz_display_set_current_clipboard(topazDisplay_t * t, const topazArray_t * s) {
    t->api.display_set_current_clipboard(t, t->apiData, s);
}

void topaz_display_auto_refresh_camera(topazDisplay_t * d, int doIt) {
    d->autoRefresh = doIt;
}





