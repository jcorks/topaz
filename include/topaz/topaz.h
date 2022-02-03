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

#ifndef H_TOPAZDC__TOPAZ__INCLUDED
#define H_TOPAZDC__TOPAZ__INCLUDED

#include <stdint.h>

typedef struct topazSystem_t topazSystem_t;
typedef struct topazGraphics_t topazGraphics_t;
typedef struct topazScriptManager_t topazScriptManager_t;
typedef struct topazEntity_t topazEntity_t;
typedef struct topazFilesystem_t topazFilesystem_t;
typedef struct topazInput_t topazInput_t;
typedef struct topazViewManager_t topazViewManager_t;
typedef struct topazResources_t topazResources_t;
typedef struct topazConsole_t topazConsole_t;
typedef struct topazArray_t topazArray_t;
typedef struct topazString_t topazString_t;
typedef struct topazFontManager_t topazFontManager_t;
typedef struct topazAudio_t topazAudio_t;
typedef struct topazDisplay_t topazDisplay_t;


///
/// The context instance for the toolkit. 
/// Most data within the topaz project is contextualized
/// under some a topaz instance at some level, so it is 
/// often that a topaz context is needed.
///
/// Contexts must be created and destroyed, and are not 
/// automatically created.
///
typedef struct topaz_t topaz_t;


/// Creates a new topaz context. Default system backends are used.
///
topaz_t * topaz_context_create();


/// Creates a new topaz context with the given topazSystem_t configuration.
///
topaz_t * topaz_context_create_from_system(
    /// The pre-configured system instance to use.
    topazSystem_t * system
);


 
/// Creates an empty topaz context with no environment-specific 
/// behavior. Not particularly useful.
///
topaz_t * topaz_context_create_empty();

/// Gets the system that belongs to the context.
///
topazSystem_t * topaz_context_get_system();

/// Destroys and frees the topaz context.
///
void topaz_context_destroy();





/// Begins the Engine execution loop.
///
/// Once run is initiated, all drawing to the screen and logic updating
/// is automated. While running, it is also possible to utilize the
/// debugging features such as the console and Entity debugger.
int topaz_context_run(
    /// The context to run.
    topaz_t * context
);
    
    
/// Pauses the Engine execution loop.
///
/// Once in a paused state, only critical objects (managers marked with pausable == false),
/// will be updated.
void topaz_context_pause(
    /// The context to pause.
    topaz_t * context
);


    
/// Immediately pauses the execution loop.
///
/// Like Pause() but disrupts the engine immediately rather than safely waiting 
/// until the next iteration. Most useful for debugging purposes.
void topaz_context_pause_now(
    /// The context to break.
    topaz_t * context
);



///  Resumes from a paused or broken state.
///
void topaz_context_resume(
    /// The context to resume.
    topaz_t * context
);

    
/// Updates the main engine.
///
/// Does not include frame throttling
void topaz_context_iterate(
    /// The context to iterate.
    topaz_t * context
);



/// Performs a stepping operation for the entire engine. This is normally
/// called through topaz_context_iterate(), which is called by topaz_context_run().
///
void topaz_context_step(
    /// The context to step.
    topaz_t * context
);


/// Performs a drawing operation for the entire engine. This is normally
/// called through topaz_context_iterate(), which is called by topaz_context_run().
///
void topaz_context_draw(
    /// The context to draw.
    topaz_t * context
);

///  returns whether the engine is in a paused or broken state.
///
int topaz_context_is_paused(
    /// The context to query.
    const topaz_t * context
);





/// Attaches a management-type entity.
///
/// If pausable is false, the manager will continue to update even when 
/// the engine is in a paused state.
/// Managers are kept separate from the root entity tree and are unaffected
/// switching of the root entity.
/// the pre_manager* calls ensure that these managers are stepped BEFORE 
/// the normal entity tree. 
/// Managers are never drawn.
///
void topaz_context_attach_pre_manager(
    /// The context to modify.
    topaz_t * topaz, 

    /// The entity to become a manager.
    topazEntity_t * entity
);

