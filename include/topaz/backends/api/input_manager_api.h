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


#ifndef H_TOPAZDC__INPUT_MANAGER_API__INCLUDED
#define H_TOPAZDC__INPUT_MANAGER_API__INCLUDED

#include <stdint.h>


typedef struct topazInputManager_t topazInputManager_t;
typedef struct topazDisplay_t topazDisplay_t;
typedef struct topaz_t topaz_t;
/*

    InputManagerAPI
    -----
    
    The set of functions that define how the time abstraction should 
    behave. Creating filsystem abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/time.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazInputManagerAPI_t topazInputManagerAPI_t;


/// Each function is an implementation-facing copy of 
/// the user-side API for topazInputManager_t. See <topaz/backends/input_manager.h>
///
struct topazInputManagerAPI_t {
    void *                  (*input_manager_create)           (topazInputManager_t *, topaz_t *);
    void                    (*input_manager_destroy)          (topazInputManager_t *, void *);

    int                  (*input_manager_handle_events)          (topazInputManager_t *, void *);
    topazInputDevice_t * (*input_manager_query_device)           (topazInputManager_t *, void *, int ID);
    int                  (*input_manager_query_auxiliary_devices)(topazInputManager_t *, void *, int * IDs);
    int                  (*input_manager_max_devices)            (topazInputManager_t *, void *);
    void                 (*input_manager_set_focus)              (topazInputManager_t *, void *, topazDisplay_t *);
    topazDisplay_t *     (*input_manager_get_focus)              (topazInputManager_t *, void *);
    void                 (*input_manager_show_virtual_keyboard)  (topazInputManager_t *, void *, int);


};



#endif
