#include <topaz/entity.h>

typedef struct {
    topazScript_Object_t * self;
    topazScriptManager_t * manager;
} TopazScriptEntity;


static void topaz_script_entity__on_attach(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onAttach"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_detach(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onDetach"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_remove(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onRemove"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
    topaz_script_object_reference_unref(scr->self);
    void * context = scr->manager;
    TSO_OBJECT_DESTROY(scr);
    free(scr);
}

static void topaz_script_entity__on_pre_step(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onPreStep"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_step(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onStep"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_pre_draw(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onPreDraw"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_draw(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(scr->self, TOPAZ_STR_CAST("onDraw"));
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}




TSO_SCRIPT_API_FN(entity_api__create) {
    topazEntity_t * entity = topaz_entity_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW(entity, TSO_OBJECT_ID__ENTITY, NULL);



    topazEntity_Attributes_t attribs;
    attribs.on_attach   = (topaz_entity_attribute_callback)topaz_script_entity__on_attach;
    attribs.on_detach   = (topaz_entity_attribute_callback)topaz_script_entity__on_detach;
    attribs.on_remove   = (topaz_entity_attribute_callback)topaz_script_entity__on_remove;
    attribs.on_pre_step = (topaz_entity_attribute_callback)topaz_script_entity__on_pre_step;
    attribs.on_step     = (topaz_entity_attribute_callback)topaz_script_entity__on_step;
    attribs.on_pre_draw = (topaz_entity_attribute_callback)topaz_script_entity__on_pre_draw;
    attribs.on_draw     = (topaz_entity_attribute_callback)topaz_script_entity__on_draw;
    attribs.userData = calloc(1, sizeof(TopazScriptEntity));
    TopazScriptEntity * tsoData = attribs.userData;
    tsoData->manager  = context;
    tsoData->self = topaz_script_object_from_object(script, object);

    if (topaz_script_object_reference_get_feature_mask(object) & topazScript_Object_Feature_Map) {
        topaz_entity_set_attributes(entity, &attribs);
    } else {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: script implementation does not support topazScript_Object_Feature_Map for its empty object. This limits the use of many scripting features.\n");
        #endif
    }


    return object;
}

TSO_SCRIPT_API_FN(entity_api__null) {
    topazEntity_t * out = topaz_entity_null();
    topazScript_Object_t * a = TSO_OBJECT_FETCH_NATIVE(out);
    if (a) return a;

    TSO_OBJECT_NEW(out, TSO_OBJECT_ID__ENTITY, NULL);
    return object;

}

TSO_SCRIPT_API_FN(entity_api__is_valid) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);
    return topaz_script_object_from_int(script, topaz_entity_is_valid(native));
}

TSO_SCRIPT_API_FN(entity_api__remove) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_remove(native);    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__get_child_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_int(script, topaz_array_get_size(topaz_entity_get_children(native)));
}

TSO_SCRIPT_API_FN(entity_api__get_nth_child) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    
    const topazArray_t * children = topaz_entity_get_children(native);
    int n = topaz_script_object_as_int(arg1);
    if (n < 0 || n >= topaz_array_get_size(children)) TSO_NO_RETURN;
    topazEntity_t * child = topaz_array_at(children, topazEntity_t *, n);

    topazScript_Object_t * a = TSO_OBJECT_FETCH_NATIVE(child);
    if (a) return a;

    TSO_OBJECT_NEW(child, TSO_OBJECT_ID__ENTITY, NULL);
    return object;

}

TSO_SCRIPT_API_FN(entity_api__step) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_step(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__draw) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_draw(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__attach) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    TSO_NATIVIZE_1(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   

    topaz_entity_attach(native, native1);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__detach) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_detach(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__get_parent) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazEntity_t * out = topaz_entity_get_parent(native);
    if (!out) out = topaz_entity_null();
    topazScript_Object_t * a = TSO_OBJECT_FETCH_NATIVE(out);
    if (a) return a;

    TSO_OBJECT_NEW(out, TSO_OBJECT_ID__ENTITY, NULL);
    return object;

}

TSO_SCRIPT_API_FN(entity_api__query) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazEntity_t * out = topaz_entity_query(native, topaz_script_object_as_string(arg1));
    if (!out) out = topaz_entity_null();
    topazScript_Object_t * a = TSO_OBJECT_FETCH_NATIVE(out);
    if (a) return a;

    TSO_OBJECT_NEW(out, TSO_OBJECT_ID__ENTITY, NULL);
    return object;

}

TSO_SCRIPT_API_FN(entity_api__search) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazEntity_t * out = topaz_entity_search(native, topaz_script_object_as_string(arg1));
    if (!out) out = topaz_entity_null();
    topazScript_Object_t * a = TSO_OBJECT_FETCH_NATIVE(out);
    if (a) return a;

    TSO_OBJECT_NEW(out, TSO_OBJECT_ID__ENTITY, NULL);
    return object;

}



TSO_SCRIPT_API_FN(entity_api__set_priority) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_set_priority(native, topaz_script_object_as_int(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_priority_last) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_set_priority_last(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_priority_first) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_set_priority_first(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__get_priority) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_get_priority(native));
}




