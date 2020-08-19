#include <topaz/compat.h>
#include <topaz/backends/renderer.h>
#include <topaz/containers/bin.h>
#include <topaz/containers/array.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




struct topazRenderer_t {
    topazRendererAPI_t api;
    topazBackend_t * backend;


    topazRenderer_Buffer_t * mv;
    topazRenderer_Buffer_t * proj;

    
    topazRenderer_Framebuffer_t * fb;
    topazBin_t * renderer2dList;
    topazBin_t * bufferList;
    topazBin_t * framebufferList;
    topazBin_t * lightList;
    topazBin_t * programList;
    topazBin_t * textureList;
};





/////////////// Buffer 
struct topazRenderer_Buffer_t {
    topazRenderer_BufferAPI_t * api; 
    uint32_t numElt; // number of elements
    uint32_t binID;
    void * bufferData;
    topazRenderer_t * src;
};


topazRenderer_Buffer_t * topaz_renderer_buffer_create(topazRenderer_t * t, float * data, uint32_t numElements) {
    topazRenderer_Buffer_t * b = calloc(1, sizeof(topazRenderer_Buffer_t));
    b->src = t;
    b->numElt = 0;
    b->api = &(t->api.buffer);
    b->bufferData = b->api->renderer_buffer_create(&t->api, data, numElements);   
    b->binID = topaz_bin_add(t->bufferList, b);
    return b;
}

void topaz_renderer_buffer_destroy(topazRenderer_Buffer_t * t) {
    t->api->renderer_buffer_destroy(t->bufferData);
    topaz_bin_remove(t->src->bufferList, t->binID);
    free(t);
}

void topaz_renderer_buffer_update(topazRenderer_Buffer_t * t, const float * newData, uint32_t offset, uint32_t numElements) {
    t->api->renderer_buffer_update(t->bufferData, newData, offset, numElements);
}


void topaz_renderer_buffer_read(topazRenderer_Buffer_t * t, float * outputData, uint32_t offset, uint32_t numELements) {
    t->api->renderer_buffer_read(t->bufferData, outputData, offset, numELements);
}

int topaz_renderer_buffer_get_size(topazRenderer_Buffer_t * t) {
    return t->numElt;
}




////////////////////// framebuffer 
struct topazRenderer_Framebuffer_t {
    topazRenderer_FramebufferAPI_t api;
    int w;
    int h;
    int filterHint;
    uint32_t binID;
    void * framebufferData;
    topazRenderer_t * src;
};






topazRenderer_Framebuffer_t * topaz_renderer_framebuffer_create(topazRenderer_t * t) {
    topazRenderer_Framebuffer_t * out = calloc(1, sizeof(topazRenderer_FramebufferAPI_t));
    out->src = t;
    out->api = t->api.fb;
    out->w = -1;
    out->h = -1;
    out->filterHint = TRUE;
    out->framebufferData = out->api.renderer_framebuffer_create(&t->api, &out->api);
    out->binID = topaz_bin_add(t->framebufferList, out);
    return out;
}


void topaz_renderer_framebuffer_destroy(topazRenderer_Framebuffer_t * t) {
    t->api.renderer_framebuffer_destroy(&t->api, t->framebufferData);
    topaz_bin_remove(t->src->framebufferList, t->binID);
    free(t);
}


int topaz_renderer_framebuffer_resize(topazRenderer_Framebuffer_t * t, int newW, int newH) {
    if (t->api.renderer_framebuffer_resize(&t->api, newW, newH, t->framebufferData)) {
        t->w = newW;
        t->h = newH;
        return TRUE;    
    }
    return FALSE;
}


int topaz_renderer_framebuffer_get_width(topazRenderer_Framebuffer_t * t) {
    return t->w;
}

int topaz_renderer_framebuffer_get_height(topazRenderer_Framebuffer_t * t) {
    return t->h;
}


topazRenderer_Framebuffer_Handle topaz_renderer_framebuffer_get_handle_type(topazRenderer_Framebuffer_t * t) {
    return t->api.renderer_framebuffer_get_handle_type(&t->api, t->framebufferData);
}

void * topaz_renderer_framebuffer_get_handle(topazRenderer_Framebuffer_t * t) {
    return t->api.renderer_framebuffer_get_handle(&t->api, t->framebufferData);
}

int topaz_renderer_framebuffer_get_raw_data(topazRenderer_Framebuffer_t * t, uint8_t * d) {
    return t->api.renderer_framebuffer_get_raw_data(&t->api, d, t->framebufferData);
}

void topaz_renderer_framebuffer_set_filtered_hint(topazRenderer_Framebuffer_t * t, int filter) {
    t->api.renderer_framebuffer_set_filtered_hint(&t->api, filter, t->framebufferData);
    t->filterHint = filter;
}

