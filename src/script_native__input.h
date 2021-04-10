#include <topaz/modules/input.h>

typedef struct {
    topazScript_Object_t * obj;
    topazScript_t * script;
} ScriptInputListener;

static void script_input_on_press(topazInput_t * i, int input, void * data) {
    ScriptInputListener * l = data;
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(l->obj, TOPAZ_STR_CAST("onPress"));
    topazScript_Object_t * arg = topaz_script_object_from_int(l->script, input);

    topazScript_Object_t * args[2] = {
        l->obj,
        arg
    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn, 
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2)
        )
    );
    topaz_script_object_destroy(arg);
}

static void script_input_on_active(topazInput_t * i, int input, float value, void * data) {
    ScriptInputListener * l = data;
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(l->obj, TOPAZ_STR_CAST("onActive"));
    topazScript_Object_t * args[3] = {
        l->obj,
        topaz_script_object_from_int(l->script, input),
        topaz_script_object_from_number(l->script, value),
    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn, 
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 3)
        )
    );

    topaz_script_object_destroy(args[1]);
    topaz_script_object_destroy(args[2]);
}

static void script_input_on_release(topazInput_t * i, int input, void * data) {
    ScriptInputListener * l = data;
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(l->obj, TOPAZ_STR_CAST("onRelease"));
    topazScript_Object_t * arg = topaz_script_object_from_int(l->script, input);
    topazScript_Object_t * args[2] = {
        l->obj,
        arg
    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn, 
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2)
        )
    );
    topaz_script_object_destroy(arg);
}

static void script_input_on_update(topazInput_t * i, int input, float value, void * data) {
    ScriptInputListener * l = data;
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(l->obj, TOPAZ_STR_CAST("onUpdate"));
    topazScript_Object_t * args[3] = {
        l->obj,
        topaz_script_object_from_int(l->script, input),
        topaz_script_object_from_number(l->script, value),
    };


    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn, 
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 3)
        )
    );
    topaz_script_object_destroy(args[1]);
    topaz_script_object_destroy(args[2]);
}

static void script_input_on_new_unicode(topazInput_t * i, int input, void * data) {
    ScriptInputListener * l = data;
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(l->obj, TOPAZ_STR_CAST("onNewUnicode"));
    topazScript_Object_t * arg = topaz_script_object_from_int(l->script, input);

    topazScript_Object_t * args[2] = {
        l->obj,
        arg
    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn, 
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2)
        )
    );
    topaz_script_object_destroy(arg);
}

static void script_input_on_repeat_unicode(topazInput_t * i, int input, void * data) {
    ScriptInputListener * l = data;
    topazScript_Object_t * fn = topaz_script_object_reference_map_get_property(l->obj, TOPAZ_STR_CAST("onRepeatUnicode"));
    topazScript_Object_t * arg = topaz_script_object_from_int(l->script, input);

    topazScript_Object_t * args[2] = {
        l->obj,
        arg
    };

    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            fn, 
            TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2)
        )
    );
    topaz_script_object_destroy(arg);
}






TSO_SCRIPT_API_FN(input_api__add_keyboard_listener) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    ScriptInputListener * ldata = calloc(1, sizeof(ScriptInputListener));
    ldata->obj = topaz_script_object_from_object(script, arg0);
    ldata->script = script;
    topaz_script_object_reference_set_native_data(ldata->obj, ldata, TSO_OBJECT_ID__INPUTLISTENER);
    topazInput_Listener_t listener;

    listener.on_press = script_input_on_press;
    listener.on_active = script_input_on_active;
    listener.on_release = script_input_on_release;
    listener.on_update = script_input_on_update;
    listener.userData = ldata;
    topaz_input_add_keyboard_listener(
        input,
        &listener
    );

    return ldata->obj;
}


