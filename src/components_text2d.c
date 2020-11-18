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
#include <topaz/components/text2d.h>
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
static char * MAGIC_ID__TEXT_2D = "t0p4zt3xt2d";
#endif




typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topaz_t * ctx;
    topazAsset_t * id;
    topazVector_t center;
    topazString_t * text;

    float width;
    float height;
    topazArray_t * glyphs;

} Text2D;





static void generate_text_renderable_kerning(
    Text2D * t, 
    int pixelSize
) {
    topazArray_t * arr = topaz_array_create(sizeof(topazRenderer_2D_Vertex_t));
    topazRenderer_2D_Vertex_t v; 

    uint32_t slen = topaz_string_get_length(t->text);
    topazFontRenderer_Spacing_t space = {};
    for(i = 0; i < slen; ++i) {
        
    }
}



static void text2d__on_draw(topazComponent_t * c, Text2D * s) {
    // now that its finalized, send the shape2d as-is to the graphics 
    // module for it to be renderered.
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->glyphs);
    for(i = 0; i < len; ++i) {
        topazRender2D_t * t = topaz_array_at(s->glyphs, topazRender2D_t *, i);
        topaz_graphics_request_draw_2d(
            topaz_context_get_graphics(s->ctx),
            t
        );
    }
}


// on attach, the render2d transform instance is attached to the 
// entity directly rather than through the component. 
static void text2d__on_attach(topazComponent_t * c, Text2D * s) {
    topazEntity_t * e = topaz_component_get_host(c);
    #ifdef TOPAZDC_DEBUG 
        assert(topaz_entity_is_valid(e) && "Text2D attached to a non-valid entity.");
    #endif

    uint32_t i;
    uint32_t len = topaz_array_get_size(s->glyphs);
    for(i = 0; i < len; ++i) {
        topazRender2D_t * t = topaz_array_at(s->glyphs, topazRender2D_t *, i);
        topaz_spatial_set_as_parent(
            topaz_render2d_get_spatial(t),
            topaz_entity_get_spatial(e)
        );
    }
}

// unparenting should happen when detaching to not affect 
// the operation of the render2d instance if used independently of an
// entity somehow
static void text2d__on_detach(topazComponent_t * c, Text2D * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->glyphs);
    for(i = 0; i < len; ++i) {
        topazRender2D_t * t = topaz_array_at(s->glyphs, topazRender2D_t *, i);
        topaz_spatial_set_as_parent(
            topaz_render2d_get_spatial(t),
            NULL        
        );
    }
}


// retrieves the component and asserts accuracy in debug
static Text2D * text2d__retrieve(topazComponent_t * c) {
    Shape2D * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "Text2D instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__TEXT_2D);
    #endif
    return s;
}



topazComponent_t * topaz_text2d_create(topaz_t * t) {
    Text2D * data = calloc(1, sizeof(Text2D));
    data->ctx = t;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__TEXT_2D;
    #endif
    data->text = topaz_string_create();
    data->glyphs = topaz_array_create(sizeof(topazRender2D_t *));

    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_attach  = (topaz_component_attribute_callback) text2d__on_attach;
    attribs.on_draw    = (topaz_component_attribute_callback) text2d__on_draw;
    attribs.on_detach  = (topaz_component_attribute_callback) text2d__on_detach;

    attribs.userData = data;
    topazComponent_t * out = topaz_component_create_with_attributes(TOPAZ_STR_CAST("Text2D"), t, &attribs);
    return out;
}



void topaz_text2d_set_text(
    topazComponent_t * c,
    const topazString_t * str,
    int pixelSize,
) {
    Text2D * t = text2d__retrieve(c);
    topaz_string_set(t->text, str);
    generate_text_renderable_kerning(t, pixelSize);
}



/// Sets the text content and font size to use,
/// but uses the same spacing between all characters
///
void topaz_text2d_set_text_monospace(
    topazComponent_t * c,
    const topazString_t * str,
    int pixelSize,
) {
    Text2D * t = text2d__retrieve(c);
    topaz_string_set(t->text, str);
    generate_text_renderable_monospace(t, pixelSize);

}

/// Gets the text being displayed by the component.
///
const topazString_t * topaz_text2d_get_text(topazComponent_t * c) {
    Text2D * t = text2d__retrieve(c);
    return t->text;
}

/// Gets the width of the text without transformation.
///
float topaz_text2d_get_extent_width(topazComponent_t * c) {
    Text2D * t = text2d__retrieve(c);
    return t->width;
}


float topaz_text2d_get_extent_height(topazComponent_t * c) {
    Text2D * t = text2d__retrieve(c);
    return t->width;
}

float topaz_text2d_get_char_x(topazComponent_t * c, int charIndex) {
    Text2D * t = text2d__retrieve(c);
    if (charIndex < 0 || charIndex >= topaz_array_get_size(t->glyphs)) return 0;

    topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, charIndex);
    topazArray_t * vtx = topaz_render2d_get_vertices(r);    
    return topaz_array_index(vtx, topazRenderer_2D_Vertex_t, 0).x;
}

float topaz_text2d_get_char_y(topazComponent_t * c, int charIndex) {
    Text2D * t = text2d__retrieve(c);
    if (charIndex < 0 || charIndex >= topaz_array_get_size(t->glyphs)) return 0;

    topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, charIndex);
    topazArray_t * vtx = topaz_render2d_get_vertices(r);    
    return topaz_array_index(vtx, topazRenderer_2D_Vertex_t, 0).y;
}


topazTransform_t * topaz_text2d_get_node(topazComponent_t * c) {
    Text2D * t = text2d__retrieve(c);
    return topaz_spatial_get_node(topaz_render2d_get_spatial(t->render2d));
}

void topaz_text2d_set_color_section(
    topazComponent_t * c, 
    int fromIndex,
    int toIndex,
    const topazColor_t * clr
) {
    Text2D * t = text2d__retrieve(c);
    topazArray_t * vtx = topaz_array_clone(topaz_render2d_get_vertices(t->render2d));

    uint32_t i;
    uint32_t len = topaz_array_get_size(vtx);
    for(i = (uint32_t)fromIndex; i < len && i <= (uint32_t)toIndex; ++i) {
        topazRenderer_2D_Context_t * v = &topaz_array_at(vtx, topazRenderer_2D_Context_t, i);
        v->r = clr->r / 255.0;
        v->g = clr->g / 255.0;
        v->b = clr->b / 255.0;
        v->a = clr->a / 255.0;
    }

    topaz_render2d_set_vertices(t->render2d, vtx);
    topaz_array_destroy(vtx);
}

/// Sets the color for all characters.
///
void topaz_text2d_set_color(
    topazComponent_t * c,
    const topazColor_t * clr 
) {
    Text2D * t = text2d__retrieve(c);

    topaz_text2d_set_color_section(
        c,
        0,
        topaz_string_get_length(t->text)-1,
        clr
    );
}