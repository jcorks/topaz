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
#include <topaz/containers/array.h>
#include <topaz/camera.h>
#include <topaz/entity.h>
#include <topaz/matrix.h>
#include <topaz/backends/renderer.h>
#include <topaz/topaz.h>
#include <topaz/entity.h>
#include <topaz/assets/image.h>
#include <topaz/modules/graphics.h>
#include <topaz/spatial.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif

typedef struct {

    topazRenderer_Buffer_t * mv;
    topazRenderer_Buffer_t * proj;
    topazCamera_Type type;

    topazRenderer_Framebuffer_t * fb;
    topazRenderer_Framebuffer_t * fbAux;

    int lastW;
    int lastH;
    int autoRefresh;
    topazRenderer_t * renderer;
    topaz_t * ctx;
    float nearClip;
    float farClip;

    topazMatrix_t projectionMatrix;
    #ifdef TOPAZDC_DEBUG
        char * MAGIC_ID;
    #endif
} TopazCamera;



#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__CAMERA = "t0p4zc4m3r4";
#endif


static TopazCamera * camera__retrieve(topazEntity_t * e) {
    TopazCamera * c = topaz_entity_get_attributes(e)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(c && "Shape2D instance is missing or instance invalid.");
        assert(c->MAGIC_ID == MAGIC_ID__CAMERA);
    #endif
    return c;
}





static void camera__on_step(topazEntity_t *, void *);


static topazMatrix_t matrix_projection_perspective(float fovy, float ratio, float zNear, float zFar);
static topazMatrix_t matrix_view_look_at(
    const topazVector_t * camPos,
    const topazVector_t * target,
    const topazVector_t * upVec
);
static topazMatrix_t matrix_projection_orthographic(
    float left,    float right,
    float bottom,  float top,
    float zNear,   float zFar
);


static topazAsset_t * camera__copy_fb(topaz_t *, topazRenderer_Framebuffer_t *);


topazEntity_t * topaz_camera_create(topaz_t * t) {
    TopazCamera * c = calloc(1, sizeof(TopazCamera));
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(t));
    #ifdef TOPAZDC_DEBUG
        c->MAGIC_ID = MAGIC_ID__CAMERA;
    #endif
    c->mv    = topaz_renderer_buffer_create(r, NULL, 32);
    c->proj  = topaz_renderer_buffer_create(r, NULL, 16);
    c->fb    = topaz_renderer_framebuffer_create(r);
    c->fbAux = topaz_renderer_framebuffer_create(r);
    c->type  = topazCamera_Type_Manual;
    c->autoRefresh = TRUE;
    c->renderer = r;
    c->ctx = t;

    c->nearClip = 0.01;
    c->farClip = 100;
    
    topazEntity_Attributes_t attr = {};

    attr.userData = c;
    attr.on_step = camera__on_step;

    return topaz_entity_create_with_attributes(t, &attr);
}


void topaz_camera_set_type(topazEntity_t * e, topazCamera_Type t) {
    TopazCamera * c = camera__retrieve(e);
    c->type = t;
    if (t == topazCamera_Type_Perspective3D) {
        c->projectionMatrix = matrix_projection_perspective(60, 1, c->nearClip, c->farClip);
        topaz_matrix_reverse_majority(&c->projectionMatrix);
        topaz_renderer_buffer_update(c->proj, &c->projectionMatrix.data[0], 0, 16);
    } else {
        // force normal update. We dont do this for perspective because 
        // we only need to set it up once.
    }
}

void topaz_camera_refresh(topazEntity_t * e) {
    TopazCamera * c = camera__retrieve(e);
    topazRenderer_t * r = c->renderer;
    topazRenderer_Framebuffer_t * old = topaz_renderer_get_target(r);

    if (old != c->fb) {
        topaz_renderer_sync(r);
        topaz_renderer_attach_target(r, c->fb);
        topaz_renderer_clear_layer(r, topazRenderer_DataLayer_All);
        topaz_renderer_attach_target(r, old);
    } else {
        topaz_renderer_clear_layer(r, topazRenderer_DataLayer_All);
    }
}

void topaz_camera_set_auto_refresh(topazEntity_t * e, int ar) {
    TopazCamera * c = camera__retrieve(e);
    c->autoRefresh = ar;
}


