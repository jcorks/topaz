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

/*

    System
    -----

    Bundles and abstracts system-level middleware that topaz 
    uses to implement the engine.

*/

typedef struct topazSystem_t topazSystem_t;




/* 

    SystemBackend
    -----

    Represents a discreet API implementation for System 
*/
typedef struct topazSystem_Backend_t topazSystem_Backend_t;


/// Populates the backend with information.
/// Only allowed to be called within backend_callback calls 
/// given to topaz_system_config_add_handler
///
void topaz_system_backend_bind(
    topazSystem_Backend_t *,

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
    void (*on_pre_step)(topazSystem_Backend_t *, void *),

    /// Called after all the backends are stepped. Backend is passed.
    ///
    void (*on_post_step)(topazSystem_Backend_t *, void *),

    /// Called when the backend is drawn. Backend is passed.
    ///
    void (*on_pre_draw)(topazSystem_Backend_t *, void *),

    /// Called after all the backends have drawn. Backend is passed.
    ///
    void (*on_post_draw)(topazSystem_Backend_t *, void *),

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


/// Populates the decoder with information.
/// Only allowed to be called within backend_callback calls 
/// given to topaz_system_config_add_handler
///
void topaz_system_decoder_bind(
    topazSystem_Backend_t *,

    /// The name of the decoder.
    const topazString_t * name,

    /// The version of the backend
    const topazString_t * version,

    /// The author of the backend
    const topazString_t * author,

    /// A description of the backend
    const topazString_t * description,







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
int topaz_system_backend_get_topaz_major_version(const topazSystem_Backend_t *);

/// Returns the minor version of the topaz library 
/// that this backend was built with.
///
int topaz_system_backend_get_topaz_minor_version(const topazSystem_Backend_t *);

/// Returns the micro version of the topaz library 
/// that this backend was build with.
///
int topaz_system_backend_get_topaz_micro_version(const topazSystem_Backend_t *);





/// Gets the name of this backend.
///
const topazString_t * topaz_system_backend_get_name   (const topazSystem_Backend_t *);


/// Gets the name of this backend.
///
const topazString_t * topaz_system_backend_get_version(const topazSystem_Backend_t *);

/// Gets the author name 
///
const topazString_t * topaz_system_backend_get_author (const topazSystem_Backend_t *);

/// Gets the description 
///
const topazString_t * topaz_system_backend_get_description(const topazSystem_Backend_t *);





/// Steps the backend
///
void topaz_system_backend_pre_step(topazSystem_Backend_t *);

/// Posts that initialization has finished
///
void topaz_system_backend_post_step(topazSystem_Backend_t *);

/// Draws the backend
///
void topaz_system_backend_pre_draw(topazSystem_Backend_t *);

/// Posts that initialization has finished
///
void topaz_system_backend_post_draw(topazSystem_Backend_t *);

/// Retrieves the user data associated with the backend.
///
void * topaz_system_backend_get_user_data(topazSystem_Backend_t * t);





/// Called to register support system backends for this 
/// instance. Only needs to be called once, but should be called 
/// before any other call to topaz_system_ family of functions
///
void topaz_system_configure();



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
topazSystem_Backend_t * topaz_system_create_backend(
    const topazSystem_t *, 
    const topazString_t * backendType, 
    void * APImappingPtr
);


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

    /// The function to be called that populates a new 
    /// backend instance and API. The API is specific to the backend.
    ///
    void (*backend_callback)(topazSystem_t *, topazSystem_Backend_t *, void * api)
);


/// Pre-steps all backends. This is normally called within the topaz_context
/// before the engine's managers and entities
///
void topaz_system_pre_step(topazSystem_t *);

/// Post-steps all backends. This is normally called within the topaz_context
/// after the engine's managers and entities
///
void topaz_system_post_step(topazSystem_t *);

/// Pre-draws all backends. This is normally called within the topaz_context
/// before the engine's managers and entities
///
void topaz_system_pre_draw(topazSystem_t *);

/// Post-draws all backends. This is normally called within the topaz_context
/// after the engine's managers and entities
///
void topaz_system_post_draw(topazSystem_t *);










#endif