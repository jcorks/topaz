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

#include <topaz/compat.h>
#include <topaz/modules/input.h>
#include <topaz/modules/view_manager.h>
#include <topaz/input_device.h>
#include <topaz/containers/table.h>
#include <topaz/system.h>
#include <topaz/backends/display.h>
#include <topaz/backends/input_manager.h>
#include <topaz/entity.h>
#include <topaz/topaz.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

typedef struct DeviceState DeviceState;


typedef struct {
    float prev;
    float current;
    topazString_t * mapped;
    topazInputDevice_t * device;
} InputState;

struct topazInput_t {
    topazInputManager_t * manager;

    int previousUnicode;
    int lastUnicode;
    int mouseX;
    int mouseY;


    topazTable_t * stringMapInput; // [string *] -> MappedInputData*
    topazTable_t * inputMapString; // [mappedInputData] -> string*
    topazTable_t * stringListeners;  //[string] -> array[input listeners]

    DeviceState ** devices; 
    topazDisplay_t * focus;
    topazEntity_t * contextEnt;
    topaz_t * context;
    topazArray_t * queriedPads;
    topazArray_t * unicodeListeners;


    uint64_t time;
    uint64_t startTime;

};

typedef struct {
    DeviceState * device;
    int input;
} MappedInputData;

static void get_unicode(topazInput_t *, float prevState, const topazInputDevice_Event_t *);

static int mouse_x_device_to_world_2d(const topazInput_t * input, int x);
static int mouse_y_device_to_world_2d(const topazInput_t * input, int x);

/// holds the entire state of a device
struct DeviceState {
    topazInput_t * parent;
    topazTable_t * inputs; // [int]->input state *
    topazArray_t * listeners; /// topazInput_Listener_t
    topazTableIter_t * iter;
    topazInputDevice_t * device;

