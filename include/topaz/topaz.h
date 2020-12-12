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
typedef struct topazFilesys_t topazFilesys_t;
typedef struct topazInput_t topazInput_t;
typedef struct topazViewManager_t topazViewManager_t;
typedef struct topazResources_t topazResources_t;
typedef struct topazConsole_t topazConsole_t;
typedef struct topazArray_t topazArray_t;
typedef struct topazString_t topazString_t;
typedef struct topazFontRenderer_t topazFontRenderer_t;



/*
    Topaz   
    -----

    The context instance for the toolkit. 

*/
typedef struct topaz_t topaz_t;


/// Creates a new topaz context. Default system backends are used.
///
topaz_t * topaz_context_create();


/// Creates a new topaz context with the given topazSystem_t configuration.
///
topaz_t * topaz_context_create_from_system(topazSystem_t *);


 
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
int topaz_context_run(topaz_t *);
    
    
/// Pauses the Engine execution loop.
///
/// Once in a paused state, only critical objects (managers marked with pausable == false),
/// will be updated.
void topaz_context_pause(topaz_t *);

    
/// Immediately pauses the execution loop.
///
/// Like Pause() but disrupts the engine immediately rather than safely waiting 
/// until the next iteration. Most useful for debugging purposes.
void topaz_context_break(topaz_t *);



///  resumes from a paused or broken state.
///
void topaz_context_resume(topaz_t *);    
    
/// Updates the main engine.
///
/// Does not include frame throttling
void topaz_context_iterate(topaz_t *); 


/// Performs a stepping operation for the entire engine. This is normally
/// called through topaz_context_iterate(), which is called by topaz_context_run().
///
void topaz_context_step(topaz_t *);


/// Performs a drawing operation for the entire engine. This is normally
/// called through topaz_context_iterate(), which is called by topaz_context_run().
///
void topaz_context_draw(topaz_t *);

///  returns whether the engine is in a paused or broken state.
///
int topaz_context_is_paused(const topaz_t *);

/// Returns the toplevel Entity. 
///
/// From here, you can 
/// set the Entity that holds the toplevel of the project. 
/// By default there is none.
topazEntity_t *topaz_context_get_root(const topaz_t *);

void topaz_context_set_root(topaz_t *, topazEntity_t *);


/// Attaches a management-type entity.
///
/// If pausable is false, the manager will continue to update even when 
/// the engine is in a paused state.
/// Managers are kept separate from the root entity tree and are unaffected
/// switching of the root entity.
/// the pre_manager* calls ensure that these managers are stepped/drawn BEFORE 
/// the normal entity tree. 
///
void topaz_context_attach_pre_manager(topaz_t *, topazEntity_t * id);

/// Same as topaz_context_attach_pre_manager, but the when the pause 
/// state is entered, these managers are unaffected.
///
void topaz_context_attach_pre_manager_unpausable(topaz_t *, topazEntity_t * id);

/// Same as topaz_context_attach_pre_manager, but the manager is updated 
/// AFTER the main entity tree.
///
void topaz_context_attach_post_manager(topaz_t *, topazEntity_t * id);

/// Same as topaz_context_attach_post_manager, but the manager is updated 
/// even when the paused state is entered
///
void topaz_context_attach_post_manager_unpausable(topaz_t *, topazEntity_t * id);


/// Ends the Engine execution loop.
///
void topaz_context_quit(topaz_t *);

/// Sleeps until the time required for the target frames-per-second is reached.
///
/// @param FPS  The target FPS; useful for loops.
///
/// The actual resolution is machine-dependent, but it tends to be millisecond resolution.
///
void topaz_context_wait(topaz_t *, int FPS);


/// Gets the number of milliseconds since creating this context.
///
uint64_t topaz_context_get_time(topaz_t *);


/// Retrieves the specified parameter.
///
/// List of known parameters:
///
///     "version-micro"     - micro version
///     "version-minor"     - minor version 
///     "version-major"     - major version 
const topazString_t * topaz_context_get_parameter(const topaz_t *, const topazString_t *);

/// Sets a parameter. Some parameters are read-only. In such a case, 
/// 0 is returned.
int topaz_context_set_parameter(const topaz_t *, const topazString_t *, const topazString_t *);

/// Retrieves an array of all parameter names known.
/// The array should be freed when done.
///
topazArray_t * topaz_context_get_parameter_names(const topaz_t *);





/// Creates a new, standard filesys instance.
///
topazFilesys_t * topaz_context_filesys_create(topaz_t *);

/// Gets the font renderer for the topaz instance.
///
topazFontRenderer_t * topaz_context_get_font_renderer(topaz_t *);





/// Retrieves the standard input instance.
///
topazInput_t * topaz_context_get_input(topaz_t *);

/// Retrieves the standard view manager instance.
///
topazViewManager_t * topaz_context_get_view_manager(topaz_t *);

/// Retrieves the standard script manager
///
topazScriptManager_t * topaz_context_get_script_manager(topaz_t *);

/// Retrieves the standard resources instance.
///
topazResources_t * topaz_context_get_resources(topaz_t *);

/// Gets the console for the topaz instance
///
topazConsole_t * topaz_context_get_console(topaz_t *);


/// 
topazGraphics_t * topaz_context_get_graphics(topaz_t *);








#endif
