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

#include "backend.h"
#include <topaz/containers/bin.h>
#include "srgs.h"
#include <stdlib.h>
#include <topaz/version.h>
typedef struct {
    srgs_t * ctx;

} SRGSTOPAZCore;










///// textures 
/////
typedef struct {
    srgs_t * ctx;
    uint32_t handle;
    int w;
    int h;
} SRGSTOPAZTexture;

static void * srgstopaz_texture_create(
    topazRendererAPI_t * r, 
    int w, int h, const uint8_t * rgbaTextureData
) {
    SRGSTOPAZTexture * out = malloc(sizeof(SRGSTOPAZTexture));
    out->ctx = ((SRGSTOPAZCore*)r->implementationData)->ctx;
    out->handle = srgs_texture_create(out->ctx, w, h);
    out->w = w;
    out->h = h;
    if (rgbaTextureData) {
        srgs_texture_update(
            out->ctx, 
            out->handle,

            rgbaTextureData,
            0, 0,
            0, 0,
            w, h  
        );
    } else {
        srgs_texture_blank(
            out->ctx,
            out->handle,
            0
        );
    }
    return out;
}


static void srgstopaz_texture_destroy(void * textureObjectData) {
    SRGSTOPAZTexture * t = textureObjectData;
    srgs_texture_destroy(t->ctx, t->handle);
    free(t);
}


static void srgstopaz_texture_update(void * textureObjectData, const uint8_t * newData) {
    SRGSTOPAZTexture * t = textureObjectData;
    srgs_texture_update(
        t->ctx, 
        t->handle,

        newData,
        0, 0,
        0, 0,
        t->w, t->h  
    );

}
static void srgstopaz_texture_get(void * textureObjectData, uint8_t * target) {
    SRGSTOPAZTexture * t = textureObjectData;
    uint32_t count = srgs_texture_get_width(t->ctx, t->handle) * srgs_texture_get_height(t->ctx, t->handle) * 4;
    memcpy(
        target,
        srgs_texture_get_data(t->ctx, t->handle),
        count
    );
}

/////







/// program
///



static void * srgstopaz_program_create(
    topazRendererAPI_t * a ,
    const topazString_t * b, 
    const topazString_t * c , 
    topazString_t * d) {
    return (void*)0xfffffff;
}
static void * srgstopaz_program_get_preset(
    topazRendererAPI_t * a,
    topazRenderer_PresetProgram b) {
    return (void*)0xfffffff;
}

static void srgstopaz_program_destroy(void * programData){}



///






//// buffer 
///

typedef struct {
    void * buffer;
    uint32_t size;
    void * render2dObject;
} SRGSTOPAZBuffer;

/// in the case that a buffer is atttached to a 2d object, 
/// the 2d object's vertices will need to be updated as well
static void srgstopaz_buffer_update_2d_object(SRGSTOPAZBuffer * b);



static void * srgstopaz_buffer_create(
    topazRendererAPI_t * api, 
    float * data, uint32_t numElements
) {
    SRGSTOPAZBuffer * b = malloc(sizeof(SRGSTOPAZBuffer));
    b->buffer = calloc(sizeof(float), numElements);
    b->size = numElements;
    return b;
}
static void srgstopaz_buffer_destroy(void * bufferObjectData) {
    SRGSTOPAZBuffer * b = bufferObjectData;
    free(b->buffer);
    free(b);
}


static void srgstopaz_buffer_update(
    void * bufferObjectData, 
    const float * newData, 
    uint32_t offset, 
    uint32_t numElements
) {
    SRGSTOPAZBuffer * b = bufferObjectData;
    memcpy(((float*)b->buffer) + offset, newData, numElements * sizeof(float));
    srgstopaz_buffer_update_2d_object(b);
}

static void srgstopaz_buffer_read(void * bufferObjectData, float * newData, uint32_t offset, uint32_t numElements) {
    SRGSTOPAZBuffer * b = bufferObjectData;
    memcpy(newData, ((float*)b->buffer) + offset, numElements * sizeof(float));
}


///


/// lights
/// not used


static void *  srgstopaz_light_create(topazRendererAPI_t * r, topazRenderer_LightType type) {return NULL;}
static void    srgstopaz_light_destroy(void * r){}

