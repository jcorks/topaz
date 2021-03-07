#include <topaz/components/automation.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(automation_api__create) {
    topazComponent_t * component = topaz_automation_create(
        ((topazScriptManager_t*)context)->ctx
    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__AUTOMATION, NULL, NULL);
    return object;
}





TSO_SCRIPT_API_FN(automation_api__add_keyframe) {
    TSO_ASSERT_ARG_COUNT(4);
    TSO_ARG_0;
    TSO_ARG_1; // value
    TSO_ARG_2; // lerpfn
    TSO_ARG_3; // offset

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    topaz_automation_add_keyframe(
        native,
        topaz_script_object_as_number (arg1),
        topaz_script_object_as_int    (arg2),
        topaz_script_object_as_number (arg3)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__add_vector_keyframe) {
    TSO_ASSERT_ARG_COUNT(4);
    TSO_ARG_0;
    TSO_ARG_1; // value
    TSO_ARG_2; // lerpfn
    TSO_ARG_3; // offset

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   
    topaz_automation_add_vector_keyframe(
        native,
        native1,
        topaz_script_object_as_int    (arg2),
        topaz_script_object_as_number (arg3)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__clear) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    topaz_automation_clear(
        native
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(automation_api__add_automation) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_add_automation(
        native,
        native1
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(automation_api__blend) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_blend(
        native,
        native1
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__smooth) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_smooth(
        native
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__add_from_string) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_add_from_string(
        native,
        topaz_script_object_as_string(arg1)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__get_length) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);


    return topaz_script_object_from_number(
        script,
        topaz_automation_get_length(
            native
        )
    );
}

TSO_SCRIPT_API_FN(automation_api__skip_to) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_skip_to(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__set_duration_frames) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_set_duration_frames(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(automation_api__set_duration_seconds) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_set_duration_seconds(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(automation_api__get_duration) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);


    return topaz_script_object_from_number(
        script,
        topaz_automation_get_duration(
            native
        )
    );
}


TSO_SCRIPT_API_FN(automation_api__set_looped) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_set_looped(
        native,
        topaz_script_object_as_int(
            arg1
        )
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__get_looped) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    return topaz_script_object_from_int(
        script,
        topaz_automation_get_looped(
            native
        )
    );
}

TSO_SCRIPT_API_FN(automation_api__set_speed) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_set_speed(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__get_speed) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    return topaz_script_object_from_number(
        script,
        topaz_automation_get_speed(
            native
        )
    );
}

TSO_SCRIPT_API_FN(automation_api__pause) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    topaz_automation_pause(
        native
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(automation_api__resume) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);
    topaz_automation_resume(
        native
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(automation_api__to_string) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    return topaz_script_object_from_string(
        script,
        topaz_automation_to_string(
            native
        )
    );
}

TSO_SCRIPT_API_FN(automation_api__set_from_string) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topaz_automation_set_from_string(
        native,
        topaz_script_object_as_string(
            arg1
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(automation_api__vector_at) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = topaz_automation_vector_at(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );
    return out;
}


TSO_SCRIPT_API_FN(automation_api__at) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);


    return topaz_script_object_from_number(
        script,
        topaz_automation_at(
            native,
            topaz_script_object_as_number(
                arg1
            )
        )
    );
}




TSO_SCRIPT_API_FN(automation_api__current_vector) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);

    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_automation_current_vector(
        native
    );
    return out;
}


TSO_SCRIPT_API_FN(automation_api__current) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__AUTOMATION);


    return topaz_script_object_from_number(
        script,
        topaz_automation_current(
            native
        )
    );
}







static void add_refs__automation_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_automation__create", automation_api__create);
    TS_MAP_NATIVE_FN("topaz_automation__add_keyframe", automation_api__add_keyframe);
    TS_MAP_NATIVE_FN("topaz_automation__add_vector_keyframe", automation_api__add_vector_keyframe);
    TS_MAP_NATIVE_FN("topaz_automation__clear", automation_api__clear);
    TS_MAP_NATIVE_FN("topaz_automation__add_automation", automation_api__add_automation);
    TS_MAP_NATIVE_FN("topaz_automation__blend", automation_api__blend);
    TS_MAP_NATIVE_FN("topaz_automation__smooth", automation_api__smooth);
    TS_MAP_NATIVE_FN("topaz_automation__add_from_string", automation_api__add_from_string);
    TS_MAP_NATIVE_FN("topaz_automation__get_length", automation_api__get_length);
    TS_MAP_NATIVE_FN("topaz_automation__skip_to", automation_api__skip_to);
    TS_MAP_NATIVE_FN("topaz_automation__set_duration_seconds", automation_api__set_duration_seconds);
    TS_MAP_NATIVE_FN("topaz_automation__set_duration_frames", automation_api__set_duration_frames);
    TS_MAP_NATIVE_FN("topaz_automation__get_duration", automation_api__get_duration);
    TS_MAP_NATIVE_FN("topaz_automation__set_looped", automation_api__set_looped);
    TS_MAP_NATIVE_FN("topaz_automation__get_looped", automation_api__get_looped);
    TS_MAP_NATIVE_FN("topaz_automation__set_speed", automation_api__set_speed);
    TS_MAP_NATIVE_FN("topaz_automation__get_speed", automation_api__get_speed);
    TS_MAP_NATIVE_FN("topaz_automation__pause", automation_api__pause);
    TS_MAP_NATIVE_FN("topaz_automation__resume", automation_api__resume);
    TS_MAP_NATIVE_FN("topaz_automation__to_string", automation_api__to_string);
    TS_MAP_NATIVE_FN("topaz_automation__set_from_string", automation_api__set_from_string);
    TS_MAP_NATIVE_FN("topaz_automation__vector_at", automation_api__vector_at);
    TS_MAP_NATIVE_FN("topaz_automation__at", automation_api__at);
    TS_MAP_NATIVE_FN("topaz_automation__current_vector", automation_api__current_vector);
    TS_MAP_NATIVE_FN("topaz_automation__current", automation_api__current);
    
}
