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
/// Interacts with device drivers to retrieve the requested input information
///
typedef struct topazInputManager_t topazInputManager_t;



/// Standard input buttons for built-in device types.
///
typedef enum topazKey topazKey;
enum topazKey {
    /// Not an input.
    topazNotAnInput = 0,
    /// The 0 key.
    topazKey_0= 1,
    /// The 1 key.
    topazKey_1= 2,
    /// The 2 key.
    topazKey_2= 3, 
    /// The 3 key.
    topazKey_3= 4,
    /// The 4 key.
    topazKey_4= 5,
    /// The 5 key.
    topazKey_5= 6,
    /// The 6 key.
    topazKey_6= 7,
    /// The 7 key.
    topazKey_7= 8,
    /// The 8 key.
    topazKey_8= 9,
    /// The 9 key.
    topazKey_9= 10,
    /// The a key.
    topazKey_a= 11,
    /// The b key.
    topazKey_b= 12, 
    /// The c key.
    topazKey_c= 13, 
    /// The d key.
    topazKey_d= 14, 
    /// The e key.
    topazKey_e= 15, 
    /// The f key.
    topazKey_f= 16, 
    /// The g key.
    topazKey_g= 17, 
    /// The h key.
    topazKey_h= 18, 
    /// The i key.
    topazKey_i= 19, 
    /// The j key.
    topazKey_j= 20,  
    /// The k key.
    topazKey_k= 21, 
    /// The l key.
    topazKey_l= 22, 
    /// The m key.
    topazKey_m= 23, 
    /// The n key.
    topazKey_n= 24, 
    /// The o key.
    topazKey_o= 25, 
    /// The p key.
    topazKey_p= 26, 
    /// The q key.
    topazKey_q= 27, 
    /// The r key.
    topazKey_r= 28, 
    /// The s key.
    topazKey_s= 29, 
    /// The t key.
    topazKey_t= 30, 
    /// The u key.
    topazKey_u= 31, 
    /// The v key.
    topazKey_v= 32, 
    /// The w key.
    topazKey_w= 33, 
    /// The x key.
    topazKey_x= 34, 
    /// The y key.
    topazKey_y= 35, 
    /// The z key.
    topazKey_z= 36, 
    /// The lshift key.
    topazKey_lshift= 37, 
    /// The rshift key.
    topazKey_rshift= 38, 
    /// The lctrl key.
    topazKey_lctrl= 39,  
    /// The rctrl key.
    topazKey_rctrl= 40,  
    /// The lalt key.
    topazKey_lalt= 41,   
    /// The ralt key.
    topazKey_ralt= 42,   
    /// The tab key.
    topazKey_tab= 43,    
    /// The F1 key.
    topazKey_F1= 44,     
    /// The F2 key.
    topazKey_F2= 45,     
    /// The F3 key.
    topazKey_F3= 46,     
    /// The F4 key.
    topazKey_F4= 47,     
    /// The F5 key.
    topazKey_F5= 48,     
    /// The F6 key.
    topazKey_F6= 49,     
    /// The F7 key.
    topazKey_F7= 50,     
    /// The F8 key.
    topazKey_F8= 51,     
    /// The F9 key.
    topazKey_F9= 52,     
    /// The F10 key.
    topazKey_F10= 53,    
    /// The F11 key.
    topazKey_F11= 54,    
    /// The F12 key.
    topazKey_F12= 55,    
    /// The up key.
    topazKey_up= 100,     
    /// The down key.
    topazKey_down= 101,   
    /// The left key.
    topazKey_left= 102,   
    /// The right key.
    topazKey_right= 103,  
    /// The minus key.
    topazKey_minus= 104,  
    /// The equal key.
    topazKey_equal= 105,  
    /// The backspace key.
    topazKey_backspace= 106,  
    /// The grave key.
    topazKey_grave= 107,  
    /// The esc key.
    topazKey_esc= 108,    
    /// The home key.
    topazKey_home= 109,   
    /// The pageUp key.
    topazKey_pageUp= 110, 
    /// The pageDown key.
    topazKey_pageDown= 111,  
    /// The end key.
    topazKey_end= 112,    
    /// The backslash key.
    topazKey_backslash= 113, 
    /// The lbracket key.
    topazKey_lbracket= 114, 
    /// The rbracket key.
    topazKey_rbracket= 115, 
    /// The semicolon key.
    topazKey_semicolon= 116, 
    /// The apostrophe key.
    topazKey_apostrophe= 117, 
    /// The frontslash key.
    topazKey_frontslash= 118, 
    /// The enter key.
    topazKey_enter= 119, 
    /// The delete key.
    topazKey_delete= 120, 
    /// The numpad0 key.
    topazKey_numpad0= 121, 
    /// The numpad1 key.
    topazKey_numpad1= 122, 
    /// The numpad2 key.
    topazKey_numpad2= 123, 
    /// The numpad3 key.
    topazKey_numpad3= 124, 
    /// The numpad4 key.
    topazKey_numpad4= 125, 
    /// The numpad5 key.
    topazKey_numpad5= 126, 
    /// The numpad6 key.
    topazKey_numpad6= 127, 
    /// The numpad7 key.
    topazKey_numpad7= 128, 
    /// The numpad8 key.
    topazKey_numpad8= 129, 
    /// The numpad9 key.
    topazKey_numpad9= 130, 
    /// The prtscr key.
    topazKey_prtscr= 131, 
    /// The lsuper key.
    topazKey_lsuper= 132, 
    /// The rsuper key.
    topazKey_rsuper= 133, 
    /// The space key.
    topazKey_space= 134,  
    /// The insert key.
    topazKey_insert= 135, 
    /// The comma key.
    topazKey_comma= 136, 
    /// The period key.
    topazKey_period= 137 , 
    /// The world1 key.
    topazKey_world1= 138, 
    /// The world2 key.
    topazKey_world2= 139, 
    /// The world3 key.
    topazKey_world3= 140, 
    /// The world4 key.
    topazKey_world4= 141,  
    /// The world5 key.
    topazKey_world5= 142,  
    /// The world6 key.
    topazKey_world6= 143, 
    /// The world7 key.
    topazKey_world7= 144,  
    /// The world8 key.
    topazKey_world8= 145,  
    /// The world9 key.
    topazKey_world9= 146,  

