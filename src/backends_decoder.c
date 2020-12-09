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

#include <topaz/backends/decoder.h>
#include <topaz/containers/string.h>
struct topazDecoder_t {
    topazAsset_Type type;
    topazString_t * ext;
    uint64_t recBufferSize;

    topazConsoleDisplayAPI_t api;
    topazSystem_Backend_t * backend;
    void * userData;    
    

    topazTable_t * streams;
};


// keyed by asset
typedef struct {
    // instance data from api implementation
    void * userData;

    // alloc size of stream buffer
    uint64_t threshold;

    // current size of buffer
    uint64_t tsize;

    // buffer for stream
    uint8_t * buffer;
} DecoderAssetState;


topazDecoder_t * topaz_decoder_create(
    topaz_t * topaz, 
    topazSystem_Backend_t * b, 
    topazConsoleDisplayAPI_t api
) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(api.decoder_create);
        assert(api.decoder_destroy);
        assert(api.decoder_stream_start);
        assert(api.decoder_stream);
        assert(api.decoder_stream_cancel);
        assert(api.decoder_stream_finish);
    #endif
    topazDecoder_t * out = calloc(1, sizeof(topazDecoder_t));
    out->api = api;
    out->backend = b;
    out->ext = topaz_string_create();
    out->streams = topaz_table_create_hash_pointer();
    out->recBufferSize = TOPAZ_ASSET__STREAM_THRESHOLD;
    out->type = topazAsset_Type_None;
    out->userData = out->api.decoder_create(out, topaz, out->ext, &out->type, &out->recBufferSize);
  

    return out;
}




void topaz_decoder_destroy(topazDecoder_t * d) {

}



topazSystem_Backend_t * topaz_decoder_get_backend(topazDecoder_t * d) {
    return d->backend;
}

topazDecoderAPI_t topaz_deocder_get_api(topazDecoder_t * d) {
    return d->api;
}

const topazString_t * topaz_decoder_get_extension(const topazDecoder_t * d) {
    return d->name;
}

topazAsset_Type_t topaz_decoder_get_type(const topazDecoder_t * d) {
    return d->type;
}

void topaz_decoder_stream_set_threshold(topazDecoder_t * d, uint64_t amt) {
    d->threshold = amt;
}

void topaz_decoder_stream_start(topazDecoder_t * d, topazAsset_t * asset) {
    DecoderAssetState * state = calloc(1, sizeof(DecoderAssetState));
    state->buffer = malloc(d->recBufferSize);
    state->threshold = d->recBufferSize;
    topaz_table_insert(d->streams, asset, state);
    state->userData = d->api.decoder_stream_start(d, d->userData, asset);
}

void topaz_decoder_stream_set_threshold(topazDecoder_t * d, topazAsset_t * asset, uint64_t th) {
    DecoderAssetState * state = topaz_table_find(d->streams, asset);
    state->threshold = th;
    state->buffer = realloc(state->buffer);
}


static void stream_round(DecoderAssetState * a, const void * data, uint64_t * numBytes) {
    int left = a->threshold - a->tsize;
    uint64_t count = left < *numBytes ? left : *numBytes;
    memcpy(a->buffer, data, count);

    *numBytes -= count;

    if (a->tsize == a->threshold) {
        topaz_decoder_stream_flush(a);
    }
}

void topaz_decoder_stream(topazAsset_t * a, const void * data, uint64_t numBytes) {
    if (!a->stream) return;

    topaz_array_push_n(
        a->stream,
        data,
        numBytes
    );


    while(numBytes) {
        stream_round(a, data, &numBytes);
    }

}

void topaz_asset_stream_flush(topazAsset_t * a) {
    if (!(a->stream && a->streamSize)) return;
    if (a->loading.on_stream) {
        a->loading.on_stream(
            a,
            a->streamBuffer,
            a->streamSize
        );
    }
    a->streamSize = 0;
}


void topaz_asset_stream_end(topazAsset_t * a) {
    if (!a->stream) return;
    topaz_asset_stream_flush(a);


    // trigger on_load 
    a->isLoaded = a->loading.on_load(
        a, 
        topaz_array_get_data(a->stream),
        topaz_array_get_size(a->stream)
    );
    

    // cleanup
    topaz_asset_stream_cancel(a);
}

void topaz_asset_stream_cancel(topazAsset_t * a) {
    if (!a->stream) return;

    topaz_array_destroy(a->stream);
    free(a->streamBuffer);
    a->streamSize = 0;
    a->stream = NULL;
    a->streamBuffer = NULL;    
    if (a->loading.on_stream_cancel) {
        a->loading.on_stream_cancel(
            a, NULL, 0
        );
    }
}






int topaz_asset_load(
    topazAsset_t * a, 
    const void * data,
    uint64_t dataSize
) {
    a->isLoaded = a->loading.on_load(a, data, dataSize);
    return a->isLoaded;
}

void topaz_asset_stream_start(topazAsset_t * a) {
    // stream array is the flag for stream mode;
    if (a->stream) return;

    a->stream = topaz_array_create(sizeof(uint8_t));
    a->streamBuffer = malloc(TOPAZ_ASSET__STREAM_THRESHOLD);
    a->streamSize = 0;
}
