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


#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

#include "modules_resources__bundle"


topazAsset_t * topaz_sound_create_empty(
    topaz_t * context
);


/// Creates a sound asset with the given name.
topazAsset_t * topaz_sound_create(
    /// The topaz context.
    topaz_t * context,

    /// Name of the asset.    
    const topazString_t * name
);

topazAsset_t * topaz_data_create(
    topaz_t *, 
    const topazString_t *
);

topazAsset_t * topaz_data_create_empty(topaz_t *);


topazAsset_t * topaz_image_create(
    topaz_t *, 
    const topazString_t *
);

topazAsset_t * topaz_image_create_empty(topaz_t *);



topazAsset_t * topaz_material_create(
    topaz_t *, 
    const topazString_t *
);

topazAsset_t * topaz_material_empty(
    topaz_t *
);


topazAsset_t * topaz_mesh_create(
    topaz_t *, 
    const topazString_t *
);



struct topazResources_t {   
    topaz_t * ctx;
    topazTable_t * name2asset;

    topazTable_t * ioxs; //extension -> iox
    topazTable_t * bundles; //package repository. maps bundleName -> topazAsset_t (bundle)
    const topazFilesystem_Path_t * path;
};




topazResources_t * topaz_resources_create(topaz_t * ctx) {
    topazResources_t * out = calloc(1, sizeof(topazResources_t));
    out->ctx = ctx;
    out->name2asset = topaz_table_create_hash_topaz_string();
    out->bundles = topaz_table_create_hash_topaz_string();
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
    topaz_table_destroy(r->bundles);
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
      // bundles are NOT allowed to be created freely.
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

    if(!topaz_resources_read_data_asset_from_path(
        r,
        a,
        path
    )) {
        topaz_asset_destroy(a);
        topaz_table_remove(r->name2asset, name);    
        return NULL;
    }
    
    return a;
}

int topaz_resources_read_data_asset_from_path(
    topazResources_t * r,
    /// The asset to modify.
    topazAsset_t * a,
    /// The path to the asset.
    const topazString_t * path
) {

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
        return 0;
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
    return 1;
}

//// PRIVATE:



topazAsset_t * topaz_resources_pack_bundle(
    topazResources_t * res,
    const topazString_t * assetName,
    const topazString_t * bundleName,
    int versionMajor,
    int versionMinor,
    int versionMicro,
    const topazString_t * description,
    const topazString_t * author,
    uint32_t dependsCount,
    topazString_t * const dependsName[],
    const int * dependsMajor,
    const int * dependsMinor, 
    uint32_t assetCount,
    topazString_t * const assetNames[],
    topazString_t * const assetExtensions[]
) {
    if (versionMajor < 0 || versionMinor < 0 || versionMicro < 0)
        return 0;

    topazAsset_t * bundle = topaz_bundle_create(res->ctx, TOPAZ_STR_CAST(""));
    topaz_bundle_set_attributes(
        bundle,
        bundleName,
        versionMajor,
        versionMinor,
        versionMicro,
        description,
        author,
        dependsCount,
        dependsName,
        dependsMajor,
        dependsMinor
    );
    
    
    uint32_t i;
    for(i = 0; i < assetCount; ++i) {
        topazAsset_t * a = topaz_resources_fetch_asset(res, assetNames[i]);
        if (!(a && topaz_asset_get_type(a) == topazAsset_Type_Data)) {
            topaz_asset_destroy(bundle);
            return 0;
        }
        
        
        topaz_bundle_add_item(
            bundle,
            topaz_asset_get_name(a),
            assetExtensions[i],
            topaz_data_get_as_bytes(a)
        );
    }
    
    topazAsset_t * bundleAsData = topaz_resources_create_asset(
        res,
        assetName,
        topazAsset_Type_Data        
    );

    uint32_t bytesSize;
    const uint8_t * bytes = topaz_bundle_get_state_as_bytes(bundle, &bytesSize);

    topaz_data_set_from_bytes(
        bundleAsData,
        TOPAZ_ARRAY_CAST(bytes, uint8_t, bytesSize)
    );
        
    topaz_asset_destroy(bundle);
    return bundleAsData;
}


static int version_check(int reqMaj, int reqMin, int maj, int min) {


    if (maj != TOPAZ_RESOURCES_BUNDLE_VERSION_ANY) {
        if (maj < reqMaj) return 0;
        if (maj > reqMaj) return 1;

        if (
            (min != TOPAZ_RESOURCES_BUNDLE_VERSION_ANY && min < reqMin)
        ) {
            return 0;
        }
    }
    return 1;
}

