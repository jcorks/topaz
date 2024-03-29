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

#include <topaz/compat.h>
#include <topaz/components/shape3d.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/assets/image.h>
#include <topaz/topaz.h>
#include <topaz/modules/graphics.h>
#include <topaz/modules/view_manager.h>
#include <topaz/matrix.h>
#include <topaz/spatial.h>
#include <topaz/entity.h>
#include <topaz/assets/material.h>
#include <topaz/assets/mesh.h>
#include <topaz/viewport.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all components 
// using state control functions are actually what they claim to be
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__SHAPE_3D = "t0p4zsh4p33d";
#endif




typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topaz_t * ctx;
    topazSpatial_t * spatial;
    topazRenderer_t * r;
    topazRenderer_3D_t d3;
    topazAsset_t * mesh;
    topazAsset_t * material;

    topazAsset_t * slot0_old;
    topazAsset_t * slot1_old;
    topazAsset_t * slot2_old;

    int frame0_old;
    int frame1_old;
    int frame2_old;

    uint32_t slot0_id;
    uint32_t slot1_id;
    uint32_t slot2_id;

    topazRenderer_Attributes_t attribs;
} Shape3D;

static void shape3d__on_draw(topazComponent_t * c, Shape3D * s) {
    // need to recommit buffers;
    topazEntity_t * cam = topaz_graphics_get_current_viewport(topaz_context_get_graphics(s->ctx));
    topazMatrix_t modelview = *topaz_spatial_get_global_transform(s->spatial);
    
    topaz_renderer_buffer_update(
        s->d3.modelviewMatrix,
        (float*)&modelview,
        0, 16
    );    

    topaz_renderer_buffer_update(
        s->d3.projectionMatrix,
        (float*)topaz_viewport_get_projection_3d(cam),
        0, 16
    );    

    topazArray_t * objs;
    topaz_material_update_3d(s->material, &s->d3);
    topaz_mesh_query(
        s->mesh,
        &(s->d3.vertices),
        &objs
    );

    
    uint32_t i;
    int32_t len = topaz_array_get_size(objs);
    for(i = 0; i < len; ++i) {
        s->d3.indices = topaz_array_at(objs, topazArray_t *, i);

        topaz_graphics_request_draw_3d(
            topaz_context_get_graphics(s->ctx),
            &s->d3,
            &s->attribs
        );
    }
        
    
}


// on attach, the transform instance is attached to the 
// entity directly rather than through the component. 
static void shape3d__on_attach(topazComponent_t * c, Shape3D * s) {
    topazEntity_t * e = topaz_component_get_host(c);
    #ifdef TOPAZDC_DEBUG 
        assert(topaz_entity_is_valid(e) && "Shape3D attached to a non-valid entity.");
    #endif
    topaz_spatial_set_as_parent(
        s->spatial,
        topaz_entity_get_spatial(e)
    );
}

// unparenting should happen when detaching to not affect 
// the operation of the render2d instance if used independently of an
// entity somehow
static void shape3d__on_detach(topazComponent_t * c, Shape3D * s) {
    topaz_spatial_set_as_parent(
        s->spatial,
        NULL        
    );
}


// retrieves the component and asserts accuracy in debug
static Shape3D * shape3d__retrieve(topazComponent_t * c) {
    Shape3D * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "Shape3D instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__SHAPE_3D);
    #endif
    return s;
}



topazComponent_t * topaz_shape3d_create(topaz_t * t) {
    Shape3D * data = calloc(1, sizeof(Shape3D));
    data->ctx = t;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__SHAPE_3D;
    #endif

    
    data->spatial = topaz_spatial_create();
    data->ctx = t;
    data->r = topaz_graphics_get_renderer(topaz_context_get_graphics(t));
    // owned
    data->d3.modelviewMatrix = topaz_renderer_buffer_create(data->r, NULL, 16);
    data->d3.projectionMatrix = topaz_renderer_buffer_create(data->r, NULL, 16);

    data->attribs.primitive = topazRenderer_Primitive_Triangle;
    data->attribs.depthTest = topazRenderer_DepthTest_Less;
    data->attribs.etchRule  = topazRenderer_EtchRule_NoEtching;
    data->attribs.alphaRule = topazRenderer_AlphaRule_Opaque;
    data->attribs.textureFilter = topazRenderer_TextureFilterHint_Linear;
    
    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_attach  = (topaz_component_attribute_callback) shape3d__on_attach;
    attribs.on_draw    = (topaz_component_attribute_callback) shape3d__on_draw;
    attribs.on_detach  = (topaz_component_attribute_callback) shape3d__on_detach;

    attribs.userData = data;
    topazComponent_t * out = topaz_component_create(TOPAZ_STR_CAST("Shape3D"), t, &attribs);
    return out;
}






