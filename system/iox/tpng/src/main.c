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




#include "backend.h"
#include <topaz/compat.h>
#include <topaz/containers/array.h>
#include <topaz/assets/image.h>
#include <stdlib.h>
#include <string.h>
#include "tpng.h"


static void * tpng_create (
    topazIOX_t * d, 
    topaz_t * context, 
    topazArray_t * extensions,
    topazAsset_Type * atype,    
    uint64_t * recommendedBufferSize
) {
    *atype = topazAsset_Type_Image;
    topazString_t * str;

    str = topaz_string_create_from_c_str("png");  topaz_array_push(extensions, str);
    return NULL;
}

static void tpng_destroy(topazIOX_t * d, void * notUsed) {}

static void * tpng_stream_start(topazIOX_t * d, void * notUsed, topazAsset_t * asset) {
    topazArray_t * bytes = topaz_array_create(sizeof(uint8_t));
    if (topaz_asset_get_type(asset) == topazAsset_Type_Image) {
        // clear image
        while(topaz_image_get_frame_count(asset))
              topaz_image_remove_frame(asset, 0);
    }
    return bytes;
}

static int tpng_stream(
    topazIOX_t * d, 
    void * notUsed, 
    topazAsset_t * asset, 

    void * arraySrc,  
    const void * data, 
    uint64_t dataIn
) {
    topazArray_t * arr = arraySrc;
    topaz_array_push_n(arr, data, dataIn);
    return 1;
}


static void * tpng_encode(
    topazIOX_t * d, 
    void * notUsed, 
    topazAsset_t * asset, 
    uint64_t * size,
    const topazString_t * requestedExt
) {
    /*
    if (topaz_asset_get_type(asset) == topazAsset_Type_Data) {
        const topazArray_t * arr = topaz_data_get_as_bytes(asset);
        uint8_t * out = malloc(topaz_array_get_size(arr));
        memcpy(out, topaz_array_get_data(arr), topaz_array_get_size(arr));
        *size = topaz_array_get_size(arr);
        return out;
    }
    */
    return NULL;
}



static void tpng_stream_cancel(
    topazIOX_t * d, 
    void * notUsed, 
    topazAsset_t * asset, 
    void * arraySrc, 
    int fromFailed
) {
    topaz_array_destroy(arraySrc);
}

/// Called when all data for an asset has been streamed. At this point, the decoder implementation 
/// can finalize any asset data.
///
static int tpng_stream_finish(
    topazIOX_t * d, 
    void * notUsed, 
    topazAsset_t * asset, 
    void * arraySrc
) {
    if (topaz_asset_get_type(asset) == topazAsset_Type_Image) {
        uint32_t w;
        uint32_t h;
        uint8_t * rgba = tpng_get_rgba(
            topaz_array_get_data(arraySrc),
            topaz_array_get_size(arraySrc),
            &w,
            &h
        );


        // add new frame if valid.
        if (rgba && w && h) {
            topaz_image_resize(asset, w, h);
            topaz_image_add_frame(asset);
            topaz_image_set_frame_rgba_data(asset, 0, rgba);       
        }

        free(rgba);
    }
    topaz_array_destroy(arraySrc);
    return 1;
}







void topaz_system_iox_tpng__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazIOXAPI_t * api
) {
    
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Topaz PNG"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Reduced PNG decoder/encoder, using TINFL for zlib deflating"),





        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );

    // Map object API functions to real ones
    
    api->iox_create = tpng_create;
    api->iox_destroy = tpng_destroy;
    api->iox_stream_start = tpng_stream_start;
    api->iox_stream = tpng_stream;
    api->iox_stream_cancel = tpng_stream_cancel;
    api->iox_stream_finish = tpng_stream_finish;
    api->iox_encode = tpng_encode;
}















