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


#ifndef H_TOPAZDC__GRAPHICS__INCLUDED
#define H_TOPAZDC__GRAPHICS__INCLUDED

typedef struct topaz_t topaz_t;
typedef struct topazSystem_t topazSystem_t;
typedef struct topazRender2D_t topazRender2D_t;
typedef struct topazRenderer_2D_t topazRenderer_2D_t;
typedef struct topazRenderer_3D_t topazRenderer_3D_t;
typedef struct topazRenderer_Attributes_t topazRenderer_Attributes_t;
typedef struct topazRenderer_t topazRenderer_t;

/// The main graphics controller. It manages the context's renderer 
/// and attempts to make interaction with it more efficient.
/// Usually, its usage is implicit; a user will rarely need to 
/// interact with the graphics controller. However, if advanced 
/// features are needed, the graphics controller is part of the 
/// main pathway to access lower-level graphics features.
typedef struct topazGraphics_t topazGraphics_t;





/// Creates a new graphics instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_graphics();
///
topazGraphics_t * topaz_graphics_create(
    /// The context.
    topaz_t * context
);

/// Destroys and frees a topaz input instance.
///
void topaz_graphics_destroy(
    /// The graphics instance.
    topazGraphics_t * graphics
);


/// Pushes a viewport that the graphics instance should use.
/// The top viewport is used for graphics operations.
///
void topaz_graphics_push_viewport(
    topazGraphics_t * graphics,
    topazViewport_t * viewport
);

/// Pops the top viewport, if any. 
/// Note that this does not sync the viewport, but does 
/// detach the target framebuffer, so be sure everything 
/// desired is sync'd
///
void topaz_graphics_pop_viewport(
    topazGraphics_t * graphics
);

/// Returns the current top viewport. NULL if none.
topazViewport_t * topaz_graphics_get_current_viewport(
    topazGraphics_t *
);


/// Gets the backend rendering object for this topaz instance.
///
topazRenderer_t * topaz_graphics_get_renderer(
    /// The graphics to query.
    topazGraphics_t * graphics
);


/// Gets the backend 2d-specific rendering object for this topaz instance.
///
topazRenderer_2D_t * topaz_graphics_get_renderer_2d(
    /// The graphics to query.
    topazGraphics_t * graphics
);


/// Applies a render2d object to the renderer. This may not be immediate, but will 
/// attempt to be grouped together to accelerate the rendering process
///
void topaz_graphics_request_draw_2d(
    /// The graphics to access.
    topazGraphics_t * graphics, 
    /// The render2d to draw.
    topazRender2D_t * r2d
);

/// Applies a 3D renderering object to the renderer.
/// A thin wrapper for topaz_renderer_draw_3d.
void topaz_graphics_request_draw_3d(
    /// The graphics instance to access.
    topazGraphics_t * g,
    /// The 3D instance.
    topazRenderer_3D_t * d,    
    /// Processing attributes for the object.
    const topazRenderer_Attributes_t * att
);


/// Ensures that all graphics operations have finished and are pushed 
/// to attached displays.
///
void topaz_graphics_sync(
    /// The graphics instance to sync.
    topazGraphics_t * graphics
);

/// Clears the current display and resets any requested 2d drawn objects.
/// 
void topaz_graphics_reset_scene(
    /// The graphics instance to reset.
    topazGraphics_t * graphics
);

//void topaz_graphics_draw_3d(topazRenderMesh_t *);




#endif
