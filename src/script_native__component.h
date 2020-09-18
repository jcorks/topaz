#include <topaz/component.h>

typedef struct {
    void * childClass;
    int childClassID;
    topazScript_Object_t * self;
} TopazCompenentTSO;

TSO_SCRIPT_API_FN(component_api__create) {
    topazComponent_t * component = topaz_component_create(((topazScriptManager_t*)context)->ctx);
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
    tsoData->self = object;

    if (topaz_script_object_reference_get_feature_mask(object) & topazScript_Object_Feature_Map) {
        topaz_entity_set_attributes(entity, &attribs);
    } else {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: script implementation does not support topazScript_Object_Feature_Map for its empty object. This limits the use of many scripting features.\n");
        #endif
    }

    TSO_OBJECT_KEEP;

    return object;
}

static void add_regs__component_api() {
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
    TS_MAP_NATIVE_FN("topaz_component__null", component_api__null;

    TS_MAP_NATIVE_FN("topaz_component__emit_event", component_api__emit_event);
    TS_MAP_NATIVE_FN("topaz_component__emit_event_anonymous", component_api__emit_event_anonymous);
    TS_MAP_NATIVE_FN("topaz_component__can_handle_event", component_api__can_handle_event);

    TS_MAP_NATIVE_FN("topaz_component__install_event", component_api__install_event);
    TS_MAP_NATIVE_FN("topaz_component__uninstall_event", component_api__uninstall_event);
    TS_MAP_NATIVE_FN("topaz_component__install_hook", component_api__install_hook);
    TS_MAP_NATIVE_FN("topaz_component__uninstall_hook", component_api__uninstall_hook);
    TS_MAP_NATIVE_FN("topaz_component__install_handler", component_api__install_handler);
    TS_MAP_NATIVE_FN("topaz_component__uninstall_handler", component_api__uninstall_handler);

    TS_MAP_NATIVE_FN("topaz_component__get_known_events_count", component_api__get_known_events_count);
    TS_MAP_NATIVE_FN("topaz_component__get_known_events_nth", component_api__get_known_events_nth);


    // component-relevant functions of the entity api are defined here 
    // since it requires the component wrapper creation function.

    TS_MAP_NATIVE_FN("topaz_entity__add_component", entity_api__add_component);
    TS_MAP_NATIVE_FN("topaz_entity__add_component_after", entity_api__add_component_after);
    TS_MAP_NATIVE_FN("topaz_entity__get_component_count", entity_api__get_component_count);
    TS_MAP_NATIVE_FN("topaz_entity__get_nth_component", entity_api__get_nth_component);
    TS_MAP_NATIVE_FN("topaz_entity__query_component", entity_api__query_component);
    TS_MAP_NATIVE_FN("topaz_entity__remove_component", entity_api__remove_component);

}
