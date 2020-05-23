#include <topaz/compat.h>
#include <topaz/asset.h>
#include <topaz/containers/array.h>
#include <stdlib.h>
#include <string.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

#define TOPAZ_ASSET__STREAM_THRESHOLD (1024*16)

struct topazAsset_t {
    topaz_t * ctx;
    topazAsset_Attributes_t attribs;
    topazAsset_LoadingProfile_t loading;
    topazString_t * name;
    topazAsset_Type type;


    topazArray_t * stream;
    uint8_t * streamBuffer;
    uint32_t streamSize;
    int streamThreshold;
    int isLoaded;

    void * userdata;
};




topazAsset_t * topaz_asset_create(
    topaz_t * ctx, 
    topazAsset_Type type,
    const topazString_t * name,
    const topazAsset_Attributes_t * attribs,
    const topazAsset_LoadingProfile_t * loading
) {
    topazAsset_t * out = calloc(1, sizeof(topazAsset_t));
    out->attribs = *attribs;
    out->loading = *loading;
    out->type = type;
    out->name = topaz_string_clone(name);
    out->ctx = ctx;
    out->streamThreshold = TOPAZ_ASSET__STREAM_THRESHOLD;

    #ifdef TOPAZDC_DEBUG
        assert(attribs->on_create);
        assert(attribs->on_destroy);

        assert(loading->on_load);
        assert(loading->on_unload);

    #endif


    out->attribs.on_create(out, attribs->userData);
    return out;
}

void topaz_asset_destroy(topazAsset_t * a) {
    a->attribs.on_destroy(a, a->attribs.userData);

    topaz_asset_stream_cancel(a);
    topaz_string_destroy(a->name);
    
    free(a);   
}

const topazAsset_Attributes_t * topaz_asset_get_attributes(const topazAsset_t * a) {
    return &a->attribs;
}

const topazAsset_LoadingProfile_t * topaz_asset_get_loading_profile(const topazAsset_t * a) {
    return &a->loading;
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

static void stream_round(topazAsset_t * a, const void * data, uint64_t * numBytes) {
    int left = TOPAZ_ASSET__STREAM_THRESHOLD - a->streamSize;
    uint64_t count = left < *numBytes ? left : *numBytes;
    memcpy(a->streamBuffer, data, count);

    *numBytes -= count;

    if (a->streamSize == TOPAZ_ASSET__STREAM_THRESHOLD) {
        topaz_asset_stream_flush(a);
    }
}

void topaz_asset_stream(topazAsset_t * a, const void * data, uint64_t numBytes) {
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

int topaz_asset_stream_active(const topazAsset_t * a) {
    return a->stream != NULL;
}

int topaz_asset_is_loaded(const topazAsset_t * a) {
    return a->isLoaded;
}
const topazString_t * topaz_asset_get_name(const topazAsset_t * a) {
    return a->name;
}

topazAsset_Type topaz_asset_get_type(const topazAsset_t * a) {
    return a->type;
}