    topazArray_t * deletedListeners;
};


    static DeviceState * device_state_create(topazInput_t * input) {
        DeviceState * out = malloc(sizeof(DeviceState));
        out->iter = topaz_table_iter_create();
        out->inputs = topaz_table_create_hash_pointer();
        out->listeners = topaz_array_create(sizeof(topazInput_Listener_t));
        out->parent = input;
        out->deletedListeners = topaz_array_create(sizeof(topazInput_Listener_t));
        return out;
    }

    static void device_state_destroy(DeviceState * d) {
        topaz_array_destroy(d->listeners);
        topazTableIter_t * iter = d->iter;
        for(topaz_table_iter_start(iter, d->inputs);
            !topaz_table_iter_is_end(iter);
            topaz_table_iter_proceed(iter)) {
            free(topaz_table_iter_get_value(iter));
        }
        topaz_table_iter_destroy(iter);
        topaz_table_destroy(d->inputs);
        topaz_array_destroy(d->deletedListeners);
        free(d);
    }

    static InputState * device_state_get_input(DeviceState * d, int index) {
        InputState * input = topaz_table_find_by_int(d->inputs, index);
        if (!input) {
            input = calloc(1, sizeof(InputState));
            topaz_table_insert_by_int(d->inputs, index, input);
        }

        return input;
    }


    void device_state_update(DeviceState * d) {
        // process removal request
        if (topaz_array_get_size(d->deletedListeners)) {
            uint32_t i;
            uint32_t n;
            uint32_t lenListeners = topaz_array_get_size(d->deletedListeners);
            for(i = 0; i < lenListeners; ++i) {
                topazInput_Listener_t * l = &topaz_array_at(d->deletedListeners, topazInput_Listener_t, i);
                uint32_t totalLen = topaz_array_get_size(d->listeners);
                for(n = 0; n < totalLen; ++n) {
                    if (memcmp(l, &topaz_array_at(d->listeners, topazInput_Listener_t, n), sizeof(topazInput_Listener_t)) == 0) {
                        topaz_array_remove(d->listeners, n);
                        break;
                    }
                }
            }
            topaz_array_set_size(d->deletedListeners, 0);
        }





        topazInputDevice_Event_t ev;
        if (!d->device) return;
        while(topaz_input_device_get_event_count(d->device)) {
            topaz_input_device_pop_event(d->device, &ev);
            int index = (int)ev.id;


            InputState * input = device_state_get_input(d, index);
            input->prev = input->current;
            input->current = ev.state;

            if (topaz_array_get_size(d->listeners)) {
                uint32_t i;
                uint32_t len = topaz_array_get_size(d->listeners);
                topazInput_Listener_t * inst = topaz_array_get_data(d->listeners);
                for(i = 0; i < len; ++i) {
                    if (inst->on_update) inst->on_update(d->parent, index, input->current, inst->userData);
                    if (inst->on_press && !input->prev && input->current)  
                        inst->on_press(d->parent, index, inst->userData);
                    if (inst->on_release && !input->prev && input->current)  
                        inst->on_release(d->parent, index, inst->userData);
 
                    inst++;
                }
                
            }


            // string handlers
            if (!topaz_table_is_empty(d->parent->inputMapString) &&
                !topaz_table_is_empty(d->parent->stringListeners)) {

                MappedInputData key;
                key.device = d;
                key.input = index;

                topazString_t * iter = topaz_table_find(d->parent->inputMapString, &key);
                if (iter) {
                    topazArray_t * listeners = topaz_table_find(d->parent->stringListeners, iter);

                    uint32_t i;
                    uint32_t len = topaz_array_get_size(listeners);
                    topazInput_Listener_t * inst = topaz_array_get_data(listeners);
                    for(i = 0; i < len; ++i) {
                        if (inst->on_update) inst->on_update(d->parent, index, input->current, inst->userData);
                        if (inst->on_press && !input->prev && input->current)
                            inst->on_press(d->parent, index, inst->userData);
                        if (inst->on_release && !input->prev && input->current)
                            inst->on_release(d->parent, index, inst->userData);

                        inst++;
                    }
                }

            }

            if (topaz_input_device_get_type(d->device) == topaz_InputDevice_Class_Keyboard) {
                get_unicode(d->parent, input->current, &ev);
            }

        }

        // active input callbacks
        if (topaz_array_get_size(d->listeners)) {
            for(topaz_table_iter_start(d->iter, d->inputs);
                !topaz_table_iter_is_end(d->iter);
                topaz_table_iter_proceed(d->iter)) {

                InputState * input = topaz_table_iter_get_value(d->iter);
                if (fabs(input->current) > .0001) {
                    uint32_t i;
                    uint32_t len = topaz_array_get_size(d->listeners);
                    topazInput_Listener_t * inst = topaz_array_get_data(d->listeners);
                    for(i = 0; i < len ; ++i) {
                        if (inst->on_active)
                            inst->on_active(
                                d->parent,
                                (int)(intptr_t)topaz_table_iter_get_key(d->iter),
                                input->current,
                                inst->userData
                            );
                    }
                }

            }
        }



        // string listener actives
        if (!topaz_table_is_empty(d->parent->inputMapString)  &&
            !topaz_table_is_empty(d->parent->stringListeners)) {

            for(topaz_table_iter_start(d->iter, d->parent->inputMapString);
                !topaz_table_iter_is_end(d->iter);
                topaz_table_iter_proceed(d->iter)) {

                const MappedInputData * m = topaz_table_iter_get_key(d->iter);
                if (m->device != d) continue;

                InputState * input = device_state_get_input(m->device, m->input);
                if (fabs(input->current) > .0001) {
                    topazArray_t * listeners = topaz_table_find(d->parent->stringListeners, topaz_table_iter_get_value(d->iter));
                    uint32_t i;
                    uint32_t len = topaz_array_get_size(listeners);
                    topazInput_Listener_t * inst = topaz_array_get_data(listeners);
                    for(i = 0; i < len ; ++i) {
                        if (inst->on_active)
                            inst->on_active(
                                d->parent,
                                (int)(intptr_t)topaz_table_iter_get_key(d->iter),
                                input->current,
                                inst->userData
                            );
                    }

                }
            }
        }
    }


    static void device_state_add_listener(DeviceState * d, const topazInput_Listener_t * l) {
        topaz_array_push(d->listeners, *l);
    }

    static void device_state_remove_listener(DeviceState * d, const topazInput_Listener_t * l) {
        topaz_array_push(d->deletedListeners, *l);
    }







