#include <topaz/entity.h>

typedef struct {
    topazScript_Object_t * self;
    topazScriptManager_t * manager;

    topazScript_Object_t * onStep;
    topazScript_Object_t * onDraw;
    topazScript_Object_t * onPreStep;
    topazScript_Object_t * onPreDraw;
    topazScript_Object_t * onAttach;
    topazScript_Object_t * onDetach;
    topazScript_Object_t * onRemove;
} TopazScriptEntity;


static void topaz_script_entity__on_attach(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onAttach;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_detach(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onDetach;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_remove(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onRemove;
    if (fn) {
        topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
    }
    topaz_script_object_reference_unref(scr->self);
    if (scr->onStep) topaz_script_object_destroy(scr->onStep);
    if (scr->onDraw) topaz_script_object_destroy(scr->onDraw);
    if (scr->onPreStep) topaz_script_object_destroy(scr->onPreStep);
    if (scr->onPreDraw) topaz_script_object_destroy(scr->onPreDraw);
    if (scr->onAttach) topaz_script_object_destroy(scr->onAttach);
    if (scr->onDetach) topaz_script_object_destroy(scr->onDetach);
    if (scr->onRemove) topaz_script_object_destroy(scr->onRemove);


    void * context = scr->manager;
    TSO_OBJECT_UNKEEP_REF(scr->self, e);
    free(scr);
}

static void topaz_script_entity__on_pre_step(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onPreStep;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_step(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onStep;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_pre_draw(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onPreDraw;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}

static void topaz_script_entity__on_draw(topazEntity_t * e, TopazScriptEntity * scr) {
    topazScript_Object_t * fn = scr->onDraw;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, topaz_array_empty()));
}




TSO_SCRIPT_API_FN(entity_api__create) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;
    TSO_ARG_6;

    topazEntity_Attributes_t attribs = {};
    attribs.on_remove = (topaz_entity_attribute_callback)topaz_script_entity__on_remove;    
    attribs.userData = calloc(1, sizeof(TopazScriptEntity));
    TopazScriptEntity * e = attribs.userData;


    if (topaz_script_object_get_type(arg0) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg0) & topazScript_Object_Feature_Callable)) {
            script_error(script, "onAttach requires a function to be set.");
        } else {
            e->onAttach = topaz_script_object_from_object(script, arg0);
            attribs.on_attach   = (topaz_entity_attribute_callback)topaz_script_entity__on_attach;        
        }
    }

    if (topaz_script_object_get_type(arg1) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "onDetach requires a function to be set.");
        } else {
            e->onDetach = topaz_script_object_from_object(script, arg1);
            attribs.on_detach   = (topaz_entity_attribute_callback)topaz_script_entity__on_detach;        
        }
    }

    if (topaz_script_object_get_type(arg2) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "onRemove requires a function to be set.");
        } else {
            e->onRemove = topaz_script_object_from_object(script, arg2);
        }
    }

    if (topaz_script_object_get_type(arg3) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg3) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "onPreStep requires a function to be set.");
        } else {
            e->onPreStep = topaz_script_object_from_object(script, arg3);
            attribs.on_pre_step   = (topaz_entity_attribute_callback)topaz_script_entity__on_pre_step;        
        }
    }

    if (topaz_script_object_get_type(arg4) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg4) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "onStep requires a function to be set.");
        } else {
            e->onStep = topaz_script_object_from_object(script, arg4);
            attribs.on_step   = (topaz_entity_attribute_callback)topaz_script_entity__on_step;        
        }
    }

    if (topaz_script_object_get_type(arg5) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg5) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "onPreDraw requires a function to be set.");
        } else {
            e->onPreDraw = topaz_script_object_from_object(script, arg5);
            attribs.on_pre_draw   = (topaz_entity_attribute_callback)topaz_script_entity__on_pre_draw;        
        }
    }

    if (topaz_script_object_get_type(arg6) != topazScript_Object_Type_Undefined) {
        if (!(topaz_script_object_reference_get_feature_mask(arg6) & topazScript_Object_Feature_Callable)) { 
            script_error(script, "onDraw requires a function to be set.");
        } else {
            e->onDraw = topaz_script_object_from_object(script, arg6);
            attribs.on_draw   = (topaz_entity_attribute_callback)topaz_script_entity__on_draw;        
        }
    }

    topazEntity_t * entity = topaz_entity_create(((topazScriptManager_t*)context)->ctx, &attribs);

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(entity, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    TSO_OBJECT_KEEP_REF(entity);


    e->manager  = context;
    e->self = topaz_script_object_from_object(script, object);



    if (!topaz_script_object_reference_get_feature_mask(object) & topazScript_Object_Feature_Map) {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: script implementation does not support topazScript_Object_Feature_Map for its empty object. This limits the use of many scripting features.\n");
        #endif
    }
    return object;
}


TSO_SCRIPT_API_FN(entity_api__set_type_id) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    topaz_entity_set_type_id(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__get_type_id) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    return topaz_script_object_from_int(script, topaz_entity_get_type_id(native));
}