    /// The 0th pointer button.
    topazPointer_0= 256, 
    /// The 1st pointer button.
    topazPointer_1= 257, 
    /// The 2nd pointer button.
    topazPointer_2= 258, 

    /// The pointer x axis.
    topazPointer_X= 259, 
    /// The pointer y axis.
    topazPointer_Y= 260, 
    /// The pointer wheel axis.
    topazPointer_wheel= 261, 

    /// The a pad input.
    topazPad_a= 300,     
    /// The b pad input.
    topazPad_b= 301,     
    /// The c pad input.
    topazPad_c= 302,     
    /// The x pad input.
    topazPad_x= 303,     
    /// The y pad input.
    topazPad_y= 304,     
    /// The r pad input.
    topazPad_r= 305,     
    /// The l pad input.
    topazPad_l= 306,     
    /// The r2 pad input.
    topazPad_r2= 307,    
    /// The l2 pad input.
    topazPad_l2= 308,    
    /// The r3 pad input.
    topazPad_r3= 309,    
    /// The l3 pad input.
    topazPad_l3= 310,    
    /// The start pad input.
    topazPad_start= 311,    
    /// The select pad input.
    topazPad_select= 312,
    /// The b13 pad input.
    topazPad_b13= 313,
    /// The b14 pad input.
    topazPad_d_up= 314,
    /// The b15 pad input.
    topazPad_d_right= 315,
    /// The b16 pad input.
    topazPad_d_down= 316,
    /// The b17 pad input.
    topazPad_d_left= 317,
    /// The b18 pad input.
    topazPad_b18= 318,
    /// The b19 pad input.
    topazPad_b19= 319,
    /// The b20 pad input.
    topazPad_b20= 320,
    /// The b21 pad input.
    topazPad_b21= 321,
    /// The b22 pad input.
    topazPad_b22= 322,
    /// The b23 pad input.
    topazPad_b23= 323,
    /// The b24 pad input.
    topazPad_b24= 324,
    /// The b25 pad input.
    topazPad_b25= 325,
    /// The b26 pad input.
    topazPad_b26= 326,
    /// The b27 pad input.
    topazPad_b27= 327,
    /// The b28 pad input.
    topazPad_b28= 328,
    /// The b29 pad input.
    topazPad_b29= 329,
    /// The b30 pad input.
    topazPad_b30= 330,
    /// The b31 pad input.
    topazPad_b31= 331,
    /// The b32 pad input.
    topazPad_b32= 332,

