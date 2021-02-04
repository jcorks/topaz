#include <topaz/modules/resources.h>

TSO_SCRIPT_API_FN(resources_api__set_path) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_int(
        script,
        topaz_resources_set_path(r, topaz_script_object_as_string(arg0))
    );
}

TSO_SCRIPT_API_FN(resources_api__get_path) {
    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_string(
        script,
        topaz_resources_get_path(r)
    );
    
}

TSO_SCRIPT_API_FN(resources_api__load_asset) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * asset = topaz_resources_load_asset(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_string(arg2)
    );


    if (!asset)
        TSO_NO_RETURN;

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image: type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:  type |= TSO_OBJECT_ID__DATA; break;
      default:;
    }

    TSO_OBJECT_NEW(asset, type, NULL, NULL);
    return object;

}




TSO_SCRIPT_API_FN(resources_api__fetch_asset) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * asset = topaz_resources_fetch_asset(
        r,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_string(arg1)
    );

    if (!asset) 
        TSO_NO_RETURN;

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image: type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:  type |= TSO_OBJECT_ID__DATA; break;
      default:;
    }

    TSO_OBJECT_NEW(asset, type, NULL, NULL);
    return object;
}




TSO_SCRIPT_API_FN(resources_api__write_asset) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);           

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_int(
        script,        
        topaz_resources_write_asset(
            r,
            native,
            topaz_script_object_as_string(arg1),
            topaz_script_object_as_string(arg2)
        )
    );
}

TSO_SCRIPT_API_FN(resources_api__remove_asset) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;


    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topaz_resources_remove_asset(
        r,
        topaz_script_object_as_string(arg0)
    );

    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(resources_api__is_extension_supported) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_int(
        script,
        topaz_resources_is_extension_supported(
            r,
            topaz_script_object_as_string(arg0)
        )
    );
}



TSO_SCRIPT_API_FN(resources_api__asset_path_count) {

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_int(
        script,
        topaz_array_get_size(topaz_resources_get_asset_paths(r))
    );
}

TSO_SCRIPT_API_FN(resources_api__nth_asset_path) {
    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    int index = topaz_script_object_as_int(arg0);
    uint32_t size = topaz_array_get_size(topaz_resources_get_asset_paths(r));

    if (index < 0 || index >= size) TSO_NO_RETURN;

    return topaz_script_object_from_string(
        script,
        topaz_array_at(topaz_resources_get_asset_paths(r), topazString_t *, index)
    );
}


TSO_SCRIPT_API_FN(resources_api__query_asset_paths) {
    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topaz_resources_query_asset_paths(r);
    TSO_NO_RETURN;
}



static void add_refs__resources_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_resources__set_path", resources_api__set_path);
    TS_MAP_NATIVE_FN("topaz_resources__get_path", resources_api__get_path);
    TS_MAP_NATIVE_FN("topaz_resources__fetch_asset", resources_api__fetch_asset);
    TS_MAP_NATIVE_FN("topaz_resources__load_asset", resources_api__load_asset);
    TS_MAP_NATIVE_FN("topaz_resources__write_asset", resources_api__write_asset);
    TS_MAP_NATIVE_FN("topaz_resources__remove_asset", resources_api__remove_asset);
    TS_MAP_NATIVE_FN("topaz_resources__is_extension_supported", resources_api__is_extension_supported);
    TS_MAP_NATIVE_FN("topaz_resources__asset_path_count", resources_api__asset_path_count);
    TS_MAP_NATIVE_FN("topaz_resources__nth_asset_path", resources_api__nth_asset_path);
    TS_MAP_NATIVE_FN("topaz_resources__query_asset_paths", resources_api__query_asset_paths);
}
