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

TSO_SCRIPT_API_FN(resources_api__read_data_asset_from_path) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);

    return topaz_script_object_from_int(
        script,
        topaz_resources_read_data_asset_from_path(
            r,
            native,
            topaz_script_object_as_string(arg1)
        )
    );
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


TSO_SCRIPT_API_FN(resources_api__query_bundle) {
    TSO_ARG_0;

    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);    
    topazString_t * str = topaz_resources_query_bundle(
        r,
        topaz_script_object_as_string(arg0)
    );
    topazScript_Object_t * out = topaz_script_object_from_string(script, str);
    topaz_string_destroy(str);
    return out;
}

typedef struct {
    topazScript_t * script;
    void * context;
    topazScript_Object_t * fn;
} UnpackCallbackData;
static void resources_api__unpack_bundle__on_new_item(
    topazResources_t * res,
    topazAsset_t * asset,
    void * userdata
) {
    UnpackCallbackData * data = userdata;
    topazScript_t * script = data->script;
    void * context = data->context;
    
    topazScript_Object_t * out =  TSO_OBJECT_FETCH_KEPT_NATIVE(asset);
    if (!out) {

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
        out = object;
    }
    
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            data->fn, 
            TOPAZ_ARRAY_CAST(&out, topazScript_Object_t *, 1)
        )
    );
}

TSO_SCRIPT_API_FN(resources_api__unpack_bundle) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    
    void (*onNewItem)(topazResources_t * res, topazAsset_t * newItem, void * userdata);
    onNewItem = NULL;
    void * onNewItem_data = NULL;
    if (topaz_script_object_get_type(arg3) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg3) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "resources_api__unpack_bundle: 4th arg must be a function if its non-empty.");
            TSO_NO_RETURN;
        }    
        
        onNewItem = resources_api__unpack_bundle__on_new_item;
        UnpackCallbackData * data = calloc(1, sizeof(UnpackCallbackData));
        data->fn = topaz_script_object_from_object(script, arg3);
        data->script = script;
        data->context = context;
        
        onNewItem_data = data;
    }
    


    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);    
    int res = topaz_resources_unpack_bundle(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2) ,
        onNewItem,
        onNewItem_data
    );
    
    if (onNewItem) {
        free(onNewItem_data);	
    }
    
    return topaz_script_object_from_int(script, res);
}

