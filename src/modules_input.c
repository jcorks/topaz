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
#include <topaz/spatial.h>
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
    topaz_t * ctx;
    topazInputManager_t * manager;

    int previousUnicode;
    int lastUnicode;

    float mouseX;
    float mouseY;
    float mouseDeltaX;
    float mouseDeltaY;


    topazTable_t * stringMapInput; // [string *] -> MappedInputData*
    topazTable_t * inputMapString; // [mappedInputData] -> string*
    topazTable_t * stringListeners;  //[string] -> array[input listeners]

    DeviceState ** devices; 
    topaz_t * context;
    topazArray_t * queriedPads;
    topazArray_t * unicodeListeners;

    uint32_t listenerIDPool;
    topazArray_t * listenerIDPool_recycled;


    uint64_t time;
    uint64_t startTime;

};

typedef struct {
    DeviceState * device;
    int input;
} MappedInputData;

typedef struct {
    topazInput_Listener_t listener;
    int id;
} InputListener;

typedef struct {
    topazInput_UnicodeListener_t listener;
    int id;
} UnicodeListener;

static void get_unicode(topazInput_t *, float prevState, const topazInputDevice_Event_t *);

static float mouse_x_device_to_world_2d(topazDisplay_t *, const topazInput_t * input, float x);
static float mouse_y_device_to_world_2d(topazDisplay_t *, const topazInput_t * input, float x);



static void on_press__pointer(
    /// The input instance.
    topazInput_t * input, 

    /// The input button. Usually is a value of type topazKey.
    int inputButton, 

    /// The data bound to the callback.
    void * data
) {
    topazInput_Listener_t * ev = data;
    ev->on_press(
        input,
        inputButton,
        ev->userData
    );
}


static void on_active__pointer(
    topazInput_t * input, 
    int inputButton, 
    float value, 
    void * data
) {
    topazInput_Listener_t * ev = data;
    ev->on_active(
        input,
        inputButton,
        inputButton == topazPointer_X ? 
            input->mouseX
        :
            inputButton == topazPointer_Y ?
                input->mouseY
            :
                value,
        ev->userData
    );
}

static void on_release__pointer(
    topazInput_t * input, 
    int inputButton, 
    void * data
) {
    topazInput_Listener_t * ev = data;
    ev->on_release(
        input,
        inputButton,
        ev->userData
    );
}


static void on_update__pointer(
    topazInput_t * input, 
    int inputButton, 
    float value, 
    void * data
) {
    topazInput_Listener_t * ev = data;
    ev->on_update(
        input,
        inputButton,
        inputButton == topazPointer_X ? 
            input->mouseX
        :
            inputButton == topazPointer_Y ?
                input->mouseY
            :
                value,
        ev->userData
    );
}



/// holds the entire state of a device
struct DeviceState {
    topazInput_t * parent;
    topazTable_t * inputs; // [int]->input state *
    topazArray_t * listeners; /// topazInput_Listener_t
    topazTableIter_t * iter;
    topazInputDevice_t * device;

