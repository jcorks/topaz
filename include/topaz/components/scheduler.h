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


#ifndef H_TOPAZDC__SCHEDULER__INCLUDED
#define H_TOPAZDC__SCHEDULER__INCLUDED

#include <topaz/component.h>

/// Allows for running of functions at certain intervals.
///
typedef struct topazScheduler_t topazScheduler_t;


/// Schedulers can measure intervals in different ways.
/// This enum allows for creating schedulers with these 
/// different intervals.
typedef enum topazScheduler_Mode topazScheduler_Mode;
enum topazScheduler_Mode {
    /// The scheduler will measure intervals in milliseconds.
    topazScheduler_Mode_Time,

    /// The scheduler will measure intervals in update frames.
    /// Every step of the component increments the frame.
    topazScheduler_Mode_Frame
}


/// Creates a new state control component.
/// Use component_destroy to release.
///
topazComponent_t * topaz_scheduler_create(
    /// The topaz context.
    topaz_t * context
);


/// Assigns a new task for the scheduler to process. Once 
/// the interval time has elapsed, the callback will be called.
/// Callback is called when the interval expires with the given callbackData
/// as argument.
void topaz_scheduler_start_task(
    /// The scheduler to add a task to.
    topazComponent_t * scheduler, 

    /// The name of the task to add.
    const topazString_t *   taskName, 

    /// How often the task will run. If the mode is topazScheduler_Mode_Time,
    /// the interval is a time in milliseconds. If the mode is 
    /// topazScheduler_Mode_Frame, the the interval is the number 
    /// of frames.
    uint32_t interval,

    /// The initial delay before the task starts being measured.
    uint32_t intervalDelay,

    /// The callback to call when the task's interval is reached.
    topaz_component_attribute_callback callback,

    /// The data to bind to the callback.
    void * callbackData
);


/// Same as topaz_scheduler_start_task except will simplified inputs:
/// - No interval delay is given
/// - a default name is generated 
const topazString_t * topaz_scheduler_start_task_simple(
    /// The scheduler to add a task to.
    topazComponent_t * scheduler, 

    /// How often the task will run. If the mode is topazScheduler_Mode_Time,
    /// the interval is a time in milliseconds. If the mode is 
    /// topazScheduler_Mode_Frame, the the interval is the number 
    /// of frames.
    uint32_t interval,

    /// The callback to call when the task's interval is reached.
    topaz_component_attribute_callback callback,

    /// The data to bind to the callback.
    void * callbackData
);


/// Halts the task with the given name.
/// If no such task exists, no action is taken.
///
void topaz_scheduler_end_task(
    /// The scheduler to remove a task from.
    topazComponent_t * scheduler,

    /// The name of the task to remove. 
    const topazString_t * name
);


/// Gets all tasks being maintained for the scheduler
/// A read-only array of topazString_t * is returned.
const topazArray_t * topaz_scheduler_get_tasks(
    /// The scheduler to query.
    topazComponent_t * scheduler
);


/// Stops processing and timing of tasks until topaz_scheduler_resume() is called.
///
void topaz_scheduler_pause(
    /// The scheduler to pause.
    topazComponent_t * scheduler
);


/// Resumes processing of tasks once more. If the scheduler was not 
/// paused, no action is taken.
void topaz_scheduler_resume(
    /// The scheduler to resume.
    topazComponent_t * scheduler
);


/// Gets the interval amount before the task interval
/// is next complete.
uint64_t topaz_scheduler_get_task_interval_remaining(
    /// The scheduler to query.
    topazComponent_t * scheduler,

    /// The name of the task to query.
    const topazString_t * name
);






#endif
