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
typedef struct topazArray_t topazArray_t;



///
///    Bundles and abstracts system-level middleware that topaz 
///    uses to implement the engine.
///
typedef struct topazSystem_t topazSystem_t;




 
/// Represents a discrete API implementation for a system.
///
/// Backends are built to handle function calls and 
/// data external to the main topaz codebase.
///
typedef struct topazSystem_Backend_t topazSystem_Backend_t;


/// Callback for when backends update themselves.
typedef void (*topaz_system_backend_update_callback)(
    /// The backend to be updated.
    topazSystem_Backend_t * backend,

    // The data bound to the callback.
    void * callbackData
);


/// Populates the backend with information.
/// Only allowed to be called within backend_callback calls 
/// given to topaz_system_config_add_handler
///
void topaz_system_backend_bind(
    /// The backend to bind data to.
    topazSystem_Backend_t * backend,

    /// The name of the backend.
    const topazString_t * name,

    /// The version of the backend
    const topazString_t * version,

    /// The author of the backend
    const topazString_t * author,

    /// A description of the backend
    const topazString_t * description,




    /// Called when the backend is stepped. Backend is passed.
    ///  
    topaz_system_backend_update_callback on_pre_step,

    /// Called after all the backends are stepped. Backend is passed.
    ///
    topaz_system_backend_update_callback on_post_step,

    /// Called when the backend is drawn. Backend is passed.
    ///
    topaz_system_backend_update_callback on_pre_draw,

    /// Called after all the backends have drawn. Backend is passed.
    ///
    topaz_system_backend_update_callback on_post_draw,

    /// User-provided data.
    ///
    void * userData,


    /// Topaz major version number
    ///
    int topazMajorVersion,

    /// Topaz minor version number
    ///
    int topazMinorVersion,

    /// Topaz micro version number
    ///
    int topazMicroVersion

);




/// Returns the major version of the topaz library 
/// that this backend was built with.
///
int topaz_system_backend_get_topaz_major_version(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);

/// Returns the minor version of the topaz library 
/// that this backend was built with.
///
int topaz_system_backend_get_topaz_minor_version(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);

/// Returns the micro version of the topaz library 
/// that this backend was build with.
///
int topaz_system_backend_get_topaz_micro_version(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);





/// Gets the name of this backend.
///
const topazString_t * topaz_system_backend_get_name(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);


/// Gets the name of this backend.
///
const topazString_t * topaz_system_backend_get_version(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);

/// Gets the author name 
///
const topazString_t * topaz_system_backend_get_author(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);

/// Gets the description 
///
const topazString_t * topaz_system_backend_get_description(
    /// The backend to query.
    const topazSystem_Backend_t * backend
);





/// Steps the backend
///
void topaz_system_backend_pre_step(
    /// The backend to step.
    topazSystem_Backend_t * backend
);

/// Posts that initialization has finished
///
void topaz_system_backend_post_step(
    /// The backend to step.
    topazSystem_Backend_t * backend
);


/// Draws the backend
///
void topaz_system_backend_pre_draw(
    /// The backend to draw.
    topazSystem_Backend_t * backend
);


/// Posts that initialization has finished
///
void topaz_system_backend_post_draw(
    /// The backend to draw.
    topazSystem_Backend_t * backend
);


/// Retrieves the user data associated with the backend.
///
void * topaz_system_backend_get_user_data(
    /// The backend to query.
    topazSystem_Backend_t * backend
);






/// Called to register support system backends for this 
/// instance. Only needs to be called once, but should be called 
/// before any other call to topaz_system_ family of functions
///
void topaz_system_configure();



/// Creates an system instance that uses the first 
/// added backend handlers as the defaults.
///
topazSystem_t * topaz_system_create_default();

/// Sets the backend implementation used for the backend specified.
/// Sucess is returned.
///
int topaz_system_set_backend_handler(
    /// The system to modify.
    topazSystem_t * system, 

    /// The type of backend in string form.
    const topazString_t * backendType,

    /// The name of the backend to use.
    const topazString_t * backendName
);


/// Returns whether the given backend is available for the given type.
///
int topaz_system_is_backend_available(
    /// The backend type.
    const topazString_t * backendType,

    /// The name of the backend.
    const topazString_t * backendName
);

/// Returns an array of const topazString_t * values 
/// containing all the available backend names for a particular type.
/// The array must be freed (but the strings are read-only).
/// If no such type exists or if no backends exist 
/// for the type, an empty array is returned.
///
topazArray_t * topaz_system_get_available_backends(
    /// The type of backend to query.
    const topazString_t * backendType
);

/// Creates a backend instance using the currently set backend.
/// The handlers set within topaz_system_configure() will pre-load 
/// the "default" set of backends, but they may be switched with 
/// topaz_system_set_backend().
///
topazSystem_Backend_t * topaz_system_create_backend(
    /// The system to create a backend from.
    const topazSystem_t * system, 
    /// The type of backend to create.
    const topazString_t * backendType, 
    /// A pointer to the API structure that the backend should 
    /// populate with API functions of its ownership.
    void * APImappingPtr
);


/// Callback to actually populate a backend's API.
///
typedef void (*topaz_system_backend_handler)(
    /// The source system managing / owning the backend.
    topazSystem_t * system, 

    /// The backend instance to modify within the backend
    topazSystem_Backend_t * backend, 

    /// A pointer to the API instance for the backend,
    /// which needs to be casted to the appropriate
    /// API type based on the backend type advertised.
    void * api
);



/// Adds a new backend to be recognized by the system.
/// Success is returned. Once its added, the backend can be 
/// passed as a backend argument for topaz_system_set_backend. The first 
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

    /// The function to be called that populates a new 
    /// backend instance and API. The API is specific to the backend.
    ///
    topaz_system_backend_handler handler
);


/// Pre-steps all backends. This is normally called within the topaz_context
/// before the engine's managers and entities
///
void topaz_system_pre_step(
    /// The system to step.
    topazSystem_t * system
);

/// Post-steps all backends. This is normally called within the topaz_context
/// after the engine's managers and entities
///
void topaz_system_post_step(
    /// The system to step.
    topazSystem_t * system
);


/// Pre-draws all backends. This is normally called within the topaz_context
/// before the engine's managers and entities
///
void topaz_system_pre_draw(
    /// The system to draw.
    topazSystem_t * system
);


/// Post-draws all backends. This is normally called within the topaz_context
/// after the engine's managers and entities
///
void topaz_system_post_draw(
    /// The system to draw.
    topazSystem_t * system
);











#endif