static void srgstopaz_light_update_attribs(void * r, float * a){}
static void srgstopaz_light_enable(void * r, int doIt){}



///





/// render 2d

typedef struct {
    srgs_id_t objectID;
    srgs_id_t matrixID;
    SRGSTOPAZBuffer * vertices; // 2-way relationship: the buffer is aware of the object as well.
    srgs_t * ctx;
    SRGSTOPAZTexture tex;
} SRGSTOPAZ2DObject;

typedef struct {
    srgs_t * ctx;
    srgs_id_t renderlistID;
    srgs_id_t projectionID; // always orthographic, but needs to use the width/height in context object
    topazBin_t * objects;
    topazArray_t * queue;
    srgs_id_t contextTransform; // given by the context object.
    // for the projection transform
    float width;
    float height;
} SRGSTOPAZ2D;




static SRGSTOPAZ2DObject * create_object(srgs_t * ctx) {
    SRGSTOPAZ2DObject * o = malloc(sizeof(SRGSTOPAZBuffer));
    o->objectID = srgs_object_create(ctx);
    o->matrixID = srgs_matrix_create(ctx);
    o->ctx = ctx;
    o->vertices = NULL;
    return o;
}

static void remove_object(SRGSTOPAZ2DObject * o) {
    srgs_matrix_destroy(o->ctx, o->matrixID);
    srgs_object_destroy(o->ctx, o->objectID);
    free(o);
}





static void * srgstopaz_2d_create (topazRendererAPI_t * r) {
    SRGSTOPAZ2D * out = calloc(1, sizeof(SRGSTOPAZ2D));
    out->objects  = topaz_bin_create();
    out->queue    = topaz_array_create(sizeof(uint32_t));
    out->ctx = ((SRGSTOPAZCore*)r->implementationData)->ctx;
    out->renderlistID = srgs_renderlist_create(out->ctx);
    return out;
}
static void srgstopaz_2d_destroy(void * d) {
    SRGSTOPAZ2D * t = d;
    topaz_array_destroy(t->queue);
    topaz_bin_destroy(t->objects);
    free(t);
}



static int srgstopaz_2d_add_objects(void * d, uint32_t * output, uint32_t count) {
    SRGSTOPAZ2D * t = d;

    uint32_t i;

    for(i = 0; i < count; ++i) {
        output[i] = topaz_bin_add(t->objects, create_object(t->ctx));
    }
    return 1;
}
static void srgstopaz_2d_remove_objects(void * d, uint32_t * ids, uint32_t count) {
    SRGSTOPAZ2D * t = d;
    uint32_t i;
    for(i = 0; i < count; ++i) {
        SRGSTOPAZ2DObject * obj = topaz_bin_fetch(t->objects, ids[i]);
        if (!obj) continue;

        remove_object(obj);
        topaz_bin_remove(t->objects, ids[i]);
    }
}




static void srgstopaz_2d_queue_objects(
    void * d,
    const uint32_t * objects,
    uint32_t count
) {
    SRGSTOPAZ2D * t = d;
    topaz_array_push_n(t->queue, objects, count);
}

void srgstopaz_2d_clear_queue(void * d) {
    SRGSTOPAZ2D * t = d;
    topaz_array_set_size(t->queue, 0);
}



static void srgstopaz_2d_set_object_vertices(
    void * d, 
    uint32_t object, 
    void * b // buffer object bound to buffer
) {
    SRGSTOPAZ2D * t = d;
    SRGSTOPAZ2DObject * obj = topaz_bin_fetch(t->objects, object);    
    SRGSTOPAZBuffer * buf = b;
    if (obj->vertices)
        obj->vertices->render2dObject = NULL;
    obj->vertices = buf;
    buf->render2dObject = obj;
    srgstopaz_buffer_update_2d_object(b);
}



static void srgstopaz_2d_set_object_transform(
    void * d, 
    uint32_t object, 
    const topazMatrix_t * params
) {
    SRGSTOPAZ2D * t = d;
    SRGSTOPAZ2DObject * obj = topaz_bin_fetch(t->objects, object);    
    srgs_matrix_set(obj->ctx, obj->matrixID, (srgs_matrix_t*)params);
}


