#include <topaz/compat.h>
#include <topaz/backends/filesys.h>
#include <topaz/backends/decoder.h>
#include <topaz/modules/resources.h>
#include <topaz/containers/array.h>
#include <topaz/containers/table.h>
#include <topaz/topaz.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <topaz/assets/image.h>
#include <topaz/assets/data.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



struct topazResources_t {   
    topaz_t * ctx;
    topazFilesys_t * fs;
    topazTable_t * name2asset;

    topazTable_t * decoders; //extension -> decoder
};




topazResources_t * topaz_resources_create(topaz_t * ctx) {
    topazResources_t * out = calloc(1, sizeof(topazResources_t));
    out->ctx = ctx;
    out->fs = topaz_context_filesys_create(ctx);
    out->name2asset = topaz_table_create_hash_topaz_string();

    // create all decoders 
    out->decoders = topaz_table_create_hash_topaz_string();
    topazArray_t * decoderNames = topaz_system_get_available_backends(TOPAZ_STR_CAST("decoder"));
    uint32_t i;
    uint32_t len = topaz_array_get_size(decoderNames);
    for(i = 0; i < len; ++i) {
        topazString_t * name = topaz_array_at(decoderNames, topazString_t *, i);
        topaz_resources_add_decoder(out, name);
    }
    topaz_array_destroy(decoderNames);
    
    return out;
}

void topaz_resources_destroy(topazResources_t * r) {
    topazTableIter_t * iter = topaz_table_iter_create();

    for(topaz_table_iter_start(iter, r->decoders);
       !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        topaz_decoder_destroy(topaz_table_iter_get_value(iter));
    }
    topaz_table_destroy(r->decoders);

    
    for(topaz_table_iter_start(iter, r->name2asset);
       !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        topaz_asset_destroy(topaz_table_iter_get_value(iter));
    }
    topaz_table_destroy(r->name2asset);
    topaz_filesys_destroy(r->fs);
    free(r);

}

int topaz_resources_set_path(topazResources_t * r, const topazString_t * path) {
    return topaz_filesys_set_path(r->fs, path);
}



topazAsset_t * topaz_resources_load_asset(
    topazResources_t * r,
    const topazString_t * extension,
    const topazString_t * name
) {

    topazAsset_t * asset = topaz_table_find(
        r->name2asset,
        name
    );

    
    // already loaded! return
    if (asset) return asset;


    // invalid extension if not created, return NULL'
    topazDecoder_t * dec = topaz_table_find(r->decoders, extension);
    if (!dec) return NULL;

    // Create a new asset of the type
    asset = topaz_resources_fetch_asset(r, topaz_decoder_get_type(dec), name);

    // could fail if the type is unsupported. Shouldnt happen though.
    if (!asset) return NULL;

    // at this point, we want a data buffer 
    const topazArray_t * pathTokens = topaz_filesys_split_path(r->fs, name);
    uint32_t len = topaz_array_get_size(pathTokens);

    // not a valid path/name given
    if (!len) {
        topaz_asset_destroy(asset);
        topaz_table_remove(r->name2asset, name);
        return NULL;
    }

    topazRbuffer_t * data = NULL;

    // walk through directories
    topazString_t * originalDir = topaz_string_clone(topaz_filesys_get_path(r->fs));
    int i;
    for(i = 0; i < len-1; ++i) {
        if (!topaz_filesys_go_to_child(r->fs, topaz_array_at(pathTokens, topazString_t *, i))) {
            topaz_filesys_set_path(r->fs, originalDir);
            topaz_string_destroy(originalDir);
            topaz_asset_destroy(asset);
            topaz_table_remove(r->name2asset, name);
            return NULL;
        }
    }

    data = topaz_filesys_read(r->fs, topaz_array_at(pathTokens, topazString_t *, len-1));
    topaz_filesys_set_path(r->fs, originalDir);
    topaz_string_destroy(originalDir);

    // check to see if read failed.
    if (topaz_rbuffer_is_empty(data)) {
        topaz_rbuffer_destroy(data);
        topaz_asset_destroy(asset);
        topaz_table_remove(r->name2asset, name);
        return NULL;
    } 


    // load in data into asset
    if (!topaz_decoder_load(
        dec,
        asset,
        topaz_rbuffer_get_buffer(data, topaz_rbuffer_get_size(data)),
        topaz_rbuffer_get_size(data)
    )) {
        topaz_rbuffer_destroy(data);
        topaz_asset_destroy(asset);
        topaz_table_remove(r->name2asset, name);
        return NULL;
    }
    topaz_rbuffer_destroy(data);
    return asset;
}


topazAsset_t * topaz_resources_fetch_asset(
    topazResources_t * r,
    topazAsset_Type type,
    const topazString_t * name
) {
    topazAsset_t * asset = topaz_table_find(
        r->name2asset,
        name
    );
    if (asset) return asset;
    
    switch(type) {
      case topazAsset_Type_Image:
        asset = topaz_image_create(r->ctx, name);
        break;

      case topazAsset_Type_Data:
        asset = topaz_data_create(r->ctx, name);
        break;

      default:
        #ifdef TOPAZDC_DEBUG
            printf("Unsupported resource type!!\n");
        #endif
        return NULL;
        

    }

    topaz_table_insert(
        r->name2asset,
        name,
        asset
    );
    return asset;
}




void topaz_resources_remove_asset(topazResources_t * r, const topazString_t * name) {
    topazAsset_t * asset = topaz_table_find(
        r->name2asset,
        name
    );
    if (asset) {
        topaz_asset_destroy(asset);
        topaz_table_remove(r->name2asset, name);
    }
}



int topaz_resources_is_extension_supported(const topazResources_t * r, const topazString_t * ext) {
    return topaz_table_find(r->decoders, ext) != NULL;
}

void topaz_resources_add_decoder(
    topazResources_t * r,
    const topazString_t * name
) {

    topazSystem_t * system = topaz_context_get_system(r->ctx);
    topaz_system_set_backend_handler(
        system,
        TOPAZ_STR_CAST("decoder"),
        name 
    );

    topazDecoderAPI_t api;
    topazSystem_Backend_t * backend = topaz_system_create_backend(
        system,
        TOPAZ_STR_CAST("decoder"),
        &api
    );


    topazDecoder_t * dec = topaz_decoder_create(
        r->ctx,
        backend,
        api
    );


    uint32_t i;
    uint32_t          len = topaz_array_get_size(topaz_decoder_get_extensions(dec));
    topazString_t ** exts = topaz_array_get_data(topaz_decoder_get_extensions(dec));
    for(i = 0; i < len; ++i) {
        topaz_table_insert(
            r->decoders,
            exts[i],
            dec 
        );
    }
}
