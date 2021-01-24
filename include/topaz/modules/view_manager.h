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


#ifndef H_TOPAZDC__VIEW_MANAGER__INCLUDED
#define H_TOPAZDC__VIEW_MANAGER__INCLUDED

typedef struct topaz_t topaz_t;
typedef struct topazDisplay_t topazDisplay_t;
typedef struct topazString_t topazString_t;
typedef struct topazArray_t topazArray_t;



/// Module that manages displays.
///
/// On most systems, display backends represent something that can have 
/// multiple instance per fprogram invocation. This is meant to capture that.
/// For example, on systems where displays are windows, this would allow 
/// multiple windows owned and managed by the topaz context.
///
typedef struct topazViewManager_t topazViewManager_t;




/// Creates a new view manager. This is normally not necessary to call, as 
/// topaz_t has a default input instance that it generates for you. 
/// See topaz_get_view_manager();
///
topazViewManager_t * topaz_view_manager_create(
    /// The topaz context.
    topaz_t * context
);

/// Destroys and frees a topaz view manager instance.
///
void topaz_view_manager_destroy(
    /// The view manager to destroy.
    topazViewManager_t * viewMan
);



/// Creates a new Display with the given w and h in pixels.
///
topazDisplay_t * topaz_view_manager_create_display(
    /// The view manager to create a display with
    topazViewManager_t * viewMan, 

    /// Optional display name.
    const topazString_t * displayName, 

    /// Initialized width.
    int w, 

    /// Initialized height.
    int h
);

/// Same as topaz_view_manager_create_display(), but it also sets 
/// the new display as the main display for you.
///
topazDisplay_t * topaz_view_manager_create_main(
    /// The view manager to create a display with
    topazViewManager_t * viewMan, 

    /// Optional display name.
    const topazString_t * displayName, 

    /// Initialized width.
    int w, 

    /// Initialized height.
    int h
);



/// Same as topaz_view_manager_create_main, except the width and height
/// are set as 640 and 480 respectively.
///
topazDisplay_t * topaz_view_manager_create_main_default(
    /// The view manager to create a display with
    topazViewManager_t * viewMan, 

    /// Optional display name.
    const topazString_t * displayName
);

/// Removes a display. If this display was the main display, 
/// the main display is unset and is NULL.
///
void topaz_view_manager_destroy_display(
    /// The view manager to remove the display from.
    topazViewManager_t * viewMan, 

    /// The display to remove.
    topazDisplay_t * display
);


/// Sets the given display as the one to update and draw to.
/// NULL is accepted, and will result in no display being updated.
///
void topaz_view_manager_set_main(
    /// The view manager to modify.
    topazViewManager_t * viewMan, 

    /// The display to set as a main display.
    topazDisplay_t * display
);

/// Gets the current main display. If none is set, NULL is returned.
/// 
topazDisplay_t * topaz_view_manager_get_main(
    /// The view manager to query.
    topazViewManager_t * viewMan
);



/// Forces the main display to reflect the last rendered visual.
/// Normally called for you by the engine.
///
void topaz_view_manager_update_view(
    /// The view manager to update.
    topazViewManager_t * viewMan
);


/// Gets the current main display's clipboard contents
/// as a string.
const topazString_t * topaz_view_manager_get_clipboard_as_string(
    /// The view manager to pull from.
    topazViewManager_t * viewMan
);

/// Sets the current main display's clipboard contents as a UTF8 string.
///
void topaz_view_manager_set_clipboard_from_string(
    /// The view manager to modify.
    topazViewManager_t * viewMan, 

    /// The contents of the clipboard as text.
    const topazString_t * clipboardData
);

/// Convenience function to get the view width of the main display. If 
/// none, -1 is returned.
///
int topaz_view_manager_get_view_width(
    /// The view manager to query
    const topazViewManager_t * viewMan
);

/// Convenience function to get the view height of the main display. If 
/// none, -1 is returned.
///
int topaz_view_manager_get_view_height(
    /// The view manager to query
    const topazViewManager_t * viewMan
);


/// Gets an array of all displays registered under the 
/// this view manager.
///
const topazArray_t * topaz_view_manager_get_all(
    /// The view manager to query
    const topazViewManager_t * viewMan
);




#endif
