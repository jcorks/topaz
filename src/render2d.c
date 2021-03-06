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
    topazRenderer_Buffer_t * vertexSrc;
    uint32_t objectID;
    int absolute;
    topazRenderer_Attributes_t attribs;
    uint32_t updateID;

    topazSpatial_t * spatial;   
    int inheritedSpatial; 
    topazRenderer_2D_t * renderer;
    topazArray_t * vertexCopy;
    

};


static void topaz_render2d_update_transform(topazSpatial_t * sp, topazRender2D_t * r) {
    if (!r->absolute) {
        topazMatrix_t m = *topaz_spatial_get_global_transform(sp);
        topaz_renderer_2d_set_object_transform(
            r->renderer,
            r->objectID,
            &m
        );
    } 
}

void topaz_render2d_set_texture(topazRender2D_t * r, topazRenderer_Texture_t * t) {
    topaz_renderer_2d_set_object_texture(
        r->renderer,
        r->objectID,
        t
    );
}




topazRender2D_t * topaz_render2d_create(topazRenderer_2D_t * r, topazSpatial_t * inherit) {
    topazRender2D_t * out = calloc(1, sizeof(topazRender2D_t));
    out->renderer = r;
    out->vertexSrc = NULL;

    if (!inherit) {
        out->spatial = topaz_spatial_create();
    } else {
        out->spatial = inherit;
        out->inheritedSpatial = 1;
    }
    topaz_renderer_2d_add_objects(out->renderer, &out->objectID, 1);
    out->attribs.primitive = topazRenderer_Primitive_Triangle;
    out->attribs.depthTest = topazRenderer_DepthTest_None;
    out->attribs.alphaRule = topazRenderer_AlphaRule_Allow;
    out->attribs.etchRule  = topazRenderer_EtchRule_NoEtching;
    out->attribs.textureFilter = topazRenderer_TextureFilterHint_Linear;
    out->updateID = topaz_spatial_add_update_callback(
        out->spatial,
        (void(*)(topazSpatial_t *, void *))topaz_render2d_update_transform,
        out
    );
    // populate 2d transform from current global transform.
    topaz_render2d_update_transform(out->spatial, out);
    
    return out;
}

void topaz_render2d_destroy(topazRender2D_t * t) {
    topaz_spatial_remove_update_callback(t->spatial, t->updateID);
    topaz_renderer_2d_remove_objects(t->renderer, &t->objectID, 1);
    if (t->vertexSrc)
        topaz_renderer_buffer_destroy(t->vertexSrc);

    if (!t->inheritedSpatial)
        topaz_spatial_destroy(t->spatial);
    if (t->vertexCopy) topaz_array_destroy(t->vertexCopy);
    free(t);
}










int topaz_render2d_get_absolute(const topazRender2D_t * t) {
    return t->absolute;
}

void topaz_render2d_set_absolute(topazRender2D_t * t, int a) {
    if (t->absolute == a) return;
    
    // reset transform to be identity
    if (a) {
        topazMatrix_t m; topaz_matrix_set_identity(&m);
        topaz_renderer_2d_set_object_transform(
            t->renderer,
            t->objectID,
            &m
        );
    }

    t->absolute = a;
    topaz_spatial_invalidate(t->spatial);
}





topazSpatial_t * topaz_render2d_get_spatial(topazRender2D_t * t) {
    return t->spatial;
}


const topazArray_t * topaz_render2d_get_vertices(const topazRender2D_t * t) {
    return t->vertexCopy;
}

void topaz_render2d_set_vertices(topazRender2D_t * t, const topazArray_t * src) {
    void *   srcData = topaz_array_get_data(src);
    uint32_t srcLen  = topaz_array_get_size(src);

    if (t->vertexCopy) {
        if (topaz_array_get_size(t->vertexCopy) != srcLen) {
            topaz_array_set_size(t->vertexCopy, srcLen);

            memcpy(
                topaz_array_get_data(t->vertexCopy),
                srcData,
                srcLen*sizeof(topazRenderer_2D_Vertex_t)
            );
            topazRenderer_Buffer_t * old = t->vertexSrc;
            t->vertexSrc = topaz_renderer_buffer_create(
                topaz_renderer_2d_get_context(t->renderer),
                srcData,
                (sizeof(topazRenderer_2D_Vertex_t) / sizeof(float)) * srcLen
            );            
            topaz_renderer_2d_set_object_vertices(
                t->renderer,
                t->objectID,
                t->vertexSrc
            );
            topaz_renderer_buffer_destroy(old);


        } else {
            memcpy(
                topaz_array_get_data(t->vertexCopy),
                srcData,
                srcLen*sizeof(topazRenderer_2D_Vertex_t)
            );
            topaz_renderer_buffer_update(
                t->vertexSrc, 
                srcData,
                0,
                (sizeof(topazRenderer_2D_Vertex_t) / sizeof(float)) * srcLen
            );
        }
    } else {
        t->vertexCopy = topaz_array_clone(src);
        t->vertexSrc = topaz_renderer_buffer_create(
            topaz_renderer_2d_get_context(t->renderer),
            srcData,
            (sizeof(topazRenderer_2D_Vertex_t) / sizeof(float)) * srcLen
        );
        topaz_renderer_2d_set_object_vertices(
            t->renderer,
            t->objectID,
            t->vertexSrc
        );
    }





}

uint32_t topaz_render2d_get_render_data(
    topazRender2D_t * r,
    topazRenderer_Attributes_t * p) {
    *p = r->attribs;
    return r->objectID;
}



void topaz_render2d_set_attributes(
    topazRender2D_t * r2d,
    const topazRenderer_Attributes_t * attribs
) {
    r2d->attribs = *attribs;
}

const topazRenderer_Attributes_t * topaz_render2d_get_attributes(
    topazRender2D_t * r2d
) {
    return &(r2d->attribs);
}


