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
typedef struct topaz_t topaz_t;
typedef struct topazSystem_Backend_t topazSystem_Backend_t;




/// The controller for the scripting interface.
/// The scripting backend abstracts out different scripting languages
/// that can manipulate and use most of topaz' features. In general,
/// most common operations possible with the C API would be 
/// possible with a scripting language that satisfies the backend API
/// for scripting.
///    
typedef struct topazScript_t topazScript_t;
typedef struct topazScript_Object_t topazScript_Object_t;





/// Events that the scripting backend can generate.
typedef enum topazScript_Event_t topazScript_Event_t;
enum topazScript_Event_t {
    /// The script engine has encountered a fatal error.
    /// The callback will be given string arguments containing info
    /// on the error that occurred.
    ///
    topaz_Script_Event_OnError,

};


/// For scripting objects that are not pure-value types, 
/// there is often behavior that is scripting-language dependent.
/// These are the features for script-level objects that 
/// topaz uses / relies on.
typedef enum topazScript_Object_Feature_t topazScript_Object_Feature_t;
enum topazScript_Object_Feature_t {
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

};


/// Function that is called from the scripting context.
typedef topazScript_Object_t * (*topaz_script_native_function)(
    /// The script generating the call.
    topazScript_t * script, 

    /// The arguments passed to the function from 
    /// the scripting context. Each argument is a topazScript_Object_t *
    /// The count always matches the bound native argument count.
    topazScript_Object_t **, 

    /// Data bound to the function.
    void * userData
);


/// Function that is called from the scripting context.
typedef void (*topaz_script_event_function)(
    /// The script generating the call.
    topazScript_t * script, 

    /// Arguments for the event.
    const topazArray_t *, 

    /// Data bound to the function.
    void * userData
);



/// The object's type define it's behavior and 
/// what data it can hold. Types have implicit conversions 
/// to and from each other.
///
typedef enum topazScript_Object_Type_t topazScript_Object_Type_t;
enum topazScript_Object_Type_t {
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
};


#include <topaz/backends/api/script_api.h>





/// Creates a new graphics instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_graphics();
///
topazScript_t * topaz_script_create(
    /// The topaz context.
    topaz_t * context, 


    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    const topazScriptAPI_t * api
);


/// Destroys and frees a topaz input instance.
///
void topaz_script_destroy(
    /// The script to destroy.
    topazScript_t * script
);


/// Retrieves the backend for this script object.
///
topazSystem_Backend_t * topaz_script_get_backend(
    /// The script to query.
    topazScript_t * script
);



/// Returns the API for this script.
///
topazScriptAPI_t topaz_script_get_api(
    /// The script to query.
    topazScript_t * script
);





/// Maps a native C function to a symbol name within the script context 
/// In most scripting languages, this will bind the function 
/// to the global context. Success is returned.
int topaz_script_map_native_function(
    /// The script to map with.
    topazScript_t * script,

    /// The symbol name to refer to the native function by.
    /// Must follow scripting language-dependent restrictions.
    const topazString_t * localSymbolName,

    /// The number of arguments the function will have when called.
    /// The script context MUST pass this number of arguments or an error will be 
    /// thrown.
    uint8_t argumentCount,

    /// The native function to call when the scripting dummy 
    /// function is called.
    topaz_script_native_function fn,

    /// The data to bind to the callback.
    void * userData
);


void topaz_script_register_source(
    topazScript_t * s, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
);



/// Runs the given script logic within the given string.
/// 
void topaz_script_run(
    /// The script context.
    topazScript_t * script, 
    /// The name of the script to refer to it within the script context.
    const topazString_t * sourceName,
    /// The data for the script that will be run. Usually plaintext.
    const topazString_t * scriptData
);

/// Reads a script from the filesystem. If the script contains references to import 
/// other scripts, it will do so relative to this imported directory.
/// This gives a behavior similar to C/C++'s "include".
/// Success is returned.
int topaz_script_import(
    /// The script context
    topazScript_t * script,
    /// The path to the script.
    const topazString_t * path
);


/// Runs the given script logic within the given string,
/// but will ignore the data if the sourceName has been 
/// used for either topaz_script_run or topaz_script_run_once.
///
void topaz_script_run_once(
    /// The script context.
    topazScript_t * script, 
    /// The name of the script to refer to it within the script context.
    const topazString_t * sourceName,
    /// The data for the script that will be run. Usually plaintext.
    const topazString_t * scriptData
);