TSO_SCRIPT_API_FN(entity_api__is_valid) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);
    return topaz_script_object_from_int(script, topaz_entity_is_valid(native));
}

TSO_SCRIPT_API_FN(entity_api__remove) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_remove(native);    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__get_child_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_int(script, topaz_array_get_size(topaz_entity_get_children(native)));
}

TSO_SCRIPT_API_FN(entity_api__get_nth_child) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    
    const topazArray_t * children = topaz_entity_get_children(native);
    int n = topaz_script_object_as_int(arg1);
    if (n < 0 || n >= topaz_array_get_size(children)) TSO_NO_RETURN;
    topazEntity_t * child = topaz_array_at(children, topazEntity_t *, n);

    topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(child);
    if (a) return topaz_script_object_from_object(script, a);

    TSO_OBJECT_NEW_VALUE(child, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    return object;
}

TSO_SCRIPT_API_FN(entity_api__step) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_step(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__draw) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_draw(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__attach) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TSO_NATIVIZE_1(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    topaz_entity_attach(native, native1);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__detach) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_detach(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_on_step) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_step requires a function object to be set.");
    } else {
        if (e->onStep) {
            topaz_script_object_destroy(e->onStep);
        }
        e->onStep = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_on_draw) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_draw requires a function object to be set.");
    } else {
        if (e->onDraw) {
            topaz_script_object_destroy(e->onDraw);
        }
        e->onDraw = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_on_pre_step) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_pre_step requires a function object to be set.");
    } else {
        if (e->onPreStep) {
            topaz_script_object_destroy(e->onPreStep);
        }
        e->onPreStep = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_on_pre_draw) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_pre_draw requires a function object to be set.");
    } else {
        if (e->onPreDraw) {
            topaz_script_object_destroy(e->onPreDraw);
        }
        e->onPreDraw = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(entity_api__set_on_attach) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_attach requires a function object to be set.");
    } else {
        if (e->onAttach) {
            topaz_script_object_destroy(e->onAttach);
        }
        e->onAttach = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_on_detach) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_detach requires a function object to be set.");
    } else {
        if (e->onDetach) {
            topaz_script_object_destroy(e->onDetach);
        }
        e->onDetach = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_on_remove) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    TopazScriptEntity * e = topaz_entity_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "entity_api__set_on_remove requires a function object to be set.");
    } else {
        if (e->onRemove) {
            topaz_script_object_destroy(e->onRemove);
        }
        e->onRemove = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}





TSO_SCRIPT_API_FN(entity_api__get_parent) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazEntity_t * out = topaz_entity_get_parent(native);

    if (out) {
        topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(out);
        if (a) return topaz_script_object_from_object(script, a);

        TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
        return object;
    }
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__query) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazEntity_t * out = topaz_entity_query(native, topaz_script_object_as_string(arg1));
    if (out) {
        topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(out);
        if (a) return topaz_script_object_from_object(script, a);

        TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
        return object;
    }
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__search) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazEntity_t * out = topaz_entity_search(native, topaz_script_object_as_string(arg1));

    if (out) {
        topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(out);
        if (a) return topaz_script_object_from_object(script, a);

        TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
        return object;
    }
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(entity_api__set_priority) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_set_priority(native, topaz_script_object_as_int(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_priority_last) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_set_priority_last(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_priority_first) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_set_priority_first(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__get_priority) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_get_priority(native));
}




TSO_SCRIPT_API_FN(entity_api__get_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazVector_t a = *topaz_entity_get_rotation(native);
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__get_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazVector_t a = *topaz_entity_get_position(native);
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;

}



TSO_SCRIPT_API_FN(entity_api__get_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazVector_t a = *topaz_entity_get_scale(native);
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(entity_api__set_rotation) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    topaz_entity_rotation(native)->x = topaz_script_object_as_number(arg1);
    topaz_entity_rotation(native)->y = topaz_script_object_as_number(arg2);
    topaz_entity_rotation(native)->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(entity_api__set_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    topaz_entity_position(native)->x = topaz_script_object_as_number(arg1);
    topaz_entity_position(native)->y = topaz_script_object_as_number(arg2);
    topaz_entity_position(native)->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__set_scale) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    topaz_entity_scale(native)->x = topaz_script_object_as_number(arg1);
    topaz_entity_scale(native)->y = topaz_script_object_as_number(arg2);
    topaz_entity_scale(native)->z = topaz_script_object_as_number(arg3);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__look_at) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;
    TSO_ARG_5;
    TSO_ARG_6;

    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   

    topazVector_t target = {
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2),
        topaz_script_object_as_number(arg3)
    };

    topazVector_t up = {
        topaz_script_object_as_number(arg4),
        topaz_script_object_as_number(arg5),
        topaz_script_object_as_number(arg6)
    };

    topaz_entity_look_at(native, &target, &up);

    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(entity_api__get_global_position) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazVector_t a = topaz_entity_get_global_position(native);
    topaz_script_return_vector(script, arg1, a.x, a.y, a.z);
    TSO_NO_RETURN;
}





TSO_SCRIPT_API_FN(entity_api__is_stepping) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_is_stepping(native));
}

