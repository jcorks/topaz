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


#include <topaz/system.h>
typedef struct topazEntity_t topazEntity_t;
typedef struct topaz_t topaz_t;


///
/// An abstraction for the environment's means of 
/// displaying rendered information.
///
/// In most contexts / systems, the display is a window within the 
/// desktop environment. On embedded systems, this may be the entire
/// screen display.
///
/// Because of the flexibility / variation in what a display can be
/// some backends may choose to implement features differently
/// or omit features altogether where appropriate.
///
typedef struct topazDisplay_t topazDisplay_t;



/// Callback for responding to display events,
typedef void (*topaz_display_callback)(
    /// The display responding to the event.
    topazDisplay_t * display,
    /// The data bound to the callback.
    void * data
);




/// The standard functional capabilities of a Display.
///
typedef enum topazDisplay_Capability topazDisplay_Capability;
enum topazDisplay_Capability {
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

};



///  Controls how the Display displays Rendered data.
///
typedef enum topazDisplay_ViewPolicy topazDisplay_ViewPolicy;
enum topazDisplay_ViewPolicy {
    /// The Display will show the attached Framebuffer's contents with no transformation
    /// The visual is stretched over the width and height of the display.
    ///
    topazDisplay_ViewPolicy_None,       

    /// The Display will resize the attached Framebuffer to match the windows dimensions
    /// whenever the window is resized.
    /// This is the default.
    ///
    topazDisplay_ViewPolicy_MatchSize,      
};




///  The variety of system handle types that Display can represent.
///
typedef enum topazDisplay_Handle topazDisplay_Handle;
enum topazDisplay_Handle {
    /// The display handle is an X11Display instance. In this case, the display is a window in an X11 environment
    ///
    topazDisplay_Handle_X11Display,     

    /// The display handle is a MS Windows HWND instance.
    ///
    topazDisplay_Handle_WINAPIHandle,   

    /// THe display handle is a GLFWwindow instance.
    ///
    topazDisplay_Handle_GLFWwindow,

    /// The handle's type is not known and should not be relied on.
    ///
    topazDisplay_Handle_Unknown         
};

/// The varienty of system event types that Display can give.
///
typedef enum topazDisplay_Event topazDisplay_Event;
enum topazDisplay_Event {
    /// The event is a pointer to a topazArray_t of <XEvent>.
    ///
    topazDisplay_Event_X11Event,      

    /// The event is a pointer to a topazArray_t of <MSG>.
    ///
    topazDisplay_Event_WINAPIMsg,     

    ///< The event's type is not known and should not be relied on.
    ///
    topazDisplay_Event_Unknown            
};

#include <topaz/backends/api/display_api.h>



/// Creates a new display object. Usually you can use topaz_view_manager_create_display() 
/// instead of calling this function directly.
///
topazDisplay_t * topaz_display_create(
    /// The topaz context.
    topaz_t * context, 


    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazDisplayAPI_t api
);


/// Destroys and cleans up a display API
///
void topaz_display_destroy(
    /// The display to destroy
    topazDisplay_t * display
);


/// Retrieves the camera associated with the display thats 
/// used for 2d operations. This camera is used when the display is set as 
/// the main display. see topazViewManager_t
///
topazEntity_t * topaz_display_get_2d_camera(
    /// The display to query.
    topazDisplay_t * display
);

/// Retrieves the camera associated with the display thats 
/// used for 3d operations. This camera is used when the display is set as 
/// the main display. see topazViewManager_t
///
topazEntity_t * topaz_display_get_3d_camera(
    /// The display to query.
    topazDisplay_t * display
);


/// Gets the camera used for output rendering results. This camera's 
/// framebuffers are used for storing the visual of whats shown in the display 
///
topazEntity_t * topaz_display_get_render_camera(
    /// The display to query.
    topazDisplay_t * display
);



