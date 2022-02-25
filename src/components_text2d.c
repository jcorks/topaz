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
#include <topaz/component.h>
#include <topaz/components/text2d.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/backends/font_renderer.h>
#include <topaz/modules/font_manager.h>
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
    topazVector_t center;
    topazString_t * text;
    topazSpatial_t * spatial;
    int currentSize;
    const topazAsset_t * font;

    float width;
    float height;
    topazArray_t * glyphs;
    topazRenderer_Attributes_t attribs;
    
    topazFontRenderer_Spacing_t spacingLast;
    float oobLeftLast;
    float oobTopLast;    

} Text2D;





static void text2d__on_draw(topazComponent_t * c, Text2D * s) {
    // now that its finalized, send the shape2d as-is to the graphics 
    // module for it to be renderered.
    uint32_t i;
    uint32_t len = topaz_string_get_length(s->text);
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

    topaz_spatial_set_as_parent(
        s->spatial,
        topaz_entity_get_spatial(e)
    );
}

// unparenting should happen when detaching to not affect 
// the operation of the render2d instance if used independently of an
// entity somehow
static void text2d__on_detach(topazComponent_t * c, Text2D * s) {
    topaz_spatial_set_as_parent(
        s->spatial,
        NULL        
    );
}


// retrieves the component and asserts accuracy in debug
static Text2D * text2d__retrieve(topazComponent_t * c) {
    Text2D * s = topaz_component_get_attributes(c)->userData;
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
    data->spatial = topaz_spatial_create();
    data->attribs.depthTest = topazRenderer_DepthTest_None;
    data->attribs.alphaRule = topazRenderer_AlphaRule_Allow;
    data->attribs.etchRule  = topazRenderer_EtchRule_NoEtching;
    data->attribs.textureFilter = topazRenderer_TextureFilterHint_Linear;
    data->font = NULL;
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

void topaz_text2d_set_attributes(
    /// The shape2d to modify
    topazComponent_t * c, 

    /// The new process attributes.
    const topazRenderer_Attributes_t * attributes
) {
    Text2D * t = text2d__retrieve(c);
    t->attribs = *attributes;

    uint32_t i;
    uint32_t len = topaz_array_get_size(t->glyphs);
    topazRender2D_t * glyph;
    for(i = 0; i < len; ++i) {
        glyph = topaz_array_at(t->glyphs, topazRender2D_t *, i);
        topaz_render2d_set_attributes(
            glyph,
            attributes
        );
    }
}


const topazRenderer_Attributes_t * topaz_text2d_get_attributes(
    topazComponent_t * c
) {
    Text2D * t = text2d__retrieve(c);
    return &t->attribs;
}

static void text2d_update__full(Text2D * t, const topazString_t * str, const topazAsset_t * font, int pixelSize) {
    uint32_t i;
    uint32_t len;

    topazFontRenderer_Spacing_t spacing = {};
    float originX = 0;
    float originY = 0;


    // get context
    topazFontRenderer_t * fontRenderer = topaz_font_manager_get_renderer(topaz_context_get_font_manager(t->ctx), t->font);

    
    
    if (fontRenderer) {   
        // first, unref all current glphs
        len = topaz_string_get_length(t->text);
        for(i = 0; i < len; ++i) {
            topaz_font_renderer_image_unref(
                fontRenderer,
                topaz_string_get_char(t->text, i),
                t->currentSize
            );
        }
    }
    
    if (font != t->font) {
        t->font = font;
        fontRenderer = topaz_font_manager_get_renderer(topaz_context_get_font_manager(t->ctx), t->font);        
    }
    

    // update source data
    t->currentSize = pixelSize;
    topaz_string_set(t->text, str);


    // base vertices
    topazRenderer_2D_Vertex_t vtx_base[6] = {
        {0, 0,    0, 0, 0, 0,   0, 1},
        {0, 0,    0, 0, 0, 0,   1, 0},
        {0, 0,    0, 0, 0, 0,   1, 1},

        {0, 0,    0, 0, 0, 0,   0, 1},
        {0, 0,    0, 0, 0, 0,   0, 0},
        {0, 0,    0, 0, 0, 0,   1, 0},
    };


    // glyphs act as a cache of renderables for the text,
    // so need to match in size
    while(topaz_array_get_size(t->glyphs) < topaz_string_get_length(t->text)) {
        topazRender2D_t * r = topaz_render2d_create(topaz_graphics_get_renderer_2d(topaz_context_get_graphics(t->ctx)), t->spatial);
        t->attribs.primitive = topazRenderer_Primitive_Triangle;
        topaz_render2d_set_attributes(r, &t->attribs);

        topaz_render2d_set_vertices(r, TOPAZ_ARRAY_CAST(vtx_base, topazRenderer_2D_Vertex_t, 6));
        topaz_array_push(t->glyphs, r);
    }
    
    if (!fontRenderer) {
        return;
    }



    len = topaz_string_get_length(t->text);
    t->width = 0;
    t->height = 0;
    float oobLeft = 0;
    float oobTop = 0;
    topazVector_t * p = malloc(sizeof(topazVector_t)*6*len);
    topazVector_t * pIter = p;
    topazVector_t * fme = p;


    // before we begin, we need to establish a baseline origin that is 
    // low enough to allow for horizontal fonts to have enough room for their first row 
    // of glyphs without any requiring of the user code to move the 
    // string to maintain an imaginary line of where the glyphs lie.
    // The intent of the yMinimumSpace gives a "worst case" for height 
    // of glyphs and allows to establish a reliable topleft.
    if (len) {
        topazFontRenderer_Spacing_t s = {};
        topaz_font_renderer_query_spacing(
            fontRenderer, 
            &s,
            t->currentSize,
            0,
            topaz_string_get_char(t->text, 0),
            0
        );
        originY = s.yMinimumSpace;
        spacing.yNextOrigin = s.yMinimumSpace;
    }
    // first we need the space layout for the character positions.
    // The reason we do this is the font renderer may request glyphs to be in negative 
    // space. This is not to spec, as the visual should have 0,0 be the origin.
    // oob (out of bounds) left and top hold the offset needed to 
    // prevent overflow into negative.
    for(i = 0; i < len; ++i) {
        // Get spacing 
        topaz_font_renderer_query_spacing(
            fontRenderer, 
            &spacing,
            t->currentSize,
            i == 0 ? 0 : topaz_string_get_char(t->text, i-1),
            topaz_string_get_char(t->text, i),
            i >= len ? 0 : topaz_string_get_char(t->text, i+1)
        );


        // top left
        p->x = originX+spacing.xOffset;
        p->y = originY+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // bottom right
        p->x = originX+spacing.width+spacing.xOffset;
        p->y = originY+spacing.height+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // top right
        p->x = originX+spacing.width+spacing.xOffset;
        p->y = originY+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;







        // top left
        p->x = originX+spacing.xOffset;
        p->y = originY+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // bottom left
        p->x = originX+spacing.xOffset;
        p->y = originY+spacing.height+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // bottom right
        p->x = originX+spacing.width+spacing.xOffset;
        p->y = originY+spacing.height+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;

        


        // update total extent width/height
        if (originX+spacing.width+spacing.xOffset > t->width)
            t->width = originX+spacing.width+spacing.xOffset;

        if (originY+spacing.height+spacing.yOffset > t->height)
            t->height = originY+spacing.height+spacing.yOffset;


        originX = spacing.xNextOrigin;
        originY = spacing.yNextOrigin; 
    }

    // width and height are also impacted by OOB measurements 
    t->width -= oobLeft;
    t->height -= oobTop;


    // now we apply the positions with oob measurements for the 
    // real glyph verticies.
    for(i = 0; i < len; ++i) {
        topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, i);


        // set the next texture    
        const topazAsset_t * image = topaz_font_renderer_image_ref(
            fontRenderer,
            topaz_string_get_char(t->text, i),
            t->currentSize
        );

        topazRenderer_Texture_t * texture = topaz_image_frame_get_texture(topaz_image_get_frame((topazAsset_t *)image, 0));
        topaz_render2d_set_texture(r, texture);


        // apply data
        topazRenderer_2D_Vertex_t * v = &vtx_base[0];


        // top left
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // bottom right
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // top right
        v->x = pIter->x - oobLeft;

        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;






        // top left
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // bottom left
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // bottom right
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        pIter++;
        


        topaz_render2d_set_vertices(r, TOPAZ_ARRAY_CAST(vtx_base, topazRenderer_2D_Vertex_t, 6));
    }
    t->spacingLast = spacing;
    t->oobLeftLast = oobLeft;
    t->oobTopLast = oobTop;
    free(fme);
}

