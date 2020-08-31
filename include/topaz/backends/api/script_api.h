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


#ifndef H_TOPAZDC__SCRIPT_API__INCLUDED
#define H_TOPAZDC__SCRIPT_API__INCLUDED

#include <stdint.h>


/*

    ScriptAPI
    -----
    
    The set of functions that define how the scripting abstraction should 
    behave. Creating scripting abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/script.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

    Specifically, this is designed to not focus on a particular scripting
    language, but instead, define the minimum set of functions needed for 
    scripting to be supported in a useful way.
*/
typedef struct topazScriptAPI_t topazScriptAPI_t;




typedef struct topazScript_ObjectAPI_t topazScript_ObjectAPI_t;

/// For objects that exist and are managed independently of the native context,
/// the implementation can create "wrapper" objects with externally-defined 
/// functions to facilitate script language / context-specific behaviour through 
/// a generic interface. 
///
/// Note: all functions are allowed to be NULL, in that case a default handler 
/// will be used.
///
struct topazScript_Object_ExternalAPI_t {
    /// Called when the object is first starting up. 
    ///
    void (*object_external_create)(void *);

    /// Called when the object reference is no longer needed by the native context.
    ///
    void (*object_external_destroy)(void *);

    /// This function is called if a user runs the "topaz_script_object_call" function 
    /// The intent is to have the in-script object function run, if any. If this is
    /// impossible, an undefined object should be returned.
    /// The default handler will always return undefined.
    ///
    topazScript_Object_t * (*object_external_call)(topazScript_Object_t *, topazArray_t *, void *);


    /// This implements the array accessor for the object, if any.
    /// If unsupported, an undefined object should be returned. 
    /// If the access is out of bounds, undefined should be returned.
    /// The default handler will always return undefined.
    ///
    topazScript_Object_t * (*object_external_array_get_nth)(topazScript_Object_t * , int,  void *);

    /// Returns the array count fetch. If not supported, -1 should be returned.
    /// The default handler will always return -1.
    ///
    int (*object_external_array_get_count)(topazScript_Object_t *, void *);

    /// Implements the map property fetch. If not implemented, the undefined object can be returned.
    /// The default handler will always return undefined.
    ///
    topazObject_t * (*object_external_map_get_property)(topazScript_Object_t *, const topazString_t * prop, void *);

    /// Returns a stringification of this object.
    /// The default handler will return "[Object]"
    ///
    const topazString_t * (*object_external_as_string)(topazScript_Object_t *, void *);

};


/// Each function is an implementation-facing copy of 
/// the user-side API for topazTime_t. See <topaz/backends/Time.h>
///

struct topazScriptAPI_t {
    /// Every script context will likely have external objects, where it 
    /// uses topazScript_Object_t's as wrappers for an externally stored 
    /// scripting object. This API facilitates this wrapping behavior.
    ///
    topazScript_Object_ExternalAPI_t objectAPI;



    void (*script_create)             (topazScriptAPI_t *);
    void (*script_destroy)            (topazScriptAPI_t *);
    int  (*script_map_native_function)(topazScriptAPI_t *, const topazString_t *, topaz_script_native_function, void * userData);
    void (*script_run)                (topazScriptAPI_t *, const topazString_t * sourceName, const topazString_t * scriptData);
    topazObject_t * (*script_expression) (topazScriptAPI_t *, const topazString_t *);



    /// User-given data. This is expected to data needed to persist
    /// throughout the lifetime of the Time
    ///
    void * implementationData;


};



#endif
