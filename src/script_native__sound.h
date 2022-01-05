#include <topaz/assets/sound.h>



TSO_SCRIPT_API_FN(sound_api__get_nth_sample_left) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__SOUND);   
    int n = topaz_script_object_as_int(arg1);
    uint32_t count = topaz_sound_get_sample_count(native);
    if (n < 0) TSO_NO_RETURN;
    if (n >= count) TSO_NO_RETURN; 
    
    return topaz_script_object_from_int(script, topaz_array_at(topaz_sound_get_samples(native), topazSound_Sample_t, n).leftSample);
}

TSO_SCRIPT_API_FN(sound_api__get_nth_sample_right) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__SOUND);   
    int n = topaz_script_object_as_int(arg1);
    uint32_t count = topaz_sound_get_sample_count(native);
    if (n < 0) TSO_NO_RETURN;
    if (n >= count) TSO_NO_RETURN; 
    
    return topaz_script_object_from_int(script, topaz_array_at(topaz_sound_get_samples(native), topazSound_Sample_t, n).rightSample);
}
TSO_SCRIPT_API_FN(sound_api__get_sample_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__SOUND);   
    return topaz_script_object_from_int(script, topaz_sound_get_sample_count(native));
}


TSO_SCRIPT_API_FN(sound_api__is_in_use) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__SOUND);   
    return topaz_script_object_from_int(script, topaz_sound_is_in_use(native));
}



TSO_SCRIPT_API_FN(sound_api__set_samples) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__SOUND);   
    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        topazSound_Sample_t * v = calloc(len, sizeof(topazSound_Sample_t));
        for(i = 0; i < len; i+=2) {            
            v[i/2].leftSample  = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg1, i));
            v[i/2].rightSample = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg1, i+1));
        }
        
        topaz_sound_set_samples(
            native,
            v,
            len/2
        );
        free(v);
    }

    TSO_NO_RETURN;
}





static void add_refs__sound_api(topazScript_t * script, topazScriptManager_t * context) {
    // member functions
    TS_MAP_NATIVE_FN("topaz_sound__get_nth_sample_left", sound_api__get_nth_sample_left, 2);
    TS_MAP_NATIVE_FN("topaz_sound__get_nth_sample_right", sound_api__get_nth_sample_right, 2);
    TS_MAP_NATIVE_FN("topaz_sound__get_sample_count", sound_api__get_sample_count, 1);
    TS_MAP_NATIVE_FN("topaz_sound__set_samples", sound_api__set_samples, 2);
    TS_MAP_NATIVE_FN("topaz_sound__is_in_use", sound_api__is_in_use, 1);

    

}