// adds additional text to an existing text2d instance thats of the same font and size.
// quite a bit 
static void text2d_update__add(Text2D * t, const topazString_t * str, const topazAsset_t * font, int pixelSize) {
    uint32_t i;
    uint32_t len;

    topazFontRenderer_Spacing_t spacing = t->spacingLast;
    float originX = spacing.xNextOrigin;
    float originY = spacing.yNextOrigin;


    // get context
    topazFontRenderer_t * fontRenderer = topaz_font_manager_get_renderer(topaz_context_get_font_manager(t->ctx), t->font);
    
    uint32_t oldLen = topaz_string_get_length(t->text);
    // update source data
    topaz_string_set(t->text, str);


    // base vertices
    topazRenderer_2D_Vertex_t vtx_base[6] = {
        {0, 0,    0, 0, 0, 0,   0, 1},
        {0, 0,    0, 0, 0, 0,   1, 0},
        {0, 0,    0, 0, 0, 0,   1, 1},

        {0, 0,    0, 0, 0, 0,   0, 1},
        {0, 0,    0, 0, 0, 0,   0, 0},
        {0, 0,    0, 0, 0, 0,   1, 0},
    };


    // glyphs act as a cache of renderables for the text,
    // so need to match in size
    while(topaz_array_get_size(t->glyphs) < topaz_string_get_length(t->text)) {
        topazRender2D_t * r = topaz_render2d_create(topaz_graphics_get_renderer_2d(topaz_context_get_graphics(t->ctx)), t->spatial);
        t->attribs.primitive = topazRenderer_Primitive_Triangle;
        topaz_render2d_set_attributes(r, &t->attribs);

        topaz_render2d_set_vertices(r, TOPAZ_ARRAY_CAST(vtx_base, topazRenderer_2D_Vertex_t, 6));
        topaz_array_push(t->glyphs, r);
    }
    
    if (!fontRenderer) {
        return;
    }



    len = topaz_string_get_length(t->text);
    float oobLeft = t->oobLeftLast;
    float oobTop = t->oobTopLast;
    t->width = 0;
    t->height = 0;
    topazVector_t * p = malloc(sizeof(topazVector_t)*6*len);
    topazVector_t * pIter = p;
    topazVector_t * fme = p;



    // first we need the space layout for the character positions.
    // The reason we do this is the font renderer may request glyphs to be in negative 
    // space. This is not to spec, as the visual should have 0,0 be the origin.
    // oob (out of bounds) left and top hold the offset needed to 
    // prevent overflow into negative.
    for(i = oldLen; i < len; ++i) {
        // Get spacing 
        topaz_font_renderer_query_spacing(
            fontRenderer, 
            &spacing,
            t->currentSize,
            topaz_string_get_char(t->text, i-1),
            topaz_string_get_char(t->text, i),
            topaz_string_get_char(t->text, i+1)
        );


        // top left
        p->x = originX+spacing.xOffset;
        p->y = originY+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // bottom right
        p->x = originX+spacing.width+spacing.xOffset;
        p->y = originY+spacing.height+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // top right
        p->x = originX+spacing.width+spacing.xOffset;
        p->y = originY+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;







        // top left
        p->x = originX+spacing.xOffset;
        p->y = originY+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // bottom left
        p->x = originX+spacing.xOffset;
        p->y = originY+spacing.height+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;



        // bottom right
        p->x = originX+spacing.width+spacing.xOffset;
        p->y = originY+spacing.height+spacing.yOffset;
        if (p->x < oobLeft)  oobLeft = p->x;
        if (p->y < oobTop)   oobTop  = p->y;
        p++;

        


        // update total extent width/height
        if (originX+spacing.width+spacing.xOffset > t->width)
            t->width = originX+spacing.width+spacing.xOffset;

        if (originY+spacing.height+spacing.yOffset > t->height)
            t->height = originY+spacing.height+spacing.yOffset;


        originX = spacing.xNextOrigin;
        originY = spacing.yNextOrigin; 
    }

    // width and height are also impacted by OOB measurements 
    t->width -= oobLeft;
    t->height -= oobTop;


    // now we apply the positions with oob measurements for the 
    // real glyph verticies.
    for(i = oldLen; i < len; ++i) {
        topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, i);


        // set the next texture    
        const topazAsset_t * image = topaz_font_renderer_image_ref(
            fontRenderer,
            topaz_string_get_char(t->text, i),
            t->currentSize
        );

        topazRenderer_Texture_t * texture = topaz_image_frame_get_texture(topaz_image_get_frame((topazAsset_t *)image, 0));
        topaz_render2d_set_texture(r, texture);


        // apply data
        topazRenderer_2D_Vertex_t * v = &vtx_base[0];


        // top left
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // bottom right
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // top right
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;






        // top left
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // bottom left
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        v++; pIter++;


        // bottom right
        v->x = pIter->x - oobLeft;
        v->y = pIter->y - oobTop;
        v->r = v->b = v->g = v->a = 1.f;
        pIter++;
        


        topaz_render2d_set_vertices(r, TOPAZ_ARRAY_CAST(vtx_base, topazRenderer_2D_Vertex_t, 6));
    }
    t->spacingLast = spacing;
    t->oobLeftLast = oobLeft;
    t->oobTopLast = oobTop;
    free(fme);
}

