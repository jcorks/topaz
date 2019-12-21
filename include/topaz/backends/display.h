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


#ifndef H_TOPAZDC__DISPLAY__INCLUDED
#define H_TOPAZDC__DISPLAY__INCLUDED


#include <topaz/backends/backend.h>



/*

    Display
    -----
    An abstraction for millisecond resolution timing.
    This short set of utilities allows for timing 

*/

typedef struct topazDisplay_t topazDisplay_t;






/// The standard functional capabilities of a Display.
///
typedef enum {
    /// The Display can be resized.
    ///
    topazDisplay_Capability_CanResize,      

    /// The Display's position can be moved.
    ///
    topazDisplay_Capability_CanMove,       
 
    /// The Display's size can consume the entire physical device, often in a special state.
    ///
    topazDisplay_Capability_CanFullscreen,  


    /// The Display can be hidden.
    ///
    topazDisplay_Capability_CanHide,        


    /// The Display can prevent the user from changing the dimensions of the Display.
    ///
    topazDisplay_Capability_CanLockSize     

} topazDisplay_Capability;



///  Controls how the Display displays Rendered data.
///
typedef enum {
    /// The Display will show the attached Framebuffer's contents with no transformation
    ///
    NoPolicy,       

    /// The Display will stretch the attached Framebuffer's contents to match the windows dimensions
    ///
    MatchSize,      
} topazDisplay_ViewPolicy;




///  The variety of system handle types that Display can represent.
///
typedef enum {
    /// The display handle is an X11Display instance. In this case, the display is a window in an X11 environment
    ///
    topazDisplay_Handle_X11Display,     

    /// The display handle is a MS Windows HWND instance.
    ///
    topazDisplay_Handle_WINAPIHandle,   

    /// The handle's type is not known and should not be relied on.
    ///
    topazDisplay_Handle_Unknown         
} topazDisplay_Handle;

/// The varienty of system event types that Display can give.
///
typedef enum {
    /// The event is a pointer to a topazArray_t of <XEvent>.
    ///
    topazDisplay_Event_X11Event,      

    /// The event is a pointer to a topazArray_t of <MSG>.
    ///
    topazDisplay_Event_WINAPIMsg,     

    ///< The event's type is not known and should not be relied on.
    ///
    topazDisplay_Event_Unknown            
} topazDisplay_Event;

#include <topaz/backends/api/display_api.h>



/// Creates a new display object
///
topazDisplay_t * topaz_display_create(topazBackend_t *, topazDisplayAPI_t);


/// Destroys and cleans up a display API
///
void topaz_display_destroy(topazDisplay_t *);





/// Retrieves the backend for this display object.
///
topazBackend_t * topaz_display_get_backend(topazDisplay_t *);


/// Returns the API for this display.
///
topazDisplayAPI_t topaz_display_get_api(topazDisplay_t *);






/// Resizes the display. If the display does not support resizing, no action is taken.
///    
void topaz_display_resize(topazDisplay_t *, int w, int h);

/// Sets the position of the display. 
///
/// Usually, this is relative to whatever environment
/// the display exists in. For example, in a desktop environment, this could be
/// an offset from the DE's origin. If the display does not support moving,
/// no action is taken.
///
void topaz_display_set_position(topazDisplay_t *, int x, int y);

/// Set the display into a fullscreen context. If fullscreen is not supported,
/// no action is taken.
///
void topaz_display_fullscreen(topazDisplay_t *, int);

/// Attempts to hide the display. If hiding is not supported, no action is taken.
///
void topaz_display_hide(topazDisplay_t *, int);

/// Returns whether the display has user input focus. On display implementations
/// where this doesnt apply, i.e. where there is only one logical display available,,
/// this will always return true. 
///
int topaz_display_has_input_focus(topazDisplay_t *); 