TSO_SCRIPT_API_FN(entity_api__get_rotation) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazVector_t * v = malloc(sizeof(topazVector_t));
    *v = *topaz_entity_get_rotation(native);
    return TSO_OBJECT_INSTANTIATE(vector_api__create, v);
}

TSO_SCRIPT_API_FN(entity_api__get_position) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazVector_t * v = malloc(sizeof(topazVector_t));
    *v = *topaz_entity_get_position(native);
    return TSO_OBJECT_INSTANTIATE(vector_api__create, v);
}

TSO_SCRIPT_API_FN(entity_api__get_scale) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazVector_t * v = malloc(sizeof(topazVector_t));
    *v = *topaz_entity_get_scale(native);
    return TSO_OBJECT_INSTANTIATE(vector_api__create, v);
}




TSO_SCRIPT_API_FN(entity_api__set_rotation) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_entity_rotation(native) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_position) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_entity_position(native) = *native1;
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_scale) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    *topaz_entity_scale(native) = *native1;
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(entity_api__get_global_position) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topazVector_t * v = malloc(sizeof(topazVector_t));
    *v = topaz_entity_get_global_position(native);
    return TSO_OBJECT_INSTANTIATE(vector_api__create, v);
}


TSO_SCRIPT_API_FN(entity_api__is_stepping) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_is_stepping(native));
}

TSO_SCRIPT_API_FN(entity_api__is_drawing) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_is_drawing(native));
}

TSO_SCRIPT_API_FN(entity_api__set_stepping) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_set_stepping(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_drawing) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_set_drawing(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__get_stepping) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_get_stepping(native));
}

TSO_SCRIPT_API_FN(entity_api__get_drawing) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_get_drawing(native));
}



TSO_SCRIPT_API_FN(entity_api__set_name) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    topaz_entity_set_name(native, topaz_script_object_as_string(arg1));
    TSO_NO_RETURN; 
}


TSO_SCRIPT_API_FN(entity_api__get_name) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__ENTITY);   
    return topaz_script_object_from_string(script, topaz_entity_get_name(native));
}


static void add_refs__entity_api(topazScript_t * script, topazScriptManager_t * context) {

    TS_MAP_NATIVE_FN("topaz_entity__create", entity_api__create);
    TS_MAP_NATIVE_FN("topaz_entity__null", entity_api__null);

    // member functions
    TS_MAP_NATIVE_FN("topaz_entity__is_valid", entity_api__is_valid);
    TS_MAP_NATIVE_FN("topaz_entity__remove", entity_api__remove);
    TS_MAP_NATIVE_FN("topaz_entity__get_child_count", entity_api__get_child_count);
    TS_MAP_NATIVE_FN("topaz_entity__get_nth_child", entity_api__get_nth_child);
    TS_MAP_NATIVE_FN("topaz_entity__step", entity_api__step);
    TS_MAP_NATIVE_FN("topaz_entity__draw", entity_api__draw);
    TS_MAP_NATIVE_FN("topaz_entity__attach", entity_api__attach);
    TS_MAP_NATIVE_FN("topaz_entity__detach", entity_api__detach);
    TS_MAP_NATIVE_FN("topaz_entity__get_parent", entity_api__get_parent);
    TS_MAP_NATIVE_FN("topaz_entity__query", entity_api__query);
    TS_MAP_NATIVE_FN("topaz_entity__search", entity_api__search);

    TS_MAP_NATIVE_FN("topaz_entity__set_priority", entity_api__set_priority);
    TS_MAP_NATIVE_FN("topaz_entity__set_priority_last", entity_api__set_priority_last);
    TS_MAP_NATIVE_FN("topaz_entity__set_priority_first", entity_api__set_priority_first);
    TS_MAP_NATIVE_FN("topaz_entity__get_priority", entity_api__get_priority);

    // copy of vector as a value; no references
    TS_MAP_NATIVE_FN("topaz_entity__get_rotation", entity_api__get_rotation);
    TS_MAP_NATIVE_FN("topaz_entity__get_position", entity_api__get_position);
    TS_MAP_NATIVE_FN("topaz_entity__get_scale", entity_api__get_scale);

    TS_MAP_NATIVE_FN("topaz_entity__set_rotation", entity_api__set_rotation);
    TS_MAP_NATIVE_FN("topaz_entity__set_position", entity_api__set_position);
    TS_MAP_NATIVE_FN("topaz_entity__set_scale", entity_api__set_scale);

    TS_MAP_NATIVE_FN("topaz_entity__get_global_position", entity_api__get_global_position);
    TS_MAP_NATIVE_FN("topaz_entity__is_stepping", entity_api__is_stepping);
    TS_MAP_NATIVE_FN("topaz_entity__is_drawing", entity_api__is_drawing);
    TS_MAP_NATIVE_FN("topaz_entity__set_stepping", entity_api__set_stepping);
    TS_MAP_NATIVE_FN("topaz_entity__set_drawing", entity_api__set_drawing);
    TS_MAP_NATIVE_FN("topaz_entity__get_stepping", entity_api__get_stepping);
    TS_MAP_NATIVE_FN("topaz_entity__get_drawing", entity_api__get_drawing);


    TS_MAP_NATIVE_FN("topaz_entity__set_name", entity_api__set_name);
    TS_MAP_NATIVE_FN("topaz_entity__get_name", entity_api__get_name);

}
