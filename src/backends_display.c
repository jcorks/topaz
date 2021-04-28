#include <topaz/compat.h>
#include <topaz/backends/renderer.h>
#include <topaz/backends/display.h>
#include <topaz/camera.h>
#include <topaz/topaz.h>
#include <topaz/entity.h>
#include <topaz/modules/graphics.h>
#include <topaz/assets/image.h>

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

    topazRenderer_Framebuffer_t * fbs[4];
    int currentfb;
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
    topaz_camera_set_type(out->camera3d, topazCamera_Type_Perspective3D);
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(ctx));
    out->fbs[0] = topaz_renderer_framebuffer_create(r);
    out->fbs[1] = topaz_renderer_framebuffer_create(r);
    out->fbs[2] = topaz_renderer_framebuffer_create(r);
    out->fbs[3] = topaz_renderer_framebuffer_create(r);

    out->ctx = ctx;
    out->autoRefresh = TRUE;
    topaz_context_attach_post_manager(ctx, out->camera2d);
    topaz_context_attach_post_manager(ctx, out->camera3d);
    
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
    topaz_renderer_framebuffer_destroy(t->fbs[0]);
    topaz_renderer_framebuffer_destroy(t->fbs[1]);
    topaz_renderer_framebuffer_destroy(t->fbs[2]);
    topaz_renderer_framebuffer_destroy(t->fbs[3]);
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
            topazRenderer_Framebuffer_t * fb = t->fbs[t->currentfb];
            topaz_renderer_framebuffer_resize(
                fb,
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





topazRenderer_Framebuffer_t * topaz_display_get_framebuffer(
    topazDisplay_t * d,
    topazDisplay_Framebuffer i
) {
    int id = i;
    if (id < 0 || id >= 4) return d->fbs[0];
    return d->fbs[id];
}

void topaz_display_use_framebuffer(
    topazDisplay_t * d,
    topazDisplay_Framebuffer f
) {
    d->currentfb = f;
    if (d->currentfb < 0) d->currentfb = 0;
    if (d->currentfb >=4) d->currentfb = 3;
    
}

topazRenderer_Framebuffer_t * topaz_display_get_main_framebuffer(
    topazDisplay_t * d
) {
    return d->fbs[d->currentfb];
}

void topaz_display_clear_main_framebuffer(
    topazDisplay_t * d,
    topazRenderer_DataLayer layer
) {
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(d->ctx));
    topazRenderer_Framebuffer_t * old = topaz_renderer_get_target(r);

    if (old != d->fbs[d->currentfb]) {
        topaz_renderer_sync(r);
        topaz_renderer_attach_target(r, d->fbs[d->currentfb]);
        topaz_renderer_clear_layer(r, layer);
        topaz_renderer_attach_target(r, old);
    } else {
        topaz_renderer_clear_layer(r, layer);
    }
}


void topaz_display_capture_main_framebuffer(
    /// The display to query.
    topazDisplay_t * d,
    /// The image to populate.
    topazAsset_t * image
) {
    topazRenderer_Framebuffer_t * fb = d->fbs[d->currentfb];
    while (topaz_image_get_frame_count(image) > 1) {
        topaz_image_remove_frame(image, topaz_image_get_frame_count(image)-1);
    }

    int w = topaz_renderer_framebuffer_get_width(fb);
    int h = topaz_renderer_framebuffer_get_height(fb);

    if (topaz_image_get_width(image) != w ||
        topaz_image_get_height(image) != h)
        topaz_image_resize(image, w, h);

    if (topaz_image_get_frame_count(image) < 1)   
        topaz_image_add_frame(image);




    uint8_t * data = malloc(w*h*4);
    topaz_renderer_framebuffer_get_raw_data(fb, data);
    topaz_image_frame_set_data(topaz_image_get_frame(image, 0), data);
    free(data);
}







void topaz_display_update(topazDisplay_t * t) {
    topazRenderer_Framebuffer_t * fb = t->fbs[t->currentfb];
    t->api.display_update(t, t->apiData, fb);


    if (t->autoRefresh) {
        topaz_display_clear_main_framebuffer(t, topazRenderer_DataLayer_All);
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

void topaz_display_auto_clear_framebuffer(topazDisplay_t * d, int doIt) {
    d->autoRefresh = doIt;
}