/// Returns a source line from a previously run script of the given name.
/// The lineNumber is a 0-index to the line, where the first 
/// line is always index 0. If the index is invalid, an empty 
/// string is returned.
///
const topazString_t * topaz_script_get_line(
    /// The script context.
    topazScript_t * script,
    /// the source name given before.
    const topazString_t * sourceName,
    /// Line number.
    int lineNumber
);

/// Gets the number of lines within a previously-run script of the given name.
/// If no such script exists, 0 is returned.
///
int topaz_script_get_line_count(
    /// The script context.
    topazScript_t *,
    /// The source to query.
    const topazString_t * sourceName    
);


/// Runs the given expression and returns the result as an object.
/// Note that if the expression results in a complex value (i.e. an in-script
/// object reference) an external object may be returned. See 
/// topaz_script_object_from_external().
///
topazScript_Object_t * topaz_script_expression(
    /// The script context.
    topazScript_t * t, 

    /// The expression to run. This is usually run in the global context.
    const topazString_t * expression
);


/// Sets the function that should be called in response to an event.
///
void topaz_script_set_handler(
    /// The script context
    topazScript_t * script, 
    /// The event to set a handler for.
    topazScript_Event_t event, 
    /// The function to call when an event is triggered.
    topaz_script_event_function function, 
    /// The data to bind to the function
    void * data
);

/// Triggers the event within the script instance.
/// The array and its contents, which must be topazScript_Object_t *, are owned by the caller.
///
void topaz_script_emit_event(topazScript_t *, topazScript_Event_t, const topazArray_t *);


/// Creates an empty object. If it contains the proper features, this 
/// object can be modified with the `extendable` family of functions.
/// If the object's reference count is 0, the script context may choose to 
/// remove the object from memory. In this case, the given cleanup function 
/// will be called
///
topazScript_Object_t * topaz_script_create_empty_object(
    /// The script to create the object from
    topazScript_t * script, 
    /// The function to call once all references to the object are gone.
    topaz_script_native_function cleanupFn, 
    /// The data to bind to the cleanup function.
    void * cleanupData
);


/// This allows for implementations of script to "bundle" together any
/// externally defined functions or references in a way that is meaningful 
/// to a useable interface. This is called for you after the script manager 
/// adds topaz' references.
///
void topaz_script_bootstrap(
    /// The script to bootstrap.
    topazScript_t * script
);




















///
/// Dynamically-typed object that interfaces with script primitives.
/// Because scripting languages can have their own 
/// conventions for typing, this script interface attempts to access
/// a subset of features within the scripting environment, namely object 
/// instances that support basic use-cases, i.e., number values, strings, functions 
/// and arrays.
///
typedef struct topazScript_Object_t topazScript_Object_t;



/// Creates a new object with no type.
///
topazScript_Object_t * topaz_script_object_undefined(
    /// The script to create from.
    topazScript_t * script
);

/// Creates a new integer object with the value given.
///
topazScript_Object_t * topaz_script_object_from_int(
    /// The script to create from.
    topazScript_t * script,

    /// The value to assign to the object.
    int value
);

/// Creates a new object from a double.
///
topazScript_Object_t * topaz_script_object_from_number(
    /// The script to create from.
    topazScript_t * script,

    /// The value to assign to the object.
    double value
);

/// Creates a new object from a string
///
topazScript_Object_t * topaz_script_object_from_string(
    /// The script to create from.
    topazScript_t * script,

    /// The value to assign to the object.
    const topazString_t * value
);


/// Creates a new object from an existing object.
/// If the object is an integer, number, or string, the object is copied. In the case that its a 
/// reference, the topazScript_Object_t 
topazScript_Object_t * topaz_script_object_from_object(
    /// The script to create from.
    topazScript_t * script,

    /// The value to assign to the object.
    topazScript_Object_t * value
);



/// Destroys the object. In the case the object was external, the object 
/// may still exist in the scripting context, but this particular object will 
/// be unavailable.
///
void topaz_script_object_destroy(
    /// The script to destroy.
    topazScript_Object_t * script
);


/// Gets the object's source context. Objects can only be used with 
/// the context it was created from.
///
topazScript_t * topaz_script_object_get_source(
    /// The object to query.
    topazScript_Object_t * object
);







/// Returns the integer representation of this object.
/// If there is none, 0 is returned.
///
int topaz_script_object_as_int(
    /// The object to re-interpret.
    const topazScript_Object_t * object
);


/// Returns the decimal representation of this object.
/// If there is none, 0.0 is returned.
///
double topaz_script_object_as_number(
    /// The object to re-interpret.
    const topazScript_Object_t * object
);

