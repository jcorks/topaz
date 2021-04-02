#include <topaz/compat.h>
#include <topaz/backends/filesystem.h>
#include <topaz/containers/table.h>
#include <topaz/containers/array.h>
#include <topaz/wbuffer.h>
#include <topaz/rbuffer.h>
#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif 


struct topazFilesystem_t {
    topazFilesystemAPI_t api;
    topazTable_t * allPaths;

    topazSystem_Backend_t * backend;
    void * userData;

    topazFilesystem_Path_t * defaultResources;
    topazFilesystem_Path_t * defaultTopaz;
    topazFilesystem_Path_t * defaultUserData;    
};

struct topazFilesystem_Path_t {
    topazFilesystem_t * fs;
    topazFilesystem_Path_t * parent;
    topazString_t * str;
    topazArray_t * children;
};

static topazFilesystem_Path_t * generate_path(
    topazFilesystem_t * fs,
    const topazString_t * path
) {
    topazFilesystem_Path_t * out = topaz_table_find(fs->allPaths, path);
    if (out) return out;
    
    out = calloc(1, sizeof(topazFilesystem_Path_t));
    out->fs = fs;
    out->parent = NULL;
    out->str = topaz_string_clone(path);
    out->children = topaz_array_create(sizeof(topazFilesystem_Path_t*));
    return out;
}


topazFilesystem_t * topaz_filesystem_create(topaz_t * ctx, topazSystem_Backend_t * b, topazFilesystemAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(api.filesystem_create);
        assert(api.filesystem_destroy);
        assert(api.filesystem_read);
        assert(api.filesystem_write);
        assert(api.filesystem_path_parent);
        assert(api.filesystem_path_get_children);
        assert(api.filesystem_path_validate);
    #endif
    topazFilesystem_t * out = calloc(1, sizeof(topazFilesystem_t));
    out->api = api;
    out->backend = b;
    out->allPaths = topaz_table_create_hash_topaz_string();


    topazString_t * defaultResourcesStr = topaz_string_create();
    topazString_t * defaultTopazStr     = topaz_string_create();
    topazString_t * defaultUserDataStr  = topaz_string_create();

    out->userData = out->api.filesystem_create(
        out,
        ctx,
        defaultResourcesStr,
        defaultTopazStr,
        defaultUserDataStr
    );
    
    out->defaultResources = generate_path(out, defaultResourcesStr);
    out->defaultTopaz     = generate_path(out, defaultTopazStr);
    out->defaultUserData  = generate_path(out, defaultUserDataStr);
    
    topaz_string_destroy(defaultResourcesStr);
    topaz_string_destroy(defaultTopazStr);
    topaz_string_destroy(defaultUserDataStr);

    return out;
}


void topaz_filesystem_destroy(topazFilesystem_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif
    t->api.filesystem_destroy(t, t->userData);
}





topazSystem_Backend_t * topaz_filesystem_get_backend(topazFilesystem_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazFilesystemAPI_t topaz_filesystem_get_api(topazFilesystem_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api;
}




/// Gets a read-only built-in filesystem path.
const topazFilesystem_Path_t * topaz_filesystem_get_path(
    topazFilesystem_t * fs,
    topazFilesystem_DefaultNode def
) {
    switch(def) {
      case topazFilesystem_DefaultNode_Resources: return fs->defaultResources;
      case topazFilesystem_DefaultNode_Topaz:     return fs->defaultTopaz;
      case topazFilesystem_DefaultNode_UserData:  return fs->defaultUserData;
    }
    return NULL;
}


/// Gets a read-only string representing the path.
/// For most filesystems, this is a "fully qualified" path.
const topazString_t * topaz_filesystem_path_as_string(
    const topazFilesystem_Path_t * path
) {
    return path->str;
}

/// Returns a new filesystem path from a string representation of a 
/// path. On systems that support partial / relative paths, they will be 
/// from the "from" parameter path. If no such path on the filesystem 
/// can be found, NULL is returned.
const topazFilesystem_Path_t * topaz_filesystem_get_path_from_string(
    topazFilesystem_t * fs,
    const topazFilesystem_Path_t * from,
    const topazString_t * partialOrFull
) {
    topazString_t * s = topaz_string_clone(partialOrFull);
    if (!fs->api.filesystem_path_validate(
        fs,
        fs->userData,
        from ? from->str : NULL,
        s
    )) {
        topaz_string_destroy(s);
        return NULL;
    }
    
    topazFilesystem_Path_t * p = generate_path(fs, s);
    topaz_string_destroy(s);
    return p;
}



/// Gets the parent filesystem path of the given path. 
/// If none, NULL is returned.
const topazFilesystem_Path_t * topaz_filesystem_path_get_parent(
    const topazFilesystem_Path_t * path
) {
    if (path->parent) {
        return path->parent;
    }
    
    topazString_t * s = path->fs->api.filesystem_path_parent(
        path->fs,
        path->fs->userData,
        path->str
    );
    if (s) {
        topazFilesystem_Path_t * p = generate_path(path->fs, s);
        topaz_string_destroy(s);
        return p;
    }
    return NULL;
}

/// Gets an array of children filesystem paths.
/// If none, the array returned is empty. 
const topazArray_t * topaz_filesystem_path_get_children(
    const topazFilesystem_Path_t * path
) {
    uint32_t i;
    uint32_t len;
    topaz_array_set_size(path->children, 0);
    
    
    
    topazArray_t * arr = topaz_array_create(sizeof(topazString_t *));
    path->fs->api.filesystem_path_get_children(
        path->fs,
        path->fs->userData,
        path->str, 
        arr
    );
    
    len = topaz_array_get_size(arr);
    for(i = 0; i < len; ++i) {
        topazFilesystem_Path_t * sub = generate_path(path->fs, topaz_array_at(arr, topazString_t *,  i));
        topaz_array_push(path->children, sub);
        topaz_string_destroy(topaz_array_at(arr, topazString_t *,  i));
    }
    topaz_array_destroy(arr);    
    return path->children;
}

/// Returns a buffer of data represented at the given path.
/// This buffer must be freed by the caller (topaz_rbuffer_destroy).
topazRbuffer_t * topaz_filesystem_path_read(
    const topazFilesystem_Path_t * path
) {
    topazRbuffer_t * out = topaz_rbuffer_create();
    uint32_t size;
    uint8_t * b = path->fs->api.filesystem_read(
        path->fs,
        path->fs->userData,
        path->str,
        &size
    );
    if (!b) {
        return NULL;
    }
    topaz_rbuffer_open(out, b, size);
    free(b);
    return out;
}


/// Writes the given buffer to a file on the filesystem.
/// Name should refer to the name of the file to write.
int topaz_filesystem_path_write(
    const topazFilesystem_Path_t * path,
    const topazString_t * name,
    const topazWbuffer_t * data
) {
    return path->fs->api.filesystem_write(
        path->fs,
        path->fs->userData,
        path->str,
        name,
        
        topaz_array_get_data(topaz_wbuffer_get_data(data)),
        topaz_wbuffer_get_size(data)
    );
}


/// Normally, all created filesystem paths are kept 
/// until the filesystem instance is destroyed.
/// This function will free all filesystem paths in use. 
/// This is usually not necessary, but might be needed 
/// for more complex uses of the filesystem instance.
void topaz_filesystem_expunge(
    /// The filesystem to modify.
    topazFilesystem_t * fs
) {
    assert(!"not yet");
}



