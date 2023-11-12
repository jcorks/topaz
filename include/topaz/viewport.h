/*
Copyright (c) 2023, Johnathan Corkery. (jcorkery@umich.edu)
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


#ifndef H_TOPAZDC__VIEWPORT__INCLUDED
#define H_TOPAZDC__VIEWPORT__INCLUDED


#include <topaz/system.h>
#include <topaz/backends/renderer.h>
typedef struct topazEntity_t topazEntity_t;
typedef struct topaz_t topaz_t;
typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;
typedef struct topazAsset_t topazAsset_t;

///
///
///    The symbolic viewing port for the engine.
///    It acts as the bridge between the rendered scene and the viewer.
///    Any number of Viewports may be used, but each Display has 
///    its own viewport that it delivers renderer.
///
///    Viewports can also be used to manage custom framebuffer operations 
///    to store intermediate data for advanced rendering effects.
///
///    Note: due to the nature of rendering order, attached components 
///    will not be reflected in the draw operations. Use child entities 
///    for drawing.
///
typedef struct topazViewport_t topazViewport_t;





/// Creates a new display object. Usually you can use topaz_view_manager_create_display() 
/// instead of calling this function directly.
///
topazEntity_t * topaz_viewport_create(
    topaz_t * 
);

/// Gets an image that always represents the complete framebuffer.
/// This can be used for rendering effects.
///
topazAsset_t * topaz_viewport_get_image(
    topazEntity_t *
);

/// Retrieves the current working framebuffer 
/// The working framebuffer is typically where all the current 
/// rendering operations go, so it is not ready to use for displaying.
/// Instead, the complete framebuffer will have the last swapped 
/// working buffer, so it will be ready for use.
///
/// (No script mapping)
///
topazRenderer_Framebuffer_t * topaz_viewport_get_working_framebuffer(
    topazEntity_t *
);

/// Retrieves the complete framebuffer, which is the last 
/// finished framebuffer from swapping.
///
/// (No script mapping)
///
topazRenderer_Framebuffer_t * topaz_viewport_get_complete_framebuffer(
    topazEntity_t *
);

/// Resizes the viewport. This also resizes the attached framebuffers,
/// the attached image asset, and, if automatic updating for it is 
/// enabled, the 3D transform aspect ratio.
void topaz_viewport_resize(
    topazEntity_t *,
    int width,
    int height
);

/// Gets the current width of the viewport.
///
int topaz_viewport_get_width(
    topazEntity_t *
);

/// Gets the current width of the viewport.
///
int topaz_viewport_get_height(
    topazEntity_t *
);


/// Gets the current projection matrix of the 
const topazMatrix_t * topaz_viewport_get_projection_3d(
    topazEntity_t *
);


/// Whether to have the framebuffers be filtered 
/// when displaying them as images.
void topaz_viewport_set_filtered(
    topazEntity_t *,
    int enabled
);

/// Sets whether to use automatic view mode 
/// for 3D projection. For the 3D projection 
/// matrix, this means a perspective projection 
/// matching with 60 degrees fovy, viewport size-matched 
/// aspect ratio, a near Z clip of 0.01, and a far Z 
/// clip of 100
///
/// The default is to be enabled. If not enabled, 
/// topaz_viewport_set_projection_3d can be used 
/// to set the projection.
///
void topaz_viewport_set_projection_3d_auto_mode(
    topazEntity_t *,
    int enabled
);

/// Sets the current projection matrix for 3D operations.
/// Note that this will do nothing if 
/// the 3D project is set to auto mode, which 
/// is the default. Disable auto mode before 
/// using this feature.
////
void topaz_viewport_set_projection_3d(
    topazEntity_t *,
    const topazMatrix_t * m
);


/// Retrieves a translation of a point from 3D world 
/// space into screen space based on the current projection 
/// and transform of the viewport.
///
topazVector_t topaz_viewport_world_3d_to_screen(
    topazEntity_t *, 
    const topazVector_t * point
);

/// Retrives a translation of a point from screen space 
/// into 3D world space based on the current projection 
/// and transform of the viewport.
topazVector_t topaz_viewport_screen_to_world_3d(
    topazEntity_t *, 
    const topazVector_t * point
);



/// Forces clearing of the results of drawing stored within 
/// the working framebuffer.
///
void topaz_viewport_clear(
    topazEntity_t * ,
    
    /// which information channel to clear.
    topazRenderer_DataLayer layer
);

/// Sets whether clearing of the framebuffer's contents is done automatically. 
/// The default is "true".
///
void topaz_viewport_set_auto_clear(
    topazEntity_t * ,
    /// Whether to auto-update.
    int enabled
);

/// Swaps the working framebuffer with the complete 
/// framebuffer. The complete framebuffer is the one delivered to the display
/// and to the image asset associated with this viewport.
void topaz_viewport_swap_buffers(
    topazEntity_t * 
);

/// Ensures any queued data will be reflected in 
/// the working framebuffer. This implicitly 
/// calls topaz_graphics_sync()
void topaz_viewport_sync(
    topazEntity_t *
);




#endif
