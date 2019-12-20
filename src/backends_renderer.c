#include <topaz/compat.h>
#include <topaz/backends/renderer.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




struct topazRenderer_t {
    topazRendererAPI_t api;
    topazBackend_t * backend;

    topazRenderer_BufferAPI_t   bufferAPI;  
    topazRenderer_ProgramAPI_t  programAPI;    
    topazRenderer_TextureAPI_t  textureAPI;    
    topazRenderer_2DAPI_t       twodAPI;    
    topazRenderer_LightAPI_t    lightAPI;
    topazRenderer_FramebufferAPI_t fbAPI;

    topazMatrix_t mv;
    topazMatrix_t proj;

    
    topazRenderer_Framebuffer_t * fb;
};





/////////////// Buffer 
struct topazRenderer_Buffer_t {
    topazRenderer_BufferAPI_t api; 
    int numElt; // number of elements 
};


topazRenderer_Buffer_t * topaz_renderer_buffer_create(topazRenderer_t * t, float * data, int numElements) {
    topazRenderer_Buffer_t * b = calloc(1, sizeof(topazRenderer_Buffer_t));
    b->api = t->bufferAPI;
    b->numElt = 0;
    b->api.renderer_buffer_create(&t->api, &b->api, data, numElements);   
    return b;
}

void topaz_renderer_buffer_destroy(topazRenderer_Buffer_t * t) {
    t->api.renderer_buffer_destroy(&t->api);
    free(t);
}

void topaz_renderer_buffer_update(topazRenderer_Buffer_t * t, float * newData, int offset, int numElements) {
    t->api.renderer_buffer_update(&t->api, newData, offset, numElements);
}


void topaz_renderer_buffer_read(topazRenderer_Buffer_t * t, float * outputData, int offset, int numELements) {
    t->api.renderer_buffer_read(&t->api, outputData, offset, numELements);
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
};






topazRenderer_Framebuffer_t * topaz_renderer_framebuffer_create(topazRenderer_t * t) {
    topazRenderer_Framebuffer_t * out = calloc(1, sizeof(topazRenderer_FramebufferAPI_t));
    out->api = t->fbAPI;
    out->w = -1;
    out->h = -1;
    out->filterHint = TRUE;
    out->api.renderer_framebuffer_create(&t->api, &out->api);
    return out;
}


void topaz_renderer_framebuffer_destroy(topazRenderer_Framebuffer_t * t) {
    t->api.renderer_framebuffer_destroy(&t->api);
}


