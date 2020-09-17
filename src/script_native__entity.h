#include <topaz/entity.h>
#define TSO_OBJECT_ID__ENTITY 102

typedef struct {
    topazScript_Object_t * onAttach;
    topazScript_Object_t * onDetach;
    topazScript_Object_t * onRemove;
    topazScript_Object_t * onPreStep;
    topazScript_Object_t * onStep;
    topazScript_Object_t * onPreDraw;
    topazScript_Object_t * onDraw;

    topazScript_Object_t * position;
    topazScript_Object_t * rotation;
    topazScript_Object_t * scale;
} TopazScriptEntity;


static topaz_script_entity__on_attach(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onAttach) return;
    topaz_script_object_reference_call(scr->onAttach, topaz_array_empty());
}

static topaz_script_entity__on_detach(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onAttach) return;
    topaz_script_object_reference_call(scr->onDetach, topaz_array_empty());
}

static topaz_script_entity__on_remove(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onRemove) return;
    topaz_script_object_reference_call(scr->onRemove, topaz_array_empty());
}

static topaz_script_entity__on_pre_step(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onPreStep) return;
    topaz_script_object_reference_call(scr->onPreStep, topaz_array_empty());
}

static topaz_script_entity__on_step(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onStep) return;
    topaz_script_object_reference_call(scr->onStep, topaz_array_empty());
}

static topaz_script_entity__on_pre_draw(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onPreDraw) return;
    topaz_script_object_reference_call(scr->onPreDraw, topaz_array_empty());
}

static topaz_script_entity__on_draw(topazEntity_t * e, TopazScriptEntity * scr) {
    if (!scr->onDraw) return;
    topaz_script_object_reference_call(scr->onDraw, topaz_array_empty());
}




TSO_SCRIPT_API_CREATE__NATIVE_INST(entity_api__create_) {
    topazEntity_Attributes_t attribs;
    attribs.on_attach   = topaz_script_entity__on_attach;
    attribs.on_detach   = topaz_script_entity__on_detach;
    attribs.on_remove   = topaz_script_entity__on_remove;
    attribs.on_pre_step = topaz_script_entity__on_pre_step;
    attribs.on_step     = topaz_script_entity__on_step;
    attribs.on_pre_draw = topaz_script_entity__on_pre_draw;
    attribs.on_draw     = topaz_script_entity__on_draw;
    attribs.userData = calloc(1, sizeof(TopazScriptEntity));
    return topaz_entity_create_with_attributes(scr->ctx);

}

TSO_SCRIPT_API_CREATE__LINK(entity_api__create_empty) {
    // creates new object and sets native pointer
    TSO_OBJECT_NEW(userData, TSO_OBJECT_ID__ENTITY, NULL);

    // uses implicit names to add ref
    TSO_PROP_ADD("onAttach",  entity_api__on_attach_set, entity_api__on_attach_get);
    TSO_PROP_ADD("onDetach",  entity_api__on_detach_set, entity_api__on_detach_get);
    TSO_PROP_ADD("onRemove",  entity_api__on_remove_set, entity_api__on_remove_get);
    TSO_PROP_ADD("onStep",    entity_api__on_step_set,   entity_api__on_step_get);
    TSO_PROP_ADD("onDraw",    entity_api__on_draw_set,   entity_api__on_draw_get);
    TSO_PROP_ADD("onPreStep", entity_api__on_step_set,   entity_api__on_pre_step_get);
    TSO_PROP_ADD("onPreDraw", entity_api__on_draw_set,   entity_api__on_pre_draw_get);

    return object;
}

TSO_SCRIPT_API_FN(entity_api__create) {
    TSO_SCRIPT_API_CREATE(
        entity_
    )

}


static void add_refs__entity_api(topazScript_t * script) {
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

    TS_MAP_NATIVE_FN("topaz_entity__set_priority", entity_api__set_priority);
    TS_MAP_NATIVE_FN("topaz_entity__set_priority_last", entity_api__set_priority_last);
    TS_MAP_NATIVE_FN("topaz_entity__set_priority_first", entity_api__set_priority_first);
    TS_MAP_NATIVE_FN("topaz_entity__get_priority", entity_api__get_priority);

    TS_MAP_NATIVE_FN("topaz_entity__get_rotation", entity_api__get_rotation);
    TS_MAP_NATIVE_FN("topaz_entity__get_position", entity_api__get_position);
    TS_MAP_NATIVE_FN("topaz_entity__get_scale", entity_api__get_scale);

    TS_MAP_NATIVE_FN("topaz_entity__set_rotation", entity_api__set_rotation);
    TS_MAP_NATIVE_FN("topaz_entity__set_position", entity_api__set_position);
    TS_MAP_NATIVE_FN("topaz_entity__set_scale", entity_api__set_scale);

    TS_MAP_NATIVE_FN("topaz_entity__get_global_position", entity_api__get_global_position);
    TS_MAP_NATIVE_FN("topaz_entity__get_is_stepping", entity_api__get_is_stepping);
    TS_MAP_NATIVE_FN("topaz_entity__get_is_drawing", entity_api__get_is_drawing);
    TS_MAP_NATIVE_FN("topaz_entity__set_stepping", entity_api__set_stepping);
    TS_MAP_NATIVE_FN("topaz_entity__set_drawing", entity_api__set_drawing);
    TS_MAP_NATIVE_FN("topaz_entity__get_stepping", entity_api__get_stepping);
    TS_MAP_NATIVE_FN("topaz_entity__get_drawing", entity_api__get_drawing);

    TS_MAP_NATIVE_FN("topaz_entity__add_component", entity_api__add_component);
    TS_MAP_NATIVE_FN("topaz_entity__add_component_after", entity_api__add_component_after);
    TS_MAP_NATIVE_FN("topaz_entity__get_component_count", entity_api__get_component_count);
    TS_MAP_NATIVE_FN("topaz_entity__get_nth_component", entity_api__get_nth_component);
    TS_MAP_NATIVE_FN("topaz_entity__query_component", entity_api__query_component);
    TS_MAP_NATIVE_FN("topaz_entity__remove_component", entity_api__remove_component);
    TS_MAP_NATIVE_FN("topaz_entity__set_name", entity_api__set_name);
    TS_MAP_NATIVE_FN("topaz_entity__get_name", entity_api__get_name);




}