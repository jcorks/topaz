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
#include <topaz/assets/sound.h>
#include <stdlib.h>
#include <string.h>
#include <vorbis/vorbisfile.h>


static void * ogg_create (
    topazIOX_t * d, 
    topaz_t * context, 
    topazArray_t * extensions,
    topazAsset_Type * atype,    
    uint64_t * recommendedBufferSize
) {
    *atype = topazAsset_Type_Sound;
    topazString_t * str;

    str = topaz_string_create_from_c_str("ogg");  topaz_array_push(extensions, str);
    return NULL;
}

static void ogg_destroy(topazIOX_t * d, void * notUsed) {}

static void * ogg_stream_start(topazIOX_t * d, void * notUsed, topazAsset_t * asset) {
    topazArray_t * bytes = topaz_array_create(sizeof(uint8_t));
    if (topaz_asset_get_type(asset) == topazAsset_Type_Sound) {
        // clear image
        topaz_sound_set_samples(asset, NULL, 0);
    }
    return bytes;
}

static int ogg_stream(
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


static void * ogg_encode(
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



static void ogg_stream_cancel(
    topazIOX_t * d, 
    void * notUsed, 
    topazAsset_t * asset, 
    void * arraySrc, 
    int fromFailed
) {
    topaz_array_destroy(arraySrc);
}



typedef struct {
    const uint8_t * buffer;
    uint64_t size;
    uint64_t position;
} OVDataSource;



static size_t ov_memory_read_func(void * ptr, size_t size, size_t nmemb, void * datasource) {
    OVDataSource * data = (OVDataSource*)(datasource);
    uint64_t bytesLeft = data->size - data->position;
    size_t   numRead = (bytesLeft < size*nmemb ? bytesLeft : size*nmemb);
    memcpy(ptr, data->buffer+data->position, numRead);
    data->position += numRead;
    return numRead;
}



#define TOPAZOGGBUFFERSIZE 4096
static int ogg_stream_finish(
    topazIOX_t * d, 
    void * notUsed, 
    topazAsset_t * asset, 
    void * arraySrc
) {
    if (topaz_asset_get_type(asset) == topazAsset_Type_Sound) {
        // Uses vorbisfile library to decode
        OggVorbis_File oggFile;
        
        OVDataSource datasource = {
            topaz_array_get_data(arraySrc),
            topaz_array_get_size(arraySrc),
            0
        };
        
        ov_callbacks cbs = {
            ov_memory_read_func,
            NULL,
            NULL,
            NULL
        };

        int status = ov_open_callbacks(
            &datasource,
            &oggFile,
            NULL,
            0,
            cbs
        );

        if (status != 0) {
            return 0;
        }





        // now read
        int section = 0;
        uint8_t * block = malloc(TOPAZOGGBUFFERSIZE);     
        
        topazArray_t * samples = topaz_array_create(sizeof(uint8_t));
               
        while(1) {
            int size = ov_read(
                &oggFile,
                block,
                TOPAZOGGBUFFERSIZE,
                0, 2, 0, &section // matches sound format.
            );
            if (!size) {
                break;
            }
            topaz_array_push_n(
                samples,
                block,
                size
            );
        }

        topaz_sound_set_samples(
            asset,
            topaz_array_get_data(samples),
            topaz_array_get_size(samples)/sizeof(topazSound_Sample_t)
        );
        free(block);
        topaz_array_destroy(samples);
    }
    topaz_array_destroy(arraySrc);
    return 1;
}







void topaz_system_iox_ogg__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazIOXAPI_t * api
) {
    
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Topaz OGG"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Simple libvorbis/ogg decoder."),





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
    
    api->iox_create = ogg_create;
    api->iox_destroy = ogg_destroy;
    api->iox_stream_start = ogg_stream_start;
    api->iox_stream = ogg_stream;
    api->iox_stream_cancel = ogg_stream_cancel;
    api->iox_stream_finish = ogg_stream_finish;
    api->iox_encode = ogg_encode;
}