static void input_ent__step(topazEntity_t * e, void * data) {
    topazInput_t * t = data;


    // Set the focus to the display that has input focus (which
    // isnt necessarily the main display);
    {
        const topazArray_t * dpys = topaz_view_manager_get_all(topaz_context_get_view_manager(t->context));
        topazDisplay_t * focus = NULL;
        uint32_t i;
        uint32_t len = topaz_array_get_size(dpys);
        for(i = 0; i < len; ++i) {
            focus = topaz_array_at(dpys, topazDisplay_t*, i);
            if (focus && topaz_display_has_input_focus(focus)) {
                break;
            }
            focus = NULL;
        }
        topaz_input_manager_set_focus(t->manager, focus);
        t->focus = focus;
    }



    // pool raw events
    topaz_input_manager_handle_events(t->manager);


    // process raw events
    int i;
    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        t->devices[i]->device = topaz_input_manager_query_device(t->manager, i);
        if (t->devices[i]->device)
            device_state_update(t->devices[i]);
    }

    InputState * inputX = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_X);
    InputState * inputY = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_Y);


    

    t->mouseY = mouse_y_device_to_world_2d(t, inputY->current);
    t->mouseX = mouse_x_device_to_world_2d(t, inputX->current);

}


topazInput_t * topaz_input_create(topaz_t * context) {
    topazInput_t * out = calloc(1, sizeof(topazInput_t));
    {
        topazInputManagerAPI_t api = {};
        topazSystem_Backend_t * ref = topaz_system_create_backend(
            topaz_context_get_system(context), 
            TOPAZ_STR_CAST("inputManager"), 
            &api
        );
        out->manager = topaz_input_manager_create(context, ref, api);
    }
 
    out->stringMapInput = topaz_table_create_hash_topaz_string();
    out->inputMapString = topaz_table_create_hash_buffer(sizeof(MappedInputData));
    out->stringListeners = topaz_table_create_hash_topaz_string();
    out->queriedPads = topaz_array_create(sizeof(int));
    out->devices = calloc(topazInputManager_DefaultDevice_Count, sizeof(DeviceState*));

    out->unicodeListeners         = topaz_array_create(sizeof(topazInput_UnicodeListener_t));


    uint32_t i;

    topazEntity_Attributes_t attribs = {};
    attribs.on_step = (topaz_entity_attribute_callback) input_ent__step;
    attribs.userData = out;

    out->contextEnt = topaz_entity_create_with_attributes(context, &attribs);
    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        out->devices[i] = device_state_create(out);
    }

    topaz_context_attach_pre_manager_unpausable(
        context,
        out->contextEnt
    );

    out->context = context;
    return out;
}


void topaz_input_destroy(topazInput_t * t) {
    uint32_t i;

    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        device_state_destroy(t->devices[i]);
    }
    free(t->devices);
    topaz_table_destroy(t->stringMapInput);
    topaz_table_destroy(t->inputMapString);
    topaz_table_destroy(t->stringListeners);
    topaz_array_destroy(t->queriedPads);

    topaz_array_destroy(t->unicodeListeners);
    free(t);
}



void topaz_input_add_keyboard_listener(topazInput_t * t, const topazInput_Listener_t * l) {
    DeviceState * input = t->devices[topazInputManager_DefaultDevice_Keyboard];
    device_state_add_listener(input, l);
}


void topaz_input_add_pointer_listener(topazInput_t * t, const topazInput_Listener_t * l) {
    DeviceState * input = t->devices[topazInputManager_DefaultDevice_Mouse];
    device_state_add_listener(input, l);
}