topazTransform_t * topaz_shape3d_get_node(
    topazComponent_t * shape3d
) {
    Shape3D * s = shape3d__retrieve(shape3d);
    return topaz_spatial_get_node(s->spatial);
}

static void topaz_shape3d_texture_event0(
    topazAsset_t * a,
    int frame,
    topazImage_TextureEvent event,
    void * data
) {
    Shape3D * s = data;
    if (frame != s->frame0_old) return;

    switch(event) {
      case topazImage_TextureEvent_Removed:
        s->d3.sampleTexture0 = NULL;
        break;

      case topazImage_TextureEvent_Changed:
        s->d3.sampleTexture0 = topaz_image_get_frame_texture(s->slot0_old, frame);
        break;
    }
}   

static void topaz_shape3d_texture_event1(
    topazAsset_t * a,
    int frame,
    topazImage_TextureEvent event,
    void * data
) {
    Shape3D * s = data;
    if (frame != s->frame1_old) return;

    switch(event) {
      case topazImage_TextureEvent_Removed:
        s->d3.sampleTexture1 = NULL;
        break;

      case topazImage_TextureEvent_Changed:
        s->d3.sampleTexture1 = topaz_image_get_frame_texture(s->slot1_old, frame);
        break;
    }
}   


static void topaz_shape3d_texture_event2(
    topazAsset_t * a,
    int frame,
    topazImage_TextureEvent event,
    void * data
) {
    Shape3D * s = data;
    if (frame != s->frame2_old) return;

    switch(event) {
      case topazImage_TextureEvent_Removed:
        s->d3.sampleTexture2 = NULL;
        break;

      case topazImage_TextureEvent_Changed:
        s->d3.sampleTexture2 = topaz_image_get_frame_texture(s->slot2_old, frame);
        break;
    }
}   


/// Sets the image to use for the given texture slot.
void topaz_shape3d_set_texture(
    topazComponent_t * shape3d,
    topazShape3D_TextureSlot slot,
    topazAsset_t * src
) {
    if (!topaz_image_get_frame_count(src)) return;
    Shape3D * s = shape3d__retrieve(shape3d);

    //topazImage_Frame_t * image = topaz_image_get_frame(src, 0);

    switch(slot) {
      case topazShape3D_TextureSlot_0:
        if (s->slot0_old) {
            topaz_image_remove_texture_event_notify(s->slot0_old, s->slot0_id);
        }
        s->d3.sampleTexture0 = topaz_image_get_frame_texture(src, 0);
        s->slot0_id = topaz_image_add_texture_event_notify(src, topaz_shape3d_texture_event0, s);
        s->frame0_old = 0;
        s->slot0_old = src;
        break;

      case topazShape3D_TextureSlot_1:
        if (s->slot1_old) {
            topaz_image_remove_texture_event_notify(s->slot1_old, s->slot1_id);
        }
        s->d3.sampleTexture1 = topaz_image_get_frame_texture(src, 0);
        s->slot1_id = topaz_image_add_texture_event_notify(src, topaz_shape3d_texture_event1, s);
        s->frame1_old = 0;
        s->slot1_old = src;
        break;

      case topazShape3D_TextureSlot_2:
        if (s->slot2_old) {
            topaz_image_remove_texture_event_notify(s->slot2_old, s->slot2_id);
        }
        s->d3.sampleTexture2 = topaz_image_get_frame_texture(src, 0);
        s->slot2_id = topaz_image_add_texture_event_notify(src, topaz_shape3d_texture_event2, s);
        s->frame2_old = 0;
        s->slot2_old = src;
        break;
    
      default:;
    }
}




void topaz_shape3d_set_mesh(
    topazComponent_t * shape3d,
    topazAsset_t * mesh
) {
    Shape3D * s = shape3d__retrieve(shape3d);
    s->mesh = mesh;
}


void topaz_shape3d_set_material(
    topazComponent_t * shape3d,
    topazAsset_t * material
) {
    Shape3D * s = shape3d__retrieve(shape3d);
    s->material = material;    
}


void topaz_shape3d_set_attribute(
    topazComponent_t * shape3d, 
    topazRenderer_Attribute attribute,
    int value
) {
    Shape3D * s = shape3d__retrieve(shape3d);
    
    topaz_renderer_attributes_set_attribute(
        &s->attribs,
        attribute,
        value
    );
}

int topaz_shape3d_get_attribute(
    topazComponent_t * shape3d,
    topazRenderer_Attribute attribute

) {
    Shape3D * s = shape3d__retrieve(shape3d);
    return topaz_renderer_attributes_get_attribute(
        &s->attribs,
        attribute
    );
}