static void srgstopaz_2d_set_object_texture(
    void * d,
    uint32_t object,
    void * tex
) {
    SRGSTOPAZ2D * t = d;
    SRGSTOPAZ2DObject * obj = topaz_bin_fetch(t->objects, object);    
    obj->tex = *(SRGSTOPAZTexture*)tex;
}


///






/// framebuffer
///

// to increase speed, the resolution is 1/3rd.
// it isnt good!
typedef struct {
    uint32_t w;
    uint32_t h;
    srgs_id_t framebuffer;
    srgs_id_t depthbuffer;
    srgs_id_t maskbuffer;
    srgs_t * ctx;
    uint8_t * outputRaw;
} SRGSTOPAZFramebuffer;



static void * srgstopaz_framebuffer_create(
    topazRendererAPI_t * r, 
    topazRenderer_FramebufferAPI_t * api
) {
    SRGSTOPAZFramebuffer * out = calloc(1, sizeof(SRGSTOPAZFramebuffer));
    out->w = 640;
    out->h = 480;
    out->ctx = ((SRGSTOPAZCore*)r->implementationData)->ctx;;
    out->framebuffer = srgs_texture_create(out->ctx, out->w/3, out->h/3);
    out->depthbuffer = srgs_texture_create(out->ctx, out->w/3, out->h/3);
    out->maskbuffer = srgs_texture_create(out->ctx, out->w/3, out->h/3);
    out->outputRaw = NULL;
    return out;
}
static void   srgstopaz_framebuffer_destroy (void * fbSrc) {
    SRGSTOPAZFramebuffer * fb = fbSrc;
    srgs_texture_destroy(fb->ctx, fb->framebuffer);
    srgs_texture_destroy(fb->ctx, fb->depthbuffer);
}


static int      srgstopaz_framebuffer_resize(void * fbSrc, int w, int h) {
    SRGSTOPAZFramebuffer * fb = fbSrc;
    fb->w = w;
    fb->h = h;
    srgs_texture_resize(fb->ctx, fb->framebuffer, fb->w/3, fb->h/3);
    srgs_texture_resize(fb->ctx, fb->depthbuffer, fb->w/3, fb->h/3);
    srgs_texture_resize(fb->ctx, fb->maskbuffer, fb->w/3, fb->h/3);
    free(fb->outputRaw);
    fb->outputRaw = malloc(fb->w*fb->h*4);
    return TRUE;
}

static void srgstopaz_fb_update(void * fbSrc) {
    SRGSTOPAZFramebuffer * fb = fbSrc;
    uint32_t x, y;
    const uint32_t * internalRaw = (uint32_t*)srgs_texture_get_data(fbSrc, fb->framebuffer);
    uint32_t * outputRaw_r0;
    uint32_t * outputRaw_r1;
    uint32_t * outputRaw_r2;

    for(y = 0; y < fb->h/3; ++y) {
        outputRaw_r0 = ((uint32_t*)fb->outputRaw) + fb->w*(3*y);
        outputRaw_r1 = ((uint32_t*)fb->outputRaw) + fb->w*(3*y+1);
        outputRaw_r2 = ((uint32_t*)fb->outputRaw) + fb->w*(3*y+2);

        for(x = 0; x < fb->w/3; ++x) {
            *outputRaw_r0 = *internalRaw; outputRaw_r0++;
            *outputRaw_r1 = *internalRaw; outputRaw_r1++;
            *outputRaw_r2 = *internalRaw; outputRaw_r2++;

            *outputRaw_r0 = *internalRaw; outputRaw_r0++;
            *outputRaw_r1 = *internalRaw; outputRaw_r1++;
            *outputRaw_r2 = *internalRaw; outputRaw_r2++;

            *outputRaw_r0 = *internalRaw; outputRaw_r0++;
            *outputRaw_r1 = *internalRaw; outputRaw_r1++;
            *outputRaw_r2 = *internalRaw; outputRaw_r2++;

            internalRaw++;
        }

    }
}