    topazArray_t * deletedListeners;
};


    static int listener_id_new(topazInput_t * input) {
        int id = 0;
        if (topaz_array_get_size(input->listenerIDPool_recycled)) {
            id = topaz_array_at(input->listenerIDPool_recycled, int, topaz_array_get_size(input->listenerIDPool_recycled)-1);
            topaz_array_set_size(input->listenerIDPool_recycled, topaz_array_get_size(input->listenerIDPool_recycled)-1);
        } else {
            id = input->listenerIDPool++;
        }
        return id;
    }

    static void listener_id_recycle(topazInput_t * input, uint32_t id) {
        topaz_array_push(input->listenerIDPool_recycled, id);
    }

    static DeviceState * device_state_create(topazInput_t * input) {
        DeviceState * out = malloc(sizeof(DeviceState));
        out->iter = topaz_table_iter_create();
        out->inputs = topaz_table_create_hash_pointer();
        out->listeners = topaz_array_create(sizeof(InputListener));
        out->parent = input;
        out->deletedListeners = topaz_array_create(sizeof(uint32_t));
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

    static topazInput_Listener_t * device_state_get_listener_by_id(DeviceState * d, int id) {
        uint32_t totalLen = topaz_array_get_size(d->listeners);
        uint32_t n;
        for(n = 0; n < totalLen; ++n) {
            if (topaz_array_at(d->listeners, InputListener, n).id == id) {
                return &topaz_array_at(d->listeners, InputListener, n).listener;
            }
        }
        return NULL;
    }

    static void device_state_update(topazInput_t * t, DeviceState * d) {
        // process removal request
        if (topaz_array_get_size(d->deletedListeners)) {
            uint32_t i;
            uint32_t n;
            uint32_t lenListeners = topaz_array_get_size(d->deletedListeners);
            for(i = 0; i < lenListeners; ++i) {
                uint32_t id = topaz_array_at(d->deletedListeners, uint32_t, i);
                uint32_t totalLen = topaz_array_get_size(d->listeners);
                for(n = 0; n < totalLen; ++n) {
                    if (topaz_array_at(d->listeners, InputListener, n).id == id) {
                        listener_id_recycle(t, id);
                        InputListener * list = &topaz_array_at(d->listeners, InputListener, n);
                        
                        // special case: pointer 
                        // Pointer listeners are wrappers managed by the input 
                        // module, and have an extra allocated listener that needs
                        // to be freed.
                        if (list->listener.on_active == on_active__pointer ||
                            list->listener.on_update == on_update__pointer) {
                            free(list->listener.userData);
                        }
                        

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
            if (index == 0) {
                // could be a symbolic unicode event
                if (ev.utf8 && topaz_input_device_get_type(d->device) == topaz_InputDevice_Class_Keyboard) {
                    InputState * input = device_state_get_input(d, index);
                    input->prev = input->current;
                    input->current = ev.state;
                    get_unicode(d->parent, input->current, &ev);
                }
                continue;
            }


            InputState * input = device_state_get_input(d, index);
            input->prev = input->current;
            input->current = ev.state;


            if (topaz_array_get_size(d->listeners)) {
                uint32_t i;
                uint32_t len = topaz_array_get_size(d->listeners);
                InputListener * inst = topaz_array_get_data(d->listeners);
                for(i = 0; i < len; ++i) {
                    if (inst->listener.on_update) inst->listener.on_update(d->parent, index, input->current, inst->listener.userData);
                    if (inst->listener.on_press && !input->prev && input->current)  
                        inst->listener.on_press(d->parent, index, inst->listener.userData);
                    if (inst->listener.on_release && input->prev && !input->current)  
                        inst->listener.on_release(d->parent, index, inst->listener.userData);
 
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
                    InputListener * inst = topaz_array_get_data(listeners);
                    for(i = 0; i < len; ++i) {
                        if (inst->listener.on_update) inst->listener.on_update(d->parent, index, input->current, inst->listener.userData);
                        if (inst->listener.on_press && !input->prev && input->current)
                            inst->listener.on_press(d->parent, index, inst->listener.userData);
                        if (inst->listener.on_release && input->prev && !input->current)
                            inst->listener.on_release(d->parent, index, inst->listener.userData);

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
                    InputListener * inst = topaz_array_get_data(d->listeners);
                    for(i = 0; i < len ; ++i) {
                        if (inst->listener.on_active)
                            inst->listener.on_active(
                                d->parent,
                                (int)(intptr_t)topaz_table_iter_get_key(d->iter),
                                input->current,
                                inst->listener.userData
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
                    InputListener * inst = topaz_array_get_data(listeners);
                    for(i = 0; i < len ; ++i) {
                        if (inst->listener.on_active)
                            inst->listener.on_active(
                                d->parent,
                                (int)(intptr_t)topaz_table_iter_get_key(d->iter),
                                input->current,
                                inst->listener.userData
                            );
                    }

                }
            }
        }
    }


    static int device_state_add_listener(topazInput_t * input, DeviceState * d, const topazInput_Listener_t * src) {
        int id = listener_id_new(input);
        InputListener l = {};
        l.listener = *src;
        l.id = id;
        topaz_array_push(d->listeners, l);
        return l.id;
    }

    static void device_state_remove_listener(DeviceState * d, uint32_t id) {
        topaz_array_push(d->deletedListeners, id);
    }







void topaz_input_poll(topazInput_t * t) {
    topazDisplay_t * display = topaz_context_get_iteration_display(t->ctx);
    topaz_input_manager_set_focus(t->manager, display);
    // pool raw events
    topaz_input_manager_handle_events(t->manager);


    // process raw events
    int i;
    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        t->devices[i]->device = topaz_input_manager_query_device(t->manager, i);
        if (t->devices[i]->device)
            device_state_update(t, t->devices[i]);
    }

    InputState * inputX = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_X);
    InputState * inputY = device_state_get_input(t->devices[topazInputManager_DefaultDevice_Mouse], topazPointer_Y);


    float oldX = t->mouseX;
    float oldY = t->mouseY;
    {
        topazEntity_t * e = topaz_display_get_2d_camera(display);
        const topazMatrix_t * m = topaz_spatial_get_global_transform(topaz_entity_get_spatial(e));
        topazVector_t p = {
            inputX->current - topaz_display_get_parameter(display, topazDisplay_Parameter_Width)/2.0,
            (topaz_display_get_parameter(display, topazDisplay_Parameter_Height) - inputY->current) - topaz_display_get_parameter(display, topazDisplay_Parameter_Height)/2.0,
            0
        };
        p = topaz_matrix_transform(
            m,
            &p
        );
        t->mouseX = p.x;
        t->mouseY = p.y;
    }


    t->mouseDeltaX = t->mouseX - oldX;
    t->mouseDeltaY = t->mouseY - oldY;


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
    out->ctx = context;
    out->stringMapInput = topaz_table_create_hash_topaz_string();
    out->inputMapString = topaz_table_create_hash_buffer(sizeof(MappedInputData));
    out->stringListeners = topaz_table_create_hash_topaz_string();
    out->queriedPads = topaz_array_create(sizeof(int));
    out->devices = calloc(topazInputManager_DefaultDevice_Count, sizeof(DeviceState*));

    out->unicodeListeners         = topaz_array_create(sizeof(UnicodeListener));
    out->listenerIDPool_recycled = topaz_array_create(sizeof(int));
    out->listenerIDPool = 1;

    uint32_t i;


    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        out->devices[i] = device_state_create(out);
    }



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



int topaz_input_add_keyboard_listener(topazInput_t * t, const topazInput_Listener_t * l) {
    DeviceState * input = t->devices[topazInputManager_DefaultDevice_Keyboard];
    return device_state_add_listener(t, input, l);
}



int topaz_input_add_pointer_listener(topazInput_t * t, const topazInput_Listener_t * l) {
    DeviceState * input = t->devices[topazInputManager_DefaultDevice_Mouse];

    // need to add wrappping mechanism to deliver converted
    // mouse coordinates
    if (l->on_active ||
        l->on_update)  {
        topazInput_Listener_t listener = {};


        topazInput_Listener_t * evdata = calloc(1, sizeof(topazInput_Listener_t));
        *evdata = *l;

        listener.userData = evdata;
        if (evdata->on_active)  listener.on_active  = on_active__pointer;
        if (evdata->on_press)   listener.on_press   = on_press__pointer;
        if (evdata->on_update)  listener.on_update  = on_update__pointer;
        if (evdata->on_release) listener.on_release = on_release__pointer;

        return device_state_add_listener(t, input, &listener);

    }

    return device_state_add_listener(t, input, l);
}

int topaz_input_add_pad_listener(topazInput_t * t, const topazInput_Listener_t * l, int pad) {
    if (pad >= 4) return -1;
    DeviceState * input = t->devices[topazInputManager_DefaultDevice_Pad1+pad];
    return device_state_add_listener(t, input, l);
}

int topaz_input_add_mapped_listener(topazInput_t * t, const topazInput_Listener_t * src, const topazString_t * str) {
    topazArray_t * arr = topaz_table_find(t->stringListeners, str);
    if (!arr) {
        arr = topaz_array_create(sizeof(InputListener));        
        topaz_table_insert(t->stringListeners, str, arr);
    }
    InputListener l;
    l.listener = *src;
    l.id = listener_id_new(t);
    topaz_array_push(arr, l);
    return l.id;
}

const topazInput_Listener_t * topaz_input_get_listener(
    topazInput_t * t,
    int id
) {
    uint32_t i;
    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        if (t->devices[i]) {
            const topazInput_Listener_t * out = device_state_get_listener_by_id(t->devices[i], id);
            if (out) return out;
        }
    }
    return NULL;

}




void topaz_input_remove_listener(topazInput_t * t, int id) {
    uint32_t i;
    for(i = 0; i < topazInputManager_DefaultDevice_Count; ++i) {
        if (t->devices[i]) {
            device_state_remove_listener(t->devices[i], id);
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
                if (topaz_array_at(inst, InputListener, i).id == id) {
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





int topaz_input_add_unicode_listener(topazInput_t * t, const topazInput_UnicodeListener_t * l) {
    UnicodeListener ul = {};
    ul.listener = *l;
    ul.id = listener_id_new(t);
    topaz_array_push(t->unicodeListeners, ul);
    return ul.id;
}


const topazInput_UnicodeListener_t * topaz_input_get_unicode_listener(topazInput_t * t, int id) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->unicodeListeners);
    for(i = 0; i < len; ++i) {
        UnicodeListener * ul = &topaz_array_at(t->unicodeListeners, UnicodeListener, i);
        if (ul->id == id) {
            return &ul->listener;
        }
    }
}

void topaz_input_remove_unicode_listener(topazInput_t * t, int id) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->unicodeListeners);
    for(i = 0; i < len; ++i) {
        UnicodeListener * ul = &topaz_array_at(t->unicodeListeners, UnicodeListener, i);
        if (ul->id == id) {
            listener_id_recycle(t, id);
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
    return t->mouseDeltaX;
}

int topaz_input_mouse_delta_y(const topazInput_t * t) {
    return t->mouseDeltaY;
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

