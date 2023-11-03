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
#include <topaz/backends/renderer.h>
typedef struct topazEntity_t topazEntity_t;
typedef struct topaz_t topaz_t;
typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;
typedef struct topazAsset_t topazAsset_t;

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




/// On most systems, displays have a variety of features that can be 
/// controlled. This provides a generic interface into those features,
/// where a request can be posted to change a parameter via 
/// topaz_display_set_parameter().
///
typedef enum topazDisplay_Parameter topazDisplay_Parameter;
enum topazDisplay_Parameter {
    /// Controls the X position of the display. 
    ///
    /// Usually, this is relative to whatever environment
    /// the display exists in. For example, in a desktop environment, this could be
    /// an offset from the DE's origin. If the display does not support moving,
    /// no action is taken.
    topazDisplay_Parameter_X,     
    /// Controls the Y position of the display. 
    ///
    /// Usually, this is relative to whatever environment
    /// the display exists in. For example, in a desktop environment, this could be
    /// an offset from the DE's origin. If the display does not support moving,
    /// no action is taken.
    topazDisplay_Parameter_Y,
    /// The width of the display.
    ///
    topazDisplay_Parameter_Width,
    /// The height of the display.
    ///
    topazDisplay_Parameter_Height,
    /// Controls whether the display is shown.
    topazDisplay_Parameter_Show,
    /// Set the display into a fullscreen context. If fullscreen is not supported,
    /// no action is taken.
    ///
    topazDisplay_Parameter_Fullscreen,
    /// Controls prevention of resizing on the user's side. 
    ///
    /// For example,
    /// in a desktop environment, this would disable the feature of resizing
    /// the window.
    ///
    topazDisplay_Parameter_LockClientResize,
    /// Attempts to prevent moving on the user's side. 
    /// 
    topazDisplay_Parameter_LockClientPosition,
    /// Controls how the Renderer's information is displayed. The default policy is "MatchSize"
    /// See ViewPolicy for more information. 
    ///
    topazDisplay_Parameter_ViewPolicy,
    /// Returns whether the display has user input focus. On display implementations
    /// where this doesnt apply, i.e. where there is only one logical display available,,
    /// this will always return true. 
    ///
    topazDisplay_Parameter_InputFocus,
    /// Returns whether the display is active. Active displays have their roots update 
    /// and the results reflected on the display. Active displays also have input polled for.
    /// Newly created windows are active by default.
    topazDisplay_Parameter_Active,

    /// Whether to show display decorations. On most systems this implies things 
    /// like a titlebar, resize controls, etc.
    topazDisplay_Parameter_Decorated

};

/// Every display has built in framebuffers that can be 
/// used and exchanged between for advanced rendering effects.
///
typedef enum topazDisplay_Framebuffer topazDisplay_Framebuffer;
enum topazDisplay_Framebuffer {
    /// The default framebuffer.
    topazDisplay_Framebuffer_A,
    /// The second framebuffer.
    topazDisplay_Framebuffer_B,
    /// The 3rd framebuffer.
    topazDisplay_Framebuffer_C,
    /// The 4th framebuffer.
    topazDisplay_Framebuffer_D
};


/// Callback for responding to display events,
typedef void (*topaz_display_callback)(
    /// The display responding to the event.
    topazDisplay_t * display,
    /// The data bound to the callback.
    void * data
);



/// Callback for responding to display events,
typedef void (*topaz_display_parameter_callback)(
    /// The display responding to the event.
    topazDisplay_t * display,
    /// The parameter that was successfully changed.
    topazDisplay_Parameter param,
    /// The data bound to the callback.
    void * data
);


#include <topaz/backends/api/display_api.h>



/// Creates a new display object. Usually you can use topaz_view_manager_create_display() 
/// instead of calling this function directly.
///
/// (No script mapping)
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
/// No script mapping. Use ViewManager.destroyDisplay()
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






/// Gets the framebuffer specified.
topazRenderer_Framebuffer_t * topaz_display_get_framebuffer(
    /// The disply in question.
    topazDisplay_t * display,
    /// The framebuffer to get.
    topazDisplay_Framebuffer which
);

/// Sets the framebuffer as the main framebuffer. This framebuffer 
/// is rendered into the display every frame.
void topaz_display_use_framebuffer(
    /// The display to modify.
    topazDisplay_t * display,
    /// The framebuffer to use.
    topazDisplay_Framebuffer which
);


/// Sets a custom post-processing rendering that affects how the 
/// framebuffer is rendered to the display. The default in any case 
/// is just to take the framebuffer texture and map it to 
/// the display.
///
/// The parameters available to the provided shaders 
/// are dependent on the backend, but the position vertices,
/// texture coordinates, and texture sampler / ID referring 
/// to the framebuffer will be available.
/// 
/// Upon error, an error message is allocated and returned 
/// containing information on what went wrong. NULL is returned 
/// on no error.
topazString_t * topaz_display_set_post_process_shader(
    /// The display to modify.
    topazDisplay_t * display,

    /// The text for the vertex shader.
    const topazString_t * vertexShader,

    /// The text for the fragment shader.
    const topazString_t * fragmentShader

);