int topaz_renderer_framebuffer_get_filtered_hint(topazRenderer_Framebuffer_t * t) {
    return t->filterHint;
}














//////////////// 2d
struct topazRenderer_2D_t {
    topazRenderer_2DAPI_t * api;  
    topazRenderer_t * ctx;
    void * data;
    uint32_t binID;
};


topazRenderer_2D_t * topaz_renderer_2d_create(topazRenderer_t * t) {
    topazRenderer_2D_t * out = calloc(1, sizeof(topazRenderer_2D_t));
    out->api = &(t->api.twod);
    out->data = out->api->renderer_2d_create(&t->api);
    out->binID = topaz_bin_add(t->renderer2dList, out);
    return out;
}

topazRenderer_t * topaz_renderer_2d_get_context(topazRenderer_2D_t * t) {
    return t->ctx;
}

void topaz_renderer_2d_destroy(topazRenderer_2D_t * t) {
    t->api->renderer_2d_destroy(t->data);
    topaz_bin_remove(t->ctx->renderer2dList, t->binID);
    free(t);
}

int topaz_renderer_2d_add_objects(topazRenderer_2D_t * t, uint32_t * output, uint32_t count) {
    return t->api->renderer_2d_add_objects(t->data, output, count);
}

void topaz_renderer_2d_remove_objects(topazRenderer_2D_t * t, uint32_t * ids, uint32_t count) {
    t->api->renderer_2d_remove_objects(t->data, ids, count);
}

void topaz_renderer_2d_queue_objects(
    topazRenderer_2D_t * t,
    const uint32_t * objects,
    uint32_t count
) {
    t->api->renderer_2d_queue_objects(t->data, objects, count);
}

void topaz_renderer_2d_clear_queue(topazRenderer_2D_t * t) {
    t->api->renderer_2d_clear_queue(t->data);
}

void topaz_renderer_2d_set_object_vertices(
    topazRenderer_2D_t * t, 
    uint32_t object, 
    topazRenderer_Buffer_t * params
) {
    t->api->renderer_2d_set_object_vertices(t->data, object, params->bufferData);
}


void topaz_renderer_2d_set_object_params(
    topazRenderer_2D_t * t, 
    uint32_t object, 
    const topazRenderer_2D_ObjectParams_t * params
) {
    t->api->renderer_2d_set_object_params(t->data, object, params);
}









/////////////// light 
struct topazRenderer_Light_t {
    topazRenderer_LightAPI_t * api;      
    void * data;
    topazRenderer_t * src;
    uint32_t binID;
};


topazRenderer_Light_t * topaz_renderer_light_create(topazRenderer_t * t, topazRenderer_LightType type) {
    topazRenderer_Light_t * out = calloc(1, sizeof(topazRenderer_Light_t));
    out->api = &(t->api.light);
    out->data = out->api->renderer_light_create(&t->api, type);
    out->src = t;
    out->binID = topaz_bin_add(t->lightList, out);
    return out;
}

void topaz_renderer_light_destroy(topazRenderer_Light_t * t) {
    t->api->renderer_light_destroy(t->data);
    topaz_bin_remove(t->src->lightList, t->binID);
    free(t);
}

void topaz_renderer_light_update_attribs(topazRenderer_Light_t * t, float * f) {
    t->api->renderer_light_update_attribs(t->data, f);
}

void topaz_renderer_light_enable(topazRenderer_Light_t * t, int doIt) {
    t->api->renderer_light_enable(t->data, doIt);
}







////////////////// program 
struct topazRenderer_Program_t {
    topazRenderer_ProgramAPI_t * api;
    void * data;
    topazRenderer_t * src;
    uint32_t binID;
};

topazRenderer_Program_t * topaz_renderer_program_create(
    topazRenderer_t * t,
    const topazString_t *   vertexSrc, 
    const topazString_t *   fragSrc, 
    topazString_t *         log
) {
    topazRenderer_Program_t * out = calloc(1, sizeof(topazRenderer_ProgramAPI_t));
    out->api = &(t->api.program);
    out->data = out->api->renderer_program_create(&t->api, vertexSrc, fragSrc, log);
    out->binID = topaz_bin_add(t->programList, out);
    return out;
}



topazRenderer_Program_t * topaz_renderer_program_get_preset(
    topazRenderer_t * t,
    topazRenderer_PresetProgram preset
) {
    topazRenderer_Program_t * out = calloc(1, sizeof(topazRenderer_ProgramAPI_t));
    out->api = &(t->api.program);
    out->data = out->api->renderer_program_get_preset(&t->api, preset);
    return out;
}