TSO_SCRIPT_API_FN(input_api__add_pointer_listener) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    ScriptInputListener * ldata = calloc(1, sizeof(ScriptInputListener));
    ldata->obj = topaz_script_object_from_object(script, arg0);
    ldata->script = script;
    topaz_script_object_reference_set_native_data(ldata->obj, ldata, TSO_OBJECT_ID__INPUTLISTENER);
    topazInput_Listener_t listener;

    listener.on_press = script_input_on_press;
    listener.on_active = script_input_on_active;
    listener.on_release = script_input_on_release;
    listener.on_update = script_input_on_update;
    listener.userData = ldata;
    topaz_input_add_pointer_listener(
        input,
        &listener
    );

    return ldata->obj;
}

TSO_SCRIPT_API_FN(input_api__add_pad_listener) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    ScriptInputListener * ldata = calloc(1, sizeof(ScriptInputListener));
    ldata->obj = topaz_script_object_from_object(script, arg0);
    ldata->script = script;
    topaz_script_object_reference_set_native_data(ldata->obj, ldata, TSO_OBJECT_ID__INPUTLISTENER);
    topazInput_Listener_t listener;

    listener.on_press = script_input_on_press;
    listener.on_active = script_input_on_active;
    listener.on_release = script_input_on_release;
    listener.on_update = script_input_on_update;
    listener.userData = ldata;
    topaz_input_add_pad_listener(
        input,
        &listener,
        topaz_script_object_as_int(arg1)
    );

    return ldata->obj;
}

TSO_SCRIPT_API_FN(input_api__add_mapped_listener) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    ScriptInputListener * ldata = calloc(1, sizeof(ScriptInputListener));
    ldata->obj = topaz_script_object_from_object(script, arg0);
    ldata->script = script;
    topaz_script_object_reference_set_native_data(ldata->obj, ldata, TSO_OBJECT_ID__INPUTLISTENER);
    topazInput_Listener_t listener;

    listener.on_press = script_input_on_press;
    listener.on_active = script_input_on_active;
    listener.on_release = script_input_on_release;
    listener.on_update = script_input_on_update;
    listener.userData = ldata;
    topaz_input_add_mapped_listener(
        input,
        &listener,
        topaz_script_object_as_string(arg1)
    );

    return ldata->obj;
}

TSO_SCRIPT_API_FN(input_api__remove_listener) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    int tag = 1;
    ScriptInputListener * ldata = topaz_script_object_reference_get_native_data(arg0, &tag);
    if (!ldata || tag != TSO_OBJECT_ID__INPUTLISTENER) {
        script_error(script, "Input object isnt an input listener.");
        TSO_NO_RETURN;
    }

    topazInput_Listener_t listener;
    listener.on_press = script_input_on_press;
    listener.on_active = script_input_on_active;
    listener.on_release = script_input_on_release;
    listener.on_update = script_input_on_update;
    listener.userData = ldata;
    topaz_input_remove_listener(input, &listener);

    topaz_script_object_destroy(ldata->obj);
    free(ldata);


    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(input_api__get_state) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_get_state(
            input,
            topaz_script_object_as_int(arg0)
        )
    );
}

TSO_SCRIPT_API_FN(input_api__get_pad_state) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_get_pad_state(
            input,
            topaz_script_object_as_int(arg0),
            topaz_script_object_as_int(arg1)
        )
    );
}

TSO_SCRIPT_API_FN(input_api__get_mapped_state) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_get_mapped_state(
            input,
            topaz_script_object_as_string(arg0)
        )
    );
}


TSO_SCRIPT_API_FN(input_api__set_deadzone) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    topaz_input_set_deadzone(
        input,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(input_api__query_pad_count) {
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_int(
        script, 
        topaz_array_get_size(
            topaz_input_query_pads(
                input
            )
        )
    );
}

TSO_SCRIPT_API_FN(input_api__query_pad_id) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    const topazArray_t * pads = topaz_input_query_pads(
        input
    );
    int index = topaz_script_object_as_int(arg0);
    if (index < 0 || index >= topaz_array_get_size(pads)) return topaz_script_object_from_int(script, -1);


    
    return topaz_script_object_from_int(
        script, 
        topaz_array_at(
            pads,
            int,
            index
        )
    );
}