void topaz_camera_look_at(
    topazEntity_t * e, 
    const topazVector_t * target,
    const topazVector_t * up
) {
    TopazCamera * c = camera__retrieve(e);
    if (c->type == topazCamera_Type_Perspective3D) {
        topaz_vector_reset(topaz_entity_rotation(e));
        topazSpatial_t * t = topaz_entity_get_spatial(e);
        const topazMatrix_t * gl = topaz_spatial_get_global_transform(t);
        topazVector_t p = topaz_matrix_transform(gl, topaz_entity_get_position(e));
        topazMatrix_t m = matrix_view_look_at(
            &p,
            target,
            up
        );

        topazVector_t x = topaz_vector_from_xyz(1, 0, 0);
        topazVector_t y = topaz_vector_from_xyz(0, 1, 0);
        topazVector_t z = topaz_vector_from_xyz(0, 0, 1);

        topazVector_t * rot = topaz_entity_rotation(e);
        rot->x = topaz_matrix_transform(&m, &x).z;
        rot->y = topaz_matrix_transform(&m, &z).y;
        rot->z = topaz_matrix_transform(&m, &y).x;
    } else {
        *topaz_entity_position(e) = *target;
    }


}

topazVector_t topaz_camera_screen_to_world(topazEntity_t * e, const topazVector_t * p) {
    TopazCamera * c = camera__retrieve(e);



    float w = topaz_renderer_framebuffer_get_width(c->fb);
    float h = topaz_renderer_framebuffer_get_height(c->fb);

    topazVector_t in;
    in.x = (p->x/w)*2 - 1;
    in.y = ((h-p->y)/h)*2 - 1;
    in.z = ((p->z+c->nearClip)/c->farClip);

    topazSpatial_t * s = topaz_entity_get_spatial(e);
    topazMatrix_t mvInv = *topaz_spatial_get_global_transform(s);
    topazMatrix_t pjInv = c->projectionMatrix;

    topaz_matrix_invert(&mvInv);
    topaz_matrix_invert(&pjInv);

    in = topaz_matrix_transform(&pjInv, &in);
    in = topaz_matrix_transform(&mvInv, &in);

    return in;

}

topazVector_t topaz_camera_world_to_screen(topazEntity_t * e, const topazVector_t * p) {
    TopazCamera * c = camera__retrieve(e);

    float w = topaz_renderer_framebuffer_get_width(c->fb);
    float h = topaz_renderer_framebuffer_get_height(c->fb);

    topazSpatial_t * s = topaz_entity_get_spatial(e);

    // todo: perspective? mv?
    topazVector_t out = topaz_matrix_transform(topaz_spatial_get_global_transform(s), p);
    out = topaz_matrix_transform(&c->projectionMatrix, &out);

    out.x = (out.x)*(w/2.f) + w/2.f;
    out.y = h - ((out.y)*(h/2.f) + h/2.f);
    return out;
}





void topaz_camera_set_render_resolution(topazEntity_t * e, int w, int h) {
    TopazCamera * c = camera__retrieve(e);

    topaz_renderer_framebuffer_resize(c->fb, w, h);
    topaz_renderer_framebuffer_resize(c->fbAux, w, h);
}


int topaz_camera_get_render_height(const topazEntity_t * e) {
    TopazCamera * c = camera__retrieve((topazEntity_t*)e);

    return topaz_renderer_framebuffer_get_height(c->fb);
}


int topaz_camera_get_render_width(const topazEntity_t * e) {
    TopazCamera * c = camera__retrieve((topazEntity_t*)e);

    return topaz_renderer_framebuffer_get_width(c->fb);
}


void topaz_camera_set_filtered(topazEntity_t * e, int hint) {
    TopazCamera * c = camera__retrieve(e);

    topaz_renderer_framebuffer_set_filtered_hint(c->fb,    hint);
    topaz_renderer_framebuffer_set_filtered_hint(c->fbAux, hint);
}


topazRenderer_Framebuffer_t * topaz_camera_get_framebuffer(topazEntity_t * e) {
    TopazCamera * c = camera__retrieve(e);
    return c->fb;
}

topazAsset_t * topaz_camera_get_front_visual(topazEntity_t * e) {
    TopazCamera * c = camera__retrieve(e);
    return camera__copy_fb(c->ctx, c->fb);
}

topazAsset_t * topaz_camera_get_back_visual(topazEntity_t * e) {
    TopazCamera * c = camera__retrieve(e);
    return camera__copy_fb(c->ctx, c->fbAux);
}


void topaz_camera_swap_buffers(topazEntity_t * e) {
    TopazCamera * c = camera__retrieve(e);
    topazRenderer_Framebuffer_t * fb = c->fb;
    c->fb = c->fbAux;
    c->fbAux = fb;
    
    topazRenderer_t * ctx = topaz_graphics_get_renderer(
        topaz_context_get_graphics(c->ctx)
    );

    // need to also swap targets
    if (topaz_renderer_get_target(ctx) == fb) {
        topaz_renderer_attach_target(ctx, c->fb);
    }
}

