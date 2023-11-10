
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__VIEWPORT = "t0p4zc4m3r4";
#endif

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

struct topazViewport_t {
    topaz_t * ctx;
    topazRenderer_Framebuffer_t * front;
    topazRenderer_Framebuffer_t * back;
    topazRenderer_t * renderer;
    topazAsset_t * image;
    int w;
    int h;
    int auto3d;
    int autoRefresh;
    
    topazMatrix_t projection3d;
    
    #ifdef TOPAZDC_DEBUG
        char * MAGIC_ID;
    #endif    
};

static topazViewport_t * viewport__retrieve(topazEntity_t * e) {
    topazViewport_t * c = topaz_entity_get_attributes(e)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(c && "Viewport instance is missing or instance invalid.");
        assert(c->MAGIC_ID == MAGIC_ID__VIEWPORT);
    #endif
    return c;
}


static void viewport__on_pre_draw(topazEntity_t * e, void * data) {
    topazViewport_t * v = data;
    if (v->autoRefresh) {
        topaz_viewport_clear_working_framebuffer(v, topazRenderer_DataLayer_All);
    }
    topaz_graphics_push_viewport(graphics, e);
}

static void viewport__on_draw(topazEntity_t * e, void * data) {
    topazViewport_t * v = data;
    topaz_graphics_sync(graphics);
    topaz_graphics_pop_viewport(graphics, e);
    topaz_viewport_swap_buffers(v);
}

static void viewport__on_remove(topazEntity_t * e, void * data) {
    topazViewport_t * v = data;

    topaz_renderer_framebuffer_destroy(v->front);
    topaz_renderer_framebuffer_destroy(v->back);
}



topazEntity_t * topaz_viewport_create(
    topaz_t * t
) {
    topazViewport_t * v = calloc(1, sizeof(topazViewport_t));
    #ifdef TOPAZDC_DEBUG
        v->MAGIC_ID = MAGIC_ID__VIEWPORT;
    #endif
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(t));
    topazResources_t * res = topaz_context_get_resources(t);
    v->autoRefresh = TRUE;
    v->renderer = r;
    v->ctx = t;
    v->auto3d = TRUE;
    v->w = DEFAULT_WIDTH;
    v->h = DEFAULT_HEIGHT;
    
    v->image = topaz_resources_create_asset(
        NULL,
        res
    );
    
    
    v->front = topaz_renderer_framebuffer_create(r);
    v->back  = topaz_renderer_framebuffer_create(r);
    
    topaz_renderer_framebuffer_resize(v->front, v->w, v->h);
    topaz_renderer_framebuffer_resize(v->back, v->w, v->h);

    topaz_renderer_framebuffer_set_filtered_hint(v->front, TRUE);
    topaz_renderer_framebuffer_set_filtered_hint(v->back, TRUE);



    topaz_image_add_frame(v->image);
    topaz_image_resize(v->image, v->w, v->h);
    topaz_image_set_frame_from_texture_proxy(
        v->image, 
        0, 
        topaz_renderer_framebuffer_get_texture(
            v->back
        )
    );


    
    v->projection3d = topaz_matrix_projection_perspective(
        60,
        v->w / (float) v->h,
        0.01,
        100
    );
    
    topazEntity_Attributes_t attr = {};

    attr.userData = v;
    attr.on_pre_draw = viewport__on_pre_draw;
    attr.on_draw = viewport__on_draw;
    attr.on_remove = viewport__on_remove;

    return topaz_entity_create(t, &attr);
}


topazAsset_t * topaz_viewport_get_image(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    return v->image;    
}

topazRenderer_Framebuffer_t * topaz_viewport_get_working_framebuffer(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    return v->front;    
}

topazRenderer_Framebuffer_t * topaz_viewport_get_complete_framebuffer(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    return v->back;    
}

void topaz_viewport_resize(
    topazEntity_t * e
    int w,
    int h
) {
    topazViewport_t * v = viewport__retrieve(e);
            
    topaz_renderer_framebuffer_resize(c->front, v->w, v->h);
    topaz_renderer_framebuffer_resize(c->back, v->w, v->h);

    topaz_image_resize(v->image, v->w, v->h);
    
    
    topaz_image_set_frame_from_texture_proxy(
        v->image, 
        0, 
        topaz_renderer_framebuffer_get_texture(
            v->back
        )
    );
    
    if (v->auto3d)
        v->projection3d = topaz_matrix_projection_perspective(
            60,
            v->w / (float) v->h,
            0.01,
            100
        );    
    
}