void topaz_input_add_pad_listener(topazInput_t * t, const topazInput_Listener_t * l, int pad) {
    if (pad >= 4) return;
    DeviceState * input = t->devices[topazInputManager_DefaultDevice_Pad1+pad];
    device_state_add_listener(input, l);
}

void topaz_input_add_mapped_listener(topazInput_t * t, const topazInput_Listener_t * l, const topazString_t * str) {
    topazInput_Listener_t * in = malloc(sizeof(topazInput_Listener_t));
    *in = *l;
    topaz_table_insert(t->stringListeners, str, in);
}





void topaz_input_remove_listener(topazInput_t * t, const topazInput_Listener_t * l) {
    uint32_t i;
    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        if (t->devices[i]) {
            device_state_remove_listener(t->devices[i], l);
        }
    }

    if (!topaz_table_is_empty(t->stringListeners)) {
        topazTableIter_t * iter = topaz_table_iter_create();

        for(topaz_table_iter_start(iter, t->stringListeners);
            !topaz_table_iter_is_end(iter);
            topaz_table_iter_proceed(iter)) {

            
            topazArray_t * inst = topaz_table_iter_get_value(iter);
            uint32_t len = topaz_array_get_size(inst);
            for(i = 0; i < len; ++i) {
                if (memcmp(&topaz_array_at(inst, topazInput_Listener_t, i), l, sizeof(topazInput_Listener_t)) == 0) {
                    topaz_array_remove(inst, i);
                    return;
                }
            }
        }
    }
}








float topaz_input_get_state(const topazInput_t * t, int inputKey) {
    InputState * input = NULL;
    if (inputKey < topazPointer_0) {
        input = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Keyboard], inputKey);
    } else {
        input = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], inputKey);
    }

    return input->current;
}


float topaz_input_get_pad_state(const topazInput_t * t, int pad, int i) {
    if (pad >= 4) return 0.f;
    InputState * input = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Pad1+pad], i);
    return input->current;
}


float topaz_input_get_mapped_state(const topazInput_t * t, const topazString_t * str) {
    MappedInputData * m = topaz_table_find(t->stringMapInput, str);
    if (!m) return 0;

    InputState * input = device_state_get_input(m->device, m->input);
    return input->current;
}




void topaz_input_set_deadzone(topazInput_t * t, int padID, int input, float deadZone) {
    if (padID >= 4) return;
    padID += topazInputManager_DefaultDevice_Pad1;
    if (t->devices[padID]->device) {
        topaz_input_device_set_deadzone(t->devices[padID]->device, input, deadZone);
    }
}


const topazArray_t  * topaz_input_query_pads(const topazInput_t * t) {
    int i;
    topaz_array_set_size(t->queriedPads, 0);
    for(i = topazInputManager_DefaultDevice_Pad1; 
        i < topazInputManager_DefaultDevice_Count;
        ++i) {
        
        if (t->devices[i]->device) {
            topaz_array_push(t->queriedPads, i);
        }
    }
    return t->queriedPads;
}





void topaz_input_add_unicode_listener(topazInput_t * t, const topazInput_UnicodeListener_t * l) {
    topaz_array_push(t->unicodeListeners, *l);
}

void topaz_input_remove_unicode_listener(topazInput_t * t, const topazInput_UnicodeListener_t * l) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->unicodeListeners);
    for(i = 0; i < len; ++i) {
        if (memcmp(&topaz_array_at(t->unicodeListeners, topazInput_UnicodeListener_t, i), l, sizeof(topazInput_UnicodeListener_t)) == 0) {
            topaz_array_remove(t->unicodeListeners, i);
            return;
        }
    }
}



