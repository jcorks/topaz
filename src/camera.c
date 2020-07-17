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
#include <math.h>
#include <stdlib.h>
#include <string.h>

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





static void camera__on_step(topazEntity_t * );


static topazMatrix_t matrix_projection_perspective(float fovy, float ratio, float zNear, float zFar);

topazEntity_t * topaz_camera_create(topaz_t * t) {
    TopazCamera * c = calloc(1, sizeof(TopazCamera));
    topazRenderer_t * r = topaz_context_get_backend_renderer(t);

    c->mv    = topaz_renderer_buffer_create(r, NULL, 32);
    c->proj  = topaz_renderer_buffer_create(r, NULL, 16);
    c->fb    = topaz_renderer_framebuffer_create(r);
    c->fbAux = topaz_renderer_framebuffer_create(r);
    c->type  = topazCamera_Type_Manual;
    c->autoRefresh = TRUE;
    c->renderer = r;
    
    topazEntity_Attributes_t attr;

    attr.userData = c;

    return topaz_entity_create_with_attributes(t, &attr);
}


void topaz_camera_set_type(topazEntity_t * e, topazCamera_Type t) {
    TopazCamera * c = camera__retrieve(e);
    c->type = t;
    if (t == topazCamera_Type_Perspective3D) {
        topazMatrix_t projectionMatrix = matrix_projection_perspective(60, 1, .01, 100);
        topaz_matrix_reverse_majority(&projectionMatrix);
        topaz_renderer_buffer_update(c->proj, &projectionMatrix.data[0], 0, 16);
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

        topazVector_t rot = topaz_entity_rotation(e);
        rot->x = topaz_matrix_transform(&m, &x).z;
        rot->y = topaz_matrix_transform(&m, &z).y;
        rot->z = topaz_matrix_transform(&m, &y).x;
    } else {
        *topaz_entity_position(e) = *target;
    }


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
    topazVector_t F = target - camPos;
    F = F.Normalize();


    Vector s = F.Cross(upVec);
    s = s.Normalize();

    Vector u = s.Cross(F);

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


    out.Translate(-camPos.x, -camPos.y, -camPos.z);
    return out;
}