static void *   srgstopaz_framebuffer_get_handle(void * fbSrc) {
    SRGSTOPAZFramebuffer * fb = fbSrc;
    return fb->outputRaw;
}
static int      srgstopaz_framebuffer_get_raw_data(void * fbSrc, uint8_t * output) {
    SRGSTOPAZFramebuffer * fb = fbSrc;
    memcpy(output, fb->outputRaw, fb->w*fb->w*4);
    return TRUE;
}
static void     srgstopaz_framebuffer_set_filtered_hint(void * fbSrc, int hint) {
    return;
}
static topazRenderer_Framebuffer_Handle srgstopaz_framebuffer_get_handle_type(void * fbSrc) {
    return topazRenderer_Framebuffer_Handle_RGBA_PixelArray;
}


////










/// core 
///



static void srgstopaz_create (topazRendererAPI_t * api, topazRenderer_CoreAPI_t * api2) {
    SRGSTOPAZCore * core = calloc(1, sizeof(SRGSTOPAZCore*));
    core->ctx = srgs_create(NULL, NULL, NULL);
    api->implementationData = calloc(1, sizeof(SRGSTOPAZCore));
    
}
static void srgstopaz_destroy (topazRendererAPI_t * api) {
    SRGSTOPAZCore * core = api->implementationData;
    srgs_destroy(api->implementationData);
    free(core);
}




void srgstopaz_draw_2d(
    topazRendererAPI_t * api, 
    void * d, 
    const topazRenderer_2D_Context_t * ctx, 
    const topazRenderer_ProcessAttribs_t * attribs) {


    SRGSTOPAZCore * core = api->implementationData;
    SRGSTOPAZ2D * twod = d;

    // assemble queued objects into a renderlist
    srgs_renderlist_set_objects(
        twod->ctx,
        twod->renderlistID,
        topaz_array_get_size(twod->queue),
        topaz_array_get_data(twod->queue)
    );

    // reset queue
    topaz_array_set_size(twod->queue, 0);

    // projection needs to come from the context object
    if (twod->width  != ctx->width ||
        twod->height != ctx->height) {

        twod->width = ctx->width;
        twod->height = ctx->height;
        srgs_matrix_t m;
        srgs_utility_matrix_set_identity(&m);
        srgs_utility_matrix_projection_orthographic(
            &m,
            0, twod->width,
            twod->height, 0,
            -256,
            256
        );

        srgs_matrix_set(twod->ctx, twod->projectionID, &m);        
    }

    // need to update the context transform
    srgs_matrix_t m;
    srgs_utility_matrix_set_identity(&m);
    if (ctx->transform) {
        m = *(srgs_matrix_t*)ctx->transform;
    }
    srgs_matrix_set(twod->ctx, twod->contextTransform, &m);


    // set the proper mask for all objects
    uint32_t i;
    uint32_t len = topaz_array_get_size(twod->queue);
    uint32_t * iter = topaz_array_get_data(twod->queue);
    switch(ctx->etchRule) {
      case topazRenderer_EtchRule_NoEtching: break; // fastpath: just turn off etching
      case topazRenderer_EtchRule_Define:
        for(i = 0; i < len; ++i) {
            SRGSTOPAZ2DObject * obj = topaz_bin_fetch(twod->objects, iter[i]);
            srgs_object_set_mask_mode(obj->ctx, obj->objectID, srgs__object_mask_mode__write_on);
        }
        break;





    }



}


void srgstopaz_draw_3d(topazRendererAPI_t *, topazRenderer_3D_t *, const topazRenderer_ProcessAttribs_t *);
void srgstopaz_set_3d_viewing_matrix(topazRendererAPI_t *, const topazRenderer_Buffer_t *);
void srgstopaz_set_3d_projection_matrix(topazRendererAPI_t *, const topazRenderer_Buffer_t *);


void srgstopaz_clear_layer(topazRendererAPI_t *, topazRenderer_DataLayer);

topazRenderer_Parameters_t srgstopaz_get_parameters(topazRendererAPI_t *);


void srgstopaz_sync(topazRendererAPI_t *);
void srgstopaz_attach_target(topazRendererAPI_t *, topazRenderer_Framebuffer_t *);
const topazArray_t *  srgstopaz_get_supported_framebuffers(topazRendererAPI_t *);




///






