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


#ifndef H_TOPAZDC__STATE_CONTROL__INCLUDED
#define H_TOPAZDC__STATE_CONTROL__INCLUDED

#include <topaz/component.h>

/*

    StateControl
    -----
    Adds a state machine mechanism to the entity.
   
    Each StateControl instance is effectively a dynamic state machine.  
    After desgnating what states exist within the machine using
    topaz_state_control_add(), the machine can be run using topaz_state_control_execute(). 
    Using each state's on_step, it will iterate till halted or its state altered.

*/


typedef struct topazStateControl_Loop_t topazStateControl_Loop_t;


/// Designates execution instructions for a state.
/// During each state, different logic is designated to run to control
/// what happens during this state and when the state should be altered.
/// The on_step of the state loop is is meant to hold the bulk of the logic. 
/// The on_draw can be used to produce visual effects associated with the state.
/// The state may be changed any time using Execute.
///
struct topazStateControl_Loop_t {
    /// Function called when stepping in this state
    ///
    topaz_component_attribute_callback on_step;

    /// Function called when drawing in this state
    ///
    topaz_component_attribute_callback on_draw;

    /// Function called when entering this state
    ///
    topaz_component_attribute_callback on_init;


    /// Data passed as the last argument for loop functions.
    ///
    void * data;

}; 



/// Creates a new state control component.
/// Use component_destroy to release.
///
topazComponent_t * topaz_state_control_create(topaz_t *);


/// Sets up a new valid state.
/// If an existing state fo the same name exists, it is updated.
///
void topaz_state_control_add(topazComponent_t *, const topazString_t * stateName, topazStateControl_Loop_t);


/// Removes the state.
/// No action is taken if the tag does not refer to a valid state.
///
void topaz_state_control_remove(topazComponent_t *, const topazString_t * stateName);

/// Begins the state machine execution loop from the given state.
/// If begin state does not refer to a valid state, no action is taken.
/// If done in the middle of a state loop function, any queued 
/// function calls are cancelled.
///
void topaz_state_control_execute(topazComponent_t *, const topazString_t *);

/// Stops all current execution.
/// If done during a state loop, all remaining state loop functions 
/// that would have executed are cancelled.
///
void topaz_state_control_halt(topazComponent_t *);

/// Returns whether or not the machine is in a halted state.
///
int topaz_state_control_is_halted(const topazComponent_t *);

/// Returns the current state tag. If no execution state is active, the empty string is returned.
///
const topazString_t * topaz_state_control_get_current(const topazComponent_t *);




#endif
