typedef struct topazFilesystem_t topazFilesystem_t;

#include <stdint.h>
/*

    FilesystemAPI
    -----
    
    The set of functions that define how the filesystem abstraction should 
    behave. Creating filsystem abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/filesys.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazFilesystemAPI_t topazFilesystemAPI_t;
typedef struct topaz_t topaz_t;
typedef struct topazArray_t topazArray_t;
typedef struct topazString_t topazString_t;


// Each function is an implementation-facing copy of 
// the user-side API for topazFilesystem_t. See <topaz/backends/filesystem.h>
//
// The API deals mostly with path strings at its core.
// The backend should return / modify strings with FULLY QUALIFIED PATHS 
// to prevent issues.
struct topazFilesystemAPI_t {
    // Creates a filesystem instance.
    //
    // At this time, the backend should populate the default paths,
    // corresponding to the DefaultNode enum.
    void * (*filesystem_create)(
        topazFilesystem_t *,
        topaz_t *,
        topazString_t * defaultResourcesPath,
        topazString_t * defaultTopazPath,
        topazString_t * defaultUserDataPath
    );
    
    
    // Destroys a filesystem instance.
    void (*filesystem_destroy)(
        topazFilesystem_t *,
        void *
    );
    
    // Reads the entirety of a file from the filesystem as 
    // raw bytes. size should be populated as the byte count.
    // A malloc'd buffer should be returned.
    // If the file is unreadable, NULL can be returned to indicate this status. 
    void * (*filesystem_read)(topazFilesystem_t *, void *, const topazString_t * fullpath, uint32_t * size);
    
    // Writes the entirety of a buffer as a file on the filesystem.
    // name is just the filename within the fullpathBase on the system.
    // If name contains system-specific pathing tokens, the backend 
    // may choose to reject the write request for security purposes.
    // Return 1 for success, 0 for fail.
    int (*filesystem_write)(
        topazFilesystem_t *, 
        void *, 
        const topazString_t * fullpathBase, 
        const topazString_t * name,
        
        // raw bytes
        const uint8_t * data, 
        
        // byte count
        uint32_t size
    );
    
    // Returns a full path pointing to the parent of this directory/node.
    // If not accessible, NULL is returned.
    topazString_t * (*filesystem_path_parent)(
        topazFilesystem_t *,
        void *,
        topazString_t * path
    );

    // Gets all the children of the directory/node if any.
    // The children array should be populated with topazString_t *
    // instances, each containing a full path to a child item.
    void (*filesystem_path_get_children)(
        topazFilesystem_t *,
        void *,
        const topazString_t * path,
        topazArray_t * children
    );
    
    // returns 0 or 1 for whether the given 
    // path, whether partial or full, referes to a 
    // preexisting element. After returning, the path 
    // will be guaranteed to be a fullpath.
    /// 
    int (*filesystem_path_validate)(
        topazFilesystem_t *,
        void *,
        const topazString_t * from, // may be null. If so, assume path is full.
        topazString_t * path
    );
    
    
      
    
    
    

};