    /// The axisX pad input.
    topazPad_axisX= 400, 
    /// The axisY pad input.
    topazPad_axisY= 401, 
    /// The axisZ pad input.
    topazPad_axisZ= 402, 
    /// The axisX2 pad input.
    topazPad_axisX2= 403,
    /// The axisY2 pad input.
    topazPad_axisY2= 404,
    /// The axisZ2 pad input.
    topazPad_axisZ2= 405,
    /// The axisX3 pad input.
    topazPad_axisX3= 406,
    /// The axisY3 pad input.
    topazPad_axisY3= 407,
    /// The axisZ3 pad input.
    topazPad_axisZ3= 408,
    /// The axisX4 pad input.
    topazPad_axisX4= 409,
    /// The axisY4 pad input.
    topazPad_axisY4= 410,
    /// The axisZ4 pad input.
    topazPad_axisZ4= 411,
    /// The axisX5 pad input.
    topazPad_axisX5= 412,
    /// The axisY5 pad input.
    topazPad_axisY5= 413,
    /// The axisZ5 pad input.
    topazPad_axisZ5= 414,
    /// The axisX6 pad input.
    topazPad_axisX6= 415,
    /// The axisY6 pad input.
    topazPad_axisY6= 416,
    /// The axisZ6 pad input.
    topazPad_axisZ6= 417,

    /// The axisR pad input.
    topazPad_axisR= 450,   
    /// The axisL pad input.
    topazPad_axisL= 451,    
    /// The axisR2 pad input.
    topazPad_axisR2= 452,    
    /// The axisL2 pad input.
    topazPad_axisL2= 453,    
    /// The axisR3 pad input.
    topazPad_axisR3= 454,    
    /// The axisL3 pad input.
    topazPad_axisL3= 455,    
    /// The axisR4 pad input.
    topazPad_axisR4= 456,    
    /// The axisL4 pad input.
    topazPad_axisL4= 457,    

    /// The options pad input.
    topazPad_options= 511,

    /// Number of built-in inputs.
    topazInput_count= 512
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
topazInputManager_t * topaz_input_manager_create(
    /// The topaz context.
    topaz_t * context, 


    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazInputManagerAPI_t api
);



/// Destroys and cleans up an audio manager object
///
void topaz_input_manager_destroy(
    /// The input manager to destroy.
    topazInputManager_t * input
);





/// Retrieves the backend for this audio manager object.
///
topazSystem_Backend_t * topaz_input_manager_get_backend(
    /// The input to query.
    topazInputManager_t * input
);


/// Returns the API for this audio manager.
///
topazInputManagerAPI_t topaz_input_manager_get_api(
    /// The input to query.
    topazInputManager_t * input
);




/// Returns a read-only string representation of 
/// the given input id.
///
const topazString_t * topaz_input_manager_id_to_string(
    /// Id to get a string version of. 
    int id
);


/// Updates the state of registered devices.
/// Returns whether or not there were new input events that were pulled  
///
int topaz_input_manager_handle_events(
    /// The input to query.
    topazInputManager_t * input
);

/// Returns a reference to the internally maintained input device.
/// Do not free or modify the contents of the device. The first few slots 
/// up to topazInputManager_NumDefaultDevices will match the device expected.
/// If the device is unsupported, NULL will be returned.
///
topazInputDevice_t * topaz_input_manager_query_device(
    /// The input manager to query.
    topazInputManager_t * input,

    /// The ID to query.
    int ID
);

/// Returns the number of additional devices available.
/// up to MaxDevices(). Typically, any overflow devices that weren't able to 
/// fit in the first 4 slots will be put here.
/// Given an array of sise MaxDevices(), IDs will be filled 
/// with the index to a device available through QueryDevice that isnt 
/// a default device.
///
int topaz_input_manager_query_auxiliary_devices(
    /// The input manager to query.
    topazInputManager_t * input,

    /// The devices from the input manager that will be populated.
    int * IDs
);


/// Maximum number of supported. Will always be at least NumDefaultDevices in count
///
int topaz_input_manager_max_devices(
    /// The input manager to query.
    topazInputManager_t * input
);

/// Sets the focus on which to query input from.
/// On multi-display systems, such as a desktop environment,
/// input is based around the context of the focused display.
/// On some implementations, such a distinction (and as a result, this function)  is not necessary
/// may not be necessary, but this will almost 
/// always be necessary for modern environments.
/// Passing in nullptr should disable input until a 
/// valid display is given.
///
void topaz_input_manager_set_focus(
    /// The input manager to modify.
    topazInputManager_t * input,

    /// The display to focus on. 
    topazDisplay_t * display
);

/// Returns the current focus. The default is NULL
///
topazDisplay_t * topaz_input_manager_get_focus(
    /// The input manager to query.
    topazInputManager_t * input
);


/// Attempts to open a virtual keyboard. This is intended to 
/// follow the same input detection as a normal keyboard, so no additional
/// behavior is required or guaranteed outside of the conceptual idea 
/// of the opening of a virtual keyboard. By default, this does nothing.
///
void topaz_input_manager_show_virtual_keyboard(
    /// The input manager to request.
    topazInputManager_t * inputManager,

    /// whether to show the virtual keyboard. 
    int state
);




#endif
