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


#ifndef H_TOPAZDC__SYSTEM__INCLUDED
#define H_TOPAZDC__SYSTEM__INCLUDED

#include <topaz/containers/string.h>
typedef struct topazBackend_t topazBackend_t;

/*

    System
    -----

    Bundles and abstracts system-level middleware that topaz 
    uses to implement the engine.

*/

typedef struct topazSystem_t topazSystem_t;



/// Called to register support system backends for this 
/// instance. Only needs to be called once, but should be called 
/// before any other call to topaz_system_ family of functions
///
void topaz_system_configure();


/// Adds a new backend to be recognized by the system.
/// Success is returned. Once its added, the backend can be 
/// passed as a backend argument for topaz_system_create. The first 
/// backend of a certain type added will be the so-called "default"
/// backend handler for that type.
///
int topaz_system_config_add_handler(
    /// The kind of backend.
    ///
    const topazString_t * backendType,

    /// The unique name of the backend
    ///
    const topazString_t * backendName,

    /// The function to be called that creates a new 
    /// backend instance 
    ///
    topazBackend_t * (*backend_callback)(),

    /// The function to be called that populates the 
    /// API. The api is specific to the backend
    ///
    void (*api_callback)(void *)
);






/// Creates an system instance that 
///
topazSystem_t * topaz_system_create_default();

/// Sets the backend implementation used for the backend specified.
/// Sucess is returned.
///
int topaz_system_set_backend(
    topazSystem_t *, 
    const topazString_t * backendType,
    const topazString_t * backendName
);


/// Returns whether the given backend is available for the given type.
///
int topaz_system_is_backend_available(
    const topazString_t * backendType,
    const topazString_t * backendName
);


/// Creates a backend instance 
topazBackend_t * topaz_system_create_backend(
    const topazSystem_t *, 
    const topazString_t * backendType, 
    void * APImapping
);


#endif