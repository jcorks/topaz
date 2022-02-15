#include <topaz/assets/bundle.h>


TSO_SCRIPT_API_FN(bundle_api__create_empty) {
    TSO_ARG_0; // attribs
    TSO_ARG_1; // dependsName array
    TSO_ARG_2; // dependsVersion array
    
    topazScriptManager_t * mgr = context;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__DATA);   

    if (!(topaz_script_object_reference_get_feature_mask(arg0) & topazScript_Object_Feature_Array)) {
        TSO_NO_RETURN;
    }
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array)) {
        TSO_NO_RETURN;
    }


    uint32_t len = topaz_script_object_reference_array_get_count(arg1);
    if (topaz_script_object_reference_array_get_count(arg2) < len) {
        len = topaz_script_object_reference_array_get_count(arg2);
    }
    
    
    topazScript_Object_t * versionMajor = topaz_script_object_reference_map_get_property(
        arg0,
        TOPAZ_STR_CAST("versionMajor")
    );
    topazScript_Object_t * versionMinor = topaz_script_object_reference_map_get_property(
        arg0,
        TOPAZ_STR_CAST("versionMinor")
    );
    topazScript_Object_t * versionMicro = topaz_script_object_reference_map_get_property(
        arg0,
        TOPAZ_STR_CAST("versionMicro")
    );


    topazScript_Object_t * bundleName = topaz_script_object_reference_map_get_property(
        arg0,
        TOPAZ_STR_CAST("bundleName")
    );
    topazScript_Object_t * description = topaz_script_object_reference_map_get_property(
        arg0,
        TOPAZ_STR_CAST("description")
    );
    topazScript_Object_t * author = topaz_script_object_reference_map_get_property(
        arg0,
        TOPAZ_STR_CAST("author")
    );
        
    topazString_t ** dependsNames    = malloc(len*sizeof(topazString_t*));
    topazString_t ** dependsVersions = malloc(len*sizeof(topazString_t*));
    
    uint32_t i;
    for(i = 0; i < len; ++i) {
        dependsNames[i] = (topazString_t*)topaz_script_object_as_string(topaz_script_object_reference_array_get_nth(arg1, i));
        dependsVersions[i] = (topazString_t*)topaz_script_object_as_string(topaz_script_object_reference_array_get_nth(arg2, i));
    }

    topazAsset_t * a = topaz_bundle_create_empty(
        mgr->ctx,
        topaz_script_object_as_string(bundleName),
        topaz_script_object_as_int(versionMajor),
        topaz_script_object_as_int(versionMinor),
        topaz_script_object_as_int(versionMicro),
        topaz_script_object_as_string(description),
        topaz_script_object_as_string(author),
        (const topazString_t **)dependsNames,
        (const topazString_t **)dependsVersions, 
        len
    );
    free(dependsNames);
    free(dependsVersions);
    
    topazScript_Object_t * out = TSO_OBJECT_FETCH_KEPT_NATIVE(a);
    if (out) return out;
    int type = TSO_OBJECT_TYPE__ASSET | TSO_OBJECT_ID__BUNDLE;
    TSO_OBJECT_NEW_VALUE(a, type, NULL, NULL);
    TSO_OBJECT_KEEP_REF(a);
    return object;
}


TSO_SCRIPT_API_FN(bundle_api__add_item) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    TSO_NATIVIZE_3(topazAsset_t *, TSO_OBJECT_ID__DATA);   

    if (!(topaz_script_object_reference_get_feature_mask(arg3) & topazScript_Object_Feature_Array)) {
        TSO_NO_RETURN;
    }

    topaz_bundle_add_item(
        native,
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_string(arg2),
        native3        
    );
    TSO_NO_RETURN;
}






TSO_SCRIPT_API_FN(bundle_api__get_description) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    const topazString_t * str = topaz_bundle_get_description(native);
    return topaz_script_object_from_string(script, str);
}


TSO_SCRIPT_API_FN(bundle_api__get_author) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    const topazString_t * str = topaz_bundle_get_author(native);
    return topaz_script_object_from_string(script, str);
}

