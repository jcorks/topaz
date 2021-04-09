#include <topaz/compat.h>
#include <topaz/backends/filesystem.h>
#include <topaz/backends/iox.h>
#include <topaz/modules/resources.h>
#include <topaz/containers/array.h>
#include <topaz/containers/table.h>
#include <topaz/topaz.h>
#include <topaz/rbuffer.h>
#include <topaz/wbuffer.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <topaz/assets/image.h>
#include <topaz/assets/data.h>
#include <topaz/assets/sound.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



struct topazResources_t {   
    topaz_t * ctx;
    topazTable_t * name2asset;

    topazTable_t * ioxs; //extension -> iox
    const topazFilesystem_Path_t * path;
};




topazResources_t * topaz_resources_create(topaz_t * ctx) {
    topazResources_t * out = calloc(1, sizeof(topazResources_t));
    out->ctx = ctx;
    out->name2asset = topaz_table_create_hash_topaz_string();

    // create all ioxs 
    out->ioxs = topaz_table_create_hash_topaz_string();
    topazArray_t * ioxNames = topaz_system_get_available_backends(TOPAZ_STR_CAST("iox"));
    uint32_t i;
    uint32_t len = topaz_array_get_size(ioxNames);
    for(i = 0; i < len; ++i) {
        topazString_t * name = topaz_array_at(ioxNames, topazString_t *, i);
        topaz_resources_add_translator(out, name);
    }
    topaz_array_destroy(ioxNames);


    topazFilesystem_t * fs = topaz_context_get_filesystem(ctx);
    out->path = topaz_filesystem_get_path(fs, topazFilesystem_DefaultNode_Resources);    
    return out;
}

void topaz_resources_destroy(topazResources_t * r) {
    topazTableIter_t * iter = topaz_table_iter_create();

    for(topaz_table_iter_start(iter, r->ioxs);
       !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        topaz_iox_destroy(topaz_table_iter_get_value(iter));
    }
    topaz_table_destroy(r->ioxs);

    
    for(topaz_table_iter_start(iter, r->name2asset);
       !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        topaz_asset_destroy(topaz_table_iter_get_value(iter));
    }
    topaz_table_destroy(r->name2asset);
    free(r);

}

int topaz_resources_set_path(topazResources_t * r, const topazString_t * path) {
    topazFilesystem_t * fs = topaz_context_get_filesystem(r->ctx);
    const topazFilesystem_Path_t * p = topaz_filesystem_get_path_from_string(fs, NULL, path);
    if (p) r->path = p;
    return p!=NULL;
}

const topazString_t * topaz_resources_get_path(const topazResources_t * r) {
    return topaz_filesystem_path_as_string(r->path);
}


topazAsset_t * topaz_resources_load_asset(
    topazResources_t * r,
    const topazString_t * extension,
    const topazString_t * path,
    const topazString_t * name
) {

    topazAsset_t * asset = topaz_table_find(
        r->name2asset,
        name
    );

    
    // already loaded! return
    if (asset) return asset;


    // invalid extension if not created, return NULL'
    topazIOX_t * dec = topaz_table_find(r->ioxs, extension);
    if (!dec) return NULL;

    // Create a new asset of the type
    asset = topaz_resources_fetch_asset(r, topaz_iox_get_asset_type(dec), name);

    // could fail if the type is unsupported. Shouldnt happen though.
    if (!asset) return NULL;

    // at this point, we want a data buffer 
    topazFilesystem_t * fs = topaz_context_get_filesystem(r->ctx);
    const topazFilesystem_Path_t * p = topaz_filesystem_get_path_from_string(fs, r->path, path);
    if (!p) {
        p = topaz_filesystem_get_path_from_string(fs, NULL, path);
    }

    
    topazRbuffer_t * data = NULL;
    if (p) {
        data = topaz_filesystem_path_read(p);
    }

    // check to see if read failed.
    if (!data) {
        topaz_asset_destroy(asset);
        topaz_table_remove(r->name2asset, name);
        return NULL;
    } 


    // load in data into asset
    if (!topaz_iox_load(
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


topazAsset_t * topaz_resources_load_asset_base64(
    topazResources_t * r,
    const topazString_t * extension,
    const topazString_t * data,
    const topazString_t * name
) {
    uint32_t size = 0;
    uint8_t * buffer = topaz_string_base64_to_bytes(data, &size);
    if (!buffer) {
        return NULL;
    }
    topazAsset_t * out = topaz_resources_load_asset_data(
        r, 
        extension, 
        TOPAZ_ARRAY_CAST(buffer, uint8_t, size), 
        name
    );
    return out;
}


topazAsset_t * topaz_resources_load_asset_data(
    topazResources_t * r,
    const topazString_t * extension,
    const topazArray_t * data,
    const topazString_t * name
) {

    topazAsset_t * asset = topaz_table_find(
        r->name2asset,
        name
    );

    
    // already loaded! return
    if (asset) return asset;


    // invalid extension if not created, return NULL'
    topazIOX_t * dec = topaz_table_find(r->ioxs, extension);
    if (!dec) return NULL;

    // Create a new asset of the type
    asset = topaz_resources_fetch_asset(r, topaz_iox_get_asset_type(dec), name);

    // could fail if the type is unsupported. Shouldnt happen though.
    if (!asset) return NULL;

    // load in data into asset
    if (!topaz_iox_load(
        dec,
        asset,
        topaz_array_get_data(data),
        topaz_array_get_size(data)
    )) {
        topaz_asset_destroy(asset);
        topaz_table_remove(r->name2asset, name);
        return NULL;
    }

    return asset;
}


int topaz_resources_write_asset(
    topazResources_t * r,
    topazAsset_t * asset,
    const topazString_t * extension,
    const topazString_t * name
) {

    // invalid extension if not created, return NULL'
    topazIOX_t * dec = topaz_table_find(r->ioxs, extension);
    if (!dec) return 0;




    uint64_t rawdataSize = 0;
    void * rawdata = topaz_iox_encode(
        dec,
        asset,
        &rawdataSize,
        extension
    );

    if (!rawdataSize) {
        return 0;
    }


    topazWbuffer_t * data = topaz_wbuffer_create();
    topaz_wbuffer_write_buffer(data, rawdata, rawdataSize);
    

    int success = topaz_filesystem_path_write(
        r->path, 
        name,
        data 
    );


    topaz_wbuffer_destroy(data);
    return success;
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
        
      case topazAsset_Type_Sound:
        asset = topaz_sound_create(r->ctx, name);
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
    return topaz_table_find(r->ioxs, ext) != NULL;
}

void topaz_resources_add_translator(
    topazResources_t * r,
    const topazString_t * name
) {

    topazSystem_t * system = topaz_context_get_system(r->ctx);
    topaz_system_set_backend_handler(
        system,
        TOPAZ_STR_CAST("iox"),
        name 
    );

    topazIOXAPI_t api;
    topazSystem_Backend_t * backend = topaz_system_create_backend(
        system,
        TOPAZ_STR_CAST("iox"),
        &api
    );


    topazIOX_t * dec = topaz_iox_create(
        r->ctx,
        backend,
        api
    );


    uint32_t i;
    uint32_t          len = topaz_array_get_size(topaz_iox_get_extensions(dec));
    topazString_t ** exts = topaz_array_get_data(topaz_iox_get_extensions(dec));
    for(i = 0; i < len; ++i) {
        topaz_table_insert(
            r->ioxs,
            exts[i],
            dec 
        );
    }
}