int topaz_renderer_framebuffer_resize(topazRenderer_Framebuffer_t * t, int newW, int newH) {
    if (t->api.renderer_framebuffer_resize(&t->api, newW, newH)) {
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
    return t->api.renderer_framebuffer_get_handle_type(&t->api);
}

void * topaz_renderer_framebuffer_get_handle(topazRenderer_Framebuffer_t * t) {
    return t->api.renderer_framebuffer_get_handle(&t->api);
}

int topaz_renderer_framebuffer_get_raw_data(topazRenderer_Framebuffer_t * t, uint8_t * d) {
    return t->api.renderer_framebuffer_get_raw_data(&t->api, d);
}

void topaz_renderer_framebuffer_set_filtered_hint(topazRenderer_Framebuffer_t * t, int filter) {
    t->api.renderer_framebuffer_set_filtered_hint(&t->api, filter);
    t->filterHint = filter;
}

int topaz_renderer_framebuffer_get_filtered_hint(topazRenderer_Framebuffer_t * t) {
    return t->filterHint;
}














//////////////// 2d
struct topazRenderer_2D_t {
    topazRenderer_2DAPI_t api;  
    
};


topazRenderer_2D_t * topaz_renderer_2d_create(topazRenderer_t * t) {
    topazRenderer_2D_t * out = calloc(1, sizeof(topazRenderer_2D_t));
    out->api = t->twodAPI;
    out->api.renderer_2d_create(&t->api, &out->api);
    return out;
}

void topaz_renderer_2d_destroy(topazRenderer_2D_t * t) {
    t->api.renderer_2d_destroy(&t->api);
    free(t);
}

int topaz_renderer_2d_add_objects(topazRenderer_2D_t * t, uint32_t * output, uint32_t count) {
    return t->api.renderer_2d_add_objects(&t->api, output, count);
}

void ropaz_renderer_2d_remove_objects(topazRenderer_2D_t * t, uint32_t * ids, uint32_t count) {
    t->api.renderer_2d_remove_objects(&t->api, ids, count);
}

void topaz_renderer_2d_queue_vertices(
    topazRenderer_2D_t * t,
    const uint32_t * objects,
    uint32_t count
) {
    t->api.renderer_2d_queue_vertices(&t->api, objects, count);
}

void topaz_renderer_2d_clear_queue(topazRenderer_2D_t * t) {
    t->api.renderer_2d_clear_queue(&t->api);
}

int topaz_renderer_2d_add_vertices(topazRenderer_2D_t * t, uint32_t * output, uint32_t count) {
    return t->api.renderer_2d_add_vertices(&t->api, output, count);
}

void topaz_renderer_2d_remove_vertices(topazRenderer_2D_t * t, uint32_t * objects, uint32_t count) {
    t->api.renderer_2d_remove_vertices(&t->api, objects, count);
}

void topaz_renderer_2d_set_vertices(
    topazRenderer_2D_t * t, 
    uint32_t * vertices, 
    const topazRenderer_2D_Vertex_t * p, 
    uint32_t count
) {
    t->api.renderer_2d_set_vertices(&t->api, vertices, p, count);
}

void topaz_renderer_2d_get_vertices(
    topazRenderer_2D_t * t, 
    const uint32_t * vertexIDs, 
    topazRenderer_2D_Vertex_t * output,
    uint32_t count
) {
    t->api.renderer_2d_get_vertices(&t->api, vertexIDs, output, count);
}



void topaz_renderer_2d_set_object_params(
    topazRenderer_2D_t * t, 
    uint32_t object, 
    const topazRenderer_2D_ObjectParams_t * params
) {
    t->api.renderer_2d_set_object_params(&t->api, object, params);
}









/////////////// light 
struct topazRenderer_Light_t {
    topazRenderer_LightAPI_t api;      
};


topazRenderer_Light_t * topaz_renderer_light_create(topazRenderer_t * t, topazRenderer_LightType type) {
    topazRenderer_Light_t * out = calloc(1, sizeof(topazRenderer_Light_t));
    out->api = t->lightAPI;
    out->api.renderer_light_create(&t->api, &out->api, type);
    return out;
}

void topaz_renderer_light_destroy(topazRenderer_Light_t * t) {
    t->api.renderer_light_destroy(&t->api);
    free(t);
}

void topaz_renderer_light_update_attribs(topazRenderer_Light_t * t, float * f) {
    t->api.renderer_light_update_attribs(&t->api, f);
}

void topaz_renderer_light_enable(topazRenderer_Light_t * t, int doIt) {
    t->api.renderer_light_enable(&t->api, doIt);
}







////////////////// program 
struct topazRenderer_Program_t {
    topazRenderer_ProgramAPI_t api;
        
};

topazRenderer_Program_t * topaz_renderer_program_create(
    topazRenderer_t * t,
    const topazString_t *   vertexSrc, 
    const topazString_t *   fragSrc, 
    topazString_t *         log
) {
    topazRenderer_Program_t * out = calloc(1, sizeof(topazRenderer_ProgramAPI_t));
    out->api = t->programAPI;
    out->api.renderer_program_create(&t->api, &out->api, vertexSrc, fragSrc, log);
    return out;
}



topazRenderer_Program_t * topaz_renderer_program_create_preset(
    topazRenderer_t * t,
    topazRenderer_PresetProgram preset
) {
    topazRenderer_Program_t * out = calloc(1, sizeof(topazRenderer_ProgramAPI_t));
    out->api = t->programAPI;
    out->api.renderer_program_create_preset(&t->api, &out->api, preset);
    return out;
}





void topaz_renderer_program_destroy(topazRenderer_Program_t * t) {
    t->api.renderer_program_destroy(&t->api);
    free(t);
}










/////////////// Texture 
struct  topazRenderer_Texture_t {
    topazRenderer_TextureAPI_t api;
    int w;
    int h;
};

topazRenderer_Texture_t * topaz_renderer_texture_create(topazRenderer_t * t, int w, int h, const uint8_t * rgbaTextureData) {
    topazRenderer_Texture_t * out = calloc(1, sizeof(topazRenderer_Texture_t));
    out->w = w;
    out->h = h;
    out->api = t->textureAPI;
    out->api.renderer_texture_create(&t->api, &out->api, w, h, rgbaTextureData);
    return out;
}

void topaz_renderer_texture_destroy(topazRenderer_Texture_t * t) {
    t->api.renderer_texture_destroy(&t->api);
    free(t);
}

void topaz_renderer_texture_update(topazRenderer_Texture_t * t, const uint8_t * newData) {
    t->api.renderer_texture_update(&t->api, newData);
}


void topaz_renderer_texture_get(topazRenderer_Texture_t * t, uint8_t * data) {
    t->api.renderer_texture_update(&t->api, data);
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
    t->api.renderer_destroy(&t->api);
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

    topazRendererAPI_t api,

    topazRenderer_BufferAPI_t   bufferAPI,    
    topazRenderer_ProgramAPI_t  programAPI,    
    topazRenderer_TextureAPI_t  textureAPI,    
    topazRenderer_2DAPI_t       twodAPI,    
    topazRenderer_LightAPI_t    lightAPI,
    topazRenderer_FramebufferAPI_t fbAPI
) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.renderer_create);
        assert(api.renderer_destroy);
        assert(api.renderer_draw_2d);
        assert(api.renderer_draw_3d);
        assert(api.renderer_get_3d_viewing_matrix);
        assert(api.renderer_get_3d_projection_matrix);
        assert(api.renderer_clear_data);
        assert(api.renderer_clear_layer);
        assert(api.renderer_get_parameters);
        assert(api.renderer_sync);
        assert(api.renderer_attach_target);
        assert(api.renderer_get_supported_framebuffers);
    #endif
    topazRenderer_t * out = calloc(1, sizeof(topazRenderer_t));
    out->api = api;
    out->backend = b;
    out->api.renderer_create(&out->api);

    out->bufferAPI = bufferAPI;
    out->programAPI = programAPI;
    out->textureAPI = textureAPI;
    out->twodAPI = twodAPI;
    out->lightAPI = lightAPI;
    out->fbAPI = fbAPI;

    topaz_matrix_set_identity(&out->mv);
    topaz_matrix_set_identity(&out->proj);

    topaz_renderer_set_3d_projection_matrix(out, &out->proj);
    topaz_renderer_set_3d_projection_matrix(out, &out->mv);

    return out;
}