TSO_SCRIPT_API_FN(bundle_api__get_version_major) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    int ma, mi, mic;
    topaz_bundle_get_version(
        native,
        &ma, &mi, &mic
    );
    return topaz_script_object_from_int(script, ma);
}

TSO_SCRIPT_API_FN(bundle_api__get_version_minor) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    int ma, mi, mic;
    topaz_bundle_get_version(
        native,
        &ma, &mi, &mic
    );
    return topaz_script_object_from_int(script, mi);
}

TSO_SCRIPT_API_FN(bundle_api__get_version_micro) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    int ma, mi, mic;
    topaz_bundle_get_version(
        native,
        &ma, &mi, &mic
    );
    return topaz_script_object_from_int(script, mic);
}


TSO_SCRIPT_API_FN(bundle_api__get_depends_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   

    const topazArray_t * n = topaz_bundle_get_depends_names(native);
    return topaz_script_object_from_int(script, topaz_array_get_size(n));
}

TSO_SCRIPT_API_FN(bundle_api__get_depends_nth_name) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    uint32_t index = topaz_script_object_as_int(arg1);
    const topazArray_t * n = topaz_bundle_get_depends_names(native);
    uint32_t len = topaz_array_get_size(n);
    if (index <= len) TSO_NO_RETURN;
    return topaz_script_object_from_string(script, topaz_array_at(n, topazString_t *, index));
}

TSO_SCRIPT_API_FN(bundle_api__get_depends_nth_version) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    uint32_t index = topaz_script_object_as_int(arg1);
    const topazArray_t * n = topaz_bundle_get_depends_version_strings(native);
    uint32_t len = topaz_array_get_size(n);
    if (index <= len) TSO_NO_RETURN;
    return topaz_script_object_from_string(script, topaz_array_at(n, topazString_t *, index));
}

TSO_SCRIPT_API_FN(bundle_api__get_byte_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);  
    uint32_t len; 
    topaz_bundle_get_packed_data(native, &len);
    return topaz_script_object_from_int(script, len);
}

TSO_SCRIPT_API_FN(bundle_api__get_nth_byte) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__BUNDLE);   
    uint32_t size;
    const uint8_t * data = topaz_bundle_get_packed_data(native, &size);
    
    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= size) {
        script_error(script, "Bundle asset: byte request is out of bounds.");
        TSO_NO_RETURN;
    }
    return topaz_script_object_from_int(script, data[index]);
}

static void add_refs__bundle_api(topazScript_t * script, topazScriptManager_t * context) {
    // member functions
    TS_MAP_NATIVE_FN("topaz_bundle__create_empty", bundle_api__create_empty, 3);
    
    TS_MAP_NATIVE_FN("topaz_bundle__add_item", bundle_api__add_item, 4);
    
    TS_MAP_NATIVE_FN("topaz_bundle__get_byte_count", bundle_api__get_byte_count, 1);        
    TS_MAP_NATIVE_FN("topaz_bundle__get_nth_byte", bundle_api__get_nth_byte, 1);        
    TS_MAP_NATIVE_FN("topaz_bundle__get_version_major", bundle_api__get_version_major, 1);
    TS_MAP_NATIVE_FN("topaz_bundle__get_version_minor", bundle_api__get_version_minor, 1);
    TS_MAP_NATIVE_FN("topaz_bundle__get_version_micro", bundle_api__get_version_micro, 1);
    TS_MAP_NATIVE_FN("topaz_bundle__get_description", bundle_api__get_description, 1);
    TS_MAP_NATIVE_FN("topaz_bundle__get_author", bundle_api__get_author, 1);
    TS_MAP_NATIVE_FN("topaz_bundle__get_depends_count", bundle_api__get_depends_count, 1);
    TS_MAP_NATIVE_FN("topaz_bundle__get_depends_nth_version", bundle_api__get_depends_nth_version, 2);
    TS_MAP_NATIVE_FN("topaz_bundle__get_depends_nth_name", bundle_api__get_depends_nth_name, 2);

}
