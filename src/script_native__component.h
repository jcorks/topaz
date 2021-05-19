#include <topaz/component.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


typedef struct {
    topazScript_Object_t * self;
    topazScriptManager_t * manager;

    topazScript_Object_t * onStep;
    topazScript_Object_t * onDraw;
    topazScript_Object_t * onAttach;
    topazScript_Object_t * onDetach;
    topazScript_Object_t * onDestroy;


} TopazComponentTSO;



static void topaz_script_component__on_attach(topazComponent_t * e, TopazComponentTSO * scr) {
    topazScript_Object_t * fn = scr->onAttach;
    if (!fn) return;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, TOPAZ_ARRAY_CAST(&scr->self, topazScript_Object_t *, 1)));
}

static void topaz_script_component__on_detach(topazComponent_t * e, TopazComponentTSO * scr) {
    topazScript_Object_t * fn = scr->onDetach;
    if (!fn) return;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, TOPAZ_ARRAY_CAST(&scr->self, topazScript_Object_t *, 1)));
}

static void topaz_script_component__on_destroy(topazComponent_t * e, TopazComponentTSO * scr) {
    topazScript_Object_t * fn = scr->onDestroy;
    if (!fn) return;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, TOPAZ_ARRAY_CAST(&scr->self, topazScript_Object_t *, 1)));
    topaz_script_object_reference_unref(scr->self);
    void * context = scr->manager;
    TSO_OBJECT_UNKEEP_REF(scr->self, e);
    free(scr);
}

static void topaz_script_component__on_step(topazComponent_t * e, TopazComponentTSO * scr) {
    topazScript_Object_t * fn = scr->onStep;
    if (!fn) return;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, TOPAZ_ARRAY_CAST(&scr->self, topazScript_Object_t *, 1)));
}


static void topaz_script_component__on_draw(topazComponent_t * e, TopazComponentTSO * scr) {
    topazScript_Object_t * fn = scr->onStep;
    if (!fn) return;
    topaz_script_object_destroy(topaz_script_object_reference_call(fn, TOPAZ_ARRAY_CAST(&scr->self, topazScript_Object_t *, 1)));
}

// TODO: need a better way than this.. please...
typedef struct {
    topazComponent_t * source;
    topazScriptManager_t * manager;
    topazScript_Object_t * fn;
    topazScript_t * script;
} TSOCHandlerData;

TSO_SCRIPT_API_FN(component_api__create) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazComponent_t * component = topaz_component_create(topaz_script_object_as_string(arg0), ((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__COMPONENTCUSTOM, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);


    topazComponent_Attributes_t attribs;
    attribs.on_attach   = (topaz_component_attribute_callback)topaz_script_component__on_attach;
    attribs.on_detach   = (topaz_component_attribute_callback)topaz_script_component__on_detach;
    attribs.on_destroy  = (topaz_component_attribute_callback)topaz_script_component__on_destroy;
    attribs.on_step     = (topaz_component_attribute_callback)topaz_script_component__on_step;
    attribs.on_draw     = (topaz_component_attribute_callback)topaz_script_component__on_draw;
    attribs.userData = calloc(1, sizeof(TopazComponentTSO));
    TopazComponentTSO * tsoData = attribs.userData;
    tsoData->manager = context;
    tsoData->self = topaz_script_object_from_object(script, object);

    if (topaz_script_object_reference_get_feature_mask(object) & topazScript_Object_Feature_Map) {
        topaz_component_set_attributes(component, &attribs);
    } else {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: script implementation does not support topazScript_Object_Feature_Map for its empty object. This limits the use of many scripting features.\n");
        #endif
    }


    return object;
}

TSO_SCRIPT_API_FN(component_api__destroy) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topaz_component_destroy(native);    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__step) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topaz_component_step(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__draw) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topaz_component_draw(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__set_stepping) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topaz_component_set_stepping(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__set_drawing) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topaz_component_set_drawing(native, topaz_script_object_as_number(arg1));
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__get_stepping) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    return topaz_script_object_from_int(script, topaz_component_get_stepping(native));
}