/// Returns the string representation of this object.
///
const topazString_t * topaz_script_object_as_string(
    /// The object to re-interpret.
    const topazScript_Object_t * object
);

/// Sets the object value to the value of other. 
/// This will change the current object's type to match the receiving object's type.
///
void topaz_script_object_set(
    /// The object to set.
    topazScript_Object_t * object,

    /// The object to copy from. 
    const topazScript_Object_t * other
);

/// Returns the type for the object.
///
topazScript_Object_Type_t topaz_script_object_get_type(
    /// The object to query.
    const topazScript_Object_t * object
);



/// Returns a bitwise mask of all the features that this object
/// contains. If the object isnt a reference, topazScript_Object_Feature_None
/// is returned.
///
topazScript_Object_Feature_t topaz_script_object_reference_get_feature_mask(
    /// The object to query.
    topazScript_Object_t * object
);



/// Attempts to run the object as a function with the arguments defined within the 
/// given array. If the object is not callable, undefined is returned.
///
topazScript_Object_t * topaz_script_object_reference_call(
    /// The object that can be called.
    topazScript_Object_t * object,

    /// The arguments to pass to the function. The array will be read. 
    const topazArray_t * args
);

/// Associates this data object with a pointer. This is only modifiable
/// within the native context. This is only applicable to object references.
///
void topaz_script_object_reference_set_native_data(
    /// The object to modify.
    topazScript_Object_t * object, 

    /// The data to set to the object.
    void * data, 

    /// An additional data tag if more information is needed.
    int tag
);

/// Retrieves the native data associated with the object. If none, then 
/// NULL is returned.
///
void * topaz_script_object_reference_get_native_data(
    /// The object to query.
    topazScript_Object_t * object, 

    /// The ref to populate with the tag.
    int * tag
);


/// Increments the reference count. Any time an object reference is created 
/// the object's "reference count" is incremented, and any time an object 
/// reference is destroyed, the reference count is decremented. If the reference 
/// count of an object is 0, the script context is free to reap the object 
/// (i.e. through garbage collection). Increasing the ref count can keep the 
/// object alive until it is decremented again with topaz_script_object_reference_ref();
///
void topaz_script_object_reference_ref(
    /// The object to ref.
    topazScript_Object_t * object
);

/// Decrements the reference count.
///
void topaz_script_object_reference_unref(
    /// The object to unref.
    topazScript_Object_t * object
);



/// Returns the named property of the 
/// If the property doesn't exist or the object doesnt have properties,
/// an undefined object is returned.
///
topazScript_Object_t * topaz_script_object_reference_map_get_property(
    /// The object to query.
    topazScript_Object_t * object,

    /// The name of the property.
    const topazString_t * str
);


/// Gets the number of elements that this array object has. If the object is
/// not an array, this value will always be -1.
///
int topaz_script_object_reference_array_get_count(
    /// The object to query.
    topazScript_Object_t * object
);

/// Gets the nth (starting from 0) object within the array. If the object isn't 
/// an array or accesses the array out-of-bounds, undefined is returned.
///
topazScript_Object_t * topaz_script_object_reference_array_get_nth(
    /// The object to query.
    topazScript_Object_t * object,
    /// The nth reference to get.
    int n
);




























/// Enables debugging for this script context.
///
void topaz_script_enable_debugging(
    /// The script instance to enable debugger.
    topazScript_t * script
);

/// Commands for script debuggers.
///
typedef enum topazScript_DebugCommand_t topazScript_DebugCommand_t;
enum topazScript_DebugCommand_t {
    /// Sends a command to pause the debugger.
    /// If already in a paused state, this does nothing.
    /// It is not guaranteed when the debugger will respond to the pause 
    /// request, if at all.
    /// Once the paused state is entered, the debug state is updated.
    /// Any argument is ignored.
    /// 
    topazScript_DebugCommand_Pause,

    /// Sends a command to resume normal execution of the engine.
    /// If not in a paused state, no action is taken.
    /// Any argument is ignored.
    /// 
    topazScript_DebugCommand_Resume,

    /// Sends a command to step into the next function.
    /// If not in a paused state, no action is taken.
    /// After stepping into, the debug state is updated.
    /// Any argument is ignored.
    ///
    topazScript_DebugCommand_StepInto,

    /// Sends a command to step into the next function.
    /// If not in a paused state, no action is taken.
    /// After stepping into, the debug state is updated.
    /// Any argument is ignored.
    ///
    topazScript_DebugCommand_StepOver,

