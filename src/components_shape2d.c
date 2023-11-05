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
#include <topaz/components/shape2d.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/render2d.h>
#include <topaz/assets/image.h>
#include <topaz/topaz.h>
#include <topaz/modules/graphics.h>
#include <topaz/spatial.h>
#include <topaz/entity.h>

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
static char * MAGIC_ID__SHAPE_2D = "t0p4zsh4p32d";
#endif



typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topaz_t * ctx;
    topazAsset_t * id;
    topazRender2D_t * render2d;
    topazColor_t color;
    topazColor_t realColor;
    topazVector_t center;
    float animSpeed;
    int animFrame;
    // for texture tracking
    uint32_t notifID;
    topazAsset_t * notifImg;
    int notifFrameIndex;
    float notifW;
    float notifH;
} Shape2D;

static void shape2d__on_draw(topazComponent_t * c, Shape2D * s) {
    if (!topaz_render2d_get_vertices(s->render2d)) return;
    if (memcmp(&s->realColor, &s->color, sizeof(topazColor_t)) != 0) {
        topazArray_t * arr = topaz_array_clone(topaz_render2d_get_vertices(s->render2d));
        uint32_t i;
        uint32_t size = topaz_array_get_size(arr);
        topazRenderer_2D_Vertex_t * v = topaz_array_get_data(arr);

        for(i = 0; i < size; ++i) {
            v[i].r = s->color.r;
            v[i].g = s->color.g;
            v[i].b = s->color.b;
            v[i].a = s->color.a;
        }

        s->realColor = s->color;
        topaz_render2d_set_vertices(s->render2d, arr);
        topaz_array_destroy(arr);
    }

    // now that its finalized, send the shape2d as-is to the graphics 
    // module for it to be renderered.
    topaz_graphics_request_draw_2d(
        topaz_context_get_graphics(s->ctx),
        s->render2d
    );
    // TODO: anim mode


}


// on attach, the render2d transform instance is attached to the 
// entity directly rather than through the component. 
static void shape2d__on_attach(topazComponent_t * c, Shape2D * s) {
    topazEntity_t * e = topaz_component_get_host(c);
    #ifdef TOPAZDC_DEBUG 
        assert(topaz_entity_is_valid(e) && "Shape2D attached to a non-valid entity.");
    #endif
    topaz_spatial_set_as_parent(
        topaz_render2d_get_spatial(s->render2d),
        topaz_entity_get_spatial(e)
    );
}

// unparenting should happen when detaching to not affect 
// the operation of the render2d instance if used independently of an
// entity somehow
static void shape2d__on_detach(topazComponent_t * c, Shape2D * s) {
    topaz_spatial_set_as_parent(
        topaz_render2d_get_spatial(s->render2d),
        NULL        
    );
}


// retrieves the component and asserts accuracy in debug
static Shape2D * shape2d__retrieve(topazComponent_t * c) {
    Shape2D * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "Shape2D instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__SHAPE_2D);
    #endif
    return s;
}



topazComponent_t * topaz_shape2d_create(topaz_t * t) {
    Shape2D * data = calloc(1, sizeof(Shape2D));
    data->ctx = t;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__SHAPE_2D;
    #endif
    data->color.r = 1;
    data->color.g = 1;
    data->color.b = 1;
    data->color.a = 1;

    data->render2d = topaz_render2d_create(topaz_graphics_get_renderer_2d(topaz_context_get_graphics(t)), NULL);

    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_attach  = (topaz_component_attribute_callback) shape2d__on_attach;
    attribs.on_draw    = (topaz_component_attribute_callback) shape2d__on_draw;
    attribs.on_detach  = (topaz_component_attribute_callback) shape2d__on_detach;

    attribs.userData = data;
    topazComponent_t * out = topaz_component_create(TOPAZ_STR_CAST("Shape2D"), t, &attribs);
    return out;
}








topazColor_t topaz_shape2d_get_color(topazComponent_t * c) {
    Shape2D * s = shape2d__retrieve(c);
    return s->color;
}


void topaz_shape2d_set_color(topazComponent_t * c, topazColor_t clr) {
    Shape2D * s = shape2d__retrieve(c);
    s->color = clr;
}

topazTransform_t * topaz_shape2d_get_node(topazComponent_t * c) {
    Shape2D * s = shape2d__retrieve(c);
    return topaz_spatial_get_node(topaz_render2d_get_spatial(s->render2d));
}

float topaz_shape2d_get_anim_speed(topazComponent_t * c) {
    Shape2D * s = shape2d__retrieve(c);
    return s->animSpeed;
}

