#include <topaz/backends/font_renderer.h>
#include <topaz/containers/string.h>
#include <topaz/containers/table.h>
#include <topaz/assets/image.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef struct {
    topazAsset_t * image;
    int refCount;
} GlyphReference;


struct topazFontRenderer_t {
    topazFontRendererAPI_t api;
    void * implementationData;
    topazTable_t * refs; // [size] -> topazTable_t * [char] -> GlyphReference*
    topaz_t * ctx;  
    topazSystem_Backend_t * backend;  
};



topazFontRenderer_t * topaz_font_renderer_create(
    topaz_t * t, 
    topazSystem_Backend_t * backend, 
    topazFontRendererAPI_t api
) {
    topazFontRenderer_t * out = calloc(1, sizeof(topazFontRenderer_t));
    out->api = api;
    out->backend = backend;
    out->refs = topaz_table_create_hash_pointer();
    out->ctx = t;
    #ifdef TOPAZDC_DEBUG
        assert(out->api.font_renderer_create);
        assert(out->api.font_renderer_destroy);
        assert(out->api.font_renderer_query_spacing);
        assert(out->api.font_renderer_render);
    #endif

    out->implementationData = out->api.font_renderer_create(out, t);
    return out;
}


void topaz_font_renderer_destroy(topazFontRenderer_t * r) {
    r->api.font_renderer_destroy(r, r->implementationData);
    topazTableIter_t * iter = topaz_table_iter_create();
    topazTableIter_t * subIter = topaz_table_iter_create();

    for(topaz_table_iter_start(iter, r->refs);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {

        topazTable_t * v = topaz_table_iter_get_value(iter);      

        for(topaz_table_iter_start(subIter, v);
            !topaz_table_iter_is_end(subIter);
            topaz_table_iter_proceed(subIter)) {

            GlyphReference * g = topaz_table_iter_get_value(subIter);
            topaz_asset_destroy(g->image);
            free(g);
        }

        topaz_table_destroy(v);
    }    
    
    topaz_table_iter_destroy(iter);
    topaz_table_iter_destroy(subIter);

    topaz_table_destroy(r->refs);
    free(r);
}










topazSystem_Backend_t * topaz_font_renderer_get_backend(topazFontRenderer_t * r) {
    return r->backend;
}


topazFontRendererAPI_t topaz_font_renderer_get_api(topazFontRenderer_t * r) {
    return r->api;
}



const topazAsset_t * topaz_font_renderer_image_ref(
    topazFontRenderer_t * r,
    int charcode,
    int sizePixels
) {
    topazTable_t * charTable = topaz_table_find_by_int(r->refs, sizePixels);
    if (!charTable) {
        charTable = topaz_table_create_hash_pointer();
        topaz_table_insert_by_int(r->refs, sizePixels, charTable);
    }

    GlyphReference * g = topaz_table_find_by_int(charTable, charcode);
    if (!g) {
        uint32_t w, h;
        uint8_t * data = r->api.font_renderer_render(
            r,
            r->implementationData,
            charcode,
            sizePixels,
            &w, &h
        );

        g = calloc(1, sizeof(GlyphReference));
        g->image = topaz_image_create_empty(r->ctx);
        if (data) {
            topaz_image_resize(g->image, w, h);
            topazImage_Frame_t * frame = topaz_image_add_frame(g->image);
            topaz_image_frame_set_data(frame, data);
            free(data);
        }
        g->refCount++;

        topaz_table_insert_by_int(charTable, charcode, g);
    } else {
        g->refCount++;
    }

    
    return g->image;
}

void topaz_font_renderer_image_unref(
    topazFontRenderer_t * r,    
    int charcode,
    int pixelSize
) {
    topazTable_t * charTable = topaz_table_find_by_int(r->refs, pixelSize);
    if (!charTable) return;

    GlyphReference * g = topaz_table_find_by_int(r->refs, charcode);
    if (!g) return;

    g->refCount--;
    if (g->refCount <= 0) {
        topaz_asset_destroy(g->image);
        free(g);
        topaz_table_remove_by_int(charTable, charcode);
    }
}




void topaz_font_renderer_query_spacing(
    topazFontRenderer_t * r,
    topazFontRenderer_Spacing_t * space,

    int sizePixels, 
    int prevchar,
    int thischar,
    int nextchar
) {
    r->api.font_renderer_query_spacing(
        r,
        r->implementationData,
        space,
        sizePixels,
        prevchar,
        thischar,
        nextchar
    );
}


