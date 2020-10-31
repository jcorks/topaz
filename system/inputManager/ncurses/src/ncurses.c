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
#include <stdlib.h>
#include <string.h>
#include <topaz/input_device.h>
#include <topaz/backends/display.h>
#include <ncurses.h>
#include <assert.h>





typedef struct {
    topazInputDevice_t * keyboard;
    topazInputDevice_t * mouse;
    topazDisplay_t * disp;
    topazArray_t * lastKeys;
} NCURSESINPUT;


static void ncurses_input_manager_create (topazInputManagerAPI_t * api) {
    NCURSESINPUT * im = calloc(1, sizeof(NCURSESINPUT));
    im->keyboard = topaz_input_device_create(topaz_InputDevice_Class_Keyboard);
    im->mouse    = topaz_input_device_create(topaz_InputDevice_Class_Pointer);
    im->lastKeys = topaz_array_create(sizeof(int));
    api->implementationData = im;
}
static void ncurses_input_manager_destroy(topazInputManagerAPI_t * api) {
    NCURSESINPUT * im = api->implementationData;
    topaz_input_device_destroy(im->keyboard);
    topaz_input_device_destroy(im->mouse);
    topaz_array_destroy(im->lastKeys);
    free(im);
}


static void translate_to_screen_xy(NCURSESINPUT * im, int * x, int * y) {
    int wcurses, hcurses;
    getmaxyx(stdscr, hcurses, wcurses);

    *x = (((float)*x) / wcurses) * topaz_display_get_width(im->disp);
    *y = (((float)*y) / hcurses) * topaz_display_get_height(im->disp);

}

static int ncurses_key_to_topaz_key(int ch) {
    switch(ch) {
      case KEY_UP: return topazKey_up;
      case KEY_DOWN: return topazKey_down;
      case KEY_LEFT: return topazKey_left;
      case KEY_RIGHT: return topazKey_right;
      default:;
    }
    return topazNotAnInput;
}

static int ncurses_input_manager_handle_events (topazInputManagerAPI_t * api) {
    NCURSESINPUT * im = api->implementationData;
    int hasEvents = FALSE;
    int ch;
    topazInputDevice_Event_t ev;
    MEVENT eventNC;

    if (topaz_array_get_size(im->lastKeys)) {
        int i;
        for(i = 0; i < topaz_array_get_size(im->lastKeys); ++i) {
            ev.id = topaz_array_at(im->lastKeys, int, i);
            ev.state = 0;
            ev.utf8 = 0;
            topaz_input_device_push_event(im->keyboard, &ev);
        }
        topaz_array_set_size(im->lastKeys, 0);
    }
    if (!im->disp) return FALSE;
    while((ch = getch()) != ERR) { // err means no events queued
        hasEvents = TRUE;
        switch(ch) {
          case KEY_MOUSE:
            if (getmouse(&eventNC) == OK) {

                translate_to_screen_xy(im, &eventNC.x, &eventNC.y);

                ev.id = topazPointer_X;
                ev.state = eventNC.x;
                ev.utf8 = 0;
                topaz_input_device_push_event(im->mouse, &ev);

                ev.id = topazPointer_Y;
                ev.state = eventNC.y;
                ev.utf8 = 0;
                topaz_input_device_push_event(im->mouse, &ev);

                


                //printf("EVENTS %d %d\n", eventNC.x, eventNC.y);


                if (eventNC.bstate & BUTTON1_PRESSED) {
                    ev.id = topazPointer_0;
                    ev.state = 1;
                    ev.utf8 = 0;
                    topaz_input_device_push_event(im->mouse, &ev);
                }
                if (eventNC.bstate & BUTTON1_RELEASED) {
                    ev.id = topazPointer_0;
                    ev.state = 0;
                    ev.utf8 = 0;
                    topaz_input_device_push_event(im->mouse, &ev);
                }



                if (eventNC.bstate & BUTTON2_PRESSED) {
                    ev.id = topazPointer_1;
                    ev.state = 1;
                    ev.utf8 = 0;
                    topaz_input_device_push_event(im->mouse, &ev);
                }
                if (eventNC.bstate & BUTTON2_RELEASED) {
                    ev.id = topazPointer_1;
                    ev.state = 0;
                    ev.utf8 = 0;
                    topaz_input_device_push_event(im->mouse, &ev);
                }
            }
            break;
          default: {
            int tk = ncurses_key_to_topaz_key(ch);
            if (tk != topazNotAnInput) {
                ev.id = tk;
                ev.state = 1;
                ev.utf8 = 0;
                topaz_input_device_push_event(im->keyboard, &ev);
                topaz_array_push(im->lastKeys, tk);
            }
            
          }
        }
    }
    return hasEvents;
}
static topazInputDevice_t * ncurses_input_manager_query_device(topazInputManagerAPI_t * api, int ID) {
    NCURSESINPUT * im = api->implementationData;
    switch(ID) {
      case 0: return im->keyboard;
      case 1: return im->mouse;
      default: return NULL;
    }
}

static int ncurses_input_manager_query_auxiliary_devices(topazInputManagerAPI_t * api, int * IDs) {
    return 0;
}

static int ncurses_input_manager_max_devices(topazInputManagerAPI_t * api) {
    return 2;
}

static void ncurses_input_manager_set_focus(topazInputManagerAPI_t * api, topazDisplay_t * disp) {
    NCURSESINPUT * im = api->implementationData;
    im->disp = disp;
}

static topazDisplay_t * ncurses_input_manager_get_focus(topazInputManagerAPI_t * api) {
    NCURSESINPUT * im = api->implementationData;
    return im->disp;
}
static void ncurses_input_manager_show_virtual_keyboard(topazInputManagerAPI_t * api, int doIt) {
}




void topaz_system_inputManager_ncurses__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazInputManagerAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("ncurses"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Input manager for NCURSES displays"),





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


    api->input_manager_create = ncurses_input_manager_create;
    api->input_manager_destroy = ncurses_input_manager_destroy;
    api->input_manager_handle_events = ncurses_input_manager_handle_events;
    api->input_manager_query_device = ncurses_input_manager_query_device;
    api->input_manager_query_auxiliary_devices = ncurses_input_manager_query_auxiliary_devices;
    api->input_manager_max_devices = ncurses_input_manager_max_devices;
    api->input_manager_set_focus = ncurses_input_manager_set_focus;
    api->input_manager_get_focus = ncurses_input_manager_get_focus;
    api->input_manager_show_virtual_keyboard = ncurses_input_manager_show_virtual_keyboard;

}





