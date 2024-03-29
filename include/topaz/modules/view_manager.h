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
/// (No script mapping)
///
topazViewManager_t * topaz_view_manager_create(
    /// The topaz context.
    topaz_t * context
);

/// Destroys and frees a topaz view manager instance.
///
/// (No script mapping)
///
void topaz_view_manager_destroy(
    /// The view manager to destroy.
    topazViewManager_t * viewMan
);





/// Same as topaz_view_manager_create_main, except the width and height
/// are set as 640 and 480 respectively.
///
topazDisplay_t * topaz_view_manager_create_display(
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

/// Gets the current main display. If none is set, NULL is returned.
/// 
topazDisplay_t * topaz_view_manager_get_default(
    /// The view manager to query.
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


/// Gets an array of all displays registered under the 
/// this view manager.
///
const topazArray_t * topaz_view_manager_get_all(
    /// The view manager to query
    const topazViewManager_t * viewMan
);


/// Gets an array of all displays registered under the 
/// this view manager that are active. The array should 
/// be destroyed when done.
///
/// (No script mapping)
///
topazArray_t * topaz_view_manager_get_all_active(
    /// The view manager to query
    const topazViewManager_t * viewMan
);


#endif
