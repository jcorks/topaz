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



//    The set of functions that define how the scripting abstraction should 
//    behave. Creating scripting abstractions requires this API to be 
//    populated.
//
//    These API functions are called as underlying implementations for the symbols 
//    within <topaz/backends/script.h> and provide a way for custom, possibly 
//    system-dependent behavior to account for an environment in a robust way.
//
//    Specifically, this is designed to not focus on a particular scripting
//    language, but instead, define the minimum set of functions needed for 
//    scripting to be supported in a useful way.
//
typedef struct topazScriptAPI_t topazScriptAPI_t;
typedef struct topazScript_DebugState_t topazScript_DebugState_t;





/// Can be used by the script implementation 
/// to Creates a new object that acts as a "wrapper" to an object managed by the 
/// script implementation. This is recommended for objects that are more than simple
/// values, such as arrays or maps, which may vary a lot between scripting languages.
/// This is called / used by the script implementation to create wrappers for you.
/// If you are looking to create a new empty object, see 
/// topaz_script_create_empty_object().
///
topazScript_Object_t * topaz_script_object_wrapper(
    /// The script to create from.
    topazScript_t * script,

    /// The userdata to bind to the reference.
    /// Similar to the return value for object_reference_create_from_reference,
    /// as this is the data that gets passed to all of the object api.
    void * userdata
);






// For objects that exist and are managed independently of the native context,
// the implementation can create "wrapper" objects with externally-defined 
// functions to facilitate script language / context-specific behaviour through 
// a generic interface. 
//
// Note: all functions are allowed to be NULL, in that case a default handler 
// will be used.
//
typedef struct topazScript_Object_ReferenceAPI_t topazScript_Object_ReferenceAPI_t;
struct topazScript_Object_ReferenceAPI_t {

    // Similar to object_reference_create, but its a new reference 
    // that points to the same location as an existing reference.
    // scriptData is the script implementation data
    // from is the topazScript_Object_t * reference that points to the object that this new reference should point to
    // fromData is the script object data bound to it
    void * (*object_reference_create_from_reference)(topazScript_Object_t *, void * fromRefData, topazScript_Object_t * from, void * fromData);

    // Called when the object reference is no longer needed by the native context.
    // The destroy function already unrefs references for you by decrementing the 
    // reference count. Note that even though the GC no longer needs it, 
    // it could still be valid from the POV of the script VM.
    //
    void (*object_reference_destroy)(topazScript_Object_t *, void *);

    // Returns the feature set of the object.
    //
    int (*object_reference_get_feature_mask)(topazScript_Object_t *, void *);

    // Retrieve the native data thats associated with the scripting object 
    //
    void * (*object_reference_get_native_data)(topazScript_Object_t *, int * tag, void *);

    // Sets native data that will be associated with the object across the 
    // script instance. 
    //
    void (*object_reference_set_native_data)(topazScript_Object_t *, void * native, int nativeTag, void *);

    // Increments the reference count. The script context should use the reference 
    // count to control any garbage collection done, as a nonzero ref count means 
    // that the object CANNOT be destroyed.
    //
    void (*object_reference_ref)(topazScript_Object_t *, void *);


    // Decrements the reference count. See the notes on the function above.
    //
    void (*object_reference_unref)(topazScript_Object_t *, void *);

    // This function is called if a user runs the "topaz_script_object_call" function 
    // The intent is to have the in-script object function run, if any. If this is
    // impossible, an undefined object should be returned.
    // The default handler will always return undefined.
    //
    topazScript_Object_t * (*object_reference_call)(topazScript_Object_t *, const topazArray_t *, void *);


    // This implements the array accessor for the object, if any.
    // If unsupported, an undefined object should be returned. 
    // If the access is out of bounds, undefined should be returned.
    // The default handler will always return undefined.
    //
    topazScript_Object_t * (*object_reference_array_get_nth)(topazScript_Object_t * , int,  void *);

    // Returns the array count fetch. If not supported, -1 should be returned.
    // The default handler will always return -1.
    //
    int (*object_reference_array_get_count)(topazScript_Object_t *, void *);

    // Implements the map property fetch. If not implemented, the undefined object can be returned.
    // The default handler will always return undefined.
    //
    topazScript_Object_t * (*object_reference_map_get_property)(topazScript_Object_t *, const topazString_t * prop, void *);

    // Returns a stringification of this object.
    // The default handler will return "[Object]"
    //
    void (*object_reference_to_string)(topazScript_Object_t *, topazString_t * s, void *);


};

// Gets the data pointer given from object_reference_create to be 
// used for internal implementations.
//
void * topaz_script_object_get_api_data(topazScript_Object_t *);



// Each function is an implementation-facing copy of 
// the user-side API for topazTime_t. See <topaz/backends/Time.h>
//

struct topazScriptAPI_t {
    // Every script context will likely have external objects, where it 
    // uses topazScript_Object_t's as wrappers for an externally stored 
    // scripting object. This API facilitates this wrapping behavior.
    //
    topazScript_Object_ReferenceAPI_t objectAPI;


    void *(*script_create)             (topazScript_t *, topaz_t *);
    void  (*script_destroy)            (topazScript_t *, void *);
    int   (*script_map_native_function)(topazScript_t *, void *, const topazString_t *, topaz_script_native_function, void * userData);
    void  (*script_run)                (topazScript_t *, void *, const topazString_t * sourceName, const topazString_t * scriptData);
    topazScript_Object_t * (*script_expression) (topazScript_t *, void *, const topazString_t *);
    topazScript_Object_t * (*script_create_empty_object) (topazScript_t *, void *, topaz_script_native_function, void *);

    // called when the on error event is signaled from user code, which ideally will trigger 
    // an error condition in the language's own mechanisms.
    void  (*script_throw_error)(topazScript_t *, void *, const topazArray_t * args);

    // This function is completely optional to implement, but is 
    // mostly likely needed if the scripting language API is more than 
    // just a few functions. I.E. if your scripting API requires namespacing, 
    // which isnt inherently supported by the interface, this would be a 
    // good time to organize that, as when bootstrap() is called, topaz 
    // would have added all the "base" function symbols as plain functions 
    // (this of course depends on the implementation of script_map_native_function, 
    // but using this function as part of the API preparation stage may make 
    // this simpler design-wise.)
    // 
    void (*script_bootstrap)          (topazScript_t *, void *);






    // called by the script when a user has unlocked debugging capabilities.
    // 
    void (*script_debug_start)(topazScript_t *, void *);

    // Sends a command to the debugger. Check out script.h command list.
    //
    void (*script_debug_send_command)(topazScript_t *, void *, int commandEnum, const topazString_t *);


    // Retrieves the debug state following the requirements in script.h.
    //
    const topazScript_DebugState_t * (*script_debug_get_state)(topazScript_t *, void *);




};

// Function to be called by the implementation when the script debuggerr 
// accepts the debug request. If the implementation does not support 
// debugging, this does not need to be called.
// 
void topaz_script_notify_command(topazScript_t *, int commandEnum, topazString_t * str);

#endif