/// Returns the framebuffer currently in use.
topazRenderer_Framebuffer_t * topaz_display_get_main_framebuffer(
    /// The disply in question.
    topazDisplay_t * display
);

/// Forces clearing of the results of drawing stored within the current framebuffer.
///
void topaz_display_clear_main_framebuffer(
    /// The display to modify
    topazDisplay_t * display,
    
    /// which information channel to clear.
    topazRenderer_DataLayer layer
);


/// Gets the contents of the current framebuffer and 
/// puts the data into an image asset frame. 
/// The input image is cleared, resized to match the 
/// framebuffer size, and given one frame which matches the 
/// the framebuffer data.
///
/// NOTE: in general, this operation is considered "slow" as 
/// it requires a lot of communication between the backend 
/// renderer and engine, which are most likely on different 
/// devices. Use sparringly!
void topaz_display_capture_main_framebuffer(
    /// The display to query.
    topazDisplay_t * display,
    /// The image to populate.
    topazAsset_t * image
);










/// Retrieves the backend for this display object.
///
/// (No scripting mapping)
///
topazSystem_Backend_t * topaz_display_get_backend(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns the API for this display.
///
/// (No scripting mapping)
///
topazDisplayAPI_t topaz_display_get_api(
    /// The display to query.
    topazDisplay_t * display
);



/// Requests to set a parameter within the display. Once the 
/// the parameter is applied, parameters callbacks may be called 
/// on your behalf to alert the change.
void topaz_display_set_parameter(
    /// The display to modify.
    topazDisplay_t * display,
    /// The parameter to modify.
    topazDisplay_Parameter param,
    /// The new value of the parameter.
    float value
);


/// System backends may not use or be authorized 
/// to modify certain parameters. This can be used 
/// return whether a parameter is meaningful and 
/// editable.
int topaz_display_is_parameter_modifiable(
    /// The display to query.
    topazDisplay_t * display,
    /// The parameter to query.
    topazDisplay_Parameter param  
);

/// Returns the current value of the parameter.
float topaz_display_get_parameter(
    /// The display to query.
    const topazDisplay_t * display,
    /// The parameter to query.
    topazDisplay_Parameter param
);





/// Sets whether clearing of the framebuffer's contents is done automatically. 
/// The default is "true".
///
void topaz_display_auto_clear_framebuffer(
    /// The display to modify.
    topazDisplay_t * display, 
    /// Whether to auto-update.
    int enable
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
/// the occurance of a display parameter change. When parameters 
/// are changed, they are done so as requests; the display backend 
/// may apply these parameters asynchronously. To handle this,
/// the backend will post a callback when parameters are applied.
/// Callbacks are run in the order that they
/// were added in.
int topaz_display_add_parameter_callback(
    /// The display to add a callback to.
    topazDisplay_t * display, 

    /// The callback to add.
    topaz_display_parameter_callback callback, 

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


/// Returns the toplevel Entity. 
///
/// From here, you can 
/// set the Entity that holds the toplevel of the project. 
/// By default there is none.
topazEntity_t *topaz_display_get_root(
    /// The context to query.
    const topazDisplay_t * display
);


/// Sets the root entity for the context.
/// From this entity, all other entities will be updated.
/// topaz handles entities in a hierarchical structure,
/// so the root must be populated and managed.
void topaz_display_set_root(
    /// The context to modify.
    topazDisplay_t * display, 

    /// The new root to use.
    topazEntity_t * newRoot
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
/// (No script mapping)
///
const topazArray_t * topaz_display_supported_framebuffers(
    /// The display to query.
    topazDisplay_t * display
);

/// Returns the type associated with data returned by 
/// GetSystemHandle. Meant for internal use, but can be handy when
/// doing weird things.
///
/// (No script mapping)
///
topazDisplay_Handle topaz_display_get_system_handle_type(
    /// The display to query.
    topazDisplay_t * display
);

/// Returns an implementation-specific handle that represents this
/// Display or this Display's properties. Meant for internal use, but can 
/// be handy when doing weird things.
///
/// (No script mapping)
///
void * topaz_display_get_system_handle(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns the type of the system events returned by 
/// GetLastSystemEvent(). Like getting the system handle, it's mostly meant for internal 
/// use, but can come in handy when doing weird things.
///
/// (No script mapping)
///
topazDisplay_Event topaz_display_get_system_event_type(
    /// The display to query.
    topazDisplay_t * display
);


/// Returns an implementation-specific value that represents the 
/// last processed event generated form the display. It is guaranteed to be updated
/// after Update() has been called and is valid until the next Update() call.
///
/// (No script mapping)
///
void * topaz_display_get_last_system_event(
    /// The display to query.
    topazDisplay_t * display
);



/// Returns the current contents of the clipboard, if applicable.
/// This buffer is owned by the caller.
///
/// (No script mapping)
///
topazArray_t * topaz_display_get_current_clipboard(
    /// The display to query.
    topazDisplay_t * display
);


/// Sets the current contents of the clipboard, if applicable.
///
/// (No script mapping)
///
void topaz_display_set_current_clipboard(
    /// The display to modify.
    topazDisplay_t * display, 

    /// The new contents of the clipboard.
    const topazArray_t * contents
);





#endif
