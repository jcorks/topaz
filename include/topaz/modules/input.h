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


#ifndef H_TOPAZDC__INPUT__INCLUDED
#define H_TOPAZDC__INPUT__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
typedef struct topaz_t topaz_t;
typedef struct topazInputManager_t topazInputManager_t;



/// Manages input devices to provide convenient access 
/// to inputs and generated input events.
///
/// In topaz, input is split into three types of 
/// device classes: pointers, keyboards, and pads.
/// Any device can have any type of input. 
/// The standard input list is within
/// <a href="./topazInputManager.html#topazKey">topazKey</a>.
typedef struct topazInput_t topazInput_t;





/// A callback in response to a button event.
///
typedef void (*topaz_input_button_event_callback)(
    /// The input instance.
    topazInput_t * input, 

    /// The input button. Usually is a value of type topazKey.
    int inputButton, 

    /// The data bound to the callback.
    void * data
);

/// A callback in response to a button event, but includes a value as well.
///
typedef void (*topaz_input_button_value_callback)(
    /// The input instance.
    topazInput_t * input, 

    /// The input button. Usually is a value of type topazKey.
    int inputButton, 

    /// The value state of the input.
    float value, 

    /// The data bound to the callback.
    void * data
);

/// A callback in response to a unicode event.
///
typedef void (*topaz_input_unicode_callback)(
    /// The input instance.
    topazInput_t * input, 

    /// The character requested by the event.
    int unicodeValue, 

    /// The data bound to the callback.
    void * data
);




typedef struct topazInput_Listener_t topazInput_Listener_t;
/// Defines an interface for responding to events 
/// on a specific device.
///
struct topazInput_Listener_t {

    /// Function called upon pressing of the input.
    /// Press means: from 0 to non-zero
    ///
    topaz_input_button_event_callback on_press;

    /// Function called every update the input is non-zero.
    ///
    topaz_input_button_value_callback on_active;

    /// Function called upon releasing of the input.
    /// Release means: from non-zero to 0
    ///
    topaz_input_button_event_callback on_release;

    /// Function called when the device posts a new event for this 
    /// input. The value may or may not be changed.
    ///
    topaz_input_button_value_callback on_update;

    /// Data argument as last parameter for listener functions.
    ///
    void * userData;
};








typedef struct topazInput_UnicodeListener_t topazInput_UnicodeListener_t;
/// Defines an interface to be used when text input is 
/// generated by the user.
/// On a US keyboard, the following special key signals exist:
/// 17  -> left
/// 18  -> up
/// 19  -> right 
/// 20  -> down
///
/// For all keyboards, the following special characters exist:
/// 32  -> newline 
/// 8   -> backspace
///
///
struct topazInput_UnicodeListener_t {

    /// New incoming unicode character is detected.
    ///
    topaz_input_unicode_callback on_new_unicode;

    /// A unicode value is being requested to be processed multiple times 
    /// in a row.
    ///
    topaz_input_unicode_callback on_repeat_unicode;



    /// User data as last argument to unicode functions.
    ///
    void * userData;
};





/// Creates a new input instance. This is normally not necessary to call, as 
/// topaz_t has a default input instance that it generates for you. 
/// See topaz_get_input();
///
topazInput_t * topaz_input_create(
    /// The context to use.
    topaz_t * context
);

/// Destroys and frees a topaz input instance.
///
void topaz_input_destroy(
    /// The input instance to destroy.
    topazInput_t * input
);



/// Adds a new listener object for the keyboard, calling the given 
/// functions when the relevant event is detected. 
///
int topaz_input_add_keyboard_listener(
    /// The input to add a listener to.
    topazInput_t * input,

    /// The listener info to copy from. 
    const topazInput_Listener_t * listener
);

/// Adds a new listener object for the pointer, calling the given 
/// functions when the relevant event is detected. 
/// Values delivered by the pointer are in 2D world coordinates.
///
int topaz_input_add_pointer_listener(
    /// The input to add a listener to.
    topazInput_t * input,

    /// The listener info to copy from.
    const topazInput_Listener_t * listener
);

/// Adds a new listener object for an input pad, calling the given 
/// functions when the relevant event is detected. 
///
int topaz_input_add_pad_listener(
    /// The input to add a listener to.
    topazInput_t * input,

    /// The listener info to copy from. 
    const topazInput_Listener_t * listener, 

    /// The pad index to listen to.
    int padIndex
);

/// Adds a new listener object for an mapped input, calling the given 
/// functions when the relevant event is detected. 
///
int topaz_input_add_mapped_listener(
    /// The input to add a listener to.
    topazInput_t * input, 

    /// The listener info to copy from.
    const topazInput_Listener_t * listener,

    /// The mapped string to listen to. 
    const topazString_t * mappedName
);


