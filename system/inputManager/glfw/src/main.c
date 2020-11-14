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




#include "backend.h"
#include <topaz/version.h>
#include <GLFW/glfw3.h>

#include <topaz/containers/array.h>
#include <topaz/containers/table.h>
#include <topaz/backends/display.h>
#include <stdlib.h>
#include <string.h>






static topazTable_t * glfww2im = NULL;


typedef struct {
    GLFWwindow * ctx;
    topazDisplay_t * focus;
    topazInputDevice_t * keyboard;
    topazInputDevice_t * mouse;

    topazArray_t * queuedKeyboardEvents;
    topazArray_t * queuedPointerEvents;
} GLFWIM;







#define SMAP(__K__, __V__) case __K__: return __V__;

static int get_key(int kglfw) {
    switch(kglfw) {
        SMAP(GLFW_KEY_UNKNOWN , topazNotAnInput);
        SMAP(GLFW_KEY_SPACE , topazKey_space);
        SMAP(GLFW_KEY_APOSTROPHE , topazKey_apostrophe);
        SMAP(GLFW_KEY_COMMA , topazKey_comma);
        SMAP(GLFW_KEY_MINUS , topazKey_minus);
        SMAP(GLFW_KEY_PERIOD , topazKey_period);
        SMAP(GLFW_KEY_SLASH , topazKey_frontslash);
        SMAP(GLFW_KEY_0 , topazKey_0);
        SMAP(GLFW_KEY_1 , topazKey_1);
        SMAP(GLFW_KEY_2 , topazKey_2);
        SMAP(GLFW_KEY_3 , topazKey_3);
        SMAP(GLFW_KEY_4 , topazKey_4);
        SMAP(GLFW_KEY_5 , topazKey_5);
        SMAP(GLFW_KEY_6 , topazKey_6);
        SMAP(GLFW_KEY_7 , topazKey_7);
        SMAP(GLFW_KEY_8 , topazKey_8);
        SMAP(GLFW_KEY_9 , topazKey_9);
        SMAP(GLFW_KEY_SEMICOLON , topazKey_semicolon);
        SMAP(GLFW_KEY_EQUAL , topazKey_equal);
        SMAP(GLFW_KEY_A , topazKey_a);
        SMAP(GLFW_KEY_B , topazKey_b);
        SMAP(GLFW_KEY_C , topazKey_c);
        SMAP(GLFW_KEY_D , topazKey_d);
        SMAP(GLFW_KEY_E , topazKey_e);
        SMAP(GLFW_KEY_F , topazKey_f);
        SMAP(GLFW_KEY_G , topazKey_g);
        SMAP(GLFW_KEY_H , topazKey_h);
        SMAP(GLFW_KEY_I , topazKey_i);
        SMAP(GLFW_KEY_J , topazKey_j);
        SMAP(GLFW_KEY_K , topazKey_k);
        SMAP(GLFW_KEY_L , topazKey_l);
        SMAP(GLFW_KEY_M , topazKey_m);
        SMAP(GLFW_KEY_N , topazKey_n);
        SMAP(GLFW_KEY_O , topazKey_o);
        SMAP(GLFW_KEY_P , topazKey_p);
        SMAP(GLFW_KEY_Q , topazKey_q);
        SMAP(GLFW_KEY_R , topazKey_r);
        SMAP(GLFW_KEY_S , topazKey_s);
        SMAP(GLFW_KEY_T , topazKey_t);
        SMAP(GLFW_KEY_U , topazKey_u);
        SMAP(GLFW_KEY_V , topazKey_v);
        SMAP(GLFW_KEY_W , topazKey_w);
        SMAP(GLFW_KEY_X , topazKey_x);
        SMAP(GLFW_KEY_Y , topazKey_y);
        SMAP(GLFW_KEY_Z , topazKey_z);
        SMAP(GLFW_KEY_LEFT_BRACKET , topazKey_lbracket);
        SMAP(GLFW_KEY_BACKSLASH , topazKey_backslash);
        SMAP(GLFW_KEY_RIGHT_BRACKET , topazKey_rbracket);
        SMAP(GLFW_KEY_GRAVE_ACCENT , topazKey_grave);
        SMAP(GLFW_KEY_WORLD_1 , topazKey_world1);
        SMAP(GLFW_KEY_WORLD_2 , topazKey_world2);
        SMAP(GLFW_KEY_ESCAPE , topazKey_esc);
        SMAP(GLFW_KEY_ENTER , topazKey_enter);
        SMAP(GLFW_KEY_TAB , topazKey_tab);
        SMAP(GLFW_KEY_BACKSPACE , topazKey_backspace);
        SMAP(GLFW_KEY_INSERT , topazKey_insert);
        SMAP(GLFW_KEY_DELETE , topazKey_delete);
        SMAP(GLFW_KEY_RIGHT , topazKey_right);
        SMAP(GLFW_KEY_LEFT , topazKey_left);
        SMAP(GLFW_KEY_DOWN , topazKey_down);
        SMAP(GLFW_KEY_UP , topazKey_up);
        SMAP(GLFW_KEY_PAGE_UP , topazKey_pageUp);
        SMAP(GLFW_KEY_PAGE_DOWN , topazKey_pageDown);
        SMAP(GLFW_KEY_HOME , topazKey_home);
        SMAP(GLFW_KEY_END , topazKey_end);
        //SMAP(GLFW_KEY_CAPS_LOCK , topazKey_);
        //SMAP(GLFW_KEY_SCROLL_LOCK , topazKey_);
        //SMAP(GLFW_KEY_NUM_LOCK , topazKey_num);
        SMAP(GLFW_KEY_PRINT_SCREEN , topazKey_prtscr);
        //SMAP(GLFW_KEY_PAUSE , topazKey_);
        SMAP(GLFW_KEY_F1 , topazKey_F1);
        SMAP(GLFW_KEY_F2 , topazKey_F2);
        SMAP(GLFW_KEY_F3 , topazKey_F3);
        SMAP(GLFW_KEY_F4 , topazKey_F4);
        SMAP(GLFW_KEY_F5 , topazKey_F5);
        SMAP(GLFW_KEY_F6 , topazKey_F6);
        SMAP(GLFW_KEY_F7 , topazKey_F7);
        SMAP(GLFW_KEY_F8 , topazKey_F8);
        SMAP(GLFW_KEY_F9 , topazKey_F9);
        SMAP(GLFW_KEY_F10 , topazKey_F10);
        SMAP(GLFW_KEY_F11 , topazKey_F11);
        SMAP(GLFW_KEY_F12 , topazKey_F12);
        /*
        SMAP(GLFW_KEY_F13 , topazKey_F13);
        SMAP(GLFW_KEY_F14 , topazKey_F14);
        SMAP(GLFW_KEY_F15 , topazKey_F15);
        SMAP(GLFW_KEY_F16 , topazKey_F16);
        SMAP(GLFW_KEY_F17 , topazKey_F17);
        SMAP(GLFW_KEY_F18 , topazKey_F18);
        SMAP(GLFW_KEY_F19 , topazKey_F19);
        SMAP(GLFW_KEY_F20 , topazKey_F20);
        SMAP(GLFW_KEY_F21 , topazKey_F21);
        SMAP(GLFW_KEY_F22 , topazKey_F22);
        SMAP(GLFW_KEY_F23 , topazKey_F23);
        SMAP(GLFW_KEY_F24 , topazKey_F24);
        SMAP(GLFW_KEY_F25 , topazKey_F25);*/
        SMAP(GLFW_KEY_KP_0 , topazKey_0);
        SMAP(GLFW_KEY_KP_1 , topazKey_1);
        SMAP(GLFW_KEY_KP_2 , topazKey_2);
        SMAP(GLFW_KEY_KP_3 , topazKey_3);
        SMAP(GLFW_KEY_KP_4 , topazKey_4);
        SMAP(GLFW_KEY_KP_5 , topazKey_5);
        SMAP(GLFW_KEY_KP_6 , topazKey_6);
        SMAP(GLFW_KEY_KP_7 , topazKey_7);
        SMAP(GLFW_KEY_KP_8 , topazKey_8);
        SMAP(GLFW_KEY_KP_9 , topazKey_9);
        SMAP(GLFW_KEY_KP_DECIMAL , topazKey_period);
        SMAP(GLFW_KEY_KP_DIVIDE , topazKey_frontslash);
        //SMAP(GLFW_KEY_KP_MULTIPLY , topazKey_);
        SMAP(GLFW_KEY_KP_SUBTRACT , topazKey_minus);
        //SMAP(GLFW_KEY_KP_ADD , topazKey_);
        SMAP(GLFW_KEY_KP_ENTER , topazKey_enter);
        SMAP(GLFW_KEY_KP_EQUAL , topazKey_equal);
        SMAP(GLFW_KEY_LEFT_SHIFT , topazKey_lshift);
        SMAP(GLFW_KEY_LEFT_CONTROL , topazKey_rshift);
        SMAP(GLFW_KEY_LEFT_ALT , topazKey_lalt);
        SMAP(GLFW_KEY_LEFT_SUPER , topazKey_lsuper);
        SMAP(GLFW_KEY_RIGHT_SHIFT , topazKey_rshift);
        SMAP(GLFW_KEY_RIGHT_CONTROL , topazKey_rctrl);
        SMAP(GLFW_KEY_RIGHT_ALT , topazKey_ralt);
        SMAP(GLFW_KEY_RIGHT_SUPER , topazKey_rsuper);
    }
    return topazNotAnInput;
} 