/// Attempts to prevent resizing on the user's side. 
///
/// For example,
/// in a desktop environment, this would disable the feature of resizing
/// the window.
///
void topaz_display_lock_client_resize(topazDisplay_t *, int);

/// Attempts to prevent moving on the user's side. 
/// 
void topaz_display_lock_client_position(topazDisplay_t *, int);

/// Controls how the Renderer's information is displayed. The default policy is "MatchSize"
/// See ViewPolicy for more information. 
///
void topaz_display_set_view_policy(topazDisplay_t *, topazDisplay_ViewPolicy);

/// Returns the width of the display.
///
int topaz_display_get_width(topazDisplay_t *);

/// Returns the height of the display.
///
int topaz_display_get_height(topazDisplay_t *);

/// Returns the X position of the display.
///
int topaz_display_get_x(topazDisplay_t *);

/// Returns the Y position of the display.
///
int topaz_display_get_y(topazDisplay_t *);



/// Sets the name of the display. On some systems, this can, for example,
/// set the title bar of the application to the specified name.
void topaz_display_set_name(topazDisplay_t *, const topazString_t *);

/// Adds an additional callback function to be be called after
/// the occurance of a resize event.Callbacks are run in the order that they
/// were added in.
void topaz_display_add_resize_callback(topazDisplay_t *, void(*)(int w, int h, void *), void *);

/// Removes the callback of the same instance as one given via
/// AddResizeCallback.
void topaz_display_remove_resize_callback(topazDisplay_t *, void(*)(int w, int h, void *));

/// Adds an additional callback function to be be called after
/// the occurance of a closing event.
///
/// Typically on desktop systems,
/// this is triggered by pressing the close button on the window
/// associated with the Display. Callbacks are run in the order that they
/// were added in.
///
void topaz_display_add_close_callback(topazDisplay_t *, void(*)(void *), void *);

/// Removes the callback of the same instance as one given via
/// AddCloseCallback.
///
void topaz_display_remove_close_callback(topazDisplay_t *, void(*)(void *));



/// Returns whether or not the Display is able to 
/// perform the requested capability.
///
int topaz_display_is_capable(topazDisplay_t *, topazDisplay_Capability); 



////////////////////// Interfacing with others


/// Updates display with input visual data that is 
/// populated in the internal framebuffer. THe framebuffer's
/// data canbe populated by modifying the framebuffer in GetSource()
///
void topaz_display_update(topazDisplay_t *, topazRenderer_Framebuffer_t *);    

/// Returns the framebuffer types that this renderer supports.
/// Should the framebuffer not match one of the given types, the framebuffer 
/// attachment will fail
///
const topazArray_t * topaz_display_supported_framebuffers(topazDisplay_t *);

/// Returns the type associated with data returned by 
/// GetSystemHandle. Meant for internal use, but can be handy when
/// doing weird things.
///
topazDisplay_Handle topaz_display_get_system_handle_type(topazDisplay_t *);

/// Returns an implementation-specific handle that represents this
/// Display or this Display's properties. Meant for internal use, but can 
/// be handy when doing weird things.
///
void * topaz_display_get_system_handle(topazDisplay_t *);

/// Returns the type of the system events returned by 
/// GetLastSystemEvent(). Like getting the system handle, it's mostly meant for internal 
/// use, but can come in handy when doing weird things.
///
topazDisplay_Event topaz_display_get_system_event_type(topazDisplay_t *);

/// Returns an implementation-specific value that represents the 
/// last processed event generated form the display. It is guaranteed to be updated
/// after Update() has been called and is valid until the next Update() call.
///
void * topaz_display_get_last_system_event(topazDisplay_t *);


/// Returns the current contents of the clipboard, if applicable.
/// This buffer is owned by the caller.
///
topazArray_t * topaz_display_get_current_clipboard(topazDisplay_t *);

/// Sets the current contents of the clipboard, if applicable.
///
void topaz_display_set_current_clipboard(topazDisplay_t *, const topazArray_t *);





#endif
