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
#include <topaz/topaz.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
    topazAsset_t * id;
    topazRender2D_t * render2d;
    topazColor_t color;
    topazColor_t realColor;
    float animSpeed;
} Shape2D;

static void shape2d__on_draw(topazComponent_t * c, Shape2D * s) {
    if (!topaz_color_cmp(s->realColor, s->color)) {
        topazArray_t * arr = topaz_array_clone(topaz_render2d_get_vertices(s->render2d));
        uint32_t i;
        uint32_t size = topaz_array_get_size(arr);
        topazRenderer_2D_Vertex_t * v = topaz_array_get_data(arr);

        for(i = 0; i < size; ++i) {
            v[i].r = topaz_color_r_amt(s->color);
            v[i].g = topaz_color_g_amt(s->color);
            v[i].b = topaz_color_b_amt(s->color);
            v[i].a = topaz_color_a_amt(s->color);
        }

        s->realColor = s->color;
        topaz_render2d_set_vertices(s->render2d, arr);
    }
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



topazComponent_t * topaz_state_shape2d(topaz_t * t) {
    Shape2D * data = calloc(1, sizeof(Shape2D));

    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__SHAPE_2D;
    #endif

    data->render2d = topaz_render2d_create(topaz_context_get_backend_renderer_2d(t));

    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    //attribs.on_step    = (topaz_component_attribute_callback) shape2d__on_step;
    attribs.on_draw    = (topaz_component_attribute_callback) shape2d__on_draw;
    //attribs.on_destroy = (topaz_component_attribute_callback) shape2d__on_destroy;

    attribs.userData = data;
    return topaz_component_create_with_attributes(TOPAZ_STR_CAST("Shape2D"), t, &attribs);
}








topazColor_t topaz_shape2d_get_color(topazComponent_t * c) {
    Shape2D * s = shape2d__retrieve(c);
    return s->color;
}


void topaz_shape2d_set_color(topazComponent_t * c, topazColor_t clr) {
    Shape2D * s = shape2d__retrieve(c);
    s->color = clr;
}


float topaz_shape2d_get_anim_speed(topazComponent_t * c) {
    Shape2D * s = shape2d__retrieve(c);
    return s->animSpeed;
}

void topaz_shape2d_set_anim_speed(topazComponent_t * c, float a) {
    Shape2D * s = shape2d__retrieve(c);
    s->animSpeed = a;
}

void topaz_shape2d_form_rectangle(topazComponent_t * c, float width, float height) {
    Shape2D * s = shape2d__retrieve(c);
    s->id = NULL;
    float color[4] = {
        topaz_color_r_amt(s->color),
        topaz_color_g_amt(s->color),
        topaz_color_b_amt(s->color),
        topaz_color_a_amt(s->color)
    };

    topazRenderer_2D_Vertex_t v[6] = {
        {0, 0,  color[0], color[1], color[2], color[3],     0, 0,   0},
        {0, 0,  color[0], color[1], color[2], color[3],     0, 0,   0},
        {0, 0,  color[0], color[1], color[2], color[3],     0, 0,   0},
        {0, 0,  color[0], color[1], color[2], color[3],     0, 0,   0}
    };


    topaz_render2d_set_vertices(
        s->render2d, 
        TOPAZ_ARRAY_CAST(
            v, 
            topazRenderer_2D_Vertex_t,
            6
        )
    );

    
}