/// Returns the listener added by ID.
/// If none, NULL is returned.
/// The reference is owned by input and is guaranteed 
/// to be valid until the next topaz input call.
const topazInput_Listener_t * topaz_input_get_listener(
    /// The input to query.
    topazInput_t * input,
    /// The ID of the listener to get.
    int id
);

/// Removes an input listener.
///
void topaz_input_remove_listener(
    /// The input to remove a listener
    topazInput_t * input,

    /// The listener to remove. The ID is returned from the adding of the listener.
    int id
);








/// Returns the current state of the device input
/// This is most useful for keyboards. 
///
float topaz_input_get_state(
    /// The input to query.
    const topazInput_t * query,

    /// The input value to query. Usually of type topazKey. 
    int inputKey
);

/// Returns the current state of the device input for a given pad.
/// See topaz_input_query_pads for available pads.
///
float topaz_input_get_pad_state(
    /// The input instance to query.
    const topazInput_t * query, 

    /// The pad to query.
    int padIndex, 

    /// The button to query. Usually of type topazKey.
    int input
);

/// Returns the current state of the device input for a mapped input
///
float topaz_input_get_mapped_state(
    /// The input instance to query.
    const topazInput_t * input, 

    /// The mapped name.
    const topazString_t * name
);



/// Sets a deadzone for an input, meaning that if an input is detected that is below 
/// the given threshold, it is ignored.
///
void topaz_input_set_deadzone(
    /// The input to modify.
    topazInput_t * input, 

    /// The pad index to modify.
    int padID, 

    /// The input key to set a deadzone. Mostly useful for pad axes inputs
    int inputButton, 

    /// The deadzone amount. If an input value's absolute value is 
    /// below this amount, then the input will be ignored.
    float deadZone
);


/// Returns an array of ints signifying corrently connected 
/// input pads.
///
const topazArray_t * topaz_input_query_pads(
    /// The input to query.
    const topazInput_t * input
);





/// Adds a new unicode listener, which will call the given functions 
/// when relevant unicode events are detected. If the engine is paused, 
/// unicode events arent processed.
///
void topaz_input_add_unicode_listener(
    /// The input to add a listener to.
    topazInput_t * input, 

    /// The listener to read from.
    const topazInput_UnicodeListener_t * listener
);

/// Removes a unicode listener.
///
void topaz_input_remove_unicode_listener(
    /// The input to remove the listener from.
    topazInput_t * input, 

    /// The listener to remove.
    const topazInput_UnicodeListener_t * listener
);



/// Maps a string to a keyboard/pointer input. This then can be used 
/// for various map* functions.
///
void topaz_input_map(
    /// The input to set the mapping within.
    topazInput_t * input, 

    /// The named string to associate an input with.
    const topazString_t * name, 

    /// The value to associate. Usually of type topazKey
    int inputValue
);

/// Maps a string to a pad input. This then can be used for various map*
/// functions.
///
void topaz_input_map_pad(
    /// The input to set the mapping within.
    topazInput_t * input, 

    /// The named string to associate a pad input with.
    const topazString_t * name, 

    /// The pad index to associate the mapping with.
    int pad, 

    /// The input value within the pad to associate the mapping with. Usually of type topazKey.
    int inputValue
);

/// Dissociates a string with an input if in use.
///
void topaz_input_unmap(
    /// The input to modify.
    topazInput_t * input, 

    /// The name to remove the association with.
    const topazString_t * name
);


/// Polls for input.
void topaz_input_poll(
    /// The input to poll.
    topazInput_t * t
);

/// Convenience function that returns the current pointer x position.
/// The position is always in 2D world coordinates.
///
int topaz_input_mouse_x(
    /// The input to query.
    const topazInput_t * input
);

/// Convenience function that returns the current pointer y position.
/// The position is always in 2D world coordinates.
///
int topaz_input_mouse_y(
    /// The input to query.
    const topazInput_t * input
);


/// Convenience function that returns the change in pointer x position in world coordinates since 
/// last update.
///
int topaz_input_mouse_delta_x(
    /// The input to query.
    const topazInput_t * input
);


/// Convenience function that returns the change in pointer y position in world coordinates since 
/// last update.
///
int topaz_input_mouse_delta_y(
    /// The input to query.
    const topazInput_t * input
);


/// Convenience function that returns the state of the wheel.
///
float topaz_input_mouse_wheel(
    /// The input to query.
    const topazInput_t * input
);



#endif
