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
#include <topaz/components/state_control.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all components 
// using state control functions are actually what they claim to be
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__STATE_CONTROL = "t0p4zst4t3c0ntr0l";
#endif










typedef struct {
    topazStateControl_Loop_t loop;
    topazString_t * label;
} StateControlState;


typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif

    topazArray_t * states; // used to be table, but state count is usually very low.
    topazString_t * current;
    topazStateControl_Loop_t loop;


    int hasCurrent;
    int midTerminate;
    int queuedInit;
} StateControl;






static void state_control__on_step(topazComponent_t * c, StateControl * s) {
    // TODO: just use component_set_attributes to update the step / draw 
    // to NULL
    if (topaz_state_control_is_halted(c)) return;


    // if this is the first Step set of this state,
    // we need to step any init functions of the state loop.
    if (s->queuedInit) {
        s->queuedInit = FALSE;
        if (s->loop.on_enter) {
            s->loop.on_enter(c, s->loop.data);
        }
        if (s->midTerminate) return;
    }


    // The stepFunc, while conventional, is not necessary. Merely skip  
    // if it DNE.
    if (s->loop.on_step) {
        s->loop.on_step(c, s->loop.data);        
    }
}

static void state_control__on_draw(topazComponent_t * c, StateControl * s) {
    if (topaz_state_control_is_halted(c) || !s->loop.on_draw) return;
    s->loop.on_draw(c, s->loop.data);
}


static void state_control__on_destroy(topazComponent_t * c, StateControl * s) {
    topaz_array_destroy(s->states);
    topaz_string_destroy(s->current);
    free(s);
}



static StateControlState * state_control__find(StateControl * s, const topazString_t * name) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->states);
    StateControlState * iter = topaz_array_get_data(s->states);
    for(i = 0; i < len; ++i, ++iter) {
        if (topaz_string_test_eq(iter->label, name)) {
            return iter;
        }
    }
    return NULL;
}

static void state_control__add(StateControl * s, const topazString_t * label, topazStateControl_Loop_t loop) {
    StateControlState state;
    state.label = topaz_string_clone(label);
    state.loop  = loop;

    topaz_array_push(s->states, state);
}


static int state_control__remove(StateControl * s, const topazString_t * label) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->states);
    StateControlState * iter = topaz_array_get_data(s->states);
    for(i = 0; i < len; ++i, ++iter) {
        if (topaz_string_test_eq(iter->label, label)) {
            topaz_string_destroy(iter->label);
            topaz_array_remove(s->states, i);
            return TRUE;
        }
    }
    return FALSE;
}














topazComponent_t * topaz_state_control_create(topaz_t * t) {
    StateControl * data = calloc(1, sizeof(StateControl));

    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__STATE_CONTROL;
    #endif

    data->current = topaz_string_create();
    data->states = topaz_array_create(sizeof(StateControlState));



    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_step    = (topaz_component_attribute_callback) state_control__on_step;
    attribs.on_draw    = (topaz_component_attribute_callback) state_control__on_draw;
    attribs.on_destroy = (topaz_component_attribute_callback) state_control__on_destroy;

    attribs.userData = data;
    return topaz_component_create(TOPAZ_STR_CAST("StateControl"), t, &attribs);
}


void topaz_state_control_add(topazComponent_t * c, const topazString_t * stateName, topazStateControl_Loop_t loop) {
    StateControl * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "StateControl instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__STATE_CONTROL);
    #endif

    StateControlState * state = state_control__find(s, stateName);
    // update
    if (state) {
        state->loop = loop;
    } else {
        state_control__add(s, stateName, loop);
    }
}



void topaz_state_control_remove(topazComponent_t * c, const topazString_t * stateName) {
    StateControl * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "StateControl instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__STATE_CONTROL);
    #endif
    
    if (state_control__remove(s, stateName)) {
        if (topaz_string_test_eq(s->current, stateName)) {
                
            if (s->loop.on_leave)
                s->loop.on_leave(c, s->loop.data);
                        
            s->midTerminate = TRUE;
            s->hasCurrent = FALSE;
        }
    }
}

void topaz_state_control_execute(topazComponent_t * c, const topazString_t * stateName) {
    StateControl * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "StateControl instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__STATE_CONTROL);
    #endif

    if (s->loop.on_leave)
        s->loop.on_leave(c, s->loop.data);


    StateControlState * state = state_control__find(s, stateName);
    if (!state) return;

    

    topaz_string_set(s->current, stateName);
    s->loop = state->loop;
    s->midTerminate = FALSE;
    s->queuedInit = TRUE;
    s->hasCurrent = TRUE;    
}


void topaz_state_control_halt(topazComponent_t * c) {
    StateControl * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "StateControl instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__STATE_CONTROL);
    #endif

    if (s->loop.on_leave)
        s->loop.on_leave(c, s->loop.data);


    topaz_string_clear(s->current);
    s->hasCurrent = FALSE;
    s->midTerminate = TRUE;
}

int topaz_state_control_is_halted(const topazComponent_t * c) {
    StateControl * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "StateControl instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__STATE_CONTROL);
    #endif

    return 
        !s->hasCurrent ||
         topaz_string_get_length(s->current)==0
    ;

}


const topazString_t * topaz_state_control_get_current(const topazComponent_t * c) {
    StateControl * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "StateControl instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__STATE_CONTROL);
    #endif

    return s->current;
}

