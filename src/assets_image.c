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
#include <topaz/containers/bin.h>
#include <topaz/topaz.h>
#include <topaz/backends/renderer.h>
#include <topaz/modules/graphics.h>

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



/// An individual frame of information that holds pixel data.
/// Images contain 1 or more frames.
typedef struct topazImage_Frame_t topazImage_Frame_t;



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

    topazBin_t * notifs;

} TopazImage;

struct topazImage_Frame_t {
    topazRenderer_Texture_t * object;
    TopazImage * img;
    topazAsset_t * asset;
    int proxy;
};





typedef struct {
    topaz_image_texture_event_callback cb;
    void * data;
} topazImage_RemoveNotify_t;


static void notify_event(
    TopazImage * img,
    topazAsset_t * a, 
    topazImage_TextureEvent event,
    int index
) {
    // TODO: efficiency
    topazArray_t * arr = topaz_bin_get_all(img->notifs);
    uint32_t i;
    uint32_t len = topaz_array_get_size(arr);
    for(i = 0; i < len; ++i) {
        topazImage_RemoveNotify_t * n = topaz_array_at(arr, topazImage_RemoveNotify_t *, i);
        n->cb(
            a, index, event, n->data
        );
    }
}




static void image__destroy(topazAsset_t * a, void * userData) {
    TopazImage * img = userData;
    while(topaz_image_get_frame_count(a)) {
        topaz_image_remove_frame(a, topaz_image_get_frame_count(a)-1);
    }
    topaz_array_destroy(img->frames);
    free(img);
}

static TopazImage * image__retrieve(const topazAsset_t * a) {
    TopazImage * ret = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(ret->MAGIC_ID == MAGIC_ID__IMAGE);
    #endif
    return ret;
}
 
//static int image__asset_callback_empty(topazAsset_t * asset, const void * dataIn, uint64_t size) {
//    return TRUE;
//}


static topazImage_Frame_t * topaz_image_get_frame(const topazAsset_t * a, uint32_t index) {
    TopazImage * img = image__retrieve(a);
    uint32_t imageCount = topaz_array_get_size(img->frames);
    if (index > imageCount)
        index = index%imageCount;
    return topaz_array_at(img->frames, topazImage_Frame_t *, index);

}



topazAsset_t * topaz_image_create(topaz_t * t, const topazString_t * name) {

    TopazImage * img = calloc(1, sizeof(TopazImage));
    #ifdef TOPAZDC_DEBUG
        img->MAGIC_ID = MAGIC_ID__IMAGE;
    #endif
    img->frames = topaz_array_create(sizeof(topazImage_Frame_t *));
    img->ctx = t;
    img->notifs = topaz_bin_create();
    img->proxy = FALSE;

    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = image__destroy;
    attribs.userData = img;

    return topaz_asset_create(
        t,
        topazAsset_Type_Image,
        name, 
        &attribs
    );
}


topazAsset_t * topaz_image_create_empty(topaz_t * t) {
    return topaz_image_create(
        t,
        TOPAZ_STR_CAST("")
    );
}



uint32_t topaz_image_add_frame(topazAsset_t * a) {
    TopazImage * img = image__retrieve(a);
    topazImage_Frame_t * frame = calloc(sizeof(topazImage_Frame_t), 1);
    frame->img = img;
    frame->asset = a;
    frame->object = topaz_renderer_texture_create(topaz_graphics_get_renderer(topaz_context_get_graphics(img->ctx)), img->width, img->height, NULL);
    topaz_array_push(img->frames, frame);
    return topaz_array_get_size(img->frames)-1;
}


void topaz_image_remove_frame(topazAsset_t * a, uint32_t index) {
    TopazImage * img = image__retrieve(a);
    if (index >= topaz_array_get_size(img->frames)) return;
    topazImage_Frame_t * frame = topaz_array_at(img->frames, topazImage_Frame_t *, index);
    notify_event(img, a, topazImage_TextureEvent_Removed, index);
    if (!frame->proxy)
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
        notify_event(img, a, topazImage_TextureEvent_Removed, i);
        if (!frame->proxy) {
            topaz_renderer_texture_destroy(f->object);
            f->object = topaz_renderer_texture_create(
                topaz_graphics_get_renderer(
                    topaz_context_get_graphics(f->img->ctx)
                ),
                width, height, NULL
            );
            notify_event(img, a, topazImage_TextureEvent_Changed, i);
        }
    }
}



static int topaz_image_frame_get_index(topazImage_Frame_t * f) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(f->img->frames);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(f->img->frames, topazImage_Frame_t *, i) == f) return i;
    }
    return -1;
}

void topaz_image_set_frame_from_texture(
    /// Image to modify
    topazAsset_t * image,

    /// Frame to modify.    
    uint32_t frame, 
    
    /// Texture to clone from. 
    topazRenderer_Texture_t * t
) {
    topazImage_Frame_t * f = topaz_image_get_frame(image, frame);
    int index = topaz_image_frame_get_index(f);
    notify_event(f->img, f->asset, topazImage_TextureEvent_Removed, index);
    if (!f->proxy)
        topaz_renderer_texture_destroy(f->object);

    int w = f->img->width;
    int h = f->img->height;
    uint8_t * newData = malloc(w*h*4);
    topaz_renderer_texture_get(t, newData);

    f->object = topaz_renderer_texture_create(
        topaz_graphics_get_renderer(
            topaz_context_get_graphics(f->img->ctx)
        ),
        w, h, newData
    );
    notify_event(f->img, f->asset, topazImage_TextureEvent_Changed, index);

}


void topaz_image_set_frame_from_texture_proxy(
    /// Image to modify
    topazAsset_t * image,
    
    uint32_t frame,
    
    /// Texture to clone from. 
    topazRenderer_Texture_t * t
) {
    topazImage_Frame_t * f = topaz_image_get_frame(image, frame);
    if (!f->proxy)
        topaz_renderer_texture_destroy(f->object);
    f->object = t;    
    f->proxy = TRUE;
    notify_event(f->img, f->asset, topazImage_TextureEvent_Changed, index);

}


void topaz_image_set_frame_rgba_data(
    topazAsset_t * image,
    uint32_t frame,
    const uint8_t * rgbaData

) {
    topazImage_Frame_t * f = topaz_image_get_frame(image, frame);
    topaz_renderer_texture_update(f->object, rgbaData);
}




topazRenderer_Texture_t * topaz_image_get_frame_texture(
    const topazAsset_t * image,
    uint32_t frame
) {
    topazImage_Frame_t * f = topaz_image_get_frame(image, frame);
    return f->object;
}


topazArray_t * topaz_image_get_frame_rgba_data(
    const topazAsset_t * image,
    uint32_t frame
) {
    topazImage_Frame_t * f = topaz_image_get_frame(image, frame);
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

uint32_t topaz_image_add_texture_event_notify(
    topazAsset_t * image,
    topaz_image_texture_event_callback callback,
    void * data
) {
    TopazImage * img = image__retrieve(image);
    topazImage_RemoveNotify_t * n = calloc(1, sizeof(topazImage_RemoveNotify_t));
    n->cb = callback;
    n->data = data;
    return topaz_bin_add(img->notifs, n);    
}


void topaz_image_remove_texture_event_notify(
    topazAsset_t * image,
    uint32_t callbackID   
) {
    TopazImage * img = image__retrieve(image);
    topazImage_RemoveNotify_t * n = topaz_bin_fetch(img->notifs, callbackID);
    if (!n) return;
    free(n);
    topaz_bin_remove(img->notifs, callbackID);
}