TSO_SCRIPT_API_FN(input_api__add_unicode_listener) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    ScriptInputListener * ldata = calloc(1, sizeof(ScriptInputListener));
    ldata->obj = topaz_script_object_from_object(script, arg0);
    ldata->script = script;
    topaz_script_object_reference_set_native_data(ldata->obj, ldata, TSO_OBJECT_ID__UNICODELISTENER);
    topazInput_UnicodeListener_t listener;

    listener.on_new_unicode = script_input_on_new_unicode;
    listener.on_repeat_unicode = script_input_on_repeat_unicode;
    listener.userData = ldata;
    topaz_input_add_unicode_listener(
        input,
        &listener
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(input_api__remove_unicode_listener) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);

    int tag = 1;
    ScriptInputListener * ldata = topaz_script_object_reference_get_native_data(arg0, &tag);
    if (!ldata || tag != TSO_OBJECT_ID__INPUTLISTENER) {
        script_error(script, "Input object isnt an input listener.");
        TSO_NO_RETURN;
    }

    topazInput_UnicodeListener_t listener;
    listener.userData = ldata;
    listener.on_new_unicode = script_input_on_new_unicode;
    listener.on_repeat_unicode = script_input_on_repeat_unicode;
    topaz_input_remove_unicode_listener(input, &listener);

    topaz_script_object_destroy(ldata->obj);
    free(ldata);
    TSO_NO_RETURN;
}



TSO_SCRIPT_API_FN(input_api__mouse_x) {
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_mouse_x(
            input
        )
    );
}

TSO_SCRIPT_API_FN(input_api__mouse_y) {
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_mouse_y(
            input
        )
    );
}

TSO_SCRIPT_API_FN(input_api__mouse_delta_x) {
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_mouse_delta_x(
            input
        )
    );
}

TSO_SCRIPT_API_FN(input_api__mouse_delta_y) {
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_mouse_delta_y(
            input
        )
    );
}

TSO_SCRIPT_API_FN(input_api__mouse_wheel) {
    topazInput_t * input = topaz_context_get_input(((topazScriptManager_t*)context)->ctx);
    return topaz_script_object_from_number(
        script, 
        topaz_input_mouse_wheel(
            input
        )
    );
}







static void add_refs__input_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_input__add_keyboard_listener", input_api__add_keyboard_listener);
    TS_MAP_NATIVE_FN("topaz_input__add_pointer_listener",  input_api__add_pointer_listener);
    TS_MAP_NATIVE_FN("topaz_input__add_pad_listener",      input_api__add_pad_listener);
    TS_MAP_NATIVE_FN("topaz_input__add_mapped_listener",   input_api__add_mapped_listener);
    TS_MAP_NATIVE_FN("topaz_input__remove_listener",       input_api__remove_listener);    

    TS_MAP_NATIVE_FN("topaz_input__get_state", input_api__get_state);
    TS_MAP_NATIVE_FN("topaz_input__get_pad_state", input_api__get_pad_state);
    TS_MAP_NATIVE_FN("topaz_input__get_mapped_state", input_api__get_mapped_state);

    TS_MAP_NATIVE_FN("topaz_input__set_deadzone", input_api__set_deadzone);
    TS_MAP_NATIVE_FN("topaz_input__query_pad_count", input_api__query_pad_count);
    TS_MAP_NATIVE_FN("topaz_input__query_pad_id", input_api__query_pad_id);

    TS_MAP_NATIVE_FN("topaz_input__add_unicode_listener", input_api__add_unicode_listener);
    TS_MAP_NATIVE_FN("topaz_input__remove_unicode_listener", input_api__remove_unicode_listener);

    TS_MAP_NATIVE_FN("topaz_input__mouse_x", input_api__mouse_x);
    TS_MAP_NATIVE_FN("topaz_input__mouse_y", input_api__mouse_y);
    TS_MAP_NATIVE_FN("topaz_input__mouse_delta_x", input_api__mouse_delta_x);
    TS_MAP_NATIVE_FN("topaz_input__mouse_delta_y", input_api__mouse_delta_y);
    TS_MAP_NATIVE_FN("topaz_input__mouse_wheel", input_api__mouse_wheel); 

}

