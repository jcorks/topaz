/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/

#include <topaz/compat.h>
#include <topaz/components/state_control.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/topaz.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all components 
// using state control functions are actually what they claim to be
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__SCHEDULER = "t0p4zsch3dul3r";
#endif








typedef struct {
    topazString_t * name;
    uint64_t stamp;
    uint64_t interval;
    uint64_t intervalDelay;
    topaz_component_attribute_callback cb;
    void * cbData;
} Task;


typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif

    // low volume, no need for str hash
    topazArray_t * tasks;

    // context 
    topaz_t * ctx;

    // when pause is active, this will be nonzero.
    // pauseStartAt is the time at which pause was started.
    uint64_t pauseStartAt;
    

    // Marks whether the names array will need updates before 
    // requested next
    int needsNameUpdate;

    /// Whether the scheduler is time or framebased
    int isTime;

    /// only used for frames.
    uint64_t frameSource;

    topazArray_t * names;
} Scheduler;




Task * scheduler__task_new(Scheduler * s, const topazString_t * name) {
    Task * out = calloc(1, sizeof(Task));
    out->name = topaz_string_clone(name);
    topaz_array_push(s->tasks, out);
    s->needsNameUpdate = TRUE;
    return out;
}

void scheduler__task_destroy(Task * s) {
    topaz_string_destroy(s->name);
    free(s);
}

Task * scheduler__task_get(Scheduler * s, const topazString_t * name) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->tasks); ++i) {
        if (topaz_string_test_eq(topaz_array_at(s->tasks, Task*, i)->name, name)) {
            return topaz_array_at(s->tasks, Task*, i);
        }
    }
    return NULL;
}


void scheduler__task_remove(Scheduler * s, const topazString_t * name) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->tasks); ++i) {
        Task * t = topaz_array_at(s->tasks, Task*, i);
        if (topaz_string_test_eq(t->name, name)) {
            scheduler__task_destroy(t);
            topaz_array_remove(s->tasks, i);
            return;
        }
    }
}











// processes tasks!
static void scheduler__on_step(topazComponent_t * c, Scheduler * s) {
    if (s->pauseStartAt) return;

    uint32_t i;
    uint64_t timeNow;
    if (s->isTime) {
        timeNow = topaz_context_get_time(s->ctx);
    } else {
        s->frameSrc++;
        timeNow = s->frameSrc;
    }


    
    for(i = 0; i < topaz_array_get_size(s->tasks); ++i) {
        Task * t = topaz_array_at(s->tasks, Task *, i);
        
        if (t->stamp + t->interval + t->intervalDelay >= timeNow) {
            t->cb(c, t->cbData);
            t->stamp = timeNow;
            t->intervalDelay = 0;
        }
    }
}

// free everything
static void scheduler__on_destroy(topazComponent_t * c, Scheduler * s) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->tasks); ++i) {
        scheduler__task_destroy(topaz_array_at(s->tasks, Task*, i));
    }
    topaz_array_destroy(s->tasks);
    free(s);
}



// retrieves the component and asserts accuracy in debug
static Scheduler * scheduler__retrieve(topazComponent_t * c) {
    Scheduler * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "Scheduler instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__SCHEDULER);
    #endif
    return s;
}



topazComponent_t * topaz_scheduler_create(topaz_t * t, topazScheduler_Mode mode) {
    Scheduler * data = calloc(1, sizeof(Scheduler));

    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__SCHEDULER;
    #endif

    data->tasks = topaz_array_create(sizeof(Scheduler));
    data->ctx = t;
    data->isTime = mode == topazScheduler_Mode_Time;

    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_step    = (topaz_component_attribute_callback) scheduler__on_step;
    attribs.on_destroy = (topaz_component_attribute_callback) scheduler__on_destroy;

    attribs.userData = data;
    return topaz_component_create_with_attributes(TOPAZ_STR_CAST("Scheduler"), t, &attribs);
}


void topaz_scheduler_start_task(
    topazComponent_t *      c, 
    const topazString_t *   taskName, 
    uint32_t                interval,
    uint32_t                intervalDelay,

    topaz_component_attribute_callback callback,
    void *                             callbackData
) {
    Scheduler * s = scheduler__retrieve(c);

    
    Task * t = scheduler__task_new(s, taskName);
    t->interval = interval;
    t->stamp = s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc;
    t->interval = interval;
    t->intervalDelay = intervalDelay;

    t->cb = callback;
    t->cbData = callbackData;

    
}



const topazString_t * topaz_scheduler_start_task_simple(
    topazComponent_t * c,
    uint32_t interval,

    topaz_component_attribute_callback callback,
    void *                             callbackData
) {
    static int id=0;

    Scheduler * s = scheduler__retrieve(c);

    topazString_t * tag = topaz_string_create_from_c_str("_SCHTASK%d", id++);
    topaz_scheduler_start_task(c, tag, interval, 0, callback, callbackData);
    topaz_string_destroy(tag);

    Task * task = scheduler__task_get(s, tag);

    if (!task) return TOPAZ_STR_CAST("");
    return task->name;
}




void topaz_scheduler_end_task(topazComponent_t * c, const topazString_t * name) {
    Scheduler * s = scheduler__retrieve(c);
    scheduler__task_remove(s, name);
}


void topaz_scheduler_pause(topazComponent_t * c) {
    Scheduler * s = scheduler__retrieve(c);
    s->pauseStartAt = s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc;
}


void topaz_scheduler_resume(topazComponent_t * c) {
    Scheduler * s = scheduler__retrieve(c);
    uint32_t i;
    uint64_t diff = (s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc) - s->pauseStartAt;
    for(i = 0; i < topaz_array_get_size(s->tasks); ++i) {
        topaz_array_at(s->tasks, Task *, i)->stamp += diff;
    }
    s->pauseStartAt = 0;
}

uint64_t topaz_scheduler_get_task_interval_remaining(
    topazComponent_t * scheduler,
    const topazString_t * name
) {
    Scheduler * s = scheduler__retrieve(c);
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->tasks); ++i) {
        Task * t = topaz_array_at(s->tasks, Task*, i);
        if (topaz_string_test_eq(t->name, name)) {
            uint64_t nextEnd = (t->stamp + t->interval + t->intervalDelay);
            uint64_t timeNow = s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc;
            return nextEnd < timeNow ? 0 : timeNow - nextEnd;
        }
    }

}