TSO_SCRIPT_API_FN(component_api__get_drawing) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    return topaz_script_object_from_int(script, topaz_component_get_drawing(native));
}


TSO_SCRIPT_API_FN(component_api__set_tag) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topaz_component_set_tag(native, topaz_script_object_as_string(arg1));
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__get_tag) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    return topaz_script_object_from_string(script, topaz_component_get_tag(native));
}


TSO_SCRIPT_API_FN(component_api__get_host) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    topazEntity_t * out = topaz_component_get_host(native);
    if (!out) out = topaz_entity_null();
    topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(out);
    if (a) return topaz_script_object_from_object(script, a);

    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
    return object;
}


TSO_SCRIPT_API_FN(component_api__null) {
    topazComponent_t * out = topaz_component_null();
    TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__COMPONENT, NULL, NULL);
    return object;
}




TSO_SCRIPT_API_FN(component_api__emit_event) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1; // event name
    TSO_ARG_2; // entity
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    TSO_NATIVIZE_2(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);   
    
    return topaz_script_object_from_int(script, 
            topaz_component_emit_event(
            native,
            topaz_script_object_as_string(arg1),
            native2,
            NULL
        )
    );

}

TSO_SCRIPT_API_FN(component_api__emit_event_anonymous) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1; // event name
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    

    return topaz_script_object_from_int(script, 
        topaz_component_emit_event_anonymous(
            native,
            topaz_script_object_as_string(arg1)
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__can_handle_event) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   
    return topaz_script_object_from_int(
        script, 
        topaz_component_can_handle_event(native, topaz_script_object_as_string(arg1))
    );
}

static int component_api_callback_cleanup(topazComponent_t * c, void * data, topazEntity_t * source, void * sourceEvent) {
    TSOCHandlerData * handler = data;
    topaz_script_object_destroy(handler->fn);
    free(handler);
    return 1;
}

static int component_api_callback(topazComponent_t * c, void * data, topazEntity_t * source, void * sourceEvent) {
    TSOCHandlerData * handler = data;
    topazScript_t * script = handler->script;
    void * context = handler->manager;
    
    topazScript_Object_t * a[2];
    a[0] = TSO_OBJECT_FETCH_KEPT_NATIVE(c);

    #ifdef TOPAZDC_DEBUG
        assert(a[0] && "Base component responding to emission of event should always have a corresponding cached script object. This is indicative of engine error");
    #endif

    if (source) {
        topazScript_Object_t * sourceObject = TSO_OBJECT_FETCH_KEPT_NATIVE(source);
        if (sourceObject) {
            a[1] = sourceObject;
        } else {
            TSO_OBJECT_NEW_VALUE(source, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
            a[1] = object;
        }    
        topaz_script_object_reference_call(
            handler->fn, 
            TOPAZ_ARRAY_CAST(&a, topazScript_Object_t *, 2)
        );
        return 1;
    }

    topaz_script_object_reference_call(
        handler->fn, 
        TOPAZ_ARRAY_CAST(&a, topazScript_Object_t *, 1)
    );
    return 1;
}


TSO_SCRIPT_API_FN(component_api__install_event) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT); 

    TSOCHandlerData * data = calloc(1, sizeof(TSOCHandlerData));
    data->source = native;
    data->manager = context;
    data->script = script;
    data->fn = topaz_script_object_from_object(script, arg2);

    topaz_component_install_hook(
        native, 
        TOPAZ_STR_CAST("on-destroy"), 
        component_api_callback_cleanup, 
        data
    );

    //TODO: check callable??????
    topaz_component_install_event(
        native,
        topaz_script_object_as_string(arg1),
        component_api_callback,
        data
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__uninstall_event) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    //TODO: check callable??????
    topaz_component_uninstall_event(
        native,
        topaz_script_object_as_string(arg1)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__install_hook) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    TSOCHandlerData * data = calloc(1, sizeof(TSOCHandlerData));
    data->source = native;
    data->manager = context;
    data->script = script;
    data->fn = topaz_script_object_from_object(script, arg2);

    topaz_component_install_hook(
        native, 
        TOPAZ_STR_CAST("on-destroy"), 
        component_api_callback_cleanup, 
        data
    );


    //TODO: check callable??????
    return topaz_script_object_from_int(
        script,
        topaz_component_install_hook(
            native,
            topaz_script_object_as_string(arg1),
            component_api_callback,
            data
        )
    );


}