static void topaz_glfw_im_key_callback(
    GLFWwindow * window,
    int key,
    int scacode,
    int action,
    int mods
) {
    int keyTrans = get_key(key);
    if (keyTrans != topazNotAnInput) {
        GLFWIM * im = topaz_table_find(glfww2im, window);
        topazInputDevice_Event_t ev;
        ev.id = keyTrans;
        ev.state = action == GLFW_PRESS || action == GLFW_REPEAT;
        ev.utf8 = 0;
        topaz_array_push(im->queuedKeyboardEvents, ev);    
    }
}



static intptr_t api_nothing(){return 0;}



    

static void * topaz_glfw_im_create(topazInputManager_t * api, topaz_t * ctx) {
    
    GLFWIM * out = calloc(1, sizeof(GLFWIM));
    out->ctx = NULL;
    out->keyboard = topaz_input_device_create(topaz_InputDevice_Class_Keyboard);
    out->mouse    = topaz_input_device_create(topaz_InputDevice_Class_Pointer);
    out->queuedKeyboardEvents = topaz_array_create(sizeof(topazInputDevice_Event_t));
    out->queuedPointerEvents  = topaz_array_create(sizeof(topazInputDevice_Event_t));

    return out;
}

static int topaz_glfw_im_handle_events(topazInputManager_t * imSrc, void * userData) {
    GLFWIM * im = userData;
    if (!im->ctx) return FALSE;

    int hasEvents = 0;

    // likely will fire off the keyboard/pointer/gamepad events for 
    // glfw. This caches them in the queued event arrays.
    glfwPollEvents();

    
    // we then manually transfer them here. This is because we want to stay as 
    // close to spec as possible and add events to the devices when they are 
    // expected.
    uint32_t i;
    uint32_t len = topaz_array_get_size(im->queuedKeyboardEvents);
    for(i = 0; i < len; ++i) {
        topaz_input_device_push_event(
            im->keyboard, 
            &topaz_array_at(
                im->queuedKeyboardEvents,
                topazInputDevice_Event_t,
                i
            )
        );
        hasEvents = TRUE;
    } 
    topaz_array_set_size(im->queuedKeyboardEvents, 0);
    return hasEvents;
}


