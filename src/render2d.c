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

#include <topaz/render2d.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

struct topazRender2D_t {
    topazArray_t * vertexSrc;
    uint32_t objectID;
    int absolute;
    topazRenderer_EtchRule etch;
    topazRenderer_AlphaRule alphaRule;
    topazRenderer_Primitive polygon;

    topazSpatial_t * spatial;    
    topazRenderer_2D_t * renderer;
    topazArray_t * vertexCopy;
    
    topazRenderer_2D_ObjectParams_t params;
};


static void topaz_render2d_update_transform(topazSpatial_t * sp, topazRender2D_t * r) {
    if (!r->absolute) {
        topazMatrix_t m = *topaz_spatial_get_global_transform(sp);
        topaz_matrix_reverse_majority(&m);
        r->params.transform = m;

        topaz_renderer_2d_set_object_params(
            r->renderer,
            r->objectID,
            &r->params
        );
    } 
}

void topaz_render2d_set_texture(topazRender2D_t * r, topazRenderer_Texture_t * t) {
    r->params.texture = t;
    topaz_renderer_2d_set_object_params(
        r->renderer,
        r->objectID,
        &r->params
    );
}




topazRender2D_t * topaz_render2d_create(topazRenderer_2D_t * r) {
    topazRender2D_t * out = calloc(1, sizeof(topazRender2D_t));
    out->renderer = r;
    out->vertexSrc = topaz_array_create(sizeof(uint32_t));
    out->spatial = topaz_spatial_create();

    topaz_renderer_2d_add_objects(out->renderer, &out->objectID, 1);
    out->etch = topazRenderer_EtchRule_Out;
    out->polygon = topazRenderer_Primitive_Triangle;
    out->alphaRule = topazRenderer_AlphaRule_Allow;

    topaz_spatial_add_update_callback(
        out->spatial,
        (void(*)(topazSpatial_t *, void *))topaz_render2d_update_transform,
        out
    );
    
    return out;
}

void topaz_render2d_destroy(topazRender2D_t * t) {
    topaz_renderer_2d_remove_objects(t->renderer, &t->objectID, 1);
    topaz_renderer_2d_remove_vertices(
        t->renderer,
        topaz_array_get_data(t->vertexSrc),
        topaz_array_get_size(t->vertexSrc)
    );
    topaz_array_destroy(t->vertexSrc);
    topaz_spatial_destroy(t->spatial);
    if (t->vertexCopy) topaz_array_destroy(t->vertexCopy);
    free(t);
}



topazRenderer_EtchRule topaz_render2d_get_etch_rule(const topazRender2D_t * t) {
    return t->etch;
}

void topaz_render2d_set_etch_rule(topazRender2D_t * t, topazRenderer_EtchRule e) {
    t->etch = e;
}


topazRenderer_AlphaRule topaz_render2d_get_alpha_rule(const topazRender2D_t * t) {
    return t->alphaRule;
}

void topaz_render2d_set_alpha_rule(topazRender2D_t * t, topazRenderer_AlphaRule a) {
    t->alphaRule = a;
}



int topaz_render2d_get_absolute(const topazRender2D_t * t) {
    return t->absolute;
}

void topaz_render2d_set_absolute(topazRender2D_t * t, int a) {
    if (t->absolute == a) return;
    
    // reset transform to be identity
    if (a) {
        topazMatrix_t m; topaz_matrix_set_identity(&m);
        t->params.transform = m;
        topaz_renderer_2d_set_object_params(
            t->renderer,
            t->objectID,
            &t->params
        );
    }

    t->absolute = a;
    topaz_spatial_invalidate(t->spatial);
}



topazRenderer_Primitive topaz_render2d_get_primitive(const topazRender2D_t * t) {
    return t->polygon;
}

void topaz_render2d_set_primitive(topazRender2D_t * t, topazRenderer_Primitive p) {
    t->polygon = p;
}


topazSpatial_t * topaz_render2d_get_spatial(topazRender2D_t * t) {
    return t->spatial;
}


const topazArray_t * topaz_render2d_get_vertices(const topazRender2D_t * t) {
    if (!t->vertexCopy) {
        ((topazRender2D_t *)t)->vertexCopy = topaz_array_create(sizeof(topazRenderer_2D_Vertex_t));
    }

    uint32_t len = topaz_array_get_size(t->vertexSrc);
    topaz_array_set_size(t->vertexCopy, len);

    topaz_renderer_2d_get_vertices(
        t->renderer,
        topaz_array_get_data(t->vertexSrc),
        topaz_array_get_data(t->vertexCopy),
        len
    );
    return t->vertexCopy;
}

void topaz_render2d_set_vertices(topazRender2D_t * t, const topazArray_t * src) {
    int64_t diff = topaz_array_get_size(src) - (int64_t)topaz_array_get_size(t->vertexSrc);
    if (diff > 0) {
        uint32_t oldSize = topaz_array_get_size(t->vertexSrc);
        topaz_array_set_size(t->vertexSrc, topaz_array_get_size(src));

        topaz_renderer_2d_add_vertices(
            t->renderer,         
            ((uint32_t*)topaz_array_get_data(t->vertexSrc)) + oldSize,
            diff
        );

    } else if (diff < 0) {
        uint32_t newSize = topaz_array_get_size(src) + diff;
        topaz_renderer_2d_remove_vertices(
            t->renderer,         
            ((uint32_t*)topaz_array_get_data(t->vertexSrc)) + newSize,
            -diff
        );
        topaz_array_set_size(t->vertexSrc, newSize);
    }

    topaz_renderer_2d_set_vertices(
        t->renderer,
        topaz_array_get_data(t->vertexSrc),
        topaz_array_get_data(src),
        topaz_array_get_size(t->vertexSrc)
    );

}
