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
};


/// Creates a new state control component.
/// Use component_destroy to release.
///
topazComponent_t * topaz_scheduler_create(
    /// The topaz context.
    topaz_t * context,

    /// The mode for the scheduler.
    topazScheduler_Mode mode,

    /// Whether the scheduler should repeat or 
    /// only fire once.
    int justOnce,

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


/// Resets the remaining interval 
/// to the initial amount and begins the timing process again
void topaz_scheduler_reset(
    /// The scheduler to reset.
    topazComponent_t * scheduler
);

/// Gets the interval amount before the task interval
/// is next complete.
uint64_t topaz_scheduler_get_task_interval_remaining(
    /// The scheduler to query.
    topazComponent_t * scheduler
);






#endif