void topaz_input_map(topazInput_t * t, const topazString_t * str, int input) {
    DeviceState * state;
    if (input < topazPointer_0) {
        state = t->devices[topazInputManager_DefaultDevice_Keyboard];
    } else {
        state = t->devices[topazInputManager_DefaultDevice_Mouse];
    }

    MappedInputData * data = malloc(sizeof(MappedInputData));
    data->device = state;
    data->input = input;

    topaz_table_insert(t->stringMapInput, str, data);
    topaz_table_insert(t->inputMapString, data, topaz_string_clone(str));
}

void topaz_input_map_pad(topazInput_t * t, const topazString_t * str, int pad, int input) {
    if (pad >= 4) return;

    DeviceState * state = t->devices[pad+topazInputManager_DefaultDevice_Pad1];
    MappedInputData * data = malloc(sizeof(MappedInputData));
    data->device = state;
    data->input = input;

    topaz_table_insert(t->stringMapInput, str, data);
    topaz_table_insert(t->inputMapString, data, topaz_string_clone(str));

}

void topaz_input_unmap(topazInput_t * t, const topazString_t * str) {
    MappedInputData * data = topaz_table_find(t->stringMapInput, str);
    topazString_t * otherStr = topaz_table_find(t->inputMapString, data);

    topaz_string_destroy(otherStr);
    free(data);
}





int topaz_input_mouse_x(const topazInput_t * t) {
    return t->mouseX;
}

int topaz_input_mouse_y(const topazInput_t * t) {
    return t->mouseY;
}

int topaz_input_mouse_delta_x(const topazInput_t * t) {
    InputState * input = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_X);

    return mouse_x_device_to_world_2d(t, input->current) -
           mouse_x_device_to_world_2d(t, input->prev);
}

int topaz_input_mouse_delta_y(const topazInput_t * t) {
    InputState * input = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_Y);

    return mouse_y_device_to_world_2d(t, input->current) -
           mouse_y_device_to_world_2d(t, input->prev);

}


float topaz_input_mouse_wheel(const topazInput_t * t) {
    InputState * input = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_Wheel);
    return input->current;
}

void get_unicode(topazInput_t * t, float prevState, const topazInputDevice_Event_t * event) {    
    int unicode = event->utf8;

    // Filter newline
    switch(unicode) {
      case 12:
      case 13:
        unicode = 10;
        break;
      default:;
    }    



    t->lastUnicode = unicode;
    if (!t->lastUnicode) return;



    if (t->lastUnicode != t->previousUnicode) t->startTime = topaz_context_get_time(t->context);

    uint32_t i;
    uint32_t len;
    topazInput_UnicodeListener_t * iter;
    if (event->state) {
        len = topaz_array_get_size(t->unicodeListeners);
        iter = topaz_array_get_data(t->unicodeListeners);
        for(i = 0; i < len; ++i) {
            if (iter->on_new_unicode) 
                iter->on_new_unicode(t, t->lastUnicode, iter->userData);
            iter++;
        }            
    } else if (event->state) {
    
        //printf("aaa %d vs %d\n", lastUnicode, previousUnicode);

        // Key has been held, implying a multiple key request (for us / latin keyboards)
        if (t->lastUnicode == t->previousUnicode && t->lastUnicode) {
            if ((topaz_context_get_time(t->context) - t->startTime > 500) && 
                (topaz_context_get_time(t->context) - t->time > 100)) {
                            
                len = topaz_array_get_size(t->unicodeListeners);
                iter = topaz_array_get_data(t->unicodeListeners);
                for(i = 0; i < len; ++i) {
                    if (iter->on_repeat_unicode) 
                        iter->on_repeat_unicode(t, t->lastUnicode, iter->userData);
                    iter++;
                }            

                t->time = topaz_context_get_time(t->context);
            }
        }
    }
    t->previousUnicode = t->lastUnicode;
}


int mouse_x_device_to_world_2d(const topazInput_t * t, int x) {
    if (!t->focus) return 0;
    return  // camera position x +
            // x * (camera render width / view width )
            x;
}


int mouse_y_device_to_world_2d(const topazInput_t * t, int y) {
    if (!t->focus) return 0;
    return  // camera position y +
            // y * (camera render height / view height )
            y;
}