static void topaz_glfw_im_set_focus(topazInputManager_t * imSrc, void * userData, topazDisplay_t * disp) {
    GLFWIM * im = userData;
    if (im->ctx) {
        topaz_table_remove(glfww2im, im->ctx);                  
        glfwSetKeyCallback(im->ctx, NULL);
        im->ctx = NULL;
        im->focus = NULL;
    }
    if (disp && topaz_display_get_system_handle_type(disp) == topazDisplay_Handle_GLFWwindow) {
        if (im->ctx == topaz_display_get_system_handle(disp)) {
            return;
        } 
        im->focus = disp;
        im->ctx = topaz_display_get_system_handle(disp);
        topaz_table_insert(glfww2im, im->ctx, im);    
        glfwSetKeyCallback(im->ctx, topaz_glfw_im_key_callback);

        
    }
}

static topazDisplay_t * topaz_glfw_im_get_focus(topazInputManager_t * imSrc, void * userData) {
    GLFWIM * im = userData;
    return im->focus;
}


static topazInputDevice_t * topaz_glfw_im_query_device(topazInputManager_t * imSrc, void * userData, int ID) {
    GLFWIM * im = userData;
    switch(ID) {
      case 0: return im->keyboard;
      case 1: return im->mouse;
      default: return NULL;
    }
}

static int topaz_glfw_im_query_auxiliary_devices(topazInputManager_t * imSrc, void * userData, int * IDs) {
    return 0;
}

static int topaz_glfw_im_max_devices(topazInputManager_t * imSrc, void * userData) {
    return 2;
}

void topaz_system_inputManager_glfw__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazInputManagerAPI_t * api
) {

    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("GLFW"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Input manager for GLFW displays."),




        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );

    if (!glfww2im) {
        glfww2im = topaz_table_create_hash_pointer();
    }
    api->input_manager_create = topaz_glfw_im_create;
    api->input_manager_destroy = (void (*)(topazInputManager_t *, void *)) api_nothing;
    api->input_manager_handle_events = topaz_glfw_im_handle_events;
    api->input_manager_query_device = topaz_glfw_im_query_device;
    api->input_manager_query_auxiliary_devices = topaz_glfw_im_query_auxiliary_devices;
    api->input_manager_max_devices = topaz_glfw_im_max_devices;
    api->input_manager_set_focus = topaz_glfw_im_set_focus;
    api->input_manager_get_focus = topaz_glfw_im_get_focus;
    api->input_manager_show_virtual_keyboard = (void (*)(topazInputManager_t *, void *, int)) api_nothing;

}






