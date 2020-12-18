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
#include <topaz/system.h>

typedef struct topazDisplay_t topazDisplay_t;


///
///    InputManager
///    -----
///    
///    Interacts with device drivers to retrieve the requested input information
///
typedef struct topazInputManager_t topazInputManager_t;



/// Standard input buttons for the keyboard buttons.
///
typedef enum topazKey topazKey;
enum topazKey {
    topazNotAnInput = 0,
    topazKey_0= 1, //< 0
    topazKey_1= 2, //< 1
    topazKey_2= 3, //< 2
    topazKey_3= 4, //< 3
    topazKey_4= 5, //< 4
    topazKey_5= 6, //< 5
    topazKey_6= 7, //< 6
    topazKey_7= 8, //< 7
    topazKey_8= 9, //< 8
    topazKey_9= 10, //< 9
    topazKey_a= 11, //< a
    topazKey_b= 12, //< b
    topazKey_c= 13, //< c
    topazKey_d= 14, //< d
    topazKey_e= 15, //< e
    topazKey_f= 16, //< f
    topazKey_g= 17, //< g
    topazKey_h= 18, //< h
    topazKey_i= 19, //< i
    topazKey_j= 20, //< j
    topazKey_k= 21, //< k
    topazKey_l= 22, //< l
    topazKey_m= 23, //< m
    topazKey_n= 24, //< n
    topazKey_o= 25, //< o
    topazKey_p= 26, //< p
    topazKey_q= 27, //< q
    topazKey_r= 28, //< r
    topazKey_s= 29, //< s
    topazKey_t= 30, //< t
    topazKey_u= 31, //< u
    topazKey_v= 32, //< v
    topazKey_w= 33, //< w
    topazKey_x= 34, //< x
    topazKey_y= 35, //< y
    topazKey_z= 36, //< z
    topazKey_lshift= 37, //< Left shift key
    topazKey_rshift= 38, //< Right shift key
    topazKey_lctrl= 39,  //< Left control key
    topazKey_rctrl= 40,  //< Right control key
    topazKey_lalt= 41,   //< Left alt key
    topazKey_ralt= 42,   //< Right alt key
    topazKey_tab= 43,    //< Tab
    topazKey_F1= 44,     //< F1
    topazKey_F2= 45,     //< F2
    topazKey_F3= 46,     //< F3
    topazKey_F4= 47,     //< F4
    topazKey_F5= 48,     //< F5
    topazKey_F6= 49,     //< F6
    topazKey_F7= 50,     //< F7
    topazKey_F8= 51,     //< F8
    topazKey_F9= 52,     //< F9
    topazKey_F10= 53,    //< F10
    topazKey_F11= 54,    //< F11
    topazKey_F12= 55,    //< F12
    topazKey_up= 100,     //< Up arrow
    topazKey_down= 101,   //< Down arrow
    topazKey_left= 102,   //< Left arrow
    topazKey_right= 103,  //< Right arrow
    topazKey_minus= 104,  //< -
    topazKey_equal= 105,  //< = 
    topazKey_backspace= 106,  //< Backspace
    topazKey_grave= 107,  //< `
    topazKey_esc= 108,    //< Escape
    topazKey_home= 109,   //< Home key
    topazKey_pageUp= 110, //< Page up key
    topazKey_pageDown= 111,  //< Page down key
    topazKey_end= 112,    //< End key
    topazKey_backslash= 113, //< '\'
    topazKey_lbracket= 114, //< [
    topazKey_rbracket= 115, //< ]
    topazKey_semicolon= 116, //< ;
    topazKey_apostrophe= 117, //< '
    topazKey_frontslash= 118, //< /
    topazKey_enter= 119, //< Enter
    topazKey_delete= 120, //< Delete
    topazKey_numpad0= 121, //< Numpad 0
    topazKey_numpad1= 122, //< Numpad 1
    topazKey_numpad2= 123, //< Numpad 2
    topazKey_numpad3= 124, //< Numpad 3
    topazKey_numpad4= 125, //< Numpad 4
    topazKey_numpad5= 126, //< Numpad 5
    topazKey_numpad6= 127, //< Numpad 6
    topazKey_numpad7= 128, //< Numpad 7
    topazKey_numpad8= 129, //< Numpad 8
    topazKey_numpad9= 130, //< Numpad 9
    topazKey_prtscr= 131, //< Print screen button
    topazKey_lsuper= 132, //< Left Super key (Windows key)
    topazKey_rsuper= 133, //< Right Super key (Windows key)
    topazKey_space= 134,  //< Space
    topazKey_insert= 135, //< Insert key
    topazKey_comma= 136, //< ,
    topazKey_period= 137 , //< .
    topazKey_world1= 138, // I8n key0
    topazKey_world2= 139, // I8n key1
    topazKey_world3= 140, // I8n key2
    topazKey_world4= 141, // I8n key3
    topazKey_world5= 142, // I8n key4
    topazKey_world6= 143, // I8n key5
    topazKey_world7= 144, // I8n key6
    topazKey_world8= 145, // I8n key7
    topazKey_world9= 146, // I8n key8

