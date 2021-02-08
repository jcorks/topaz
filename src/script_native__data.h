#include <topaz/assets/data.h>



TSO_SCRIPT_API_FN(data_api__set) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__DATA);   


    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array)) {
        TSO_NO_RETURN;
    }

    topazArray_t * arr = topaz_array_create(sizeof(uint8_t));
    uint32_t size = topaz_script_object_reference_array_get_count(arg1);
    uint32_t i;
    uint8_t d;
    for(i = 0; i < size; ++i) {
        d = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg1, i));
        topaz_array_push(arr, d);
    }

    topaz_data_set(native, arr);
    topaz_array_destroy(arr);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(data_api__get_as_string) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__DATA);   
    topazString_t * str = topaz_data_get_as_string(native);
    topazScript_Object_t * outStr = topaz_script_object_from_string(script, str);
    topaz_string_destroy(str);
    return outStr;
}

TSO_SCRIPT_API_FN(data_api__get_byte_count) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__DATA);   
    const topazArray_t * arr = topaz_data_get_as_bytes(native);
    return topaz_script_object_from_int(script, topaz_array_get_size(arr));
}

TSO_SCRIPT_API_FN(data_api__get_nth_byte) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__DATA);   
    const topazArray_t * arr = topaz_data_get_as_bytes(native);
    uint32_t size = topaz_array_get_size(arr);
    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= size) {
        script_error(script, "Data asset: byte request is out of bounds.");
        TSO_NO_RETURN;
    }
    return topaz_script_object_from_int(script, topaz_array_at(arr, uint8_t, index));
}



static void add_refs__data_api(topazScript_t * script, topazScriptManager_t * context) {
    // member functions
    TS_MAP_NATIVE_FN("topaz_data__set", data_api__set);
    TS_MAP_NATIVE_FN("topaz_data__get_byte_count", data_api__get_byte_count);
    TS_MAP_NATIVE_FN("topaz_data__get_nth_byte", data_api__get_nth_byte);
    TS_MAP_NATIVE_FN("topaz_data__get_as_string", data_api__get_as_string);
}