void topaz_shape2d_set_anim_speed(topazComponent_t * c, float a) {
    Shape2D * s = shape2d__retrieve(c);
    s->animSpeed = a;
}

const topazVector_t * topaz_shape2d_get_center(topazComponent_t * c) {
    Shape2D * s = shape2d__retrieve(c);
    return &s->center;
}

void topaz_shape2d_set_center(topazComponent_t * c, const topazVector_t * center) {
    Shape2D * s = shape2d__retrieve(c);
    uint32_t i;

    const topazArray_t * verts = topaz_render2d_get_vertices(s->render2d);
    if (verts) {
        topazArray_t * copyV = topaz_array_clone(topaz_render2d_get_vertices(s->render2d));
        topazRenderer_2D_Vertex_t * copyVp = topaz_array_get_data(copyV);
        uint32_t len = topaz_array_get_size(copyV);
        for(i = 0; i < len; ++i) {
            copyVp[i].x = copyVp[i].x + s->center.x - center->x;
            copyVp[i].y = copyVp[i].y + s->center.y - center->y;        
        }

        topaz_render2d_set_vertices(
            s->render2d, 
            copyV
        );

        topaz_array_destroy(copyV);
    };
    s->center = *center;
}
    
void topaz_shape2d_form_rectangle(topazComponent_t * c, float w, float h) {
    Shape2D * s = shape2d__retrieve(c);
    s->id = NULL;
    float color[4] = {
        s->color.r,
        s->color.g,
        s->color.b,
        s->color.a
    };

    topazRenderer_2D_Vertex_t v[6] = {
        { -s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 0},
        { -s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     0, 0},
        {w-s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     0, 0},

        { -s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 0},
        {w-s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     0, 0},
        {w-s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 0}
    };


    topaz_render2d_set_vertices(
        s->render2d, 
        TOPAZ_ARRAY_CAST(
            v, 
            topazRenderer_2D_Vertex_t,
            6
        )
    );

    topazRenderer_Attributes_t att = *topaz_render2d_get_attributes(s->render2d);
    att.primitive = topazRenderer_Primitive_Triangle;
    topaz_render2d_set_attributes(s->render2d, &att);

    
}


void topaz_shape2d_form_image(topazComponent_t * c, topazAsset_t * img) {
    topaz_shape2d_form_image_frame(c, img, 0);
}


void topaz_shape2d_form_image_scaled(topazComponent_t * c, topazAsset_t * img, float width, float height) {
    topaz_shape2d_form_image_frame_scaled(c, img, 0, width, height);
}

void topaz_shape2d_form_image_frame(topazComponent_t * c, topazAsset_t * img, uint32_t frame) {
    if (topaz_image_get_frame_count(img) == 0) return;
    int w = topaz_image_get_width(img);
    int h = topaz_image_get_height(img);
    topaz_shape2d_form_image_frame_scaled(c, img, frame, w, h);
}


static void topaz_shape2d_texture_event(
    topazAsset_t * img,
    int frame,
    topazImage_TextureEvent event,
    void * data
) {
    Shape2D * s = data;

    if (s->notifFrameIndex != frame) return;

    switch(event) {
      case topazImage_TextureEvent_Removed:
        topaz_render2d_set_texture(
            s->render2d, 
            NULL
        );
        break;

      case topazImage_TextureEvent_Changed: {
        topaz_render2d_set_texture(
            s->render2d, 
            topaz_image_get_frame_texture(s->notifImg, frame)
        );
        float w = s->notifW;
        float h = s->notifH;
        float color[4] = {
            s->color.r,
            s->color.g,
            s->color.b,
            s->color.a
        };


        topazRenderer_2D_Vertex_t v[6] = {
            { -s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 1 },
            { -s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     0, 0 },
            {w-s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     1, 0 },

            { -s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 1 },
            {w-s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     1, 0 },
            {w-s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     1, 1 }
        };


        topaz_render2d_set_vertices(
            s->render2d, 
            TOPAZ_ARRAY_CAST(
                v, 
                topazRenderer_2D_Vertex_t,
                6
            )
        );

        break;
      }
    }        
}   


