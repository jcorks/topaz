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
topaz_t * topaz_create();


/// Returns whether Dynacoe has been initialized successfully.
///
int topaz_is_initialized(const topaz_t *);


/// Begins the Engine execution loop.
///
/// Once run is initiated, all drawing to the screen and logic updating
/// is automated. While running, it is also possible to utilize the
/// debugging features such as the console and Entity debugger.
int topaz_run(const topaz_t *);
    
    
/// Pauses the Engine execution loop.
///
/// Once in a paused state, only critical objects (managers marked with pausable == false),
/// will be updated.
void topaz_pause(topaz_t *);

    
/// Immediately pauses the execution loop.
///
/// Like Pause() but disrupts the engine immediately rather than safely waiting 
/// until the next iteration. Most useful for debugging purposes.
void topaz_break(topaz_t *);



///  resumes from a paused or broken state.
///
void topaz_resume(topaz_t *);    
    
/// Updates the main engine.
///
///  Not equivalent to a frame, since frame throttling can occur.
void topaz_iterate(topaz_t *); 


///  returns whether the engine is in a paused or broken state.
///
bool topaz_is_paused(const topaz_t *);

/// Returns the toplevel Entity. 
///
/// From here, you can 
/// set the Entity that holds the toplevel of the project. 
/// By default there is none.
topazEntityID_t topaz_get_root(const topaz_t *);
topazEntityID_t topaz_set_root(topaz_t *);


/// Attaches a management-type entity.
///
/// If pausable is false, the manager will continue to update even when 
/// the engine is in a paused state.
void topaz_attach_manager(topazEntityID_t id);
void topaz_attach_manager_unpausable(topazEntityID_t id);



/// Ends the Engine execution loop.
///
void topaz_quit(topaz_t *);

/// Sleeps until the time required for the target frames-per-second is reached.
///
/// @param FPS  The target FPS; useful for loops.
///
/// The actual resolution is machine-dependent, but it tends to be millisecond resolution.
void topaz_wait(topaz_t *. int FPS);


/// Sets the ideal frame per second that the main loop should clock at.
///
void topaz_set_max_fps(topaz_t *, int i);


/// Gets the maximum FPS allowable by the Engine.
///
int topaz_get_max_fps(const topaz_t *);

/// Returns the starting directory of Dynacoe.
///
/// Reutrns an empty string if unavailable.
const topazString_t * topaz_get_base_directory(const topaz_t *);


/// Returns the version of the Dynacoe library as a whole.
///
const topazString_t * topaz_get_version(const topaz_t *);
    
    
    
typedef struct {
    float drawTimeMS;
    float stepTimeMS;
    float systemTimeMS;
    float engineRealTimeMS;        
    int currentFPS;
} topazDiagnostics_t;
    
topazDiagnostics_t & topaz_get_diagnostics(const topaz_t *);



#endif // H_TOPAZDC_MAIN_INCLUDED