static void text2d_update(Text2D * t, const topazString_t * str, const topazAsset_t * font, int pixelSize) {
    uint32_t i;
    uint32_t len, lenNew;

    len = topaz_string_get_length(t->text);
    lenNew = topaz_string_get_length(str);
    for(i = 0; i < len && i < lenNew; ++i) {
        if (topaz_string_get_char(t->text, i) != topaz_string_get_char(str, i)) break;
    }    



    // pixel size / font changed... need a full reset
    if (t->font != font || t->currentSize != pixelSize) {

        text2d_update__full(t, str, font, pixelSize);

    // different string entirely
    } else if (i == 0) { 
        text2d_update__full(t, str, font, pixelSize);


    // same string, no change needed
    } else if (len == lenNew && i == len) { 


    // current string is the base, new content being added.
    } else if (lenNew > len && i == len) {
        text2d_update__add(t, str, font, pixelSize);

    // default full update.
    } else {
        text2d_update__full(t, str, font, pixelSize);
    }


}

void topaz_text2d_set_font(
    topazComponent_t * c,
    const topazAsset_t * font,
    int pixelSize
) {
    Text2D * t = text2d__retrieve(c);
    text2d_update(
        t,
        t->text,
        font,
        pixelSize
    );
}


void topaz_text2d_set_text(
    topazComponent_t * c,
    const topazString_t * str
) {
    Text2D * t = text2d__retrieve(c);
    text2d_update(
        t, 
        str,
        t->font,
        t->currentSize
    );
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
    return t->height;
}

