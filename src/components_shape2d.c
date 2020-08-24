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


typedef enum {
    AnimMode_Normal,
    AnimMode_SingleFrame,
} AnimMode;

typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topazAsset_t * id;
    topazRender2D_t * render2d;
    topazColor_t color;
    topazColor_t realColor;
    topazVector_t center;
    float animSpeed;
    int animFrame;
    AnimMode animMode;
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


    aspect.CheckUpdate();


    Camera * cam2d = &GetCamera2D();
    if (!cam2d) return;


    setDisplayMode(aspect.GetPolygon(),
                   Renderer::DepthTest::NoTest,
                   (Renderer::AlphaRule)(int)aspect.mode);

    if (round(params2D.contextWidth) != cam2d->Width() ||
        round(params2D.contextHeight) != cam2d->Height()) {

        UpdateCameraTransforms();
    }

    if ((int)params2D.etchRule != (int)aspect.etch) {
        drawBuffer->Render2DVertices(params2D);
        params2D.etchRule = (Renderer::EtchRule)(int)aspect.etch;
    }



    drawBuffer->Queue2DVertices(
        &aspect.GetVertexIDs()[0],
        aspect.GetVertexIDs().size()
    );

    
    // TODO: anim mode


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
}
    
void topaz_shape2d_form_rectangle(topazComponent_t * c, float w, float h) {
    Shape2D * s = shape2d__retrieve(c);
    s->id = NULL;
    float color[4] = {
        topaz_color_r_amt(s->color),
        topaz_color_g_amt(s->color),
        topaz_color_b_amt(s->color),
        topaz_color_a_amt(s->color)
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

    topaz_render2d_set_primitive(
        s->render2d,
        topazRenderer_Primitive_Triangle
    );

    
}


void topaz_shape2d_form_image(topazComponent_t * c, topazAsset_t * img) {
    topaz_shape2d_form_image_frame(c, img, 0);
    Shape2D * s = shape2d__retrieve(c);
    s->animMode = topaz_image_get_frame_count(img) == 1 ? AnimMode_SingleFrame : AnimMode_Normal;
}


void topaz_shape2d_form_image_scaled(topazComponent_t * c, topazAsset_t * img, float width, float height) {
    topaz_shape2d_form_image_frame_scaled(c, img, 0, width, height);
    Shape2D * s = shape2d__retrieve(c);
    s->animMode = topaz_image_get_frame_count(img) == 1 ? AnimMode_SingleFrame : AnimMode_Normal;
}

void topaz_shape2d_form_image_frame(topazComponent_t * c, topazAsset_t * img, uint32_t frame) {
    if (topaz_image_get_frame_count(img) == 0) return;
    int w = topaz_image_get_width(img);
    int h = topaz_image_get_height(img);
    topaz_shape2d_form_image_scaled(c, img, w, h);
}


void topaz_shape2d_form_image_frame_scaled(topazComponent_t * c, topazAsset_t * img, uint32_t frame,  float w, float h) {
    Shape2D * s = shape2d__retrieve(c);
    s->id = img;
    s->animFrame = frame;
    s->animMode = AnimMode_SingleFrame;
    topaz_render2d_set_texture(
        s->render2d, 
        topaz_image_frame_get_texture(topaz_image_get_frame(img, frame))
    );

    float color[4] = {
        topaz_color_r_amt(s->color),
        topaz_color_g_amt(s->color),
        topaz_color_b_amt(s->color),
        topaz_color_a_amt(s->color)
    };


    topazRenderer_2D_Vertex_t v[6] = {
        { -s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 0 },
        { -s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     0, 1 },
        {w-s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     1, 1 },

        { -s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     0, 0 },
        {w-s->center.x, h-s->center.y,  color[0], color[1], color[2], color[3],     1, 1 },
        {w-s->center.x,  -s->center.y,  color[0], color[1], color[2], color[3],     1, 0 }
    };


    topaz_render2d_set_vertices(
        s->render2d, 
        TOPAZ_ARRAY_CAST(
            v, 
            topazRenderer_2D_Vertex_t,
            6
        )
    );

    topaz_render2d_set_primitive(
        s->render2d,
        topazRenderer_Primitive_Triangle
    );

}






void topaz_shape2d_form_radial(topazComponent_t * c, float radius, uint32_t numIterations) {
    Shape2D * s = shape2d__retrieve(c);

    topazVector_t * pts = malloc(sizeof(topazVector_t)*numIterations);
    topazVector_t * ptsIter = pts;
    topazVector_t curPt;
    topazVector_t empty;
    s->id = NULL;
    curPt.z = 0;
    empty.x = empty.y = empty.z = 0;

    uint32_t i;
    for(i = 0; i < numIterations; ++i) {
        curPt.x = cos(2*TOPAZ_PI * (i / (float) numIterations)) * radius;
        curPt.y = sin(2*TOPAZ_PI * (i / (float) numIterations)) * radius;        
        (*pts++) = curPt;

        curPt.x = cos(2*TOPAZ_PI * (((i+1)%numIterations) / (float) numIterations)) * radius;
        curPt.y = sin(2*TOPAZ_PI * (((i+1)%numIterations) / (float) numIterations)) * radius;

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
            numIterations
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
        topaz_color_r_amt(s->color),
        topaz_color_g_amt(s->color),
        topaz_color_b_amt(s->color),
        topaz_color_a_amt(s->color)
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

    topaz_render2d_set_primitive(
        s->render2d,
        topazRenderer_Primitive_Triangle
    );

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
    topaz_render2d_set_primitive(
        s->render2d,
        topazRenderer_Primitive_Line
    );
}



