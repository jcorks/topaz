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


#ifndef H_TOPAZDC__INPUT_DEVICE__INCLUDED
#define H_TOPAZDC__INPUT_DEVICE__INCLUDED


#include <topaz/vector.h>
#include <topaz/containers/string.h>

///
/// Input devices are devices that the user interacts with via
/// buttons (boolean input) or axes (ranged / discrete input)
///
typedef struct topazInputDevice_t topazInputDevice_t;


/// A type of event.
///
typedef enum topazInputDevice_Class topazInputDevice_Class;
enum topazInputDevice_Class {
    /// Input device id for a keyboard.
    ///
    topaz_InputDevice_Class_Keyboard,

    /// Input device id for a gamepad.
    ///
    topaz_InputDevice_Class_Gamepad,

    /// Input device id for a pointer/touchpad.
    ///
    topaz_InputDevice_Class_Pointer,
};


typedef struct topazInputDevice_Event_t topazInputDevice_Event_t;
/// A single input event state.
///
struct topazInputDevice_Event_t {
    /// input mapping. UserInput enums for ones below the count, else unmarked ints that are system-dependent
    ///
    int id;  

    /// The new state of the input event.
    ///
    float state;

    /// An associated unicode input with the event. If none, will be 0.
    ///
    int utf8;
};




/// Creates a new input device
///
topazInputDevice_t * topaz_input_device_create(
    /// The device class of the new device.
    topazInputDevice_Class cls;
);

/// Destroys an input device.
///
void topaz_input_device_destroy(
    /// The device to destroy.
    topazInputDevice_t * device
);






/// Adds a state change event for the given input within
/// this device. For devices managed by the system, events 
/// are regularly pushed to devices as events coem from the 
/// system. This function can be called for any device to 
/// add events, real or otherwise.
void topaz_input_device_push_event(
    /// The device to add an event to.
    topazInputDevice_t * device, 

    /// The event to add.
    const topazInputDevice_Event_t * event
);


/// Gets the next state change for an input device.
/// The next event in waiting is consumed.
///
int topaz_input_device_pop_event(
    /// The input device to retrieve an event from.
    topazInputDevice_t * device, 
    
    /// A pointer to an event, which will be populated with event data.
    topazInputDevice_Event_t * event
);


/// Returns the device type
///
topazInputDevice_Class topaz_input_device_get_type(
    /// The device to query.
    const topazInputDevice_t * device
);


/// Returns the number of button events pending
///
int topaz_input_device_get_event_count(
    /// The device to query.
    const topazInputDevice_t * device
);

/// Sets a dead zone for the specified input.
/// If a deadzone is specified, inputs below this value are ignored
/// and the new minimum is marked as this deadzone value and is renormalized.    
///   
void topaz_input_device_set_deadzone(
    /// The device to modify.
    topazInputDevice_t * device, 

    /// The input ID to apply the deadzone to. 
    int id, 

    /// The new deadzone.
    float newDeadzone
);







#endif