topazRenderer_Buffer_t * topaz_camera_get_view_transform(
    topazEntity_t * e
) {
    TopazCamera * c = camera__retrieve(e);
    return c->mv;
}

topazRenderer_Buffer_t * topaz_camera_get_projection_transform(
    topazEntity_t * e
) {
    TopazCamera * c = camera__retrieve(e);
    return c->proj;
}



static topazMatrix_t matrix_projection_perspective(
    float fovy, float ratio, float zNear, float zFar
) {
    topazMatrix_t out;
    float * projection = &out.data[0];
    float perspective[4];
    perspective[0] = fovy * (topaz_math_pi / 180.f);
    perspective[1] = ratio;
    perspective[2] = zNear;
    perspective[3] = zFar;

    float f = 1.f / (tan(perspective[0] / 2.f));
    projection[0] = f / perspective[1];
    projection[5] = f;
    projection[10] = (perspective[2] + perspective[3]) /
                       (perspective[2] - perspective[3]);

    projection[11] = (perspective[2] * perspective[3] * 2) /
                       (perspective[2] - perspective[3]);

    projection[14] = -1.f;

    return out;

}



topazMatrix_t matrix_view_look_at(
    const topazVector_t * camPos,
    const topazVector_t * target,
    const topazVector_t * upVec
) {
    topazMatrix_t out;

    float * laScratch = &out.data[0];
    topazVector_t F;
    F.x = target->x - camPos->x;
    F.y = target->y - camPos->y;
    F.z = target->z - camPos->z;
    topaz_vector_normalize(&F);


    topazVector_t s = topaz_vector_cross(&F, upVec);
    topaz_vector_normalize(&s);

    topazVector_t u = topaz_vector_cross(&s, &F);

    laScratch[0] = s.x;
    laScratch[1] = s.y;
    laScratch[2] = s.z;
    laScratch[3] = 0;
    laScratch[4] = u.x;
    laScratch[5] = u.y;
    laScratch[6] = u.z;
    laScratch[7] = 0;
    laScratch[8] = -F.x;
    laScratch[9] = -F.y;
    laScratch[10] = -F.z;
    laScratch[11] = 0;
    laScratch[12] = 0;
    laScratch[13] = 0;
    laScratch[14] = 0;
    laScratch[15] = 1;

    topaz_matrix_translate(&out, -camPos->x, -camPos->y, -camPos->z);
    return out;
}

static topazMatrix_t matrix_projection_orthographic(
    float left,    float right,
    float bottom,  float top,
    float zNear,   float zFar
) {
    topazMatrix_t out;
    topaz_matrix_set_identity(&out);
    float * projection = out.data+0;
    projection[0] = (2)  / (right - left);
    projection[5] = (2)  / (top - bottom);
    projection[10] = (-2) / (zFar - zNear);

    projection[3]  = -(right + left) / (right - left);
    projection[7]  = -(top + bottom) / (top - bottom);
    projection[11] = -(zFar + zNear) / (zFar - zNear);
    return out;
}


static topazAsset_t * camera__copy_fb(topaz_t * t, topazRenderer_Framebuffer_t * fb) {
    topazAsset_t * image = topaz_image_create_empty(t);

    int w = topaz_renderer_framebuffer_get_width(fb);
    int h = topaz_renderer_framebuffer_get_height(fb);
    uint8_t * data = malloc(w*h*4);
    topaz_renderer_framebuffer_get_raw_data(fb, data);

    topaz_image_add_frame(image);
    topaz_image_frame_set_data(topaz_image_get_frame(image, 0), data);
    free(data);

    return image;
}


void camera__on_step(topazEntity_t * e, void * data) {
    TopazCamera * c = camera__retrieve(e);

    topazSpatial_t * s = topaz_entity_get_spatial(e);
    topazMatrix_t m = *topaz_spatial_get_global_transform(s);
    topaz_matrix_invert(&m);
    topaz_matrix_reverse_majority(&m);
    topaz_renderer_buffer_update(c->mv, &m.data[0], 0, 16);


    if (c->fb && c->type == topazCamera_Type_Orthographic) {
        int w = topaz_renderer_framebuffer_get_width(c->fb);
        int h = topaz_renderer_framebuffer_get_height(c->fb);

        if (c->lastW != w ||
            c->lastH != h) {

            c->lastW = w;
            c->lastH = h;

            c->projectionMatrix = matrix_projection_orthographic(0, c->lastW, c->lastH, 0, -1024.f, 1024.f);

            m = c->projectionMatrix;
            topaz_matrix_reverse_majority(&m);
            topaz_renderer_buffer_update(c->proj, m.data+0, 0, 16);
        }
    }
}
