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
#include <topaz/assets/material.h>
#include <topaz/assets/mesh.h>
#include <topaz/assets/bundle.h>


#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

#include "modules_resources__bundle"


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




topazAsset_t * topaz_resources_create_asset(
    topazResources_t * r,
    const topazString_t * name,
    topazAsset_Type type
) {
    int custname = 0;
    if (!name) {
        custname = 1;
        static int ref = 0;
        name = topaz_string_create_from_c_str(
            "$TOPAZASSET_%d", ref++
        );
        // failsafe
        while (topaz_table_find(r->name2asset, name)) {
            topaz_string_concat_printf((topazString_t*)name, "%d", rand());
        }
    } else {
        if (topaz_table_find(
            r->name2asset,
            name
        )) return NULL;
    }

    topazAsset_t * asset;
    switch(type) {
      case topazAsset_Type_Data:  asset    = topaz_data_create(r->ctx, name);  break;
      case topazAsset_Type_Image: asset    = topaz_image_create(r->ctx, name); break;
      case topazAsset_Type_Sound: asset    = topaz_sound_create(r->ctx, name); break;
      case topazAsset_Type_Material: asset = topaz_material_create(r->ctx, name); break;
      case topazAsset_Type_Mesh:  asset    = topaz_mesh_create(r->ctx, name); break;
      default: {
        if (custname) topaz_string_destroy((topazString_t *)name);
        return NULL;
      }
    }

    topaz_table_insert(
        r->name2asset,
        name,
        asset
    );
    if (custname) topaz_string_destroy((topazString_t *)name);
    return asset;
}

topazAsset_t * topaz_resources_fetch_asset(
    topazResources_t * r,
    const topazString_t * name
) {
    return topaz_table_find(
        r->name2asset,
        name
    );
}

topazAsset_t * topaz_resources_create_data_asset_from_path(
    topazResources_t * r,
    const topazString_t * path,
    const topazString_t * name
) {
    topazAsset_t * a = topaz_resources_create_asset(r, name, topazAsset_Type_Data);
    if (!a) {
        return NULL;
    }

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
        topaz_asset_destroy(a);
        topaz_table_remove(r->name2asset, name);
        return NULL;
    } 

    // populate data asset
    topaz_data_set_from_bytes(
        a,
        topaz_rbuffer_read_bytes(
            data,
            topaz_rbuffer_get_size(data)
        )
    );

    topaz_rbuffer_destroy(data);
    return a;
}

//// PRIVATE:

/// Unpacks all the items in the bundle. For 
/// each item, a new asset will be created within 
/// resources. Each asset will be prefixed with 
/// this bundle asset's name followed by "."
/// and the name of the sub-asset.
/// For example, a bundle with the name 
/// "Package" that contains an asset named 
/// "Item" could be fetched under the name "Package.Item"
/// Any assets that are, themselves, bundled will also be 
/// unpacked.
int topaz_bundle_unpack_all(
    topazAsset_t * bundle
);

/// Unpacks and loads the next nItems assets within 
/// the bundle. If nItems is greater than the number of 
/// unpacked assets, the remaining assets are unpacked.
/// Returns 1 if more items are in the bundle and 0 otherwise.
int topaz_bundle_unpack_continue(
    topazAsset_t * bundle,
    int nItems
);


int topaz_resources_unpack_bundle(
    topazResources_t * res,
    const topazString_t * bundleName, 
    int min_minorVersionRequired,
    int min_majorVersionRequired,
    void * userdata,
    void (*onNewItem)(topazResources_t * res, void * userData)
) {
    topazAsset_t * src = topaz_resources_fetch_asset(res, bundleName);
    topazArray_t * srcBytes = topaz_data_get_as_bytes(src);
    topazAsset_t * bundle = topaz_bundle_create_from_data(
        res->ctx,
        topaz_array_get_data(srcBytes),
        topaz_array_get_size(srcBytes)
    );

    uint32_t i;
    uint32_t len = topaz_bundle_get_depends_count(bundle);

    // first, unpack
    for(i = 0; i < len; ++i) {

    }

    const topazArray_t * srcData = topaz_data_get_as_bytes(srcAsset);
    topazAsset_t * bundle = topaz_bundle_create_from_data(
        r->ctx, 
        topaz_array_get_data(srcData),
        topaz_array_get_size(srcData)
    );
    if (!bundle) return NULL;
    topaz_table_insert(
        r->name2asset,
        topaz_asset_get_name(srcAsset),
        NULL
    );
    
    topaz_asset_destroy(srcAsset);
    topaz_table_insert(
        r->name2asset,
        topaz_asset_get_name(bundle),
        bundle
    );
    
    return bundle;
}



topazAsset_t * topaz_resources_convert_asset(
    topazResources_t * r,
    const topazString_t * extension,
    topazAsset_t * srcAsset
) {
    if (topaz_asset_get_type(srcAsset) != topazAsset_Type_Data) {
        return NULL;
    }

    // invalid extension if not created, return NULL
    topazIOX_t * dec = topaz_table_find(r->ioxs, extension);
    if (!dec) return NULL;
    const topazString_t * name = topaz_asset_get_name(srcAsset);
    // Create a new asset based on which kind it is.
    topazAsset_t * asset;
    switch(topaz_iox_get_asset_type(dec)) {
      case topazAsset_Type_Data:  asset    = topaz_data_create(r->ctx, name);  break;
      case topazAsset_Type_Image: asset    = topaz_image_create(r->ctx, name); break;
      case topazAsset_Type_Sound: asset    = topaz_sound_create(r->ctx, name); break;
      case topazAsset_Type_Material: asset = topaz_material_create(r->ctx, name); break;
      case topazAsset_Type_Mesh:  asset    = topaz_mesh_create(r->ctx, name); break;
      case topazAsset_Type_Bundle: asset   = topaz_bunclde_create(r->ctx, name); break;
      default:
        return NULL;
    }

    const topazArray_t * srcData = topaz_data_get_as_bytes(srcAsset);

    // load in data into asset
    if (!topaz_iox_load(
        dec,
        asset,
        topaz_array_get_data(srcData),
        topaz_array_get_size(srcData)
    )) {
        topaz_asset_destroy(asset);
        return NULL;
    }

    topaz_table_insert(
        r->name2asset,
        name,
        asset
    );
    topaz_asset_destroy(srcAsset);
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







void topaz_resources_remove_asset(topazResources_t * r, topazAsset_t * asset) {
    if (asset) {
        topaz_table_remove(r->name2asset, topaz_asset_get_name(asset));    
        topaz_asset_destroy(asset);
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


