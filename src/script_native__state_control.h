#include <topaz/components/state_control.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(state_control_api__create) {
    topazComponent_t * component = topaz_state_control_create(
        ((topazScriptManager_t*)context)->ctx
    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__STATECONTROL, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}


typedef struct {
    topazScript_Object_t * onStep;
    topazScript_Object_t * onDraw;
    topazScript_Object_t * onInit;    

    topazScript_Object_t * src;
} ScriptStateLoopData;

static void script_state_control__on_step(topazComponent_t * component, void * data) {
    ScriptStateLoopData * s = data;
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            s->onStep, 
            TOPAZ_ARRAY_CAST(&s->src, topazScript_Object_t *, 1)
        )    
    );
}

static void script_state_control__on_draw(topazComponent_t * component, void * data) {
    ScriptStateLoopData * s = data;
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            s->onDraw, 
            TOPAZ_ARRAY_CAST(&s->src, topazScript_Object_t *, 1)
        )    
    );
}

static void script_state_control__on_init(topazComponent_t * component, void * data) {
    ScriptStateLoopData * s = data;
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            s->onInit, 
            TOPAZ_ARRAY_CAST(&s->src, topazScript_Object_t *, 1)
        )    
    );
}


TSO_SCRIPT_API_FN(state_control_api__add) {
    TSO_ARG_0;
    TSO_ARG_1; // state name 

    TSO_ARG_2; // onStep
    TSO_ARG_3; // onDraw
    TSO_ARG_4; // onInit



    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__STATECONTROL);
    topazStateControl_Loop_t loop = {};
    ScriptStateLoopData * s = calloc(1, sizeof(ScriptStateLoopData));
    loop.data = s;

    s->src = arg0;
    if ((topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Callable)) { 
        s->onStep = topaz_script_object_from_object(script, arg2);
        loop.on_step = script_state_control__on_step;
    } 

    if ((topaz_script_object_reference_get_feature_mask(arg3) & topazScript_Object_Feature_Callable)) { 
        s->onDraw = topaz_script_object_from_object(script, arg3);
        loop.on_draw = script_state_control__on_draw;
    } 

    if ((topaz_script_object_reference_get_feature_mask(arg4) & topazScript_Object_Feature_Callable)) { 
        s->onInit = topaz_script_object_from_object(script, arg4);
        loop.on_init = script_state_control__on_init;
    } 




    topaz_state_control_add(
        native,
        topaz_script_object_as_string(arg1),
        loop
    );

    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(state_control_api__remove) {
    TSO_ARG_0;
    TSO_ARG_1; // state name 

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__STATECONTROL);

    topaz_state_control_remove(
        native,
        topaz_script_object_as_string(arg1)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(state_control_api__execute) {
    TSO_ARG_0;
    TSO_ARG_1; // state name 

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__STATECONTROL);

    topaz_state_control_execute(
        native,
        topaz_script_object_as_string(arg1)
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(state_control_api__halt) {
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__STATECONTROL);

    topaz_state_control_halt(
        native
    );

    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(state_control_api__is_halted) {
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__STATECONTROL);

    return topaz_script_object_from_int(
        script,
        topaz_state_control_is_halted(
            native
        )
    );
}

TSO_SCRIPT_API_FN(state_control_api__get_current) {
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__STATECONTROL);

    return topaz_script_object_from_string(
        script,
        topaz_state_control_get_current(
            native
        )
    );
}




static void add_refs__state_control_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_state_control__create", state_control_api__create, 0);
    TS_MAP_NATIVE_FN("topaz_state_control__add", state_control_api__add, 5);
    TS_MAP_NATIVE_FN("topaz_state_control__remove", state_control_api__remove, 2);
    TS_MAP_NATIVE_FN("topaz_state_control__execute", state_control_api__execute, 2);
    TS_MAP_NATIVE_FN("topaz_state_control__halt", state_control_api__halt, 1);
    TS_MAP_NATIVE_FN("topaz_state_control__is_halted", state_control_api__is_halted, 1);
    TS_MAP_NATIVE_FN("topaz_state_control__get_current", state_control_api__get_current, 1);



}
