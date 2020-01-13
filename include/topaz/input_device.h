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

/*

    InputDevice
    -----
    
    Input devices are devies that the user interacts with via
    buttons (boolean input) or axes (ranged / discrete input)


*/
typedef struct topazInputDevice_t topazInputDevice_t;


/// A type of event.
///
typedef enum {
    /// Input device id for a keyboard.
    ///
    topaz_InputDevice_Class_Keyboard,

    /// Input device id for a gamepad.
    ///
    topaz_InputDevice_Class_Gamepad,

    /// Input device id for a pointer/touchpad.
    ///
    topaz_InputDevice_Class_Pointer,
} topazInputDevice_Class;


/// A single input event state.
///
typedef struct {
    /// input mapping. UserInput enums for ones below the count, else unmarked ints that are system-dependent
    ///
    int id;  

    /// The new state of the input event.
    ///
    float state;

    /// An associated unicode input with the event. If none, will be 0.
    ///
    int utf8;
} topazInputDevice_Event_t;




/// Creates a new input device
///
topazInputDevice_t * topaz_input_device_create(topazInputDevice_Class);

/// Destroys an input device.
///
void topaz_input_device_destroy(topazInputDevice_t *);






/// Adds a state change event for the given input within
/// this device
void topaz_input_device_push_event(topazInputDevice_t *, const topazInputDevice_Event_t *);


/// Gets the next state change for a device input
///
int topaz_input_device_pop_event(topazInputDevice_t *, topazInputDevice_Event_t *);


/// Returns the device type
///
topazInputDevice_Class topaz_input_device_get_type(const topazInputDevice_t *);


/// Returns the number of button events pending
///
int topaz_input_device_get_event_count(const topazInputDevice_t *);

/// Sets a dead zone for the specified input.
/// If a deadzone is specified, inputs below this value are ignored
/// and the new minimum is marked as this deadzone value and is renormalized.    
///   
void topaz_input_device_set_deadzone(topazInputDevice_t *, int id, float);







#endif