int topaz_viewport_get_width(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    return v->w;
}

int topaz_viewport_get_height(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    return v->h;
}

const topazMatrix_t * topaz_viewport_get_projection_3d(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    return &v->projection3d;
}

void topaz_viewport_set_filtered(
    topazEntity_t * e,
    int enabled
) {
    topazViewport_t * v = viewport__retrieve(e);
    topaz_renderer_framebuffer_set_filtered_hint(v->front, enabled);
    topaz_renderer_framebuffer_set_filtered_hint(v->back, enabled);
}

void topaz_viewport_set_projection_3d(
    topazEntity_t * e,
    const topazMatrix_t * m
) {
    topazViewport_t * v = viewport__retrieve(e);
    if (!v->auto3d)
        v->projection3d = *m;    
}


topazVector_t topaz_viewport_world_3d_to_screen(
    topazEntity_t * e, 
    const topazVector_t * p
) {
    topazViewport_t * v = viewport__retrieve(e);

    topazSpatial_t * s = topaz_entity_get_spatial(e);

    topazVector_t out = topaz_matrix_transform(topaz_spatial_get_global_transform(s), p);
    out = topaz_matrix_transform(&c->projection3d, &out);

    out.x = (out.x)*(v->w/2.f) + v->w/2.f;
    out.y = h - ((out.y)*(v->h/2.f) + v->h/2.f);
    return out;
}


topazVector_t topaz_viewport_screen_to_world_3d(
    topazEntity_t * e, 
    const topazVector_t * p
) {
    topazViewport_t * v = viewport__retrieve(e);

    topazVector_t in;
    in.x = (p->x/v->w)*2 - 1;
    in.y = ((h-p->y)/v->h)*2 - 1;
    in.z = ((p->z+c->nearClip)/c->farClip);

    topazSpatial_t * s = topaz_entity_get_spatial(e);
    topazMatrix_t mvInv = *topaz_spatial_get_global_transform(s);
    topazMatrix_t pjInv = v->projection3d;

    topaz_matrix_invert(&mvInv);
    topaz_matrix_invert(&pjInv);

    in = topaz_matrix_transform(&pjInv, &in);
    in = topaz_matrix_transform(&mvInv, &in);
    
    return in;
}




void topaz_viewport_set_projection_3d_auto_mode(
    topazEntity_t * e,
    int enabled
) {
    topazViewport_t * v = viewport__retrieve(e);
    v->auto3d = enabled;
}

void topaz_viewport_clear(
    topazEntity_t * e
    
    /// which information channel to clear.
    topazRenderer_DataLayer layer
) {
    topazViewport_t * v = viewport__retrieve(e);
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(d->ctx));
    topazRenderer_Framebuffer_t * old = topaz_renderer_get_target(r);


    if (old != v->front) {
        topaz_renderer_sync(r);
        topaz_renderer_attach_target(r, v->front);
        topaz_renderer_clear_layer(r, layer);
        topaz_renderer_attach_target(r, old);
    } else {
        topaz_renderer_clear_layer(r, layer);
    }
}


void topaz_viewport_set_auto_clear(
    topazEntity_t * e
    /// Whether to auto-update.
    int enable
) {
    topazViewport_t * v = viewport__retrieve(e);
    v->autoRefresh = enable;
}

void topaz_viewport_swap_buffers(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    topazRenderer_Framebuffer_t * top = v->back;
    v->back = v->front;
    v->front = top;
    
    topaz_image_set_frame_from_texture_proxy(
        v->image, 
        0, 
        topaz_renderer_framebuffer_get_texture(
            v->back 
        )
    );    
}

void topaz_viewport_sync(
    topazEntity_t * e
) {
    topazViewport_t * v = viewport__retrieve(e);
    topaz_graphics_sync(topaz_context_get_graphics(v->ctx));
}