void topaz_shape2d_form_image_frame_scaled(topazComponent_t * c, topazAsset_t * img, uint32_t frame,  float w, float h) {
    Shape2D * s = shape2d__retrieve(c);
    if (!img) img = s->id;
    if (!img) return;
    s->id = img;
    s->animFrame = frame;
    if (s->notifImg) {
        topaz_image_remove_texture_event_notify(s->notifImg, s->notifID);
    }
    s->notifID = topaz_image_add_texture_event_notify(
        img,
        topaz_shape2d_texture_event,
        s
    ); 
    s->notifImg = img;
    s->notifFrameIndex = frame;
    s->notifW = w;
    s->notifH = h;

    topaz_shape2d_texture_event(img, frame, topazImage_TextureEvent_Changed, s);


    topazRenderer_Attributes_t att = *topaz_render2d_get_attributes(s->render2d);
    att.primitive = topazRenderer_Primitive_Triangle;
    topaz_render2d_set_attributes(s->render2d, &att);

}






void topaz_shape2d_form_radial(topazComponent_t * c, float radius, uint32_t numIterations) {
    Shape2D * s = shape2d__retrieve(c);

    topazVector_t * pts = malloc(sizeof(topazVector_t)*(numIterations*3));
    topazVector_t * ptsIter = pts;
    topazVector_t curPt;
    topazVector_t empty;
    s->id = NULL;
    curPt.z = 0;
    empty.x = empty.y = empty.z = 0;

    uint32_t i;
    for(i = 0; i < numIterations; ++i) {
        curPt.x = cos(2*topaz_math_pi * (i / (float) numIterations)) * radius;
        curPt.y = sin(2*topaz_math_pi * (i / (float) numIterations)) * radius;        
        (*pts++) = curPt;

        curPt.x = cos(2*topaz_math_pi * (((i+1)%numIterations) / (float) numIterations)) * radius;
        curPt.y = sin(2*topaz_math_pi * (((i+1)%numIterations) / (float) numIterations)) * radius;

        curPt.x -= s->center.x;
        curPt.y -= s->center.y;

        (*pts++) = curPt;

        curPt = s->center;
        curPt.x *= -1.f;
        curPt.y *= -1.f;

        (*pts++) = curPt;
    }

    topaz_shape2d_form_triangles(
        c,
        TOPAZ_ARRAY_CAST(
            ptsIter,
            topazVector_t,
            numIterations*3
        )
    );
    free(ptsIter);
}


void topaz_shape2d_form_triangles(topazComponent_t * c, const topazArray_t * ptsSrc) {
    Shape2D * s = shape2d__retrieve(c);
    uint32_t i;
    uint32_t len = topaz_array_get_size(ptsSrc);
    topazRenderer_2D_Vertex_t * vertices = malloc(sizeof(topazRenderer_2D_Vertex_t)*len);

    float color[4] = {
        s->color.r,
        s->color.g,
        s->color.b,
        s->color.a
    };
    
    topazVector_t * pts = &topaz_array_at(ptsSrc, topazVector_t, 0);


    for(i = 0; i < len; ++i) {
        vertices[i].x = pts[i].x - s->center.x;
        vertices[i].y = pts[i].y - s->center.y;
        vertices[i].r = color[0];
        vertices[i].g = color[1];
        vertices[i].b = color[2];
        vertices[i].a = color[3];
        vertices[i].texX = 0;
        vertices[i].texY = 0;
        
    }

    topazRenderer_Attributes_t att = *topaz_render2d_get_attributes(s->render2d);
    att.primitive = topazRenderer_Primitive_Triangle;
    topaz_render2d_set_attributes(s->render2d, &att);

    topaz_render2d_set_vertices(
        s->render2d, 
        TOPAZ_ARRAY_CAST(
            vertices, 
            topazRenderer_2D_Vertex_t,
            len
        )
    );

    free(vertices);
}

void topaz_shape2d_form_lines(topazComponent_t * c, const topazArray_t * ptsSrc) {
    Shape2D * s = shape2d__retrieve(c);
    topaz_shape2d_form_triangles(c, ptsSrc);
    topazRenderer_Attributes_t att = *topaz_render2d_get_attributes(s->render2d);
    att.primitive = topazRenderer_Primitive_Line;
    topaz_render2d_set_attributes(s->render2d, &att);
}

void topaz_shape2d_set_attribute(topazComponent_t * c, topazRenderer_Attribute attribute, int value) {
    Shape2D * s = shape2d__retrieve(c);
    
    topazRenderer_Attributes_t att = *topaz_render2d_get_attributes(s->render2d);
    topaz_renderer_attributes_set_attribute(
        &att,
        attribute,
        value
    );
    topaz_render2d_set_attributes(s->render2d, &att);
}

int topaz_shape2d_get_attribute(topazComponent_t * c, topazRenderer_Attribute attribute) {
    Shape2D * s = shape2d__retrieve(c);
    return topaz_renderer_attributes_get_attribute(
        topaz_render2d_get_attributes(
            s->render2d
        ),
        attribute
    );
}


