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
#include <topaz/assets/image.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/topaz.h>
#include <topaz/backends/renderer.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all assets
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__IMAGE = "t0p4zim4g3";
#endif



const uint8_t default_texture_data[] = {
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,




    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0xff,

    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff,

};





typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif

    // context 
    topaz_t * ctx;

    // frames within the image
    topazArray_t * frames;

    int width;
    int height;

} TopazImage;

struct topazImage_Frame_t {
    topazRenderer_Texture_t * object;
    TopazImage * img;
};



void image__destroy(topazAsset_t * a, void * userData) {
    TopazImage * img = userData;
    while(topaz_image_get_frame_count(a)) {
        topaz_image_remove_frame(a, topaz_image_get_frame_count(a)-1);
    }
    topaz_array_destroy(img->frames);
    free(img);
}

TopazImage * image__retrieve(const topazAsset_t * a) {
    TopazImage * ret = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(ret->MAGIC_ID == MAGIC_ID__IMAGE);
    #endif
    return ret;
}



topazAsset_t * topaz_image_create(topaz_t * t, const topazString_t * name, topazAsset_LoadingProfile_t * loading) {

    TopazImage * img = calloc(1, sizeof(TopazImage));
    #ifdef TOPAZDC_DEBUG
        img->MAGIC_ID = MAGIC_ID__IMAGE;
    #endif
    img->frames = topaz_array_create(sizeof(topazImage_Frame_t *));
    img->ctx = t;


    topazAsset_Attributes_t attribs;
    attribs.on_destroy = image__destroy;
    attribs.userData = img;

    return topaz_asset_create(
        t,
        topazAsset_Type_Image,
        name, 
        &attribs,
        loading
    );
}



topazImage_Frame_t * topaz_image_add_frame(topazAsset_t * a) {
    TopazImage * img = image__retrieve(a);
    topazImage_Frame_t * frame = calloc(sizeof(topazImage_Frame_t), 1);
    frame->img = img;
    frame->object = topaz_renderer_texture_create(topaz_context_get_backend_renderer(img->ctx), 4, 4, default_texture_data);
    topaz_array_push(img->frames, frame);
    return frame;
}

topazImage_Frame_t * topaz_image_get_frame(topazAsset_t * a, uint32_t index) {
    TopazImage * img = image__retrieve(a);
    uint32_t imageCount = topaz_array_get_size(img->frames);
    if (index > imageCount)
        index = index%imageCount;
    return topaz_array_at(img->frames, topazImage_Frame_t *, index);

}

void topaz_image_remove_frame(topazAsset_t * a, uint32_t index) {
    TopazImage * img = image__retrieve(a);
    if (index >= topaz_array_get_size(img->frames)) return;
    topazImage_Frame_t * frame = topaz_array_at(img->frames, topazImage_Frame_t *, index);
    topaz_renderer_texture_destroy(frame->object);
    free(frame);
    topaz_array_remove(img->frames, index);
}

uint32_t topaz_image_get_frame_count(const topazAsset_t * a) {
    TopazImage * img = image__retrieve(a);
    return topaz_array_get_size(img->frames);
}

int topaz_image_get_width(const topazAsset_t * a) {
    TopazImage * img = image__retrieve(a);
    return img->width;;
}


int topaz_image_get_height(const topazAsset_t * a) {
    TopazImage * img = image__retrieve(a);
    return img->height;
}

void topaz_image_resize(
    topazAsset_t * a,
    int width,
    int height
) {
    TopazImage * img = image__retrieve(a);
    uint32_t i;
    uint32_t len = topaz_array_get_size(img->frames);
    topazImage_Frame_t * f;
    img->width = width;
    img->height = height;

    for(i = 0; i < len; ++i) {
        f = topaz_array_at(img->frames, topazImage_Frame_t *, i);

        topaz_renderer_texture_destroy(f->object);
        f->object = topaz_renderer_texture_create(
            topaz_context_get_backend_renderer(f->img->ctx),
            width, height, NULL
        );

    }
}





void topaz_image_frame_set_from_texture(
    topazImage_Frame_t * f, 
    topazRenderer_Texture_t * t
) {
    topaz_renderer_texture_destroy(f->object);

    int w = f->img->width;
    int h = f->img->height;
    uint8_t * newData = malloc(w*h*4);
    topaz_renderer_texture_get(t, newData);

    f->object = topaz_renderer_texture_create(
        topaz_context_get_backend_renderer(f->img->ctx),
        w, h, newData
    );
}





void topaz_image_frame_set_data(
    topazImage_Frame_t * f,
    const uint8_t * rgbaData
) {
    topaz_renderer_texture_update(f->object, rgbaData);
}




topazRenderer_Texture_t * topaz_image_frame_get_texture(const topazImage_Frame_t * f) {
    return f->object;
}


topazArray_t * topaz_image_frame_get_rgba_data(const topazImage_Frame_t * f) {
    topazArray_t * arr = topaz_array_create(sizeof(uint8_t));
    int w = f->img->width;
    int h = f->img->height;
    topaz_array_set_size(arr, w*h*4);

    topaz_renderer_texture_get(
        f->object, 
        &(topaz_array_at(arr, uint8_t, 0))
    );
    return arr;
}





