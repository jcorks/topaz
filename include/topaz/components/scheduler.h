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

/*

    Scheduler
    -----

    Allows for running of functions at certain intervals.

*/



/// Creates a new state control component.
/// Use component_destroy to release.
///
topazComponent_t * topaz_scheduler_create(topaz_t *);


/// Assigns a new task for the scheduler to process. Once 
/// the interval time has elapsed, the callback will be called.
/// taskName is a name given to identify the task later. 
/// interval is how often the function is run in MS. 
/// intervalDelay is how long to wait before actually starting the task.
///
/// Callback is called when the interval expires with the given callbackData
/// as argument.
void topaz_scheduler_start_task(
    topazComponent_t *     , 
    const topazString_t *   taskName, 
    uint32_t                interval,
    uint32_t                intervalDelay,

    topaz_component_attribute_callback callback,
    void *                             callbackData
);


/// Same as topaz_scheduler_start_task except will simplified inputs:
/// - No interval delay is given
/// - a default name is generated 
const topazString_t * topaz_scheduler_start_task_simple(
    topazComponent_t *,
    uint32_t interval,

    topaz_component_attribute_callback callback,
    void *                             callbackData
);


/// Halts the task with the given name.
/// If no such task exists, no action is taken.
///
void topaz_scheduler_end_task(topazComponent_t *, const topazString_t * name);


/// Gets all tasks being maintained for the scheduler
/// A read-only array of topazString_t * is returned.
const topazArray_t * topaz_scheduler_get_tasks(topazComponent_t *);


/// Stops processing and timing of tasks until topaz_scheduler_resume() is called.
///
void topaz_scheduler_pause(topazComponent_t *);


/// Resumes processing of tasks once more. If the scheduler was not 
/// paused, no action is taken.
void topaz_scheduler_resume(topazComponent_t *);





#endif
