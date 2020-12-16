#include <topaz/asset.h>

TSO_SCRIPT_API_FN(asset_api__get_name) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
    return topaz_script_object_from_string(script, topaz_asset_get_name(native));
}

TSO_SCRIPT_API_FN(asset_api__get_type) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   
    return topaz_script_object_from_int(script, topaz_asset_get_type(native));
}




static void add_refs__asset_api(topazScript_t * script, topazScriptManager_t * context) {

    TS_MAP_NATIVE_FN("topaz_asset__get_name", asset_api__get_name);
    TS_MAP_NATIVE_FN("topaz_asset__get_type", asset_api__get_type);

}