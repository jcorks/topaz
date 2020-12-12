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
#include <topaz/containers/table.h>
#include <topaz/containers/array.h>
#include <stdlib.h>
#include <string.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif

struct topazDecoder_t {
    topazAsset_Type type;
    topazArray_t * exts;
    uint64_t recBufferSize;

    topazDecoderAPI_t api;
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

    // whether canceling was from the decoder implementation. If false, the cancel is user-requested
    int badStream;
} DecoderAssetState;


#define TOPAZ_ASSET__STREAM_THRESHOLD (1024*16)

topazDecoder_t * topaz_decoder_create(
    topaz_t * topaz, 
    topazSystem_Backend_t * b, 
    topazDecoderAPI_t api
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
    out->exts = topaz_array_create(sizeof(topazString_t *));
    out->streams = topaz_table_create_hash_pointer();
    out->recBufferSize = TOPAZ_ASSET__STREAM_THRESHOLD;
    out->type = topazAsset_Type_None;
    out->userData = out->api.decoder_create(out, topaz, out->exts, &out->type, &out->recBufferSize);
  

    return out;
} 




void topaz_decoder_destroy(topazDecoder_t * d) {
    topazTableIter_t * iter = topaz_table_iter_create();
    topazArray_t * arr = topaz_array_create(sizeof(topazAsset_t *));
    for(topaz_table_iter_start(iter, d->streams);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        const topazAsset_t * a = topaz_table_iter_get_key(iter);
        topaz_array_push(arr, a);
    }

    uint32_t i;
    uint32_t len = topaz_array_get_size(arr);
    topazAsset_t ** assets = topaz_array_get_data(arr);
    for(i = 0; i < len; ++i) {
        topaz_decoder_stream_cancel(d, assets[i]);
    }

    d->api.decoder_destroy(d, d->userData);
    
    topaz_table_destroy(d->streams);
    topaz_table_iter_destroy(iter);
    len = topaz_array_get_size(d->exts);
    for(i = 0; i < len; ++i) {
        topaz_string_destroy(topaz_array_at(d->exts, topazString_t *, i));
    }
    topaz_array_destroy(d->exts);
    free(d);
}



topazSystem_Backend_t * topaz_decoder_get_backend(topazDecoder_t * d) {
    return d->backend;
}

topazDecoderAPI_t topaz_deocder_get_api(topazDecoder_t * d) {
    return d->api;
}

const topazArray_t * topaz_decoder_get_extensions(const topazDecoder_t * d) {
    return d->exts;
}

topazAsset_Type topaz_decoder_get_type(const topazDecoder_t * d) {
    return d->type;
}


int topaz_decoder_load(topazDecoder_t * d, topazAsset_t * asset, const void * dataIn, uint64_t numBytes) {
    topaz_decoder_stream_start(d, asset);
    topaz_decoder_stream_set_threshold(d, asset, numBytes);
    if (topaz_decoder_stream(d, asset, dataIn, numBytes)) {
        topaz_decoder_stream_finish(d, asset);
        return 1;
    } 
    return 0;
}




void topaz_decoder_stream_start(topazDecoder_t * d, topazAsset_t * asset) {
    DecoderAssetState * state = topaz_table_find(d->streams, asset);
    if (state) return;

    state = calloc(1, sizeof(DecoderAssetState));
    if (!state) return;
    state->buffer = malloc(d->recBufferSize);
    state->threshold = d->recBufferSize;
    topaz_table_insert(d->streams, asset, state);
    state->userData = d->api.decoder_stream_start(d, d->userData, asset);
}

void topaz_decoder_stream_set_threshold(topazDecoder_t * d, topazAsset_t * asset, uint64_t th) {
    DecoderAssetState * state = topaz_table_find(d->streams, asset);
    if (!state) return;
    if (!th) th = TOPAZ_ASSET__STREAM_THRESHOLD;

    if (th > state->threshold)
        state->buffer = realloc(state->buffer, th);    

    state->threshold = th;
}




int topaz_decoder_stream(topazDecoder_t * d, topazAsset_t * asset, const void * data, uint64_t numBytes) {    
    DecoderAssetState * state = topaz_table_find(d->streams, asset);


    uint64_t limit = state->tsize + numBytes < state->threshold ? numBytes : state->threshold - state->tsize;
    memcpy(
        state->buffer + state->tsize,
        data,
        limit
    );
    state->tsize += limit;

    if (state->tsize >= state->threshold) {
        state->tsize = 0; // prevents bad behavior if stream callback tries to flush the buffer
        if (!d->api.decoder_stream(
            d,
            d->userData,
            asset,
            state->userData,
            state->buffer,
            state->threshold
        )) {
            state->badStream = 1;
            topaz_decoder_stream_cancel(d, asset);
            return 0;        
        }
    }   
     
    

    // process extra data 
    if (numBytes > limit) {
        numBytes -= limit;
        data += limit;        

        // skip populating stream buffer if remaining buffer would trigger a flush anyway
        while(numBytes >= state->threshold) {

            // could support: flushing and threshold changes
            numBytes -= state->threshold;
            const void * next = data + state->threshold;
            if (!d->api.decoder_stream(
                d,
                d->userData,
                asset,
                state->userData,
                data,
                state->threshold
            )) {
                state->badStream = 1;
                topaz_decoder_stream_cancel(d, asset);
                return 0;        
            }
            data = next;
        }

        // dump remaining in buffer
        if (numBytes) {
            memcpy(state->buffer, data, numBytes);
            state->tsize = numBytes;
        }
    }
    return 1;
}


int topaz_decoder_stream_flush(topazDecoder_t * d, topazAsset_t * asset) {
    DecoderAssetState * state = topaz_table_find(d->streams, asset);
    if (!state) return 0;
    if (!state->tsize) return 0;
    
    int res = d->api.decoder_stream(
        d,
        d->userData,
        asset,
        state->userData,
        state->buffer,
        state->tsize
    );
    state->tsize = 0;

    if (!res) {
        state->badStream = 1;
        topaz_decoder_stream_cancel(d, asset);
    }   
    return res;
}



static void decoder_state_cleanup(
    topazTable_t * streams,
    topazAsset_t * key,
    DecoderAssetState * data
) {
    free(data->buffer);
    free(data);
    topaz_table_remove(streams, key);
}


void topaz_decoder_stream_finish(topazDecoder_t * d, topazAsset_t * asset) {
    DecoderAssetState * state = topaz_table_find(d->streams, asset);
    if (!state) return;

    topaz_decoder_stream_flush(d, asset);

    d->api.decoder_stream_finish(
        d,
        d->userData,
        asset, 
        state->userData
    );

    decoder_state_cleanup(d->streams, asset, state);
}

void topaz_decoder_stream_cancel(topazDecoder_t * d, topazAsset_t * asset) {
    DecoderAssetState * state = topaz_table_find(d->streams, asset);
    if (!state) return;

    d->api.decoder_stream_cancel(
        d,
        d->userData,
        asset, 
        state->userData,
        state->badStream
    );

    decoder_state_cleanup(d->streams, asset, state);    
}

int topaz_decoder_is_streaming(topazDecoder_t * d, topazAsset_t * asset) {
    return topaz_table_find(d->streams, asset) != NULL;
}