void topaz_renderer_program_destroy(topazRenderer_Program_t * t) {
    t->api->renderer_program_destroy(t->data);
    topaz_bin_remove(t->src->programList, t->binID);
    free(t);
}










/////////////// Texture 
struct  topazRenderer_Texture_t {
    topazRenderer_TextureAPI_t * api;
    int w;
    int h;
    void * data;
    topazRenderer_t * src;
    uint32_t binID;
};

topazRenderer_Texture_t * topaz_renderer_texture_create(topazRenderer_t * t, int w, int h, const uint8_t * rgbaTextureData) {
    topazRenderer_Texture_t * out = calloc(1, sizeof(topazRenderer_Texture_t));
    out->w = w;
    out->h = h;
    out->api = &(t->api.texture);
    out->data = out->api->renderer_texture_create(&t->api, w, h, rgbaTextureData);
    out->binID = topaz_bin_add(t->textureList, out);
    return out;
}

void topaz_renderer_texture_destroy(topazRenderer_Texture_t * t) {
    t->api->renderer_texture_destroy(t->data);
    topaz_bin_remove(t->src->textureList, t->binID);
    free(t);
}

void topaz_renderer_texture_update(topazRenderer_Texture_t * t, const uint8_t * newData) {
    t->api->renderer_texture_update(t->data, newData);
}


void topaz_renderer_texture_get(topazRenderer_Texture_t * t, uint8_t * data) {
    t->api->renderer_texture_update(t->data, data);
}


int topaz_renderer_texture_get_width(topazRenderer_Texture_t * t) {
    return t->w;
}

int topaz_renderer_texture_get_height(topazRenderer_Texture_t * t) {
    return t->h;
}












/////////////// Renderer

void topaz_renderer_destroy(topazRenderer_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    topazArray_t * w;
    void ** iter;
    uint32_t len;
    uint32_t i;

    w = topaz_bin_get_all(t->renderer2dList);
    len = topaz_array_get_size(w);
    iter = (void**)topaz_array_get_data(w);
    for(i = 0; i < len; ++i) {
        topaz_renderer_2d_destroy(iter[i]);
    }
    topaz_array_destroy(w);
    topaz_bin_destroy(t->renderer2dList);


    w = topaz_bin_get_all(t->bufferList);
    len = topaz_array_get_size(w);
    iter = (void**)topaz_array_get_data(w);
    for(i = 0; i < len; ++i) {
        topaz_renderer_buffer_destroy(iter[i]);
    }
    topaz_array_destroy(w);
    topaz_bin_destroy(t->bufferList);


    w = topaz_bin_get_all(t->framebufferList);
    len = topaz_array_get_size(w);
    iter = (void**)topaz_array_get_data(w);
    for(i = 0; i < len; ++i) {
        topaz_renderer_framebuffer_destroy(iter[i]);
    }
    topaz_array_destroy(w);
    topaz_bin_destroy(t->framebufferList);



    w = topaz_bin_get_all(t->lightList);
    len = topaz_array_get_size(w);
    iter = (void**)topaz_array_get_data(w);
    for(i = 0; i < len; ++i) {
        topaz_renderer_light_destroy(iter[i]);
    }
    topaz_array_destroy(w);
    topaz_bin_destroy(t->lightList);



    w = topaz_bin_get_all(t->programList);
    len = topaz_array_get_size(w);
    iter = (void**)topaz_array_get_data(w);
    for(i = 0; i < len; ++i) {
        topaz_renderer_program_destroy(iter[i]);
    }
    topaz_bin_destroy(t->programList);


    w = topaz_bin_get_all(t->textureList);
    len = topaz_array_get_size(w);
    iter = (void**)topaz_array_get_data(w);
    for(i = 0; i < len; ++i) {
        topaz_renderer_texture_destroy(iter[i]);
    }
    topaz_array_destroy(w);
    topaz_bin_destroy(t->textureList);

    t->api.core.renderer_destroy(&t->api.core);
    free(t);
}