/// Same as topaz_context_attach_pre_manager, but the when the pause 
/// state is entered, these managers are unaffected.
///
void topaz_context_attach_pre_manager_unpausable(
    /// The context to modify.
    topaz_t * topaz, 

    /// The entity to become a manager.
    topazEntity_t * entity
);
/// Same as topaz_context_attach_pre_manager, but the manager is updated 
/// AFTER the main entity tree.
///
void topaz_context_attach_post_manager(
    /// The context to modify.
    topaz_t * topaz, 

    /// The entity to become a manager.
    topazEntity_t * entity
);

/// Same as topaz_context_attach_post_manager, but the manager is updated 
/// even when the paused state is entered
///
void topaz_context_attach_post_manager_unpausable(
    /// The context to modify.
    topaz_t * topaz, 

    /// The entity to become a manager.
    topazEntity_t * entity
);



/// Ends the context execution loop.
///
void topaz_context_quit(
    /// The context to quit.
    topaz_t * context
);

/// Sleeps until the time required for the target frames-per-second is reached.
/// The actual resolution is machine-dependent, but it tends to be millisecond resolution.
/// This will pause the main thread.
///
void topaz_context_wait(
    /// The context to wait with.
    topaz_t * context, 
    /// The target updates per second.
    int FPS
);


/// Gets the number of milliseconds since creating this context.
///
uint64_t topaz_context_get_time(
    /// The context to query.
    topaz_t * context
);


/// Retrieves the specified parameter.
///
/// List of known parameters:
///
///     "version-micro"     - micro version
///     "version-minor"     - minor version 
///     "version-major"     - major version 
const topazString_t * topaz_context_get_parameter(
    /// The context to query.
    const topaz_t * context, 

    /// The paramter to retrieve a value from.
    const topazString_t * paramName
);

/// Sets a parameter. Some parameters are read-only. In such a case, 
/// 0 is returned.
int topaz_context_set_parameter(
    /// The context to modify.
    const topaz_t * context,

    /// The parameter to set.
    const topazString_t * paramName, 

    /// The new value of the parameter.
    const topazString_t * value
);

/// Retrieves an array of all parameter names known.
/// The array should be freed when done.
///
topazArray_t * topaz_context_get_parameter_names(
    /// The context to query.
    const topaz_t * context
);



/// Gets the current iteration display.
/// This is the display thats currently getting 
/// modified. If none, returns NULL.
topazDisplay_t * topaz_context_get_iteration_display(
    topaz_t * context
);

/// Creates a new, standard filesys instance.
///
topazFilesystem_t * topaz_context_get_filesystem(
    /// The relevant context.
    topaz_t * context
);

/// Gets the font manager for the topaz instance.
///
topazFontManager_t * topaz_context_get_font_manager(
    /// The relevant context.
    topaz_t * context
);






/// Retrieves the standard input instance.
///
topazInput_t * topaz_context_get_input(
    /// The relevant context.
    topaz_t * context
);

/// Retrieves the standard view manager instance.
///
topazViewManager_t * topaz_context_get_view_manager(
    /// The relevant context.
    topaz_t * context
);

/// Retrieves the standard script manager
///
topazScriptManager_t * topaz_context_get_script_manager(
    /// The relevant context.
    topaz_t * context
);

/// Retrieves the standard resources instance.
///
topazResources_t * topaz_context_get_resources(
    /// The relevant context.
    topaz_t * context
);

/// Gets the console for the topaz instance
///
topazConsole_t * topaz_context_get_console(
    /// The relevant context.
    topaz_t * context
);


/// Gets the graphics instance for the topaz context.
topazGraphics_t * topaz_context_get_graphics(
    /// The relevant context.
    topaz_t * context
);

/// Gets the audio instance.
topazAudio_t * topaz_context_get_audio(
    /// The relevant context.
    topaz_t * context
);








#endif
