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


#ifndef H_TOPAZDC__INPUT_MANAGER__INCLUDED
#define H_TOPAZDC__INPUT_MANAGER__INCLUDED

#include <topaz/input_device.h>
#include <topaz/backends/api/input_manager_api.h>
#include <topaz/backends/backend.h>


/// Forward declare.
///
typedef struct topazDisplay_t topazDisplay_t;


/*

    InputManager
    -----
    
    Interacts with device drivers to retrieve the requested input information



*/
typedef struct topazInputManager_t topazInputManager_t;



/// Standard input buttons for the keyboard buttons.
///
enum {
    topazNotAnInput,
    topazKey_0, ///< 0
    topazKey_1, ///< 1
    topazKey_2, ///< 2
    topazKey_3, ///< 3
    topazKey_4, ///< 4
    topazKey_5, ///< 5
    topazKey_6, ///< 6
    topazKey_7, ///< 7
    topazKey_8, ///< 8
    topazKey_9, ///< 9
    topazKey_a, ///< a
    topazKey_b, ///< b
    topazKey_c, ///< c
    topazKey_d, ///< d
    topazKey_e, ///< e
    topazKey_f, ///< f
    topazKey_g, ///< g
    topazKey_h, ///< h
    topazKey_i, ///< i
    topazKey_j, ///< j
    topazKey_k, ///< k
    topazKey_l, ///< l
    topazKey_m, ///< m
    topazKey_n, ///< n
    topazKey_o, ///< o
    topazKey_p, ///< p
    topazKey_q, ///< q
    topazKey_r, ///< r
    topazKey_s, ///< s
    topazKey_t, ///< t
    topazKey_u, ///< u
    topazKey_v, ///< v
    topazKey_w, ///< w
    topazKey_x, ///< x
    topazKey_y, ///< y
    topazKey_z, ///< z
    topazKey_lshift, ///< Left shift key
    topazKey_rshift, ///< Right shift key
    topazKey_lctrl,  ///< Left control key
    topazKey_rctrl,  ///< Right control key
    topazKey_lalt,   ///< Left alt key
    topazKey_ralt,   ///< Right alt key
    topazKey_tab,    ///< Tab
    topazKey_F1,     ///< F1
    topazKey_F2,     ///< F2
    topazKey_F3,     ///< F3
    topazKey_F4,     ///< F4
    topazKey_F5,     ///< F5
    topazKey_F6,     ///< F6
    topazKey_F7,     ///< F7
    topazKey_F8,     ///< F8
    topazKey_F9,     ///< F9
    topazKey_F10,    ///< F10
    topazKey_F11,    ///< F11
    topazKey_F12,    ///< F12
    topazKey_up,     ///< Up arrow
    topazKey_down,   ///< Down arrow
    topazKey_left,   ///< Left arrow
    topazKey_right,  ///< Right arrow
    topazKey_minus,  ///< -
    topazKey_equal,  ///< = 
    topazKey_backspace,  ///< Backspace
    topazKey_grave,  ///< `
    topazKey_esc,    ///< Escape
    topazKey_home,   ///< Home key
    topazKey_pageUp, ///< Page up key
    topazKey_pageDown,  ///< Page down key
    topazKey_end,    ///< End key
    topazKey_backslash, ///< '\'
    topazKey_lbracket, ///< [
    topazKey_rbracket, ///< ]
    topazKey_semicolon, ///< ;
    topazKey_apostrophe, ///< '
    topazKey_frontslash, ///< /
    topazKey_enter, ///< Enter
    topazKey_delete, ///< Delete
    topazKey_numpad0, ///< Numpad 0
    topazKey_numpad1, ///< Numpad 1
    topazKey_numpad2, ///< Numpad 2
    topazKey_numpad3, ///< Numpad 3
    topazKey_numpad4, ///< Numpad 4
    topazKey_numpad5, ///< Numpad 5
    topazKey_numpad6, ///< Numpad 6
    topazKey_numpad7, ///< Numpad 7
    topazKey_numpad8, ///< Numpad 8
    topazKey_numpad9, ///< Numpad 9
    topazKey_prtscr, ///< Print screen button
    topazKey_lsuper, ///< Left Super key (Windows key)
    topazKey_rsuper, ///< Right Super key (Windows key)
    topazKey_space,  ///< Space
    topazKey_insert, ///< Insert key
    topazKey_comma, ///< ,
    topazKey_period, ///< .

    topazPointer_0, ///< Left click
    topazPointer_1, ///< Right click
    topazPointer_2, ///< Middle click

    topazPointer_X, ///< Horizontal axis. Usually for the X axis of the pointer
    topazPointer_Y, ///< Horizontal axis. Usually for the X axis of the pointer
    topazPointer_Wheel, ///< Mouse wheel.

