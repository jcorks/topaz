#include <topaz/components/scheduler.h>
#include <topaz/transform.h>

typedef struct {
    topazScript_Object_t * src;
    topazScript_Object_t * func;
} SchedulerData;

static void script_scheduler_callback(topazComponent_t * component, void * data) {
    SchedulerData * s = data;
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            s->func, 
            topaz_array_empty()
        )
    );

}



TSO_SCRIPT_API_FN(scheduler_api__create) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_ARG_4;

    if (!(topaz_script_object_reference_get_feature_mask(arg4) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "scheduler_api__start_rask requires a function object to be set.");
        TSO_NO_RETURN;
    } 
    SchedulerData * s = calloc(1, sizeof(SchedulerData));
    s->src = arg0;
    s->func = topaz_script_object_from_object(script, arg4);

    topazComponent_t * component = topaz_scheduler_create(
        ((topazScriptManager_t*)context)->ctx,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_int(arg2),
        topaz_script_object_as_int(arg3),
        script_scheduler_callback,
        s

    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__SCHEDULER, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}









TSO_SCRIPT_API_FN(scheduler_api__pause) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    topaz_scheduler_pause(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(scheduler_api__resume) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    topaz_scheduler_resume(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(scheduler_api__reset) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    topaz_scheduler_reset(native);
    TSO_NO_RETURN;
}






TSO_SCRIPT_API_FN(scheduler_api__get_task_interval_remaining) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    return topaz_script_object_from_int(
        script,
        topaz_scheduler_get_task_interval_remaining(native)
    );
}


static void add_refs__scheduler_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_scheduler__create", scheduler_api__create, 5);
    TS_MAP_NATIVE_FN("topaz_scheduler__pause", scheduler_api__pause, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__resume", scheduler_api__resume, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__reset", scheduler_api__reset, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__get_task_interval_remaining", scheduler_api__get_task_interval_remaining, 1);
}
