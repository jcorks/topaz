#include <topaz/assets/mesh.h>



TSO_SCRIPT_API_FN(mesh_api__get_vertex_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   

    return topaz_script_object_from_int(
        script,
        topaz_mesh_get_vertex_count(native)
    );
}



TSO_SCRIPT_API_FN(mesh_api__set_vertex_count) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   

    topaz_mesh_set_vertex_count(native, 
        topaz_script_object_as_int(arg1)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(mesh_api__define_vertices) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);
    
    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        float * v = malloc(len*sizeof(float));
        for(i = 0; i < len; ++i) {
            v[i] = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
        }
        topaz_mesh_define_vertices(
            native,
            TOPAZ_ARRAY_CAST(v, topazRenderer_3D_Vertex_t, len/(3+3+2+4))
        );
        free(v);
    }
    TSO_NO_RETURN;
}
TSO_SCRIPT_API_FN(mesh_api__get_vertex) {
    TSO_ARG_0;
    TSO_ARG_1; // vertex
    TSO_ARG_2; // returner
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);  


    uint32_t len = topaz_mesh_get_vertex_count(native);
    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= len) TSO_NO_RETURN;
    
    
    topazRenderer_3D_Vertex_t vtx = topaz_mesh_get_vertex(
        native,
        index
    );
    
    topazScript_Object_t * objects[12];
    objects[0] = topaz_script_object_from_number(script, vtx.x);
    objects[1] = topaz_script_object_from_number(script, vtx.y);
    objects[2] = topaz_script_object_from_number(script, vtx.z);

    objects[3] = topaz_script_object_from_number(script, vtx.normalX);
    objects[4] = topaz_script_object_from_number(script, vtx.normalY);
    objects[5] = topaz_script_object_from_number(script, vtx.normalZ);

    objects[6] = topaz_script_object_from_number(script, vtx.texX);
    objects[7] = topaz_script_object_from_number(script, vtx.texY);

    objects[8]  = topaz_script_object_from_number(script, vtx.userDefinedData[0]);
    objects[9]  = topaz_script_object_from_number(script, vtx.userDefinedData[1]);
    objects[10] = topaz_script_object_from_number(script, vtx.userDefinedData[2]);
    objects[11] = topaz_script_object_from_number(script, vtx.userDefinedData[3]);

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            arg2,
            TOPAZ_ARRAY_CAST(objects, topazScript_Object_t *, 12)
        )
    );
    int i;
    for(i = 0; i < 12; ++i)
        topaz_script_object_destroy(objects[i]);
   
}
TSO_SCRIPT_API_FN(mesh_api__set_vertex) {
    TSO_ARG_0;
    TSO_ARG_1; // vertex
    TSO_ARG_2; // data array

    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);  

    uint32_t len = topaz_mesh_get_vertex_count(native);
    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= len) TSO_NO_RETURN;
    



    if (topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg2);
        uint32_t reqLen = 3+3+2+4;
        float data[12];
        
        if (!reqLen || reqLen != len) TSO_NO_RETURN;
        uint32_t i;
        for(i = 0; i < len; ++i) {
            data[i] = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg2, i));
        }

        topaz_mesh_set_vertex(
            native,
            index,
            *(topazRenderer_3D_Vertex_t*)data
        );
    }    
    TSO_NO_RETURN;


}




TSO_SCRIPT_API_FN(mesh_api__add_object) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   

    topaz_mesh_add_object(native);

    TSO_NO_RETURN;
}
TSO_SCRIPT_API_FN(mesh_api__get_object_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   


    return topaz_script_object_from_int(
        script,
        topaz_mesh_get_object_count(native)
    );
}

TSO_SCRIPT_API_FN(mesh_api__remove_object) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   

    topaz_mesh_remove_object(native,
        topaz_script_object_as_int(
            arg1   
        )
    );
    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(mesh_api__set_object_indices) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   


    if (topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg2);
        uint32_t i;
        topazArray_t * a = topaz_array_create(sizeof(uint32_t));
        for(i = 0; i < len; ++i) {
            uint32_t v = topaz_script_object_as_int(topaz_script_object_reference_array_get_nth(arg2, i));
            topaz_array_push(a, v);
        }
        
        topaz_mesh_set_object_indices(native, topaz_script_object_as_int(arg1), a);
        topaz_array_destroy(a);
    }

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(mesh_api__get_object_indices) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazAsset_t *, TSO_OBJECT_ID__MESH);   


    uint32_t len = topaz_script_object_reference_array_get_count(arg2);
    uint32_t i;
    topazArray_t * a = topaz_mesh_get_object_indices(native, topaz_script_object_as_int(arg1));
    if (!a) TSO_NO_RETURN;


    topazScript_Object_t * count = topaz_script_object_from_int(
        script,
        topaz_array_get_size(a)
    );
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            arg2,
            TOPAZ_ARRAY_CAST(&count, topazScript_Object_t *, 1)
        )
    );
    topaz_script_object_destroy(count);
    


    topaz_array_set_size(a, 0);
    for(i = 0; i < len; ++i) {
        topazScript_Object_t * ind = topaz_script_object_from_int(script, topaz_array_at(a, uint32_t, i));
        topaz_script_object_destroy(
            topaz_script_object_reference_call(
                arg3,
                TOPAZ_ARRAY_CAST(&ind, topazScript_Object_t *, 1)
            )
        );
        topaz_script_object_destroy(ind);
    }

    TSO_NO_RETURN;
}

    


static void add_refs__mesh_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_mesh__get_vertex_count", mesh_api__get_vertex_count, 1);
    TS_MAP_NATIVE_FN("topaz_mesh__set_vertex_count", mesh_api__set_vertex_count, 2);
    TS_MAP_NATIVE_FN("topaz_mesh__define_vertices", mesh_api__define_vertices, 2);
    TS_MAP_NATIVE_FN("topaz_mesh__get_vertex", mesh_api__get_vertex, 3);
    TS_MAP_NATIVE_FN("topaz_mesh__set_vertex", mesh_api__set_vertex, 3);

    TS_MAP_NATIVE_FN("topaz_mesh__add_object", mesh_api__add_object, 1);
    TS_MAP_NATIVE_FN("topaz_mesh__get_object_indices", mesh_api__get_object_indices, 4);
    TS_MAP_NATIVE_FN("topaz_mesh__set_object_indices", mesh_api__set_object_indices, 3);
    
    TS_MAP_NATIVE_FN("topaz_mesh__get_object_count", mesh_api__get_object_count, 1);
    TS_MAP_NATIVE_FN("topaz_mesh__remove_object", mesh_api__remove_object, 2);


}
