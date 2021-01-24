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


#ifndef H_TOPAZDC__SCRIPT_MANAGER__INCLUDED
#define H_TOPAZDC__SCRIPT_MANAGER__INCLUDED

typedef struct topaz_t topaz_t;
typedef struct topazSystem_t topazSystem_t;
typedef struct topazScript_t topazScript_t;


/// Creates script contexts and links these contexts with 
/// functions and references to native topaz primitives. Which 
/// primitives linked is based on a permissions mask
typedef struct topazScriptManager_t topazScriptManager_t;



/// Permissions for a script instance. (Bitmask) 
/// Scripts allow effectively arbitrary code to be externally run
/// within the running program. Thus, for security, the script VM can 
/// be denied access to symbols that could modify the system.
/// Importance of this is on a case-by-case basis and depends on the 
/// backend implementation, but it is highlighly advised to 
/// carefully choose the permission level according to your use
/// instead of using the catch-all permission.
typedef enum topazScriptManager_Permission_t topazScriptManager_Permission_t;

enum topazScriptManager_Permission_t {
    /// Basic permissions that exclude all other features.
    topazScriptManager_Permission_Basic = 0,

    /// Adds permissions to access the filesystem backend.
    /// Namely allows the reading and writing of arbitrary files.
    topazScriptManager_Permission_FullFilesystem = 1,

    /// Adds permissions to access the resource tree.
    /// Namely adds reading and writing assets through the resource 
    /// API.
    topazScriptManager_Permission_Resources = 2,

    /// Adds permissions to read / interact with input.
    /// Namely, a program can log / read from any input device that topaz recognizes 
    /// and may modify the input queue of a device as recognized by topaz.
    topazScriptManager_Permission_Input = 4,

    /// Adds permissions for all the other features.
    topazScriptManager_Permission_All = 0xffffffff
} topazScriptManager_Permission_t;




/// Creates a new script manager instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_script_manager();
///
topazScriptManager_t * topaz_script_manager_create(
    /// The topaz context.
    topaz_t * context
);

/// Destroys and frees a topaz script manager instance.
///
void topaz_script_manager_destroy(
    /// The script manager to destroy.
    topazScriptManager_t * manager
);


/// Creates and bootstraps a script context.
topazScript_t * topaz_script_manager_create_context(
    /// The script manager to use to create the script instance.
    topazScriptManager_t * manager,

    /// The permission bitmask to use.
    topazScriptManager_Permission_t permissions
);







#endif
