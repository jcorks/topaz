#include <topaz/compat.h>
#include <topaz/modules/resources.h>
#include <topaz/containers/array.h>
#include <topaz/topaz.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


typedef struct {
    topazAsset_LoadingProfile_t loading;
    topazAsset_Attributes_t attribs;
    topazAsset_Type type;

} LoadingProfile;

struct topazResources_t {   
    topaz_t * ctx;
    topazFilesys_t * fs;
    topazTable_t * name2asset;
    topazTable_t * ext2profile;
};




topazResources_t * topaz_resources_create(topaz_t * ctx) {
    topazResources_t * out = calloc(1, sizeof(topazResources_t));
    out->ctx = ctx;
    out->fs = topaz_context_filesys_create(ctx);
    out->name2asset = topaz_table_create_hash_topaz_string();
    out->ext2profile = topaz_table_create_hash_topaz_string();

    return out;
}

void topaz_resources_destroy(topazResources_t * r) {
    topazTableIter_t * iter = topaz_table_iter_create();

    for(topaz_table_iter_start(iter, r->ext2profile);
       !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        free(topaz_table_iter_get_value(iter));
    }
    topaz_table_destroy(r->ext2profile);

    
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
    const topazString_t * fileType,
    const topazString_t * name
) {
    // first fetch it
    topazAsset_t * asset = topaz_resources_fetch_asset(r, fileType, name);
    
    // already loaded! return
    if (asset && topaz_asset_is_loaded(asset)) return asset;

    // invalid extension if not created, return NULL'
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
    if (!topaz_asset_load(
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
    const topazString_t * fileType,
    const topazString_t * name
) {
    LoadingProfile * profile = topaz_table_find(
        r->ext2profile,
        fileType
    );

    // type isnt supported.
    if (!profile) {
        #ifdef TOPAZDC_DEBUG
            printf("Unrecognized extension \"%s\"\n", topaz_string_get_c_str(fileType));
        #endif
        return NULL;
    }
    topazAsset_t * out = topaz_asset_create(
        r->ctx,
        profile->type,
        name,
        &profile->attribs,
        &profile->loading
    );

    topaz_table_insert(
        r->name2asset,
        name,
        out
    );
    return out;
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
    return topaz_table_find(r->ext2profile, ext) != NULL;
}

void topaz_resources_add_extension(
    topazResources_t * r,
    const topazString_t * ext, 
    topazAsset_Type type,
    const topazAsset_LoadingProfile_t * loading,
    const topazAsset_Attributes_t * attribs
) {

    LoadingProfile * existing = topaz_table_find(r->ext2profile, ext);
    if (existing) {
        existing->loading = *loading;
        existing->attribs = *attribs;
        existing->type = type;
    } else {
        LoadingProfile * profile = malloc(sizeof(LoadingProfile));
        profile->loading = *loading;
        profile->attribs = *attribs;
        profile->type = type;
        topaz_table_insert(r->ext2profile, ext, profile);
    }
}