    topazPointer_0= 256, //< Left click
    topazPointer_1= 257, //< Right click
    topazPointer_2= 258, //< Middle click

    topazPointer_X= 259, //< Horizontal axis. Usually for the X axis of the pointer
    topazPointer_Y= 260, //< Horizontal axis. Usually for the X axis of the pointer
    topazPointer_Wheel= 261, //< Mouse wheel.

    topazPad_a= 300,     //< Button 0
    topazPad_b= 301,     //< Button 1
    topazPad_c= 302,     //< Button 2
    topazPad_x= 303,     //< Button 3
    topazPad_y= 304,     //< Button 4
    topazPad_r= 305,     //< Button 5
    topazPad_l= 306,     //< Button 6
    topazPad_r2= 307,    //< Button 7
    topazPad_l2= 308,    //< Button 8
    topazPad_r3= 309,    //< Button 9
    topazPad_l3= 310,    //< Button 10
    topazPad_start= 311,    //< Button 11
    topazPad_select= 312,//< Button 12
    topazPad_b13= 313,//< Button 13
    topazPad_b14= 314,//< Button 14
    topazPad_b15= 315,//< Button 15
    topazPad_b16= 316,//< Button 16
    topazPad_b17= 317,//< Button 17
    topazPad_b18= 318,//< Button 18
    topazPad_b19= 319,//< Button 19
    topazPad_b20= 320,//< Button 20
    topazPad_b21= 321,//< Button 21
    topazPad_b22= 322,//< Button 22
    topazPad_b23= 323,//< Button 23
    topazPad_b24= 324,//< Button 24
    topazPad_b25= 325,//< Button 25
    topazPad_b26= 326,//< Button 26
    topazPad_b27= 327,//< Button 27
    topazPad_b28= 328,//< Button 28
    topazPad_b29= 329,//< Button 29
    topazPad_b30= 330,//< Button 30
    topazPad_b31= 331,//< Button 31
    topazPad_b32= 332,//< Button 32

    topazPad_axisX= 400, //< X button
    topazPad_axisY= 401, //< Y button
    topazPad_axisZ= 402, //< Z button
    topazPad_axisX2= 403,//< X2 button 
    topazPad_axisY2= 404,//< Y2 button
    topazPad_axisZ2= 405,//< Z2 button
    topazPad_axisX3= 406,//< X3 button
    topazPad_axisY3= 407,//< Y3 button
    topazPad_axisZ3= 408,//< Z3 button
    topazPad_axisX4= 409,//< X4 button
    topazPad_axisY4= 410,//< Y4 button
    topazPad_axisZ4= 411,//< Z4 button
    topazPad_axisX5= 412,//< X4 button
    topazPad_axisY5= 413,//< Y4 button
    topazPad_axisZ5= 414,//< Z4 button
    topazPad_axisX6= 415,//< X4 button
    topazPad_axisY6= 416,//< Y4 button
    topazPad_axisZ6= 417,//< Z4 button

    topazPad_axisR= 450,   
    topazPad_axisL= 451,    
    topazPad_axisR2= 452,    
    topazPad_axisL2= 453,    
    topazPad_axisR3= 454,    
    topazPad_axisL3= 455,    
    topazPad_axisR4= 456,    
    topazPad_axisL4= 457,    

    topazPad_options= 511,
    topazInput_Count= 512
};



/// Preset device IDs
/// See topaz_input_manager_query_device()
///
typedef enum topazInputManager_DefaultDevice topazInputManager_DefaultDevice;
enum topazInputManager_DefaultDevice {
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
    topazInputManager_DefaultDevice_Count
};





/// Creates a new audio manager object. 
///
topazInputManager_t * topaz_input_manager_create(topaz_t *, topazSystem_Backend_t *, topazInputManagerAPI_t);


/// Destroys and cleans up an audio manager object
///
void topaz_input_manager_destroy(topazInputManager_t *);





/// Retrieves the backend for this audio manager object.
///
topazSystem_Backend_t * topaz_input_manager_get_backend(topazInputManager_t *);


/// Returns the API for this audio manager.
///
topazInputManagerAPI_t topaz_input_manager_get_api(topazInputManager_t *);



/// Returns a read-only string representation of 
/// the given input id.
///
const topazString_t * topaz_input_manager_id_to_string(int);


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
