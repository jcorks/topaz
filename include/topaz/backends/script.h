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
typedef struct topazArray_t topazArray_t;
typedef struct topazString_t topazString_t;
typedef struct topazBackend_t topazBackend_t;



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



typedef enum {
    /// The object has no features.
    ///
    topazScript_Object_Feature_None,

    /// The object can be called like a function using the 
    /// topaz_script_object_call() method.
    ///
    topazScript_Object_Feature_Callable = 1,

    /// The object can be used like an array via the 
    /// topaz_script_object_array* functions.
    ///
    topazScript_Object_Feature_Array = 2,

    /// The object can be used like a hashtable with 
    /// named sub-objects via the topaz_script_object_reference_map* functions.
    /// 
    topazScript_Object_Feature_Map = 4,

    /// The object can have manageable setters / getters for properties
    /// and can have member functions added to it using the 
    /// topaz_script_object_reference_extendable_* family of functions.
    topazScript_Object_Feature_Extendable = 8,

} topazScript_Object_Feature_t;


/// Function that is called from 
typedef topazScript_Object_t * (*topaz_script_native_function)(topazScript_t *, const topazArray_t * args, void * userData);




/// The object's type define it's behavior and 
/// what data it can hold. Types have implicit conversions 
/// to and from each other.
///
typedef enum {
    /// The object type used for unset data, errors, and similar data.
    ///
    topazScript_Object_Type_Undefined,

    /// The object contains an integer value. 
    ///
    topazScript_Object_Type_Integer,

    /// The object contains a decimal value.
    ///
    topazScript_Object_Type_Number,

    /// The object contains a string value.
    ///
    topazScript_Object_Type_String,

    /// The object is a script-specific reference.
    /// Script object references are wrappers for real objects within 
    /// the script context and vary based on the script. To facilitate this 
    /// References can have certain "feature sets" which can allow use of 
    /// additional functions for more in-depth interaction.
    /// See topaz_script_object_get_reference_features().
    /// 
    topazScript_Object_Type_Reference
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
topazScript_Object_t * topaz_script_expression(topazScript_t * t, const topazString_t *);


/// Sets the function that should be called in response to an event.
///
void topaz_script_set_handler(topazScript_t *, topazScript_Event_t, topaz_script_native_function, void * data);

/// Triggers the event within the script instance.
/// The array and its contents, which must be topazScript_Object_t *, are owned by the caller.
///
void topaz_script_emit_event(topazScript_t *, topazScript_Event_t, const topazArray_t *);


/// Creates an empty object. If it contains the proper features, this 
/// object can be modified with the `extendable` family of functions.
///
topazScript_Object_t * topaz_script_create_empty_object(topazScript_t *);


/// This allows for implementations of script to "bundle" together any
/// externally defined functions or references in a way that is meaningful 
/// to a useable interface. This is called for you after the script manager 
/// adds topaz' references.
///
void topaz_script_bootstrap(topazScript_t *);








/*

    Script_Object
    -----
    Dynamically-typed object that interfaces with script primitives.
    
*/
typedef struct topazScript_Object_t topazScript_Object_t;



/// Creates a new object with no type.
///
topazScript_Object_t * topaz_script_object_undefined(topazScript_t *);

/// Creates a new integer object with the value given.
///
topazScript_Object_t * topaz_script_object_from_int(topazScript_t *, int);

/// Creates a new object from a double.
///
topazScript_Object_t * topaz_script_object_from_number(topazScript_t *, double);

/// Creates a new object from a string
///
topazScript_Object_t * topaz_script_object_from_string(topazScript_t *, const topazString_t *);

/// Creates a new object as a shallow clone of the given object
///
topazScript_Object_t * topaz_script_object_from_object(topazScript_t *, const topazScript_Object_t *);




/// Creates a new object that acts as a "wrapper" to an object managed by the 
/// script implementation. This is recommended for objects that are more than simple
/// values, such as arrays or maps, which may vary a lot between scripting languages.
/// This is called / used by the script implementation to create wrappers for you.
/// If you are looking to create a new empty object, see 
/// topaz_script_create_empty_object().
///
topazScript_Object_t * topaz_script_object_from_reference(
    topazScript_t * context,    
    void * userdata
);

/// Destroys the object. In the case the object was external, the object 
/// may still exist in the scripting context, but this particular object will 
/// be unavailable.
///
void topaz_script_object_destroy(topazScript_Object_t *);


/// Gets the object's source context. Objects can only be used with 
/// the context it was created from.
///
topazScript_t * topaz_script_object_get_source(topazScript_Object_t *);







/// Returns the integer representation of this object.
/// If there is none, 0 is returned.
///
int topaz_script_object_as_int(const topazScript_Object_t *);

/// Returns the decimal representation of this object.
/// If there is none, 0.0 is returned.
///
double topaz_script_object_as_number(const topazScript_Object_t *);

/// Returns the string representation of this object.
///
const topazString_t * topaz_script_object_as_string(const topazScript_Object_t *);

/// Sets the object value to the value of other. 
/// This will change the current object's type to match the receiving object's type.
///
void topaz_script_object_set(topazScript_Object_t * , const topazScript_Object_t * other);

/// Returns the type for the object.
///
topazScript_Object_Type_t topaz_script_object_get_type(const topazScript_Object_t *);




/// Returns a bitwise mask of all the features that this object
/// contains. If the object isnt a reference, topazScript_Object_Feature_None
/// is returned.
///
topazScript_Object_Feature_t topaz_script_object_reference_get_features(topazScript_Object_t *);


/// Attempts to run the object as a function with the arguments defined within the 
/// given array. If the object is not callable, undefined is returned.
///
topazScript_Object_t * topaz_script_object_reference_call(topazScript_Object_t *, const topazArray_t *);

/// Associates this data object with a pointer. This is only modifiable
/// within the native context. This is only applicable to 
///
void topaz_script_object_reference_set_native_data(topazScript_Object_t *, void *);

/// Retrieves the native data associated with the object. If none, then 
/// NULL is returned.
///
void * topaz_script_object_reference_get_native_data(topazScript_Object_t *);


/// Increments the reference count. Any time an object reference is created 
/// the object's "reference count" is incremented, and any time an object 
/// reference is destroyed, the reference count is decremented. If the reference 
/// count of an object is 0, the script context is free to reap the object 
/// (i.e. through garbage collection). Increasing the ref count can keep the 
/// object alive until it is decremented again with topaz_script_object_reference_ref();
///
void topaz_script_object_reference_ref(topazScript_Object_t *);

/// Decrements the reference count.
///
void topaz_script_object_reference_unref(topazScript_Object_t *);


/// Returns the named property of the 
/// If the property doesn't exist or the object doesnt have properties,
/// an undefined object is returned.
///
topazScript_Object_t * topaz_script_object_reference_map_get_property(
    topazScript_Object_t *,
    const topazString_t *
);


/// Gets the number of elements that this array object has. If the object is
/// not an array, this value will always be -1.
///
int topaz_script_object_reference_array_get_count(
    topazScript_Object_t *
);

/// Gets the nth (starting from 0) object within the array. If the object isn't 
/// an array or accesses the array out-of-bounds, undefined is returned.
///
topazScript_Object_t * topaz_script_object_reference_array_get_nth(
    topazScript_Object_t *,
    int n
);

/// Adds a named value property to this object. defaultValue is 
/// the value that the property should have at first.
/// onSet will be called any time the property is edited within the 
/// script, it will have one argument which is the new value to set to.
/// onGet will be called to retrieve the value in question. It will have no arguments,
/// but its return value will be used as its value in the script context.
/// onSet can be NULL. In this case, a default handler is used which does nothing.
/// This can be used to represent a read-only value.
///
void topaz_script_object_reference_extendable_add_property(
    topazScript_Object_t *,
    const topazString_t * propName,
    topaz_script_native_function onSet,
    topaz_script_native_function onGet
);

/// Adds a method of the given name. When this method is called in-script, 
/// its arguments will be populated within the argument array,
/// and the functions return value will be used in the script context.
///
void topaz_script_object_reference_extendable_add_method(
    topazScript_Object_t *,
    const topazString_t * methodName,
    topaz_script_native_function function
);




#endif