TSO_SCRIPT_API_FN(component_api__uninstall_hook) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    //TODO: check callable??????
    topaz_component_uninstall_hook(
        native,
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_int(arg2)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__install_handler) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    TSOCHandlerData * data = calloc(1, sizeof(TSOCHandlerData));
    data->source = native;
    data->manager = context;
    data->script = script;
    data->fn = topaz_script_object_from_object(script, arg2);

    topaz_component_install_hook(
        native, 
        TOPAZ_STR_CAST("on-destroy"), 
        component_api_callback_cleanup, 
        data
    );


    //TODO: check callable??????
    return topaz_script_object_from_int(
        script,
        topaz_component_install_handler(
            native,
            topaz_script_object_as_string(arg1),
            component_api_callback,
            data
        )
    );


}


TSO_SCRIPT_API_FN(component_api__uninstall_handler) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    //TODO: check callable??????
    topaz_component_uninstall_handler(
        native,
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_int(arg2)
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__set_on_step) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__COMPONENTCUSTOM);   
    TopazComponentTSO * e = topaz_component_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "component_api__set_on_step requires a callable object to be set.");
    } else {
        if (e->onStep) {
            topaz_script_object_destroy(e->onStep);
        }
        e->onStep = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(component_api__set_on_draw) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__COMPONENTCUSTOM);   
    TopazComponentTSO * e = topaz_component_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "component_api__set_on_draw requires a callable object to be set.");
    } else {
        if (e->onDraw) {
            topaz_script_object_destroy(e->onDraw);
        }
        e->onDraw = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__set_on_attach) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__COMPONENTCUSTOM);   
    TopazComponentTSO * e = topaz_component_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "component_api__set_on_attach requires a callable object to be set.");
    } else {
        if (e->onAttach) {
            topaz_script_object_destroy(e->onAttach);
        }
        e->onAttach = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__set_on_detach) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__COMPONENTCUSTOM);   
    TopazComponentTSO * e = topaz_component_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "component_api__set_on_detach requires a callable object to be set.");
    } else {
        if (e->onDetach) {
            topaz_script_object_destroy(e->onDetach);
        }
        e->onDetach = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(component_api__set_on_destroy) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__COMPONENTCUSTOM);   
    TopazComponentTSO * e = topaz_component_get_attributes(native)->userData;
    if (!(topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "component_api__set_on_destroy requires a callable object to be set.");
    } else {
        if (e->onDestroy) {
            topaz_script_object_destroy(e->onDestroy);
        }
        e->onDestroy = topaz_script_object_from_object(script, arg1);
    }    
    TSO_NO_RETURN;
}












// entity

TSO_SCRIPT_API_FN(entity_api__add_component) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE( topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    topaz_entity_add_component(
        native,
        native1
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(entity_api__add_component_after) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE( topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);   

    topaz_entity_add_component_after(
        native,
        native1
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(entity_api__get_component_count) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);

    uint32_t count = topaz_array_get_size(topaz_entity_get_components(native));
    return topaz_script_object_from_int(script, count);
}

TSO_SCRIPT_API_FN(entity_api__get_nth_component) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);

    int index = topaz_script_object_as_int(arg1);
    uint32_t count = topaz_array_get_size(topaz_entity_get_components(native));

    topazComponent_t * component;
    if (index < 0 || index >= count) {
        component = topaz_component_null();
    } else {
        component = topaz_array_at(topaz_entity_get_components(native), topazComponent_t *, index);
    }
    topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(component);
    if (a) return topaz_script_object_from_object(script, a);

    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT, NULL, NULL);
    return object;
}