float topaz_text2d_get_char_x(topazComponent_t * c, int charIndex) {
    Text2D * t = text2d__retrieve(c);
    if (charIndex < 0 || charIndex >= topaz_string_get_length(t->text)) return 0;

    topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, charIndex);
    const topazArray_t * vtx = topaz_render2d_get_vertices(r);    
    return topaz_array_at(vtx, topazRenderer_2D_Vertex_t, 0).x;
}

float topaz_text2d_get_char_y(topazComponent_t * c, int charIndex) {
    Text2D * t = text2d__retrieve(c);
    if (charIndex < 0 || charIndex >= topaz_string_get_length(t->text)) return 0;

    topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, charIndex);
    const topazArray_t * vtx = topaz_render2d_get_vertices(r);    
    return topaz_array_at(vtx, topazRenderer_2D_Vertex_t, 0).y;
}


topazTransform_t * topaz_text2d_get_node(topazComponent_t * c) {
    Text2D * t = text2d__retrieve(c);
    return topaz_spatial_get_node(t->spatial);
}

void topaz_text2d_set_color_section(
    topazComponent_t * c, 
    int fromIndex,
    int toIndex,
    topazColor_t clr
) {
    Text2D * t = text2d__retrieve(c);

    uint32_t i, n;
    uint32_t len = topaz_string_get_length(t->text);
    for(i = (uint32_t)fromIndex; i < len && i <= (uint32_t)toIndex; ++i) {
        topazRender2D_t * r = topaz_array_at(t->glyphs, topazRender2D_t *, i);
        topazArray_t * vtx = topaz_array_clone(topaz_render2d_get_vertices(r));
        for(n = 0; n < 6; ++n) {
            topazRenderer_2D_Vertex_t * v = &topaz_array_at(vtx, topazRenderer_2D_Vertex_t, n);
            v->r = clr.r;
            v->g = clr.g;
            v->b = clr.b;
            v->a = clr.a;
        }
        topaz_render2d_set_vertices(r, vtx);
        topaz_array_destroy(vtx);
    }
}

/// Sets the color for all characters.
///
void topaz_text2d_set_color(
    topazComponent_t * c,
    topazColor_t clr 
) {
    Text2D * t = text2d__retrieve(c);

    topaz_text2d_set_color_section(
        c,
        0,
        topaz_string_get_length(t->text)-1,
        clr
    );
}
