#include <topaz/backends/filesystem.h>


TSO_SCRIPT_API_FN(filesystem_api__get_path) {
    TSO_ARG_0;

    topazFilesystem_t * fs = topaz_context_get_filesystem(((topazScriptManager_t*)context)->ctx);
    const topazFilesystem_Path_t * ptr = topaz_filesystem_get_path(
        fs,
        topaz_script_object_as_int(arg0)
    ); 
    
    if (!ptr) {
        TSO_NO_RETURN;
    }
    
    

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE((void*)ptr, TSO_OBJECT_ID__FILESYSTEM_PATH, NULL, NULL);

    return object;
}




TSO_SCRIPT_API_FN(filesystem_api__get_path_from_string) {
    TSO_ARG_0;
    TSO_ARG_1;


    if (topaz_script_object_get_type(arg0) == topazScript_Object_Type_Undefined) {

        topazFilesystem_t * fs = topaz_context_get_filesystem(((topazScriptManager_t*)context)->ctx);
        const topazFilesystem_Path_t * ptr = topaz_filesystem_get_path_from_string(
            fs,
            NULL,
            topaz_script_object_as_string(arg0)
        ); 
        
        if (!ptr) {
            TSO_NO_RETURN;
        }
        
        

        // creates new object and sets native pointer
        TSO_OBJECT_NEW_VALUE((void*)ptr, TSO_OBJECT_ID__FILESYSTEM_PATH, NULL, NULL);
        return object;
                
    } else {
        TSO_NATIVIZE(topazFilesystem_Path_t *, TSO_OBJECT_ID__FILESYSTEM_PATH);

        topazFilesystem_t * fs = topaz_context_get_filesystem(((topazScriptManager_t*)context)->ctx);
        const topazFilesystem_Path_t * ptr = topaz_filesystem_get_path_from_string(
            fs,
            native,
            topaz_script_object_as_string(arg1)
        ); 
        
        if (!ptr) {
            TSO_NO_RETURN;
        }

        // creates new object and sets native pointer
        TSO_OBJECT_NEW_VALUE((void*)ptr, TSO_OBJECT_ID__FILESYSTEM_PATH, NULL, NULL);
        return object;    
    }

}


TSO_SCRIPT_API_FN(filesystem_api_path__as_string) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazFilesystem_Path_t *, TSO_OBJECT_ID__FILESYSTEM_PATH);


    return topaz_script_object_from_string(
        script,
        topaz_filesystem_path_as_string(native)
    );
}

TSO_SCRIPT_API_FN(filesystem_api_path__get_parent) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazFilesystem_Path_t *, TSO_OBJECT_ID__FILESYSTEM_PATH);



    const topazFilesystem_Path_t * ptr = topaz_filesystem_path_get_parent(
        native
    ); 
    
    if (!ptr) {
        TSO_NO_RETURN;
    }

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE((void*)ptr, TSO_OBJECT_ID__FILESYSTEM_PATH, NULL, NULL);
    return object;    
}


TSO_SCRIPT_API_FN(filesystem_api_path__get_child_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazFilesystem_Path_t *, TSO_OBJECT_ID__FILESYSTEM_PATH);

    uint32_t size = topaz_array_get_size(topaz_filesystem_path_get_children(native));
    return topaz_script_object_from_int(
        script,
        size
    );
}


TSO_SCRIPT_API_FN(filesystem_api_path__get_nth_child) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazFilesystem_Path_t *, TSO_OBJECT_ID__FILESYSTEM_PATH);

    const topazArray_t * children = topaz_filesystem_path_get_children(native);
    uint32_t size = topaz_array_get_size(children);
    int n = topaz_script_object_as_int(arg1);
    if (n < 0 || n >= size) {
        TSO_NO_RETURN;
    }

    const topazFilesystem_Path_t * ptr = topaz_array_at(children, topazFilesystem_Path_t *, n);
    if (!ptr) {
        TSO_NO_RETURN;
    }


    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE((void*)ptr, TSO_OBJECT_ID__FILESYSTEM_PATH, NULL, NULL);
    return object;    
}



static void add_refs__filesystem_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_filesystem__get_path", filesystem_api__get_path, 1);
    TS_MAP_NATIVE_FN("topaz_filesystem__get_path_from_string", filesystem_api__get_path_from_string, 1);

    TS_MAP_NATIVE_FN("topaz_filesystem_path__as_string", filesystem_api_path__as_string, 1);
    TS_MAP_NATIVE_FN("topaz_filesystem_path__get_parent", filesystem_api_path__get_parent, 1);
    TS_MAP_NATIVE_FN("topaz_filesystem_path__get_child_count", filesystem_api_path__get_child_count, 1);
    TS_MAP_NATIVE_FN("topaz_filesystem_path__get_nth_child", filesystem_api_path__get_nth_child, 2);

    // reading / writing need to be done via resources.
}
