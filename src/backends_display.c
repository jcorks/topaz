#include <topaz/compat.h>
#include <topaz/backends/display.h>
#include <topaz/camera.h>
#include <topaz/topaz.h>
#include <topaz/entity.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazDisplay_t {
    topaz_t * ctx;
    topazDisplayAPI_t api;
    topazBackend_t * backend;
    topazEntity_t * camera2d;
    topazEntity_t * camera3d;
    topazEntity_t * cameraRender;
    int autoRefresh;
};





topazDisplay_t * topaz_display_create(topaz_t * ctx, topazBackend_t * b, topazDisplayAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.display_create);
        assert(api.display_destroy);

        assert(api.display_set_position);
        assert(api.display_fullscreen);
        assert(api.display_hide);
        assert(api.display_has_input_focus);
        assert(api.display_lock_client_position);
        assert(api.display_lock_client_position);
        assert(api.display_set_view_policy);
        assert(api.display_get_width);
        assert(api.display_get_height);
        assert(api.display_get_x);
        assert(api.display_get_y);
        assert(api.display_set_name);
        assert(api.display_add_resize_callback);
        assert(api.display_remove_resize_callback);
        assert(api.display_add_close_callback);
        assert(api.display_remove_close_callback);
        assert(api.display_is_capable);
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
    out->api.display_create(&out->api);
    out->camera2d = topaz_camera_create(ctx);
    out->camera3d = topaz_camera_create(ctx);
    out->cameraRender = topaz_camera_create(ctx);
    out->ctx = ctx;
    out->autoRefresh = TRUE;
    topaz_context_attach_manager(ctx, out->camera2d);
    topaz_context_attach_manager(ctx, out->camera3d);
    topaz_context_attach_manager(ctx, out->cameraRender);
    return out;
}


void topaz_display_destroy(topazDisplay_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif
    topaz_entity_remove(t->camera2d);
    topaz_entity_remove(t->camera3d);
    topaz_entity_remove(t->cameraRender);
    t->api.display_destroy(&t->api);
}





topazBackend_t * topaz_display_get_backend(topazDisplay_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazDisplayAPI_t topaz_display_get_api(topazDisplay_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api;
}


topazEntity_t * topaz_display_get_camera_2d(topazDisplay_t * t) {
    return t->camera2d;
}

topazEntity_t * topaz_display_get_camera_3d(topazDisplay_t * t) {
    return t->camera3d;
}






void topaz_display_resize(topazDisplay_t * t, int w, int h) {
    t->api.display_resize(&t->api, w, h);
}

void topaz_display_set_position(topazDisplay_t * t, int x, int y) {
    t->api.display_set_position(&t->api, x, y);
}

void topaz_display_fullscreen(topazDisplay_t * t, int h) {
    t->api.display_fullscreen(&t->api, h);
}

void topaz_display_hide(topazDisplay_t * t, int h) {
    t->api.display_hide(&t->api, h);
}

int topaz_display_has_input_focus(topazDisplay_t * t) {
    return t->api.display_has_input_focus(&t->api);
}

void topaz_display_lock_client_resize(topazDisplay_t * t, int h) {
    t->api.display_lock_client_resize(&t->api, h);
}

void topaz_display_lock_client_position(topazDisplay_t * t, int h) {
    t->api.display_lock_client_position(&t->api, h);
}

void topaz_display_set_view_policy(topazDisplay_t * t, topazDisplay_ViewPolicy p) {
    t->api.display_set_view_policy(&t->api, p);
}



int topaz_display_get_width(topazDisplay_t * t) {
    return t->api.display_get_width(&t->api);
}

int topaz_display_get_height(topazDisplay_t * t) {
    return t->api.display_get_height(&t->api);
}

int topaz_display_get_x(topazDisplay_t * t) {
    return t->api.display_get_x(&t->api);
}

int topaz_display_get_y(topazDisplay_t * t) {
    return t->api.display_get_y(&t->api);
}




void topaz_display_set_name(topazDisplay_t * t, const topazString_t * s) {
    t->api.display_set_name(&t->api, s);
}

void topaz_display_add_resize_callback(topazDisplay_t * t, void(*cb)(int w, int h, void *), void * cbData) {
    t->api.display_add_resize_callback(&t->api, cb, cbData);
}

void topaz_display_remove_resize_callback(topazDisplay_t * t, void(*cb)(int w, int h, void *)) {
    t->api.display_remove_resize_callback(&t->api, cb);
}

void topaz_display_add_close_callback(topazDisplay_t * t, void(*cb)(void *), void * data) {
    t->api.display_add_close_callback(&t->api, cb, data);
}

void topaz_display_remove_close_callback(topazDisplay_t * t, void(*cb)(void *)) {
    t->api.display_remove_close_callback(&t->api, cb);
}




int topaz_display_is_capable(topazDisplay_t * t, topazDisplay_Capability c) {
    return t->api.display_is_capable(&t->api, c);
}






void topaz_display_update(topazDisplay_t * t) {
    topazRenderer_Framebuffer_t * fb = topaz_camera_get_framebuffer(t->cameraRender);
    t->api.display_update(&t->api, fb);

    topaz_camera_swap_buffers(t->cameraRender);

    if (t->autoRefresh) {
        topaz_camera_refresh(t->cameraRender);
    }


    // need to re-attach to renderer if previous fb was attach to it 
    // since now it has been swapped.
    if (topaz_renderer_get_target(topaz_context_get_backend_renderer(t->ctx)) == fb) {
        topaz_renderer_attach_target(
            topaz_context_get_backend_renderer(t->ctx),
            topaz_camera_get_framebuffer(t->cameraRender)
        );
    }


}

const topazArray_t * topaz_display_supported_framebuffers(topazDisplay_t * t) {
    return t->api.display_supported_framebuffers(&t->api);
}

topazDisplay_Handle topaz_display_get_system_handle_type(topazDisplay_t * t) {
    return t->api.display_get_system_handle_type(&t->api);
}

void * topaz_display_get_system_handle(topazDisplay_t * t) {
    return t->api.display_get_system_handle(&t->api);
}

topazDisplay_Event topaz_display_get_system_event_type(topazDisplay_t * t) {
    return t->api.display_get_system_event_type(&t->api);
}

void * topaz_display_get_last_system_event(topazDisplay_t * t) {
    return t->api.display_get_last_system_event(&t->api);
}


topazArray_t * topaz_display_get_current_clipboard(topazDisplay_t * t) {
    return t->api.display_get_current_clipboard(&t->api);
}

void topaz_display_set_current_clipboard(topazDisplay_t * t, const topazArray_t * s) {
    t->api.display_set_current_clipboard(&t->api, s);
}

void topaz_display_auto_refresh_camera(topazDisplay_t * d, int doIt) {
    d->autoRefresh = doIt;
}