    /// Sends a command to evaluate an expression in the currently paused scope.
    /// If not in a paused state, no action is taken.
    /// The argument string is of the format:
    ///
    ///     [scope level]:[expression]
    ///
    /// For example, with javascript as the scripting language, an expression 
    /// could look like:
    /// 
    ///     0:alert('hello world!')
    ///
    /// The string returned is the return value of the expression. 
    /// If an error occurs, the empty string is returned.
    ///
    topazScript_DebugCommand_ScopedEval,

    /// Sends a command to add a breakpoint to watch for. 
    /// The argument string is the breakpoint to add.
    /// If not in a paused state, no action is taken.
    /// Breakpoints are language specific, but may be of the following formats:
    /// [filename]:[line number]
    ///
    /// If successful, a unique string representing that breakpoint is returned. 
    /// If failed, an empty string is returned.
    ///
    topazScript_DebugCommand_AddBreakpoint,

    /// Sends a command to no longer listen for a breakpoint of the given ID.
    /// The argument is the ID return from an AddBreakpoint command.
    /// If not in a paused state, no action is taken.
    /// If failed, an empty string is returned.
    /// If successful, the label removed is returned.
    ///
    topazScript_DebugCommand_RemoveBreakpoint,

    /// Some language contexts will except additional commands, this will be used 
    /// in such a case.
    ///
    topazScript_DebugCommand_Custom
};


/// Sends a command to the debugger.
/// All commands are pushed through as a deferred request, where
/// this call will return relatively quickly 
/// which a debugger implementation may 
/// not be able to carry out the request immediately.
/// The string returned should be freed by the caller, as ownership 
/// is passed to the called.
///
void topaz_script_debug_send_command(
    /// The script instance to send a command to.
    topazScript_t * script, 

    /// The command to send.
    topazScript_DebugCommand_t command,

    /// The argument to the command.
    const topazString_t * argument
);


/// Callback for when the debugger accepts and has finished 
/// carrying out a debug command request.
///
typedef void (*topaz_script_debug_command_callback)(
    /// The script that was responded to the callback.
    topazScript_t * script, 
    /// The command that was responded to.
    topazScript_DebugCommand_t command, 
    /// The result of the command.
    const topazString_t * result,
    /// The data bounded to callback.
    void * data
);

/// Adds a function to be called when debugger a command has been 
/// processed and committed by the debugger.
/// The ID of the callback is returned, which may be used 
/// to remove the callback.
/// The debugger is allowed to issue its own commands to itself and send notifications 
/// even though the user code may not have issued a command. 
/// For example, breakpoints will issue a pause command callback 
/// to facilitate debugging
///
uint32_t topaz_script_debug_add_command_callback(
    /// The script to add a callback to.
    topazScript_t * script,
    /// The callback to add.
    topaz_script_debug_command_callback cb,
    /// The data to bind to the callback.
    void * data
);

/// Removes the callback references by ID from the list 
/// of callbacks registered to be called when the debugger 
/// excepts the pause request.
void topaz_script_debug_remove_command_callback(
    /// The script to remove the callback from.
    topazScript_t *,
    /// ID for the callback.
    uint32_t id
);

/// Returns whether the debugger is in a paused state.
/// Most commands to the debugger only are accepted when the 
/// debugger is in a paused state.
///
int topaz_script_debug_is_paused(
    /// The script to query.
    topazScript_t * script
);


typedef struct topazScript_CallstackFrame_t topazScript_CallstackFrame_t;

/// Holds information on a callstack frame. 
///
struct topazScript_CallstackFrame_t {
    /// The line number of the frame.
    int lineNumber;

    /// The filename of the frame.
    ///
    const topazString_t * filename;

    /// The function (if any) of the frame
    ///
    const topazString_t * functionName;

    /// A language-agnostic object name associated with the frame.
    ///
    const topazString_t * entityName;
};


typedef struct topazScript_DebugState_t topazScript_DebugState_t;
/// Hold the entire state of the debugger.
///
struct topazScript_DebugState_t {
    /// The label of the breakpoint reached, if any.
    ///
    topazString_t * breakpoint;


    /// An array of topazScript_CallstackFrame_t objects.
    /// The 0'th element is the "bottom" of the callstack, which is the current 
    /// frame. The size-1'th element is the top frame.
    ///
    topazArray_t * callstack;
};

/// Retrieves read-only information on the debugging state.
/// The data is guaranteed to be proper until the next call to this 
/// function.
///
const topazScript_DebugState_t * topaz_script_debug_get_state(
    /// The script to query.
    topazScript_t * script
);








#endif