int topaz_resources_unpack_bundle(
    topazResources_t * res,
    const topazString_t * bundleName,     
    int min_majorVersionRequired,
    int min_minorVersionRequired,
    void (*onNewItem)(topazResources_t * res, topazAsset_t *, void * userData),
    void * userdata
) {
    topazAsset_t * b = topaz_table_find(res->bundles, bundleName);
    if (!b) return 0;
    
    int maj, min, mic;
    topaz_bundle_get_version(b, &maj, &min, &mic);
    
    if (!version_check(
        min_majorVersionRequired,
        min_minorVersionRequired,
        maj,
        min
    )) return 0;
    
    
    
    // depency checking and loading
    uint32_t i, len;
    len = topaz_bundle_get_depends_count(b);
    for(i = 0; i < len; ++i) {
        int reqMaj, reqMin;
        const topazString_t * name = topaz_bundle_get_depends(
            b,
            i,
            &reqMaj, &reqMin
        );
        
        topazAsset_t * dep = topaz_table_find(res->bundles, bundleName);
        if (!dep) {
            // missing dependency
            return 0;
        }
        
        if (!topaz_bundle_is_unpacked(dep)) {
            if (!topaz_resources_unpack_bundle(
                res,
                name,
                reqMaj, reqMin,
                onNewItem,
                userdata
            )) {
                // failed
                return 0;
            }
        } else {
            topaz_bundle_get_version(dep, &maj, &min, &mic);
            if (!version_check(
                reqMaj,
                reqMin,
                maj,
                min            
            )) return 0;
        }
        
    }
    

    topazAsset_t * newItem = NULL;
    while(topaz_bundle_unpack_continue(b, &newItem)) {
        if (newItem && onNewItem) {
            onNewItem(
                res,
                newItem,
                userdata
            );
        }
    }
    
    
    return 1; 
}

topazString_t * topaz_resources_query_bundle(
    topazResources_t * res,
    const topazString_t * bundleName
) {
    topazAsset_t * b = topaz_table_find(res->bundles, bundleName);
    if (!b) return topaz_string_create();

    
    int maj, min, mic;
    topaz_bundle_get_version(b, &maj, &min, &mic);

    topazString_t * out = topaz_string_create_from_c_str(
        "%s v.%d.%d.%d\n\n"
        "author: \n%s\n\n"
        "description:\n%s\n\n"
        "dependencies:\n",
        
        topaz_string_get_c_str(topaz_bundle_get_name(b)),
        maj, min, mic,
        topaz_string_get_c_str(topaz_bundle_get_author(b)),
        topaz_string_get_c_str(topaz_bundle_get_description(b))
    );
    
    uint32_t i;
    uint32_t len = topaz_bundle_get_depends_count(b);
    for(i = 0; i < len; ++i) {
        int dMaj, dMin;
        const topazString_t * dep = topaz_bundle_get_depends(
            b,
            i,
            &dMaj,
            &dMin
        );
        
        
        topaz_string_concat_printf(
            out,
            "  %s ",
            topaz_string_get_c_str(dep)
        );
        
        if (dMaj == TOPAZ_RESOURCES_BUNDLE_VERSION_ANY) {
            topaz_string_concat_printf(
                out,
                "(any version)\n"
            );

        } else {
            topaz_string_concat_printf(
                out,
                "v %d.\n",
                dMaj                
            );

            if (dMin == TOPAZ_RESOURCES_BUNDLE_VERSION_ANY) {
                topaz_string_concat_printf(
                    out,
                    "*\n"
                );                        
            } else {
                topaz_string_concat_printf(
                    out,
                    "%d\n",
                    dMin                
                );                                    
            }
        }
    }
    
    return out;
    
}



topazAsset_t * topaz_resources_convert_asset(
    topazResources_t * r,
    const topazString_t * extension,
    topazAsset_t * srcAsset
) {
    if (topaz_asset_get_type(srcAsset) != topazAsset_Type_Data) {
        return NULL;
    }
    
    // already data, return itself.
    if (extension == NULL ||
        topaz_string_test_eq(extension, TOPAZ_STR_CAST(""))) {
        return srcAsset;   
    }
    const topazString_t * name = topaz_asset_get_name(srcAsset);
    topazAsset_t * asset;
    
    // SPECIAL CASE: bundles are not handled by normal IOX and are translated
    // directly by the resources instance.
    if (topaz_string_test_eq(extension, TOPAZ_STR_CAST("bundle"))) {
        asset = topaz_bundle_create(r->ctx, name);
        const topazArray_t * srcData = topaz_data_get_as_bytes(srcAsset);
        topaz_bundle_set_state_from_bytes(
            asset,
            topaz_array_get_data(srcData),
            topaz_array_get_size(srcData)
        );
        topaz_table_insert(
            r->bundles, 
            topaz_bundle_get_name(asset),
            asset 
        );

    // normal case: use an io translator to transform the data externally.
    } else {
        

        // invalid extension if not created, return NULL
        topazIOX_t * dec = topaz_table_find(r->ioxs, extension);
        if (!dec) return NULL;

        // Create a new asset based on which kind it is.

        switch(topaz_iox_get_asset_type(dec)) {
          case topazAsset_Type_Data:  asset    = topaz_data_create(r->ctx, name);  break;
          case topazAsset_Type_Image: asset    = topaz_image_create(r->ctx, name); break;
          case topazAsset_Type_Sound: asset    = topaz_sound_create(r->ctx, name); break;
          case topazAsset_Type_Material: asset = topaz_material_create(r->ctx, name); break;
          case topazAsset_Type_Mesh:  asset    = topaz_mesh_create(r->ctx, name); break;
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
        if (topaz_asset_get_type(asset) == topazAsset_Type_Bundle) {
            topaz_table_remove(r->bundles, topaz_asset_get_name(asset));
        }
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