TSO_SCRIPT_API_FN(resources_api__pack_bundle) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
 
    topazScript_Object_t * argsConv0[7];

    uint32_t dependsCount;
    topazString_t ** dependsName;
    int * dependsMajor;
    int * dependsMinor;

    uint32_t assetCount;
    topazString_t ** assetNames;
    topazString_t ** assetExtensions;


    uint32_t i;
    
    for(i = 0; i < 6; ++i) {
        argsConv0[i] = topaz_script_object_reference_array_get_nth(arg1, i);
    }
    
    dependsCount = topaz_script_object_reference_array_get_count(arg2);
    dependsName = malloc(sizeof(topazString_t*)*dependsCount);
    dependsMajor = malloc(sizeof(int)*dependsCount);
    dependsMinor = malloc(sizeof(int)*dependsCount);
    for(i = 0; i < dependsCount; ++i) {
        topazScript_Object_t * o = topaz_script_object_reference_array_get_nth(arg2, i);
        topazScript_Object_t * name_obj = topaz_script_object_reference_array_get_nth(o, 0);
        topazScript_Object_t * maj_obj = topaz_script_object_reference_array_get_nth(o, 1);
        topazScript_Object_t * min_obj = topaz_script_object_reference_array_get_nth(o, 2);
        
        dependsName[i] = topaz_string_clone(topaz_script_object_as_string(name_obj));
        dependsMajor[i] = topaz_script_object_as_int(maj_obj);
        dependsMinor[i] = topaz_script_object_as_int(min_obj);
        
        topaz_script_object_destroy(name_obj);
        topaz_script_object_destroy(maj_obj);
        topaz_script_object_destroy(min_obj);
        topaz_script_object_destroy(o);
    }
    
    assetCount = topaz_script_object_reference_array_get_count(arg3);
    assetNames = malloc(sizeof(topazString_t*)*assetCount);
    assetExtensions = malloc(sizeof(topazString_t*)*assetCount);
    for(i = 0; i < assetCount; ++i) {
        topazScript_Object_t * o = topaz_script_object_reference_array_get_nth(arg3, i);
        topazScript_Object_t * name_obj = topaz_script_object_reference_array_get_nth(o, 0);
        topazScript_Object_t * ext_obj = topaz_script_object_reference_array_get_nth(o, 1);
        
        assetNames[i] = topaz_string_clone(topaz_script_object_as_string(name_obj));
        assetExtensions[i] = topaz_string_clone(topaz_script_object_as_string(ext_obj));
        
        topaz_script_object_destroy(name_obj);
        topaz_script_object_destroy(ext_obj);
        topaz_script_object_destroy(o);
    }    
    
    
    
    
    
    
    topazResources_t * r = topaz_context_get_resources(((topazScriptManager_t*)context)->ctx);    

    topazAsset_t * asset = topaz_resources_pack_bundle(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_string(argsConv0[1]),
        topaz_script_object_as_int(argsConv0[2]),
        topaz_script_object_as_int(argsConv0[3]),
        topaz_script_object_as_int(argsConv0[4]),
        topaz_script_object_as_string(argsConv0[5]),
        topaz_script_object_as_string(argsConv0[6]),
        
        dependsCount,
        dependsName,
        dependsMajor,
        dependsMinor,
        
        
        assetCount,
        assetNames,
        assetExtensions
    );
    
    for(i = 0; i < 6; ++i) {
        topaz_script_object_destroy(argsConv0[i]);
    }
    
    for(i = 0; i < dependsCount; ++i) {
        topaz_string_destroy(dependsName[i]);
    }
    free(dependsName);
    free(dependsMajor);	
    free(dependsMinor);
    
    for(i = 0; i < assetCount; ++i) {
        topaz_string_destroy(assetNames[i]);
        topaz_string_destroy(assetExtensions[i]);
    }
    free(assetNames);
    free(assetExtensions);


    if (!asset)
        TSO_NO_RETURN;
       

    int type = TSO_OBJECT_TYPE__ASSET;
    switch(topaz_asset_get_type(asset)) {
      case topazAsset_Type_Data:    type |= TSO_OBJECT_ID__DATA; break;
      default:;
    }
    TSO_OBJECT_NEW_VALUE(asset, type, NULL, NULL);
    TSO_OBJECT_KEEP_REF(asset);
    return object;
    
}



static void add_refs__resources_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_resources__set_path", resources_api__set_path, 1);
    TS_MAP_NATIVE_FN("topaz_resources__get_path", resources_api__get_path, 0);

    TS_MAP_NATIVE_FN("topaz_resources__create_asset", resources_api__create_asset, 2);
    TS_MAP_NATIVE_FN("topaz_resources__fetch_asset", resources_api__fetch_asset, 1);

    TS_MAP_NATIVE_FN("topaz_resources__create_data_asset_from_path",  resources_api__create_data_asset_from_path, 2);
    TS_MAP_NATIVE_FN("topaz_resources__read_data_asset_from_path",  resources_api__read_data_asset_from_path, 2);

    TS_MAP_NATIVE_FN("topaz_resources__convert_asset", resources_api__convert_asset, 2);
    TS_MAP_NATIVE_FN("topaz_resources__write_asset", resources_api__write_asset, 3);
    TS_MAP_NATIVE_FN("topaz_resources__remove_asset", resources_api__remove_asset, 1);
    TS_MAP_NATIVE_FN("topaz_resources__is_extension_supported", resources_api__is_extension_supported, 1);
    
    TS_MAP_NATIVE_FN("topaz_resources__pack_bundle", resources_api__pack_bundle, 3);
    TS_MAP_NATIVE_FN("topaz_resources__unpack_bundle", resources_api__unpack_bundle, 4);
    TS_MAP_NATIVE_FN("topaz_resources__query_bundle", resources_api__query_bundle, 1);
}
