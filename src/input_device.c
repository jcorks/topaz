#include <topaz/compat.h>
#include <topaz/input_device.h>
#include <topaz/math.h>
#include <topaz/containers/table.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazInputDevice_t {
    topazTable_t * deadZone;
    topazInputDeviceEvent_t * data;
    int allocated;
    int iterFront;
    int iterBack;
    topazInputDevice_Class type;
};

#define default_action_queue_size 1024


/// Creates a new input device
///
topazInputDevice_t * topaz_input_device_create(topazInputDevice_Class c) {
    topazInputDevice_t * out = calloc(1, sizeof(topazInputDevice_t));
    out->data = calloc(sizeof(topazInputDeviceEvent_t), default_action_queue_size);
    out->allocated = default_action_queue_size;
    out->deadZone = topaz_table_create_hash_pointer();
    return out;
}

/// Destroys an input device.
///
void topaz_input_device_destroy(topazInputDevice_t * t) {
    topazTableIter_t * iter = topaz_table_iter_create();

    for(topaz_table_iter_start(iter, t->deadZone);
        topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        free((void*)topaz_table_iter_get_key(iter));
    }
    topaz_table_destroy(t->deadZone);
    free(t->data);
    free(t);
}






/// Adds a state change event for the given input within
/// this device
void topaz_input_device_push_event(topazInputDevice_t * t, const topazInputDeviceEvent_t * evt) {
    
    if (t->iterBack >= t->allocated) {
        // move to the front
        if (t->iterFront > 0) {
            memmove(t->data, t->data+t->iterFront, (t->iterBack-t->iterFront)*sizeof(topazInputDeviceEvent_t));
            t->iterBack -= t->iterFront;
            t->iterFront = 0;
        }

        if (t->iterBack >= t->allocated) {
            t->allocated *= 1.4;
            t->data = realloc(t->data, t->allocated*sizeof(topazInputDeviceEvent_t));
        }
    }

    t->data[t->iterBack++] = *evt;

    // deadzone calculation
    float * deadZPtr = topaz_table_find_by_int(t->deadZone, evt->id);
    if (deadZPtr) {
        float deadz = *deadZPtr;
        if (deadz > .001) {
            float val = t->data[t->iterBack-1].state; 
            if (val > deadz) {
                val = (val - deadz) / (1 - deadz);
                t->data[t->iterBack-1].state = val;
            } else if (val < -deadz) {
                val = (val + 1) / (-deadz + 1);
                t->data[t->iterBack-1].state = val-1;
            } else {
                t->data[t->iterBack-1].state = 0;
            }
        }
    }
}


int topaz_input_device_pop_event(topazInputDevice_t * t, topazInputDeviceEvent_t * ev) {
    if (t->iterFront >= t->iterBack) return FALSE;
    *ev = t->data[t->iterFront++];
    return TRUE;
}

topazInputDevice_Class topaz_input_device_get_type(const topazInputDevice_t * t) {
    return t->type;
}


int topaz_input_device_get_event_count(const topazInputDevice_t * t) {
    return t->iterBack - t->iterFront;
}

void topaz_input_device_set_deadzone(topazInputDevice_t * t, int id, float f) {
    float * v = topaz_table_find_by_int(t->deadZone, id);
    if (!v) {
        v = malloc(sizeof(float));
        topaz_table_insert_by_int(t->deadZone, id, v);
    }
    *v = f;
}