TSO_SCRIPT_API_FN(entity_api__query_component) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);

    const topazString_t * str = topaz_script_object_as_string(arg1);

    topazComponent_t * component = topaz_entity_query_component(native, str);
    if (!component) component = topaz_component_null();

    topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(component);
    if (a) return topaz_script_object_from_object(script, a);

    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT, NULL, NULL);
    return object;
}

TSO_SCRIPT_API_FN(entity_api__remove_component) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_TYPE__ENTITY);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_TYPE__COMPONENT);

    topaz_entity_remove_component(native, native1);
    TSO_NO_RETURN;
}




static void add_refs__component_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_component__create", component_api__create);
    TS_MAP_NATIVE_FN("topaz_component__destroy", component_api__destroy);


    // attach / detach are removed for simplicity.
    // use topaz_entity__add/remove_component instead.
    TS_MAP_NATIVE_FN("topaz_component__step", component_api__step);
    TS_MAP_NATIVE_FN("topaz_component__draw", component_api__draw);


    TS_MAP_NATIVE_FN("topaz_component__get_stepping", component_api__get_stepping);
    TS_MAP_NATIVE_FN("topaz_component__get_drawing", component_api__get_drawing);
    TS_MAP_NATIVE_FN("topaz_component__set_stepping", component_api__set_stepping);
    TS_MAP_NATIVE_FN("topaz_component__set_drawing", component_api__set_drawing);
    TS_MAP_NATIVE_FN("topaz_component__set_tag", component_api__set_tag);
    TS_MAP_NATIVE_FN("topaz_component__get_tag", component_api__get_tag);
    TS_MAP_NATIVE_FN("topaz_component__get_host", component_api__get_host);
    TS_MAP_NATIVE_FN("topaz_component__null", component_api__null);

    TS_MAP_NATIVE_FN("topaz_component__emit_event", component_api__emit_event);
    TS_MAP_NATIVE_FN("topaz_component__emit_event_anonymous", component_api__emit_event_anonymous);
    TS_MAP_NATIVE_FN("topaz_component__can_handle_event", component_api__can_handle_event);

    TS_MAP_NATIVE_FN("topaz_component__install_event", component_api__install_event);
    TS_MAP_NATIVE_FN("topaz_component__uninstall_event", component_api__uninstall_event);
    TS_MAP_NATIVE_FN("topaz_component__install_hook", component_api__install_hook);
    TS_MAP_NATIVE_FN("topaz_component__uninstall_hook", component_api__uninstall_hook);
    TS_MAP_NATIVE_FN("topaz_component__install_handler", component_api__install_handler);
    TS_MAP_NATIVE_FN("topaz_component__uninstall_handler", component_api__uninstall_handler);

    TS_MAP_NATIVE_FN("topaz_component__set_on_attach", component_api__set_on_attach);
    TS_MAP_NATIVE_FN("topaz_component__set_on_detach", component_api__set_on_detach);
    TS_MAP_NATIVE_FN("topaz_component__set_on_destroy", component_api__set_on_destroy);
    TS_MAP_NATIVE_FN("topaz_component__set_on_step", component_api__set_on_step);
    TS_MAP_NATIVE_FN("topaz_component__set_on_draw", component_api__set_on_draw);



    // not implementing topaz_component_get_known_events at this time


    // component-relevant functions of the entity api are defined here 
    // since it requires the component wrapper creation function.

    TS_MAP_NATIVE_FN("topaz_entity__add_component", entity_api__add_component);
    TS_MAP_NATIVE_FN("topaz_entity__add_component_after", entity_api__add_component_after);
    TS_MAP_NATIVE_FN("topaz_entity__get_component_count", entity_api__get_component_count);
    TS_MAP_NATIVE_FN("topaz_entity__get_nth_component", entity_api__get_nth_component);
    TS_MAP_NATIVE_FN("topaz_entity__query_component", entity_api__query_component);
    TS_MAP_NATIVE_FN("topaz_entity__remove_component", entity_api__remove_component);

}