topazBackend_t * topaz_renderer_get_backend(topazRenderer_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazRendererAPI_t topaz_renderer_get_api(topazRenderer_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api;
}









///////////// Renderer






topazRenderer_t * topaz_renderer_create(
    topazBackend_t * b, 
    topazRendererAPI_t api
) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");

        assert(api.texture.renderer_texture_create);
        assert(api.texture.renderer_texture_destroy);
        assert(api.texture.renderer_texture_update);
        assert(api.texture.renderer_texture_get);



        assert(api.program.renderer_program_create);
        assert(api.program.renderer_program_get_preset);
        assert(api.program.renderer_program_destroy);


        assert(api.twod.renderer_2d_create);
        assert(api.twod.renderer_2d_destroy);
        assert(api.twod.renderer_2d_add_objects);
        assert(api.twod.renderer_2d_remove_objects);
        assert(api.twod.renderer_2d_queue_objects);
        assert(api.twod.renderer_2d_clear_queue);
        assert(api.twod.renderer_2d_set_object_vertices);
        assert(api.twod.renderer_2d_set_object_params);




        assert(api.light.renderer_light_create);
        assert(api.light.renderer_light_destroy);
        assert(api.light.renderer_light_update_attribs);
        assert(api.light.renderer_light_enable);




        assert(api.buffer.renderer_buffer_create);
        assert(api.buffer.renderer_buffer_destroy);
        assert(api.buffer.renderer_buffer_update);
        assert(api.buffer.renderer_buffer_read);


        assert(api.fb.renderer_framebuffer_create);
        assert(api.fb.renderer_framebuffer_destroy);
        assert(api.fb.renderer_framebuffer_resize);
        assert(api.fb.renderer_framebuffer_get_handle);
        assert(api.fb.renderer_framebuffer_get_raw_data);
        assert(api.fb.renderer_framebuffer_set_filtered_hint);
        assert(api.fb.renderer_framebuffer_get_handle_type);



        assert(api.core.renderer_create);
        assert(api.core.renderer_destroy);
        assert(api.core.renderer_draw_2d);
        assert(api.core.renderer_draw_3d);
        assert(api.core.renderer_set_3d_projection_matrix);
        assert(api.core.renderer_set_3d_viewing_matrix);
        assert(api.core.renderer_clear_layer);
        assert(api.core.renderer_get_parameters);
        assert(api.core.renderer_sync);
        assert(api.core.renderer_attach_target);
        assert(api.core.renderer_get_supported_framebuffers);
    #endif
    topazRenderer_t * out = calloc(1, sizeof(topazRenderer_t));
    out->api = api;
    out->backend = b;
    out->api.core.renderer_create(&out->api.core);
    out->renderer2dList = topaz_bin_create();
    out->bufferList = topaz_bin_create();
    out->textureList = topaz_bin_create();  
    out->programList = topaz_bin_create();
    out->lightList = topaz_bin_create();
    out->framebufferList = topaz_bin_create();
    return out;
}






void topaz_renderer_draw_3d(
    topazRenderer_t * t,
    topazRenderer_3D_t * threed,
    const topazRenderer_ProcessAttribs_t * attribs
) {
    t->api.core.renderer_draw_3d(&t->api.core, threed, attribs);    
}



void topaz_renderer_draw_2d(
    topazRenderer_t * t,
    topazRenderer_2D_t * twod,
    const topazRenderer_2D_Context_t * ctx,
    const topazRenderer_ProcessAttribs_t * attribs

) {
    t->api.core.renderer_draw_2d(&t->api.core, twod->api, ctx, attribs);
}

topazRenderer_Buffer_t * topaz_renderer_get_3d_viewing_matrix(topazRenderer_t * t) {
    return t->mv;    
}
topazRenderer_Buffer_t * topaz_renderer_get_3d_projection_matrix(topazRenderer_t * t) {
    return t->proj;
}


void topaz_renderer_set_3d_viewing_matrix(topazRenderer_t * t, topazRenderer_Buffer_t * m) {
    t->mv = m;
    t->api.core.renderer_set_3d_viewing_matrix(&t->api.core, m);
}
void topaz_renderer_set_3d_projection_matrix(topazRenderer_t * t, topazRenderer_Buffer_t * m) {
    t->proj = m;
    t->api.core.renderer_set_3d_projection_matrix(&t->api.core, m);
}


void topaz_renderer_clear_layer(topazRenderer_t * t, topazRenderer_DataLayer layer) {
    t->api.core.renderer_clear_layer(&t->api.core, layer);
}

topazRenderer_Parameters_t topaz_renderer_get_parameters(topazRenderer_t * t) {
    return t->api.core.renderer_get_parameters(&t->api.core);
}

void topaz_renderer_sync(topazRenderer_t * t) {
    t->api.core.renderer_sync(&t->api.core);
} 

void topaz_renderer_attach_target(topazRenderer_t * t, topazRenderer_Framebuffer_t * f) {
    t->fb = f;
    t->api.core.renderer_attach_target(&t->api.core, f);
}


topazRenderer_Framebuffer_t * topaz_renderer_get_target(topazRenderer_t * t) {
    return t->fb;
}


const topazArray_t * topaz_renderer_get_supported_framebuffers(topazRenderer_t * t) {
    return t->api.core.renderer_get_supported_framebuffers(&t->api.core);
}






