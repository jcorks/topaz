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

typdef struct {
    srgs_t * ctx;

} SRGSTOPAZCore;










///// textures 
/////
typedef struct {
    srgs_t * ctx;
    uint32_t handle;
} SRGSTOPAZTexture;

static void * srgstopaz_texture_create(
    topazRendererAPI_t * r, 
    topazRenderer_TextureAPI_t * tex,  
    int w, int h, const uint8_t * rgbaTextureData
) {
    SRGSTOPAZTexture * out = malloc(sizeof(SRGSTOPAZTexture));
    out->ctx = r->implimentationData;
    out->handle = srgs_texture_create(out->ctx);

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
    srgs_texture_destroy(t->ctx, out->handle);
}


static void srgstopaz_texture_update(void * textureObjectData, const uint8_t * newData) {
    SRGSTOPAZTexture * t = textureObjectData;
    srgs_texture_update(
        out->ctx, 
        out->handle,

        newData,
        0, 0,
        0, 0,
        w, h  
    );

}
static void srgstopaz_texture_get(void * textureObjectData, uint8_t * target) {
    SRGSTOPAZTexture * t = textureObjectData;
    uint32_t count = srgs_texture_get_width(t->ctx, t->handle) * srgs_texture_get_height(t->ctx, t->handle) * 4;
    memcpy(
        target,
        srgs_get_data(t->ctx, t->handle),
        count
    );
}

/////







/// program
///



static void * srgstopaz_program_create(
    topazRendererAPI_t *,
    const topazString_t *, 
    const topazString_t *, 
    topazString_t *) {
    return (void*)0xfffffff;
}
static void * srgstopaz_program_get_preset(
    topazRendererAPI_t *,
    topazRenderer_PresetProgram) {
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
    topazRendererAPI_t *, 
    topazRenderer_BufferAPI_t *, 
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
    memcpy(b->buffer + offset, newData, numElements * sizeof(float));
}

static void srgstopaz_buffer_read(void * bufferObjectData, float * ouputData, uint32_t offset, uint32_t numElements) {
    SRGSTOPAZBuffer * b = bufferObjectData;
    memcpy(newData, b->buffer + offset, numElements * sizeof(float));
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
    uint32_t objectID;
    uint32_t matrixID;
    SRGSTOPAZBuffer * vertices; // 2-way relationship: the buffer is away of the object as well.
    srgs_t * ctx;
} SRGSTOPAZ2DObject;

typedef struct {
    srgs_t * ctx;
    uint32_t renderlistID;
    topazBin_t * objects;
    topazArray_t * queue;
} SRGSTOPAZ2D;




static SRGSTOPAZ2DObject * create_object(srgs_t * ctx) {
    SRGSTOPAZ2DObject * o = malloc(sizeof(SRGSTOPAZBuffer));
    o->objectID = srgs_object_create(t->ctx);
    o->matrixID = srgs_matrix_create(t->ctx);
    o.ctx = ctx;
    o.vertices = NULL;
    return o;
}

static remove_object(SRGSTOPAZ2DObject * o) {
    srgs_matrix_destroy(o->matrixID);
    srgs_object_destroy(o->objectID);
    free(o);
}





static void * srgstopaz_2d_create (topazRendererAPI_t * r) {
    SRGSTOPAZ2D * out = malloc(sizeof(SRGSTOPAZ2D));
    out->objects  = topaz_bin_create();
    out->queue    = topaz_array_create(sizeof(uint32_t));
    out->ctx = r->implimentationData;
    out->renderlistID = srgs_renderlist_create(out->ctx);
    return out;
}
static void srgstopaz_2d_destroy(void * d) {
    SRGSTOPAZ2D * t = d;
    topaz_array_destroy(out->vertices);
    topaz_array_destroy(out->objects);
    free(t);
}



static int srgstopaz_2d_add_objects(void * d, uint32_t * output, uint32_t count) {
    SRGSTOPAZ2D * t = d;
    uint32_t i;
    topaz_array_set_size(t->objects, topaz_array_get_size(t->objects)+count);
    for(i = 0; i < count; ++i) {
        topaz_array_at(t->objects, SRGSTOPAZ2DObject, i) = create_object(t->ctx);
    }
    return TRUE;
}
static void srgstopaz_2d_remove_objects(void * d, uint32_t * ids, uint32_t count) {
    SRGSTOPAZ2D * t = d;
    uint32_t i;
    for(i = 0; i < count; ++i) {
        remove_object(&topaz_array_at(t->objects, SRGSTOPAZ2DObject, i));
        topaz_array_remove(t->objects, ids[count]);
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



void srgstopaz_2d_set_object_vertices(
    void * d, 
    uint32_t object, 
    void * b // buffer object bound to buffer
) {
    SRGSTOPAZ2D * t = d;
    SRGSTOPAZBuffer * buffer = b;
    SRGSTOPAZ2DObject * obj = topaz_bin_fetch(t->objects, object);
    
    if (b == )
}



void (srgstopaz_2d_set_object_params)(
    void *, 
    uint32_t object, 
    const topazRenderer_2D_ObjectParams_t *
);


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

void topaz_system_renderer_srgs__backend(topazRendererAPI_t * api) {
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

    api->buffer.renderer_light_create = srgstopaz_light_create;
    api->buffer.renderer_light_destroy = srgstopaz_light_destroy;
    api->buffer.renderer_light_update_attribs = srgstopaz_light_update_attribs;
    api->buffer.renderer_light_enable = srgstopaz_light_enable;

}






// update function to re-cut topazRenderer_2D_Vertex_t into srgs vertex stuff
static void srgstopaz_buffer_update_2d_object(SRGSTOPAZBuffer * b) {
    SRGSTOPAZ2DObject * obj = b->render2dObject;
    uint32_t numVertices = (sizeof(topazRenderer_2D_Vertex_t) / sizeof(float);
    uint32_t i;
    topazRenderer_2D_Vertex_t * src = b->buffer;
    srgs_object_set_vertex_count(obj->ctx, obj->objectID, b->size / numVertices));

    topazVector_t pos;
    pos.z = 0;
    for(i = 0; i < numVertices; ++i, src++) {
        pos.x = src->x;
        pos.y = src->y;
        srgs_object_update_vertices(
            obj->ctx,
            obj->objectID,
            srgs_object_vertex_channel__position
            i, i,
            &pos
        );

        srgs_object_update_vertices(
            obj->ctx,
            obj->objectID,
            srgs_object_vertex_channel__uvs
            i, i,
            &(src->texX)
        );

        srgs_object_update_vertices(
            obj->ctx,
            obj->objectID,
            srgs_object_vertex_channel__color
            i, i,
            &(src->r)
        );
    }
}

