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





typedef enum {
    /// The script engine has encountered a fatal error.
    /// The callback will be given string arguments containing info
    /// on the error that occurred.
    ///
    topaz_Script_Event_OnError,

} topazScript_Event_t;



/// Creates a new graphics instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_graphics();
///
topazScript_t * topaz_script_create(topaz_t *, const topazSystem_t *);

/// Destroys and frees a topaz input instance.
///
void topaz_script_destroy(topazScript_t *);



/// Function that is called from 
typedef topazScript_Object_t * (*topaz_script_native_function)(topazScript_t *, topazArray_t * args, void * userData);


/// Maps a native C function to a symbol name within the script context 
/// 
int topaz_script_map_native_function(
    topazScript_t *,
    const topazScript_t * localSymbolName,
    topaz_script_native_function,
    void * userData
);



/// Runs the given script logic within the given string.
/// 
void topaz_script_exec(
    topazScript_t * t, 
    const topazString_t * sourceName,
    const topazString_t * scriptData
);

/// Runs the given expression and returns the result as an object
///
topazScript_t * topaz_script_expression(topazScript_t * t, const topazString_t *);

void topaz_script_set_handler()







typedef struct topazScript_Object_t topazScript_Object_t;




typedef enum {
    topaz_Script_Object_Type_Undefined,
    topaz_Script_Object_Type_Integer,
    topaz_Script_Object_Type_Number,
    topaz_Script_Object_Type_String,
    topaz_Script_Object_Type_Array,
    topaz_Script_Object_Type_Map
} topazScript_Object_Type_t;



topazScript_Object_t * topaz_script_object_create();

const topazScript_Object_t * topaz_script_object_undefined();

topazScript_Object_t * topaz_script_object_from_int(int);

topazScript_Object_t * topaz_script_object_from_number(double);

topazScript_Object_t * topaz_script_object_from_string(const topazString_t *);

topazScript_Object_t * topaz_script_object_from_object(const topazScript_Object_t *);

/// If the property doesn't exist or the object doesnt have properties,
/// the
topazScript_Object_t * topaz_script_object_map_get_property(
    const topazScript_Object_t *,
    const topazString_t *
);



int topaz_script_object_array_get_count(
    const topazScript_Object_t *
);


topazScript_Object_t * topaz_script_obejct_array_get_nth(
    const topazScript_Object_t *,
    int n
);




int topaz_script_object_as_int(const topazScipt_Object_t *);

double topaz_script_object_as_number(const topazScript_Object_t *);

const topazString_t * topaz_script_object_as_string(const topazScript_Object_t *);

void topaz_script_object_set(topazScript_Object_t * , const topazScript_Object_t *);

void topaz_script_object_set_type(topazScript_Object_t *, topazScript_Object_Type_t);

topazScript_Object_Type_t topaz_script_object_get_type(const topazScript_t *);

void topaz_script_object_destroy(topazScript_Object_t *);





#endif
