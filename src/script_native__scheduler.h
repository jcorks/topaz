#include <topaz/components/scheduler.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(scheduler_api__create) {
    TSO_ARG_0;
    topazComponent_t * component = topaz_scheduler_create(
        ((topazScriptManager_t*)context)->ctx,
        topaz_script_object_as_int(arg0)
    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__SCHEDULER, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}


typedef struct {
    topazScript_Object_t * src;
    topazScript_Object_t * func;
} SchedulerData;

static void script_scheduler_callback(topazComponent_t * component, void * data) {
    SchedulerData * s = data;
    topaz_script_object_destroy(
        topaz_script_object_reference_call(
            s->func, 
            TOPAZ_ARRAY_CAST(&s->src, topazScript_Object_t *, 1)
        )    
    );

}

TSO_SCRIPT_API_FN(scheduler_api__start_task) {
    TSO_ARG_0;
    TSO_ARG_1; // taskname
    TSO_ARG_2; // interval
    TSO_ARG_3; // intervalDelay
    TSO_ARG_4; // callback


    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);



    if (!(topaz_script_object_reference_get_feature_mask(arg4) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "scheduler_api__start_rask requires a function object to be set.");
        TSO_NO_RETURN;
    } 
    SchedulerData * s = calloc(1, sizeof(SchedulerData));
    s->src = arg0;
    s->func = topaz_script_object_from_object(script, arg4);

    topaz_scheduler_start_task(
        native,
        topaz_script_object_as_string(arg1),
        topaz_script_object_as_int(arg2),
        topaz_script_object_as_int(arg3),
        script_scheduler_callback,
        s
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(scheduler_api__start_task_simple) {
    TSO_ARG_0;
    TSO_ARG_1; // interval
    TSO_ARG_2; // callback


    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);


    if (!(topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Callable)) { 
        script_error(script, "scheduler_api__start_rask requires a function object to be set.");
        TSO_NO_RETURN;
    } 
    SchedulerData * s = calloc(1, sizeof(SchedulerData));
    s->src = arg0;
    s->func = topaz_script_object_from_object(script, arg2);


    return topaz_script_object_from_string(
        script,
        topaz_scheduler_start_task_simple(
            native,
            topaz_script_object_as_int(arg1),
            script_scheduler_callback,
            s
        )
    );
}



TSO_SCRIPT_API_FN(scheduler_api__end_task) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    topaz_scheduler_end_task(
        native,
        topaz_script_object_as_string(arg1)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(scheduler_api__get_task_count) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);
    return topaz_script_object_from_int(
        script,
        topaz_array_get_size(topaz_scheduler_get_tasks(native))
    );
}


TSO_SCRIPT_API_FN(scheduler_api__get_task) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    const topazArray_t * tasks = topaz_scheduler_get_tasks(native);
    int index = topaz_script_object_as_int(arg1);
    if (index < 0 || index >= topaz_array_get_size(tasks)) {
        TSO_NO_RETURN;        
    }

    return topaz_script_object_from_string(
        script,
        topaz_array_at(tasks, topazString_t *, index)
    );
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







TSO_SCRIPT_API_FN(scheduler_api__get_task_interval_remaining) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__SCHEDULER);

    return topaz_script_object_from_int(
        script,
        topaz_scheduler_get_task_interval_remaining(native, topaz_script_object_as_string(arg1))
    );
}


static void add_refs__scheduler_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_scheduler__create", scheduler_api__create, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__start_task", scheduler_api__start_task, 5);
    TS_MAP_NATIVE_FN("topaz_scheduler__start_task_simple", scheduler_api__start_task_simple, 3);


    TS_MAP_NATIVE_FN("topaz_scheduler__end_task", scheduler_api__end_task, 2);
    TS_MAP_NATIVE_FN("topaz_scheduler__get_task_count", scheduler_api__get_task_count, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__get_task", scheduler_api__get_task, 2);
    TS_MAP_NATIVE_FN("topaz_scheduler__pause", scheduler_api__pause, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__resume", scheduler_api__resume, 1);
    TS_MAP_NATIVE_FN("topaz_scheduler__get_task_interval_remaining", scheduler_api__get_task_interval_remaining, 2);
}