TSO_SCRIPT_API_FN(entity_api__is_drawing) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_is_drawing(native));
}

TSO_SCRIPT_API_FN(entity_api__set_stepping) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_set_stepping(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__set_drawing) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_set_drawing(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__get_stepping) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_get_stepping(native));
}

TSO_SCRIPT_API_FN(entity_api__get_drawing) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_int(script, topaz_entity_get_drawing(native));
}

TSO_SCRIPT_API_FN(entity_api__get_local_matrix) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazMatrix_t a = *topaz_entity_get_local_matrix(native);
    topaz_script_return_matrix(script, arg1, &a);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__get_global_matrix) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topazMatrix_t a = *topaz_entity_get_global_matrix(native);
    topaz_script_return_matrix(script, arg1, &a);
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__set_name) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    topaz_entity_set_name(native, topaz_script_object_as_string(arg1));
    TSO_NO_RETURN; 
}


TSO_SCRIPT_API_FN(entity_api__get_name) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    return topaz_script_object_from_string(script, topaz_entity_get_name(native));
}


static void add_refs__entity_api(topazScript_t * script, topazScriptManager_t * context) {

    TS_MAP_NATIVE_FN("topaz_entity__create", entity_api__create, 7);

    // member functions
    TS_MAP_NATIVE_FN("topaz_entity__set_type_id", entity_api__set_type_id, 2);    
    TS_MAP_NATIVE_FN("topaz_entity__get_type_id", entity_api__get_type_id, 1);    
    TS_MAP_NATIVE_FN("topaz_entity__is_valid", entity_api__is_valid, 1);
    TS_MAP_NATIVE_FN("topaz_entity__remove", entity_api__remove, 1);
    TS_MAP_NATIVE_FN("topaz_entity__get_child_count", entity_api__get_child_count, 1);
    TS_MAP_NATIVE_FN("topaz_entity__get_nth_child", entity_api__get_nth_child, 2);
    TS_MAP_NATIVE_FN("topaz_entity__step", entity_api__step, 1);
    TS_MAP_NATIVE_FN("topaz_entity__draw", entity_api__draw, 1);
    TS_MAP_NATIVE_FN("topaz_entity__attach", entity_api__attach, 2);
    TS_MAP_NATIVE_FN("topaz_entity__detach", entity_api__detach, 1);
    TS_MAP_NATIVE_FN("topaz_entity__get_parent", entity_api__get_parent, 1);
    TS_MAP_NATIVE_FN("topaz_entity__query", entity_api__query, 2);
    TS_MAP_NATIVE_FN("topaz_entity__search", entity_api__search, 2);

    TS_MAP_NATIVE_FN("topaz_entity__set_priority", entity_api__set_priority, 2);
    TS_MAP_NATIVE_FN("topaz_entity__set_priority_last", entity_api__set_priority_last, 1);
    TS_MAP_NATIVE_FN("topaz_entity__set_priority_first", entity_api__set_priority_first, 1);
    TS_MAP_NATIVE_FN("topaz_entity__get_priority", entity_api__get_priority, 1);

    // copy of vector as a value; no references
    TS_MAP_NATIVE_FN("topaz_entity__get_rotation", entity_api__get_rotation, 2);
    TS_MAP_NATIVE_FN("topaz_entity__get_position", entity_api__get_position, 2);
    TS_MAP_NATIVE_FN("topaz_entity__get_scale", entity_api__get_scale, 2);

    TS_MAP_NATIVE_FN("topaz_entity__set_rotation", entity_api__set_rotation, 4);
    TS_MAP_NATIVE_FN("topaz_entity__set_position", entity_api__set_position, 4);
    TS_MAP_NATIVE_FN("topaz_entity__set_scale", entity_api__set_scale, 4);
    
    TS_MAP_NATIVE_FN("topaz_entity__look_at", entity_api__look_at, 7);

    TS_MAP_NATIVE_FN("topaz_entity__get_global_position", entity_api__get_global_position, 2);
    TS_MAP_NATIVE_FN("topaz_entity__is_stepping", entity_api__is_stepping, 1);
    TS_MAP_NATIVE_FN("topaz_entity__is_drawing", entity_api__is_drawing, 1);
    TS_MAP_NATIVE_FN("topaz_entity__set_stepping", entity_api__set_stepping, 2);
    TS_MAP_NATIVE_FN("topaz_entity__set_drawing", entity_api__set_drawing, 2);
    TS_MAP_NATIVE_FN("topaz_entity__get_stepping", entity_api__get_stepping, 1);
    TS_MAP_NATIVE_FN("topaz_entity__get_drawing", entity_api__get_drawing, 1);
    
    
    TS_MAP_NATIVE_FN("topaz_entity__get_local_matrix", entity_api__get_local_matrix, 2);
    TS_MAP_NATIVE_FN("topaz_entity__get_global_matrix", entity_api__get_global_matrix, 2);


    TS_MAP_NATIVE_FN("topaz_entity__set_name", entity_api__set_name, 2);
    TS_MAP_NATIVE_FN("topaz_entity__get_name", entity_api__get_name, 1);

}
