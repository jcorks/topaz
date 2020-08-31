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


#ifndef H_TOPAZDC__SCRIPT__INCLUDED
#define H_TOPAZDC__SCRIPT__INCLUDED

typedef struct topazScript_Object_t topazScript_Object_t;





/*

    Script
    -----
    The controller for the scripting interface
    
*/
typedef struct topazScript_t topazScript_t;
typedef struct topazScript_Object_t topazScript_Object_t;





typedef enum {
    /// The script engine has encountered a fatal error.
    /// The callback will be given string arguments containing info
    /// on the error that occurred.
    ///
    topaz_Script_Event_OnError,

} topazScript_Event_t;






/// Function that is called from 
typedef topazScript_Object_t * (*topaz_script_native_function)(topazScript_t *, topazArray_t * args, void * userData);




/// The object's type define it's behavior and 
/// what data it can hold. Types have implicit conversions 
/// to and from each other.
///
typedef enum {
    /// The object type used for unset data, errors, and similar data.
    ///
    topaz_Script_Object_Type_Undefined,

    /// The object contains an integer value. 
    ///
    topaz_Script_Object_Type_Integer,

    /// The object contains a decimal value.
    ///
    topaz_Script_Object_Type_Number,

    /// The object contains a string value.
    ///
    topaz_Script_Object_Type_String,

    /// The object is an array of items. 
    /// The item count may be accessed with topaz_script_object_array_get_count()
    /// The items may be accessed with topaz_script_object_array_get_nth()
    ///
    topaz_Script_Object_Type_Array,


    /// The object contains sub-object properties t
    topaz_Script_Object_Type_Map,

    /// The object is callable
    topaz_Script_Object_Type_Callable
} topazScript_Object_Type_t;


#include <topaz/backends/api/script_api.h>





/// Creates a new graphics instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_graphics();
///
topazScript_t * topaz_script_create(topazBackend_t *, const topazScriptAPI_t *);

/// Destroys and frees a topaz input instance.
///
void topaz_script_destroy(topazScript_t *);


/// Maps a native C function to a symbol name within the script context 
/// 
int topaz_script_map_native_function(
    topazScript_t *,
    const topazString_t * localSymbolName,
    topaz_script_native_function,
    void * userData
);



/// Runs the given script logic within the given string.
/// 
void topaz_script_run(
    topazScript_t * t, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
);

/// Runs the given expression and returns the result as an object.
/// Note that if the expression results in a complex value (i.e. an in-script
/// object reference) an external object may be returned. See 
/// topaz_script_object_from_external().
///
topazObject_t * topaz_script_expression(topazScript_t * t, const topazString_t *);


/// Sets the function that should be called in response to an event.
///
void topaz_script_set_handler(topazScript_t *, topazScript_Event_t, topaz_script_native_function);

/// Triggers the event within the script instance.
/// The array and its contents, which must be topazScript_Object_t *, are owned by the caller.
///
void topaz_script_emit_event(topazScript_t *, topazScript_Event_t, const topazArray_t *, void * data);












/*

    Script_Object
    -----
    Dynamically-typed object that interfaces with script primitives.
    
*/
typedef struct topazScript_Object_t topazScript_Object_t;



/// Creates a new object with no type.
///
topazScript_Object_t * topaz_script_object_undefined();

/// Creates a new integer object with the value given.
///
topazScript_Object_t * topaz_script_object_from_int(int);

/// Creates a new object from a double.
///
topazScript_Object_t * topaz_script_object_from_number(double);

/// Creates a new object from a string
///
topazScript_Object_t * topaz_script_object_from_string(const topazString_t *);

/// Creates a new object as a shallow clone of the given object
///
topazScript_Object_t * topaz_script_object_from_object(const topazScript_Object_t *);


/// Creates a new object that acts as a "wrapper" to an object managed by the 
/// script implementation. This is recommended for objects that are more than simple
/// values, such as arrays or maps, which may vary a lot between scripting languages.
///
topazScript_Object_t * topaz_script_object_from_external(
    topazScript_t * context,    
    topazScript_Object_ExternalAPI_t * api,
    topazScript_Object_Type_t type,
    int isCallable,
    void * userdata
);

/// Destroys the object. In the case the object was external, the object 
/// may still exist in the scripting context, but this particular object will 
/// be unavailable.
///
void topaz_script_object_destroy(topazScript_Object_t *);



/// Returns the named property of the 
/// If the property doesn't exist or the object doesnt have properties,
/// an undefined object is returned.
///
topazScript_Object_t * topaz_script_object_map_get_property(
    const topazScript_Object_t *,
    const topazString_t *
);


/// Gets the number of elements that this array object has. If the object is
/// not an array, this value will always be -1.
///
int topaz_script_object_array_get_count(
    const topazScript_Object_t *
);

/// Gets the nth (starting from 0) object within the array. If the object isn't 
/// an array or accesses the array out-of-bounds, undefined is returned.
///
topazScript_Object_t * topaz_script_object_array_get_nth(
    const topazScript_Object_t *,
    int n
);


/// Associates this data object with a pointer. This is only modifiable
/// within the native context.
///
void topaz_script_object_set_native_data(topazScript_Object_t *, void *);

/// Retrieves the native data associated with the object. If none, then 
/// NULL is returned.
///
void * topaz_script_object_get_native_data(topazScript_Object_t *);



/// Returns the integer representation of this object.
/// If there is none, 0 is returned.
///
int topaz_script_object_as_int(const topazScipt_Object_t *);

/// Returns the decimal representation of this object.
/// If there is none, 0.0 is returned.
double topaz_script_object_as_number(const topazScript_Object_t *);

/// Returns the string representation of this object.
///
const topazString_t * topaz_script_object_as_string(const topazScript_Object_t *);

/// Sets the object value to the value of other. 
/// This will change the current object's type to match the receiving object's type.
///
void topaz_script_object_set(topazScript_Object_t * , const topazScript_Object_t * other);

/// Sets the type of the object. This will also attempt to re-express its current value 
/// into one that make sense for this object.
///
void topaz_script_object_set_type(topazScript_Object_t *, topazScript_Object_Type_t);

/// Returns the type for the object.
///
topazScript_Object_Type_t topaz_script_object_get_type(const topazScript_t *);


/// Attempts to run the object as a function with the arguments defined within the 
/// given array. If the object is not callable, undefined is returned.
///
topazScript_Object_t * topaz_script_object_call(topazScript_Object_t *, const topazArray_t *);


/// Returns whether the object in question is external. External objects 
/// have their data stored specific to the script context implementation. 
/// See topaz_script_object_from_external().
///
int topaz_script_object_is_external(topazScript_Object_t *);



#endif
