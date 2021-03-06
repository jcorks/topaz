#include <topaz/components/animator.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(animator_api__create) {
    topazComponent_t * component = topaz_animator_create(
        ((topazScriptManager_t*)context)->ctx
    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__ANIMATOR, NULL, NULL);
    return object;
}





TSO_SCRIPT_API_FN(animator_api__add_keyframe) {
    TSO_ASSERT_ARG_COUNT(4);
    TSO_ARG_0;
    TSO_ARG_1; // value
    TSO_ARG_2; // lerpfn
    TSO_ARG_3; // offset

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    topaz_animator_add_keyframe(
        native,
        topaz_script_object_as_number (arg1),
        topaz_script_object_as_int    (arg2),
        topaz_script_object_as_number (arg3)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__add_vector_keyframe) {
    TSO_ASSERT_ARG_COUNT(4);
    TSO_ARG_0;
    TSO_ARG_1; // value
    TSO_ARG_2; // lerpfn
    TSO_ARG_3; // offset

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    TSO_NATIVIZE_1(topazVector_t *, TSO_OBJECT_ID__VECTOR);   
    topaz_animator_add_vector_keyframe(
        native,
        native1,
        topaz_script_object_as_int    (arg2),
        topaz_script_object_as_number (arg3)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__clear) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    topaz_animator_clear(
        native
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(animator_api__add_animation) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_add_animation(
        native,
        native1
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(animator_api__blend) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    TSO_NATIVIZE_1(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_blend(
        native,
        native1
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__smooth) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_smooth(
        native
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__add_from_string) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_add_from_string(
        native,
        topaz_script_object_as_string(arg1)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__get_length) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);


    return topaz_script_object_from_number(
        script,
        topaz_animator_get_length(
            native
        )
    );
}

TSO_SCRIPT_API_FN(animator_api__skip_to) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_skip_to(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__set_duration_frames) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_set_duration_frames(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(animator_api__set_duration_seconds) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_set_duration_seconds(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(animator_api__get_duration) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);


    return topaz_script_object_from_number(
        script,
        topaz_animator_get_duration(
            native
        )
    );
}


TSO_SCRIPT_API_FN(animator_api__set_looped) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_set_looped(
        native,
        topaz_script_object_as_int(
            arg1
        )
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__get_looped) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    return topaz_script_object_from_int(
        script,
        topaz_animator_get_looped(
            native
        )
    );
}

TSO_SCRIPT_API_FN(animator_api__set_speed) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_set_speed(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__get_speed) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    return topaz_script_object_from_number(
        script,
        topaz_animator_get_speed(
            native
        )
    );
}

TSO_SCRIPT_API_FN(animator_api__pause) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    topaz_animator_pause(
        native
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(animator_api__resume) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);
    topaz_animator_resume(
        native
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(animator_api__to_string) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    return topaz_script_object_from_string(
        script,
        topaz_animator_to_string(
            native
        )
    );
}

TSO_SCRIPT_API_FN(animator_api__set_from_string) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topaz_animator_set_from_string(
        native,
        topaz_script_object_as_string(
            arg1
        )
    );

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(animator_api__vector_at) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = topaz_animator_vector_at(
        native,
        topaz_script_object_as_number(
            arg1
        )
    );
    return out;
}


TSO_SCRIPT_API_FN(animator_api__at) {
    TSO_ASSERT_ARG_COUNT(2);
    TSO_ARG_0;
    TSO_ARG_1;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);


    return topaz_script_object_from_number(
        script,
        topaz_animator_at(
            native,
            topaz_script_object_as_number(
                arg1
            )
        )
    );
}




TSO_SCRIPT_API_FN(animator_api__current_vector) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);

    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = *topaz_animator_current_vector(
        native
    );
    return out;
}


TSO_SCRIPT_API_FN(animator_api__current) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__ANIMATOR);


    return topaz_script_object_from_number(
        script,
        topaz_animator_current(
            native
        )
    );
}







static void add_refs__animator_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_animator__create", animator_api__create);
    TS_MAP_NATIVE_FN("topaz_animator__add_keyframe", animator_api__add_keyframe);
    TS_MAP_NATIVE_FN("topaz_animator__add_vector_keyframe", animator_api__add_vector_keyframe);
    TS_MAP_NATIVE_FN("topaz_animator__clear", animator_api__clear);
    TS_MAP_NATIVE_FN("topaz_animator__add_animation", animator_api__add_animation);
    TS_MAP_NATIVE_FN("topaz_animator__blend", animator_api__blend);
    TS_MAP_NATIVE_FN("topaz_animator__smooth", animator_api__smooth);
    TS_MAP_NATIVE_FN("topaz_animator__add_from_string", animator_api__add_from_string);
    TS_MAP_NATIVE_FN("topaz_animator__get_length", animator_api__get_length);
    TS_MAP_NATIVE_FN("topaz_animator__skip_to", animator_api__skip_to);
    TS_MAP_NATIVE_FN("topaz_animator__set_duration_seconds", animator_api__set_duration_seconds);
    TS_MAP_NATIVE_FN("topaz_animator__set_duration_frames", animator_api__set_duration_frames);
    TS_MAP_NATIVE_FN("topaz_animator__get_duration", animator_api__get_duration);
    TS_MAP_NATIVE_FN("topaz_animator__set_looped", animator_api__set_looped);
    TS_MAP_NATIVE_FN("topaz_animator__get_looped", animator_api__get_looped);
    TS_MAP_NATIVE_FN("topaz_animator__set_speed", animator_api__set_speed);
    TS_MAP_NATIVE_FN("topaz_animator__get_speed", animator_api__get_speed);
    TS_MAP_NATIVE_FN("topaz_animator__pause", animator_api__pause);
    TS_MAP_NATIVE_FN("topaz_animator__resume", animator_api__resume);
    TS_MAP_NATIVE_FN("topaz_animator__to_string", animator_api__to_string);
    TS_MAP_NATIVE_FN("topaz_animator__set_from_string", animator_api__set_from_string);
    TS_MAP_NATIVE_FN("topaz_animator__vector_at", animator_api__vector_at);
    TS_MAP_NATIVE_FN("topaz_animator__at", animator_api__at);
    TS_MAP_NATIVE_FN("topaz_animator__current_vector", animator_api__current_vector);
    TS_MAP_NATIVE_FN("topaz_animator__current", animator_api__current);
    
}
