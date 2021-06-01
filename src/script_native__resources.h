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
      case topazAsset_Type_Image:  type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:   type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:  type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;
      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    return object;

}

TSO_SCRIPT_API_FN(resources_api__load_asset_data) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);


    topazArray_t * arr = topaz_array_create(sizeof(uint8_t));
    int size = topaz_script_object_reference_array_get_count(arg1);
    uint32_t i;
    uint8_t d;
    for(i = 0; i < size; ++i) {
        d = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg1, i));
        topaz_array_push(arr, d);
    }
    topazAsset_t * asset = topaz_resources_load_asset_data(
        r,
        topaz_script_object_as_string(arg0),
        arr,
        topaz_script_object_as_string(arg2)
    );
    topaz_array_destroy(arr);

    if (!asset)
        TSO_NO_RETURN;

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image:  type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:   type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:  type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;

      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    return object;

}


TSO_SCRIPT_API_FN(resources_api__load_asset_base64) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);


    topazAsset_t * asset = topaz_resources_load_asset_base64(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_string(arg2)
    );

    if (!asset)
        TSO_NO_RETURN;

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image:  type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:   type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:  type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;

      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    return object;

}


TSO_SCRIPT_API_FN(resources_api__create_asset) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * asset = topaz_resources_create_asset(
        r,
        topaz_script_object_as_int(arg0)
    );

    if (!asset) 
        TSO_NO_RETURN;

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image:     type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:      type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:     type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;
      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
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
      case topazAsset_Type_Image:     type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:      type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:     type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;
      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
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






static void add_refs__resources_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_resources__set_path", resources_api__set_path);
    TS_MAP_NATIVE_FN("topaz_resources__get_path", resources_api__get_path);
    TS_MAP_NATIVE_FN("topaz_resources__fetch_asset", resources_api__fetch_asset);
    TS_MAP_NATIVE_FN("topaz_resources__create_asset", resources_api__create_asset);
    TS_MAP_NATIVE_FN("topaz_resources__load_asset", resources_api__load_asset);
    TS_MAP_NATIVE_FN("topaz_resources__load_asset_data", resources_api__load_asset_data);
    TS_MAP_NATIVE_FN("topaz_resources__load_asset_base64", resources_api__load_asset_base64);
    TS_MAP_NATIVE_FN("topaz_resources__write_asset", resources_api__write_asset);
    TS_MAP_NATIVE_FN("topaz_resources__remove_asset", resources_api__remove_asset);
    TS_MAP_NATIVE_FN("topaz_resources__is_extension_supported", resources_api__is_extension_supported);
}
