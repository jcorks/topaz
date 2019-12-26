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


/*
    Topaz   
    -----

    The context instance for the toolkit. 

*/
typedef struct topaz_t topaz_t;


/// Initializes all topaz assets.
///
/// Upon encountering an irrecoverable error, the statup
/// function will return 0. Otherwise, it will return 1.
/// No other functions should be used prior to calling this function.
///
///
topaz_t * topaz_context_create(
    topazBackend_t *,       topazTimeAPI_t,
    topazBackend_t *,       topazFilesysAPI_t

);



/// Begins the Engine execution loop.
///
/// Once run is initiated, all drawing to the screen and logic updating
/// is automated. While running, it is also possible to utilize the
/// debugging features such as the console and Entity debugger.
int topaz_context_run(const topaz_t *);
    
    
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
///  Not equivalent to a frame, since frame throttling can occur.
void topaz_context_iterate(topaz_t *); 


///  returns whether the engine is in a paused or broken state.
///
bool topaz_context_is_paused(const topaz_t *);

/// Returns the toplevel Entity. 
///
/// From here, you can 
/// set the Entity that holds the toplevel of the project. 
/// By default there is none.
topazEntityID_t topaz_context_get_root(const topaz_t *);
topazEntityID_t topaz_context_set_root(topaz_t *);


/// Attaches a management-type entity.
///
/// If pausable is false, the manager will continue to update even when 
/// the engine is in a paused state.
void topaz_context_attach_manager(topazEntityID_t id);
void topaz_context_attach_manager_unpausable(topazEntityID_t id);



/// Ends the Engine execution loop.
///
void topaz_context_quit(topaz_t *);

/// Sleeps until the time required for the target frames-per-second is reached.
///
/// @param FPS  The target FPS; useful for loops.
///
/// The actual resolution is machine-dependent, but it tends to be millisecond resolution.
void topaz_context_wait(topaz_t *. int FPS);


/// Retrieves the specified parameter.
///
/// List of known parameters:
///
///     "fps"               - frames per second for the engine loop 
///     "version-minor"     - minor version 
///     "version-major"     - major version 
///     "git-hash"          - hash for this commit
///     "base-directory"    - start directory for this instance 
///     "draw-time-ms"      - time it took to complete a draw cycle last frame 
///     "step-time-ms"      - time it took to complete a step cycle last frame 
///     "system-time-ms"    - time it took to complete engine tasks last frame 
///     "engine-real-ms"    - time it took to complete the entire last frame
double topaz_get_parameter(const topaz_t *, const topazString_t *);

/// Sets a parameter. Some parameters are read-only. In such a case, 
/// 0 is returned.
int topaz_context_set_parameter(const topaz_t *, const topazString_t *, double);

/// Retrieves an array of all parameter names known.
///
const topazArray_t * topaz_get_parameter_names(const topaz *);



#endif // H_TOPAZDC_MAIN_INCLUDED
