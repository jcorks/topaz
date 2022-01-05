#include <topaz/assets/material.h>

TSO_SCRIPT_API_FN(material_api__set_program_data) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MATERIAL);   

    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= 28) TSO_NO_RETURN;

    topazMaterial_ProgramData_t p = *topaz_material_get_program_data(native);
    p.userData[index] = topaz_script_object_as_number(arg2);
    topaz_material_set_program_data(native, &p);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(material_api__get_program_data) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MATERIAL);   

    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= 28) TSO_NO_RETURN;

    topazMaterial_ProgramData_t p = *topaz_material_get_program_data(native);
    return topaz_script_object_from_number(
        script,
        p.userData[index]
    );
}

TSO_SCRIPT_API_FN(material_api__set_program_source) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MATERIAL);   
    
    
    topaz_material_set_program_source(
        native,
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_string(arg2),
        topaz_script_object_as_string(arg3)
    );
    
    TSO_NO_RETURN;
};


TSO_SCRIPT_API_FN(material_api__clear_sources) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MATERIAL);   

    topaz_material_clear_sources(native);
    
    TSO_NO_RETURN;
}


static void add_refs__material_api(topazScript_t * script, topazScriptManager_t * context) {
    // member functions
    
    // set / get on a per-float basis and hope performance would be too bad
    TS_MAP_NATIVE_FN("topaz_material__set_program_data", material_api__set_program_data, 3);
    TS_MAP_NATIVE_FN("topaz_material__get_program_data", material_api__get_program_data, 2);

    TS_MAP_NATIVE_FN("topaz_material__set_program_source", material_api__set_program_source, 4);
    TS_MAP_NATIVE_FN("topaz_material__clear_sources", material_api__clear_sources, 1);
    
}