    topazPad_a,     ///< Button 0
    topazPad_b,     ///< Button 1
    topazPad_c,     ///< Button 2
    topazPad_x,     ///< Button 3
    topazPad_y,     ///< Button 4
    topazPad_r,     ///< Button 5
    topazPad_l,     ///< Button 6
    topazPad_r2,    ///< Button 7
    topazPad_l2,    ///< Button 8
    topazPad_r3,    ///< Button 9
    topazPad_l3,    ///< Button 10
    topazPad_start,    ///< Button 11
    topazPad_select,///< Button 12
    topazPad_b13,///< Button 13
    topazPad_b14,///< Button 14
    topazPad_b15,///< Button 15
    topazPad_b16,///< Button 16
    topazPad_b17,///< Button 17
    topazPad_b18,///< Button 18
    topazPad_b19,///< Button 19
    topazPad_b20,///< Button 20
    topazPad_b21,///< Button 21
    topazPad_b22,///< Button 22
    topazPad_b23,///< Button 23
    topazPad_b24,///< Button 24
    topazPad_b25,///< Button 25
    topazPad_b26,///< Button 26
    topazPad_b27,///< Button 27
    topazPad_b28,///< Button 28
    topazPad_b29,///< Button 29
    topazPad_b30,///< Button 30
    topazPad_b31,///< Button 31
    topazPad_b32,///< Button 32

    topazPad_axisX, ///< X button
    topazPad_axisY, ///< Y button
    topazPad_axisZ, ///< Z button
    topazPad_axisX2,///< X2 button 
    topazPad_axisY2,///< Y2 button
    topazPad_axisZ2,///< Z2 button
    topazPad_axisX3,///< X3 button
    topazPad_axisY3,///< Y3 button
    topazPad_axisZ3,///< Z3 button
    topazPad_axisX4,///< X4 button
    topazPad_axisY4,///< Y4 button
    topazPad_axisZ4,///< Z4 button
    topazPad_axisX5,///< X4 button
    topazPad_axisY5,///< Y4 button
    topazPad_axisZ5,///< Z4 button
    topazPad_axisX6,///< X4 button
    topazPad_axisY6,///< Y4 button
    topazPad_axisZ6,///< Z4 button

    topazPad_axisR,   
    topazPad_axisL,    
    topazPad_axisR2,    
    topazPad_axisL2,    
    topazPad_axisR3,    
    topazPad_axisL3,    
    topazPad_axisR4,    
    topazPad_axisL4,    

    topazPad_options,
    topazInput_Count
};



/// Preset device IDs
/// See topaz_input_manager_query_device()
///
enum {
    /// The standard device ID for the system keyboard.
    ///
    topazInputManager_DefaultDevice_Keyboard,

    /// The standard device ID for the system pointer/mouse.
    ///
    topazInputManager_DefaultDevice_Mouse,

    /// The standard device ID for the system touchpad
    ///
    topazInputManager_DefaultDevice_Touchpad,

    /// The standard device ID for the gamepad, slot 1
    ///
    topazInputManager_DefaultDevice_Pad1,

    /// The standard device ID for the gamepad, slot 2
    ///
    topazInputManager_DefaultDevice_Pad2,

    /// The standard device ID for the gamepad, slot 3
    ///
    topazInputManager_DefaultDevice_Pad3,

    /// The standard device ID for the gamepad, slot 4
    ///
    topazInputManager_DefaultDevice_Pad4,

    /// The count of default devices.
    ///
    topazInputManager_NumDefaultDevices
};





/// Creates a new audio manager object. 
///
topazInputManager_t * topaz_input_manager_create(topazBackend_t *, topazInputManagerAPI_t);


/// Destroys and cleans up an audio manager object
///
void topaz_input_manager_destroy(topazInputManager_t *);





/// Retrieves the backend for this audio manager object.
///
topazBackend_t * topaz_input_manager_get_backend(topazInputManager_t *);


/// Returns the API for this audio manager.
///
topazInputManagerAPI_t topaz_input_manager_get_api(topazInputManager_t *);



/// Returns a read-only C-string representation of 
/// the given input id.
///
const char * topaz_input_manager_id_to_string(int);


/// Updates the state of registered devices.
/// Returns whether or not there were new input events that were pulled  
///
int topaz_input_manager_handle_events(topazInputManager_t *);

/// Returns a reference to the internally maintained input device.
/// Do not free or modify the contents of the device. The first few slots 
/// up to topazInputManager_NumDefaultDevices will match the device expected.
/// If the device is unsupported, NULL will be returned.
///
topazInputDevice_t * topaz_input_manager_query_device(topazInputManager_t *, int ID);

/// Returns the number of additional devices available.
/// up to MaxDevices(). Typically, any overflow devices that weren't able to 
/// fit in the first 4 slots will be put here.
/// Given an array of sise MaxDevices(), IDs will be filled 
/// with the index to a device available through QueryDevice that isnt 
/// a default device.
///
int topaz_input_manager_query_auxiliary_devices(topazInputManager_t *, int * IDs);


/// Maximum number of supported. Will always be at least NumDefaultDevices in count
///
int topaz_input_manager_max_devices(topazInputManager_t *);

/// Sets the focus on which to query input from.
/// On multi-display systems, such as a desktop environment,
/// input is based around the context of the focused display.
/// On some implementations, such a distinction (and as a result, this function)  is not necessary
/// may not be necessary, but this will almost 
/// always be necessary for modern environments.
/// Passing in nullptr should disable input until a 
/// valid display is given.
///
void topaz_input_manager_set_focus(topazInputManager_t *, topazDisplay_t *);

/// Returns the current focus. The default is NULL
///
topazDisplay_t * topaz_input_manager_get_focus(topazInputManager_t *);


/// Attempts to open a virtual keyboard. This is intended to 
/// follow the same input detection as a normal keyboard, so no additional
/// behavior is required or guaranteed outside of the conceptual idea 
/// of the opening of a virtual keyboard. By default, this does nothing.
///
void topaz_input_manager_show_virtual_keyboard(topazInputManager_t *, int);




#endif