void topaz_renderer_draw_3d(
    topazRenderer_t * t,
    topazRenderer_3D_t * threed,
    const topazRenderer_ProcessAttribs_t * attribs
) {
    t->api.renderer_draw_3d(&t->api, threed, attribs);    
}



void topaz_renderer_draw_2d(
    topazRenderer_t * t,
    topazRenderer_2D_t * twod,
    const topazRenderer_2D_Context_t * ctx,
    const topazRenderer_ProcessAttribs_t * attribs

) {
    t->api.renderer_draw_2d(&t->api, &twod->api, ctx, attribs);
}

const topazMatrix_t * topaz_renderer_get_3d_viewing_matrix(topazRenderer_t * t) {
    return &t->mv;    
}
const topazMatrix_t * topaz_renderer_get_3d_projection_matrix(topazRenderer_t * t) {
    return &t->proj;
}


void topaz_renderer_set_3d_viewing_matrix(topazRenderer_t * t, const topazMatrix_t * m) {
    t->mv = *m;
    t->api.renderer_set_3d_viewing_matrix(&t->api, m);
}
void topaz_renderer_set_3d_projection_matrix(topazRenderer_t * t, const topazMatrix_t * m) {
    t->proj = *m;
    t->api.renderer_set_3d_projection_matrix(&t->api, m);
}


void topaz_renderer_clear_data(topazRenderer_t * t) {
    t->api.renderer_clear_data(&t->api);
}

void topaz_renderer_clear_layer(topazRenderer_t * t, topazRenderer_DataLayer layer) {
    t->api.renderer_clear_layer(&t->api, layer);
}

topazRenderer_Parameters_t topaz_renderer_get_parameters(topazRenderer_t * t) {
    return t->api.renderer_get_parameters(&t->api);
}

void topaz_renderer_sync(topazRenderer_t * t) {
    t->api.renderer_sync(&t->api);
} 

void topaz_renderer_attach_target(topazRenderer_t * t, topazRenderer_Framebuffer_t * f) {
    t->fb = f;
    t->api.renderer_attach_target(&t->api, f);
}


topazRenderer_Framebuffer_t * topaz_renderer_get_target(topazRenderer_t * t) {
    return t->fb;
}


const topazArray_t * topaz_renderer_get_supported_framebuffers(topazRenderer_t * t) {
    return t->api.renderer_get_supported_framebuffers(&t->api);
}






