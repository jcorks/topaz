#include <topaz/assets/image.h>



TSO_SCRIPT_API_FN(image_api__get_width) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   
    return topaz_script_object_from_int(script, topaz_image_get_width(native));
}

TSO_SCRIPT_API_FN(image_api__get_height) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   
    return topaz_script_object_from_int(script, topaz_image_get_height(native));
}


TSO_SCRIPT_API_FN(image_api__resize) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   

    topaz_image_resize(
        native,
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(image_api__add_frame) {
    TSO_ARG_0;

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   

    topaz_image_add_frame(native);
    TSO_NO_RETURN;    
}


TSO_SCRIPT_API_FN(image_api__remove_frame) {
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   
    topaz_image_remove_frame(native, topaz_script_object_as_int(arg1));
    TSO_NO_RETURN;    
}



TSO_SCRIPT_API_FN(image_api__get_frame_count) {
    TSO_ARG_0;

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   
    return topaz_script_object_from_int(script, topaz_image_get_frame_count(native));
}


TSO_SCRIPT_API_FN(image_api__frame_set_rgba) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    if (!(topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Array)) {
        TSO_NO_RETURN;
    }

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__IMAGE);   

    int frameIndex = topaz_script_object_as_int(arg1);
    topazImage_Frame_t * frame = topaz_image_get_frame(native, frameIndex);
    if (!frame) TSO_NO_RETURN;


    int w = topaz_image_get_width(native);
    int h = topaz_image_get_height(native);

    uint32_t len = topaz_script_object_reference_array_get_count(arg2);

    if (len != w*h*4) {
        TSO_NO_RETURN;
    }


    uint8_t * buffer = malloc(w*h*4);
    uint32_t i;
    for(i = 0; i < len; i+=4) {
        buffer[i+0] = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg2, i+0));
        buffer[i+1] = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg2, i+1));
        buffer[i+2] = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg2, i+2));
        buffer[i+3] = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg2, i+3));
    }
    topaz_image_frame_set_data(
        frame,
        buffer
    );

    free(buffer);
    TSO_NO_RETURN;
}








static void add_refs__image_api(topazScript_t * script, topazScriptManager_t * context) {
    // member functions
    TS_MAP_NATIVE_FN("topaz_image__get_width", image_api__get_width, 1);
    TS_MAP_NATIVE_FN("topaz_image__get_height", image_api__get_height, 1);

    TS_MAP_NATIVE_FN("topaz_image__resize", image_api__resize, 3);
    TS_MAP_NATIVE_FN("topaz_image__add_frame", image_api__add_frame, 1);
    TS_MAP_NATIVE_FN("topaz_image__remove_frame", image_api__remove_frame, 2);
    TS_MAP_NATIVE_FN("topaz_image__get_frame_count", image_api__get_frame_count, 1);
    TS_MAP_NATIVE_FN("topaz_image__frame_set_rgba", image_api__frame_set_rgba, 3);
    //TS_MAP_NATIVE_FN("topaz_image__frame_get_rgba", image_api__frame_get_rgba);
    

}