/// Retrieves the backend for this display object.
///
topazSystem_Backend_t * topaz_display_get_backend(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns the API for this display.
///
topazDisplayAPI_t topaz_display_get_api(
    /// The display to query.
    topazDisplay_t * display
);


/// Sets whether refreshing of the main-display's camera should be handled by the 
/// display itself. The default is "true".
///
void topaz_display_auto_refresh_camera(
    /// The display to modify.
    topazDisplay_t * display, 
    /// Whether to auto-update.
    int autoUpdate
);




/// Resizes the display. If the display does not support resizing, no action is taken.
///    
void topaz_display_resize(
    /// The display to resize.
    topazDisplay_t * display, 

    /// The new width.
    int w, 
    
    /// The new height.
    int h
);

/// Sets the position of the display. 
///
/// Usually, this is relative to whatever environment
/// the display exists in. For example, in a desktop environment, this could be
/// an offset from the DE's origin. If the display does not support moving,
/// no action is taken.
///
void topaz_display_set_position(
    /// The display to move.
    topazDisplay_t * display, 
    /// The x position. This is context dependent.
    int x, 
    /// The y position. This is context dependent.
    int y
);

/// Set the display into a fullscreen context. If fullscreen is not supported,
/// no action is taken.
///
void topaz_display_fullscreen(
    /// The display to make fullscreen
    topazDisplay_t * display, 

    /// Whether to set fullscreen
    int fullscreen
);

/// Attempts to hide the display. If hiding is not supported, no action is taken.
///
void topaz_display_hide(
    /// The display to hide
    topazDisplay_t * display, 

    /// Whether to hide.
    int state
);

/// Returns whether the display has user input focus. On display implementations
/// where this doesnt apply, i.e. where there is only one logical display available,,
/// this will always return true. 
///
int topaz_display_has_input_focus(
    /// The display to query.
    topazDisplay_t * display
); 

/// Attempts to prevent resizing on the user's side. 
///
/// For example,
/// in a desktop environment, this would disable the feature of resizing
/// the window.
///
void topaz_display_lock_client_resize(
    /// The display to lock.
    topazDisplay_t * display, 

    /// Whether to lock.
    int state
);

/// Attempts to prevent moving on the user's side. 
/// 
void topaz_display_lock_client_position(
    /// The display to lock.
    topazDisplay_t * display, 

    /// Whether to lock.
    int state
);

/// Controls how the Renderer's information is displayed. The default policy is "MatchSize"
/// See ViewPolicy for more information. 
///
void topaz_display_set_view_policy(
    /// The display to modify.
    topazDisplay_t * display,

    /// The new policy. 
    topazDisplay_ViewPolicy policy
);

/// Returns the width of the display.
///
int topaz_display_get_width(
    /// The display to query.
    topazDisplay_t * display
);

/// Returns the height of the display.
///
int topaz_display_get_height(
    /// The display to query.
    topazDisplay_t * display
);

/// Returns the X position of the display.
///
int topaz_display_get_x(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns the Y position of the display.
///
int topaz_display_get_y(
    /// The display to query.
    topazDisplay_t * display
);




/// Sets the name of the display. On some systems, this can, for example,
/// set the title bar of the application to the specified name.
void topaz_display_set_name(
    /// The display to modify.
    topazDisplay_t * display, 

    /// The new displayName
    const topazString_t * name
);



/// Adds an additional callback function to be be called after
/// the occurance of a resize event.Callbacks are run in the order that they
/// were added in.
int topaz_display_add_resize_callback(
    /// The display to add a callback to.
    topazDisplay_t * display, 

    /// The callback to add.
    topaz_display_callback callback, 

    /// The data to bind to the callback.
    void * data
);


/// Adds an additional callback function to be be called after
/// the occurance of a closing event.
///
/// Typically on desktop systems,
/// this is triggered by pressing the close button on the window
/// associated with the Display. Callbacks are run in the order that they
/// were added in.
///
int topaz_display_add_close_callback(
    /// The display to add a callback to.
    topazDisplay_t * display, 

    /// The callback to add.
    topaz_display_callback callback, 

    /// The data to bind to the callback.
    void * data
);


/// Removes the callback by id.
///
void topaz_display_remove_callback(
    /// The display to modify.
    topazDisplay_t * display, 

    /// The callback to remove.
    int id
);




/// Returns whether or not the Display is able to 
/// perform the requested capability.
///
int topaz_display_is_capable(
    /// The display to query.
    topazDisplay_t * display,

    /// The capability to query.    
    topazDisplay_Capability capability
); 



/// Updates display with input visual data that is 
/// populated in the internal framebuffer.
///
void topaz_display_update(
    /// The display to update.
    topazDisplay_t * display
);    

/// Returns the framebuffer types that this renderer supports.
/// Should the framebuffer not match one of the given types, the framebuffer 
/// attachment will fail
///
const topazArray_t * topaz_display_supported_framebuffers(
    /// The display to query.
    topazDisplay_t * display
);

/// Returns the type associated with data returned by 
/// GetSystemHandle. Meant for internal use, but can be handy when
/// doing weird things.
///
topazDisplay_Handle topaz_display_get_system_handle_type(
    /// The display to query.
    topazDisplay_t * display
);

/// Returns an implementation-specific handle that represents this
/// Display or this Display's properties. Meant for internal use, but can 
/// be handy when doing weird things.
///
void * topaz_display_get_system_handle(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns the type of the system events returned by 
/// GetLastSystemEvent(). Like getting the system handle, it's mostly meant for internal 
/// use, but can come in handy when doing weird things.
///
topazDisplay_Event topaz_display_get_system_event_type(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns an implementation-specific value that represents the 
/// last processed event generated form the display. It is guaranteed to be updated
/// after Update() has been called and is valid until the next Update() call.
///
void * topaz_display_get_last_system_event(
    /// The display to query.
    topazDisplay_t * display
);



/// Returns the current contents of the clipboard, if applicable.
/// This buffer is owned by the caller.
///
topazArray_t * topaz_display_get_current_clipboard(
    /// The display to query.
    topazDisplay_t * display
);


/// Sets the current contents of the clipboard, if applicable.
///
void topaz_display_set_current_clipboard(
    /// The display to modify.
    topazDisplay_t * display, 

    /// The new contents of the clipboard.
    const topazArray_t * contents
);





#endif
