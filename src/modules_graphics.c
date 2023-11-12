#include <topaz/modules/graphics.h>
#include <topaz/backends/renderer.h>
#include <topaz/render2d.h>
#include <topaz/viewport.h>
#include <topaz/modules/view_manager.h>
#include <topaz/matrix.h>
#include <topaz/system.h>
#include <topaz/entity.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>

struct topazGraphics_t {
    topaz_t * ctx;
    topazRenderer_2D_t * renderer2d;
    topazRenderer_t * renderer;
    topazRenderer_Attributes_t lastAttr;
    topazRenderer_2D_Context_t ctx2d;
    topazMatrix_t ctxMatrix;
    topazArray_t * viewports;
};


topazGraphics_t * topaz_graphics_create(topaz_t * ctx) {
    topazGraphics_t * out = calloc(1, sizeof(topazGraphics_t));
    out->ctx = ctx;

    topazRendererAPI_t api = {};
    topazSystem_Backend_t * ref = topaz_system_create_backend(
        topaz_context_get_system(ctx), 
        TOPAZ_STR_CAST("renderer"), 
        &api
    );
    out->renderer   = topaz_renderer_create(ref, api);
    out->renderer2d = topaz_renderer_2d_create(out->renderer);    
    out->ctx2d.transform = &out->ctxMatrix;
    out->viewports = topaz_array_create(sizeof(topazEntity_t*));
    topaz_matrix_set_identity(&out->ctxMatrix);
    return out;
}


void topaz_graphics_destroy(topazGraphics_t * g) {
    topaz_renderer_destroy(g->renderer);
    free(g);
}

void topaz_graphics_push_viewport(
    topazGraphics_t * g,
    topazEntity_t * viewport
) {
    topaz_array_push(g->viewports, viewport);
    topaz_renderer_attach_target(
        g->renderer,
        topaz_viewport_get_working_framebuffer(viewport)
    );
}

/// Pops the top viewport, if any. 
void topaz_graphics_pop_viewport(
    topazGraphics_t * g
) {
    uint32_t len = topaz_array_get_size(g->viewports);
    if (len == 0) return;
    topaz_array_set_size(g->viewports, len-1);
    topazEntity_t * view = topaz_graphics_get_current_viewport(g);
    if (view) {
        topaz_renderer_attach_target(
            g->renderer,
            topaz_viewport_get_working_framebuffer(view)
        );    
    } else {
        topaz_renderer_attach_target(g->renderer, NULL);
    }
}

topazEntity_t * topaz_graphics_get_current_viewport(
    topazGraphics_t * g
) {
    uint32_t len = topaz_array_get_size(g->viewports);
    if (len == 0) return NULL;
    return topaz_array_at(g->viewports, topazEntity_t *, len-1);
}


topazRenderer_t * topaz_graphics_get_renderer(topazGraphics_t * t) {
    return t->renderer;
}

topazRenderer_2D_t * topaz_graphics_get_renderer_2d(topazGraphics_t * t) {
    return t->renderer2d;
}



static void set_display_mode_2d(
    topazGraphics_t * g,
    const topazRenderer_Attributes_t * attr 
) {
    if (attr->primitive != g->lastAttr.primitive ||
        attr->depthTest != g->lastAttr.depthTest ||
        attr->alphaRule != g->lastAttr.alphaRule ||
        attr->etchRule  != g->lastAttr.etchRule  ||
        attr->textureFilter != g->lastAttr.textureFilter) {
        topaz_renderer_draw_2d(
            g->renderer,
            g->renderer2d,
            &g->ctx2d,
            &g->lastAttr
        );
        topaz_renderer_2d_clear_queue(g->renderer2d);
        g->lastAttr = *attr;
    }

}


void topaz_graphics_request_draw_2d(topazGraphics_t * g, topazRender2D_t * object) {
    topazRenderer_Attributes_t attribs;
    topazSpatial_t * tf = topaz_render2d_get_spatial(object);

    // force update
    topaz_spatial_check_update(tf);

    
    // retrieve the actual object
    uint32_t objectID = topaz_render2d_get_render_data(object, &attribs);
    set_display_mode_2d(g, &attribs);


    topazEntity_t * viewport = topaz_graphics_get_current_viewport(g);
    



    // TODO: update on some sort of signal from the camera
    if ((int)g->ctx2d.width  != (int)topaz_viewport_get_width(viewport) ||
        (int)g->ctx2d.height != (int)topaz_viewport_get_height(viewport)) {

        g->ctx2d.width = topaz_viewport_get_width(viewport);
        g->ctx2d.height = topaz_viewport_get_height(viewport);
    }


    // recommit 
    memcpy(
        (float*)&g->ctxMatrix,
        topaz_entity_get_global_matrix(viewport),
        sizeof(float)*16
    );

    topaz_renderer_2d_queue_objects(
        g->renderer2d,
        &objectID,
        1
    );
}


void topaz_graphics_request_draw_3d(
    topazGraphics_t * g,
    topazRenderer_3D_t * d,
    const topazRenderer_Attributes_t * att
) {
    topaz_renderer_draw_3d(
        g->renderer,
        d,
        att
    );
}


void topaz_graphics_sync(topazGraphics_t * g) {
    topaz_renderer_draw_2d(
        g->renderer,
        g->renderer2d,
        &g->ctx2d,
        &g->lastAttr
    );
    topaz_renderer_sync(g->renderer);
    topaz_renderer_2d_clear_queue(g->renderer2d);
}


void topaz_graphics_reset_scene(topazGraphics_t * g) {
    topaz_renderer_2d_clear_queue(g->renderer2d);
}
