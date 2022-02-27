#include <topaz/modules/resources.h>

TSO_SCRIPT_API_FN(resources_api__set_path) {
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


TSO_SCRIPT_API_FN(resources_api__create_asset) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * asset = topaz_resources_create_asset(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_int(arg1)
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
      case topazAsset_Type_Bundle:    type |= TSO_OBJECT_ID__BUNDLE; break;
      default:;
    }
    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    TSO_OBJECT_KEEP_REF(asset);
    return object;
}

TSO_SCRIPT_API_FN(resources_api__fetch_asset) {
    TSO_ARG_0;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * asset = topaz_resources_fetch_asset(
        r,
        topaz_script_object_as_string(arg0)
    );

    if (!asset) 
        TSO_NO_RETURN;

    topazScript_Object_t * out =  TSO_OBJECT_FETCH_KEPT_NATIVE(asset);
    if (out) return out;

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image:     type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:      type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:     type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;
      case topazAsset_Type_Bundle:    type |= TSO_OBJECT_ID__BUNDLE; break;
      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    TSO_OBJECT_KEEP_REF(asset);
    return object;
}



TSO_SCRIPT_API_FN(resources_api__create_data_asset_from_path) {
    TSO_ARG_0;
    TSO_ARG_1;
    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topazAsset_t * asset = topaz_resources_create_data_asset_from_path(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_string(arg1)
    );

    if (!asset) 
        TSO_NO_RETURN;

    int type = TSO_OBJECT_TYPE__ASSET;
    type |= TSO_OBJECT_ID__DATA; 
    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    TSO_OBJECT_KEEP_REF(asset);
    return object;
}






TSO_SCRIPT_API_FN(resources_api__convert_asset) {
    TSO_ARG_0;
    TSO_ARG_1;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
    topazAsset_t * asset = topaz_resources_convert_asset(
        r,
        topaz_script_object_as_string(arg0),
        native1
    );
 
    if (!asset)
        TSO_NO_RETURN;
        
    topazScript_Object_t * o = TSO_OBJECT_FETCH_KEPT_NATIVE(native1);
    TSO_OBJECT_UNKEEP_REF(o, native1);

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Image:     type |= TSO_OBJECT_ID__IMAGE; break;
      case topazAsset_Type_Data:      type |= TSO_OBJECT_ID__DATA; break;
      case topazAsset_Type_Sound:     type |= TSO_OBJECT_ID__SOUND; break;
      case topazAsset_Type_Material:  type |= TSO_OBJECT_ID__MATERIAL; break;
      case topazAsset_Type_Mesh:      type |= TSO_OBJECT_ID__MESH; break;
      case topazAsset_Type_Bundle:    type |= TSO_OBJECT_ID__BUNDLE; break;
      default:;
    }

    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    TSO_OBJECT_KEEP_REF(asset);
    return object;
}


TSO_SCRIPT_API_FN(resources_api__write_asset) {
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
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);           


    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);
    topaz_resources_remove_asset(
        r,
        native
    );

    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(resources_api__is_extension_supported) {
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
    TS_MAP_NATIVE_FN("topaz_resources__set_path", resources_api__set_path, 1);
    TS_MAP_NATIVE_FN("topaz_resources__get_path", resources_api__get_path, 0);

    TS_MAP_NATIVE_FN("topaz_resources__create_asset", resources_api__create_asset, 2);
    TS_MAP_NATIVE_FN("topaz_resources__fetch_asset", resources_api__fetch_asset, 1);

    TS_MAP_NATIVE_FN("topaz_resources__create_data_asset_from_path",  resources_api__create_data_asset_from_path, 2);

    TS_MAP_NATIVE_FN("topaz_resources__convert_asset", resources_api__convert_asset, 3);
    TS_MAP_NATIVE_FN("topaz_resources__write_asset", resources_api__write_asset, 3);
    TS_MAP_NATIVE_FN("topaz_resources__remove_asset", resources_api__remove_asset, 1);
    TS_MAP_NATIVE_FN("topaz_resources__is_extension_supported", resources_api__is_extension_supported, 1);
}
