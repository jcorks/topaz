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
#include <topaz/components/scheduler.h>
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







// processes tasks!
typedef struct {
    topaz_component_attribute_callback fn;
    void * data;

} TaskCallback;

typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif


    // context 
    topaz_t * ctx;

    // when pause is active, this will be nonzero.
    // pauseStartAt is the time at which pause was started.
    uint64_t pauseStartAt;
    


    /// Whether the scheduler is time or framebased
    int isTime;

    int repeat;

    /// only used for frames.
    uint64_t frameSrc;


    uint64_t stamp;
    uint64_t interval;
    uint64_t intervalDelay;
    uint64_t intervalInitial;
    TaskCallback callback;

} Scheduler;












static void scheduler__on_step(topazComponent_t * c, Scheduler * s) {
    if (s->pauseStartAt || s->repeat == -1) return;

    uint64_t timeNow;
    if (s->isTime) {
        timeNow = topaz_context_get_time(s->ctx);
    } else {
        s->frameSrc++;
        timeNow = s->frameSrc;
    }
    
    uint64_t marker = s->intervalDelay ? s->intervalDelay : s->interval;

    if (timeNow >= s->stamp + marker) {
        s->stamp += marker;
        s->intervalDelay = 0;
        s->callback.fn(c, s->callback.data);
        if (s->repeat == 0) s->repeat = -1;
    }
}

// free everything
static void scheduler__on_destroy(topazComponent_t * c, Scheduler * s) {
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



topazComponent_t * topaz_scheduler_create(
    topaz_t * t, 
    topazScheduler_Mode mode,
    int justOnce,
    uint32_t interval,
    uint32_t intervalDelay,


    topaz_component_attribute_callback callback,
    void * callbackData
    
) {
    Scheduler * data = calloc(1, sizeof(Scheduler));

    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__SCHEDULER;
    #endif

    data->ctx = t;
    data->isTime = mode == topazScheduler_Mode_Time;
    data->repeat = !justOnce;
    data->stamp = data->isTime ? topaz_context_get_time(data->ctx) : data->frameSrc;


    data->interval = interval;
    data->intervalDelay = intervalDelay;
    data->intervalInitial = intervalDelay;

    data->callback.fn = callback;
    data->callback.data = callbackData;


    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_step    = (topaz_component_attribute_callback) scheduler__on_step;
    attribs.on_destroy = (topaz_component_attribute_callback) scheduler__on_destroy;

    attribs.userData = data;

    return topaz_component_create_with_attributes(TOPAZ_STR_CAST("Scheduler"), t, &attribs);
}




void topaz_scheduler_pause(topazComponent_t * c) {
    Scheduler * s = scheduler__retrieve(c);
    s->pauseStartAt = s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc;
}

void topaz_scheduler_reset(topazComponent_t * c) {
    Scheduler * s = scheduler__retrieve(c);
    if (s->repeat == -1) s->repeat = 0;
    s->stamp = s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc;
    s->intervalDelay = s->intervalInitial;

}




void topaz_scheduler_resume(topazComponent_t * c) {
    Scheduler * s = scheduler__retrieve(c);
    if (!s->pauseStartAt) return;
    uint64_t diff = (s->isTime ? topaz_context_get_time(s->ctx) : s->frameSrc) - s->pauseStartAt;
    s->stamp += diff;
    s->pauseStartAt = 0;
}




uint64_t topaz_scheduler_get_task_interval_remaining(
    topazComponent_t * c
) {
    Scheduler * t = scheduler__retrieve(c);
    uint64_t nextEnd = (t->stamp + t->interval + t->intervalDelay);
    uint64_t timeNow = t->isTime ? topaz_context_get_time(t->ctx) : t->frameSrc;
    return nextEnd < timeNow ? 0 : nextEnd - timeNow;
}