topazBackend_t * topaz_system_renderer_srgs__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("SRGS Software Renderer"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Rudimentary fallback api for graphics. Not recommended at all."),




        // On init
        NULL,

        // On init late
        NULL,

        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );
}





void topaz_system_renderer_srgs__api(topazRendererAPI_t * api) {
    
    api->texture.renderer_texture_create = srgstopaz_texture_create;
    api->texture.renderer_texture_destroy = srgstopaz_texture_destroy;
    api->texture.renderer_texture_update = srgstopaz_texture_update;
    api->texture.renderer_texture_get = srgstopaz_texture_get;

    api->program.renderer_program_create = srgstopaz_program_create;
    api->program.renderer_program_get_preset = srgstopaz_program_get_preset;
    api->program.renderer_program_destroy = srgstopaz_program_destroy;

    api->buffer.renderer_buffer_create = srgstopaz_buffer_create;
    api->buffer.renderer_buffer_destroy = srgstopaz_buffer_destroy;
    api->buffer.renderer_buffer_update = srgstopaz_buffer_update;
    api->buffer.renderer_buffer_read = srgstopaz_buffer_read;

    api->light.renderer_light_create = srgstopaz_light_create;
    api->light.renderer_light_destroy = srgstopaz_light_destroy;
    api->light.renderer_light_update_attribs = srgstopaz_light_update_attribs;
    api->light.renderer_light_enable = srgstopaz_light_enable;
    
    api->twod.renderer_2d_create = srgstopaz_2d_create;
    api->twod.renderer_2d_destroy =srgstopaz_2d_destroy;
    api->twod.renderer_2d_add_objects = srgstopaz_2d_add_objects;
    api->twod.renderer_2d_remove_objects = srgstopaz_2d_remove_objects;
    api->twod.renderer_2d_queue_objects = srgstopaz_2d_queue_objects;
    api->twod.renderer_2d_clear_queue = srgstopaz_2d_clear_queue;
    api->twod.renderer_2d_set_object_vertices = srgstopaz_2d_set_object_vertices;
    api->twod.renderer_2d_set_object_transform = srgstopaz_2d_set_object_transform;
    api->twod.renderer_2d_set_object_texture = srgstopaz_2d_set_object_texture;


    api->fb.renderer_framebuffer_create = srgstopaz_framebuffer_create;
    api->fb.renderer_framebuffer_destroy = srgstopaz_framebuffer_destroy;
    api->fb.renderer_framebuffer_resize = srgstopaz_framebuffer_resize;
    api->fb.renderer_framebuffer_get_handle = srgstopaz_framebuffer_get_handle;
    api->fb.renderer_framebuffer_get_raw_data = srgstopaz_framebuffer_get_raw_data;
    api->fb.renderer_framebuffer_set_filtered_hint = srgstopaz_framebuffer_set_filtered_hint;
    api->fb.renderer_framebuffer_get_handle_type = srgstopaz_framebuffer_get_handle_type;



}






// update function to re-cut topazRenderer_2D_Vertex_t into srgs vertex stuff

static void srgstopaz_buffer_update_2d_object(SRGSTOPAZBuffer * b) {
    SRGSTOPAZ2DObject * obj = b->render2dObject;
    uint32_t numVertices = sizeof(topazRenderer_2D_Vertex_t) / sizeof(float);
    uint32_t i;
    topazRenderer_2D_Vertex_t * src = b->buffer;
    srgs_object_set_vertex_count(obj->ctx, obj->objectID, b->size / numVertices);

    topazVector_t pos;
    pos.z = 0;
    for(i = 0; i < numVertices; ++i, src++) {
        pos.x = src->x;
        pos.y = src->y;
        srgs_object_update_vertices(
            obj->ctx,
            obj->objectID,
            srgs__object_vertex_channel__position,
            i, i,
            (float*)&pos
        );

        srgs_object_update_vertices(
            obj->ctx,
            obj->objectID,
            srgs__object_vertex_channel__uvs,
            i, i,
            (float*)&(src->texX)
        );

        srgs_object_update_vertices(
            obj->ctx,
            obj->objectID,
            srgs__object_vertex_channel__color,
            i, i,
            (float*)&(src->r)
        );
    }
}

