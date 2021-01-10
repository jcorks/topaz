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


#ifndef H_TOPAZDC__CAMERA__INCLUDED
#define H_TOPAZDC__CAMERA__INCLUDED


#include <stdint.h>
#include <topaz/vector.h>
typedef struct topaz_t topaz_t;
typedef struct topazEntity_t topazEntity_t;
typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;
typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;
typedef struct topazAsset_t topazAsset_t;


///
///
///    The symbolic viewing port for the engine.
///    It acts as the bridge between the rendered scene and the viewer.
///    Any number of Cameras can be maintained, but only one may be used at a time.
///    See graphics.h for swapping between which camera is used for the display.
///
///
typedef struct topazCamera_t topazCamera_t;


/// The type dictates how the camera should be updated.
///
typedef enum topazCamera_Type topazCamera_Type;
enum topazCamera_Type {
    /// Indicates use for 3D perspective 
    ///
    topazCamera_Type_Perspective3D,

    /// Indicates that no distortion via perspective should take place.
    ///
    topazCamera_Type_Orthographic,

    /// Indicates that no updates to the camera's view or projection
    /// matrix will be done by the camera itself and that the user 
    /// be updaing the transforms on their own.
    ///
    topazCamera_Type_Manual

};





/// Creates a new camera instance.
///
topazEntity_t * topaz_camera_create(
    /// topaz instance reference.
    topaz_t * context
);

/// Sets the type of the camera.
///
void topaz_camera_set_type(
    /// Camera to have its type set
    topazEntity_t * camera, 

    /// The type to use
    topazCamera_Type type
);


/// Forces clearing of the results of drawing stored within the camera.
///
void topaz_camera_refresh(
    /// The camera to clear.
    topazEntity_t * camera
);



/// When called, the camera will orient itself so that it "looks"
/// at the given point in 3D space
///
void topaz_camera_look_at(
    /// The camera to modify
    topazEntity_t * camera, 

    /// The 3D position of the focal target that the camera should look at
    const topazVector_t * target,

    /// What vector direction constitutes the "Up" position of the camera.
    /// This helps determine rotations of the camera.
    const topazVector_t * up
);


/// Given a point on the screen, returns the unprojected point in 
/// world-space. The z value of the given point is interpreted as a depth.
///
topazVector_t topaz_camera_screen_to_world(
    /// The camera to use.
    topazEntity_t * camera, 

    /// The screen coordinate with Z coordinate to use.
    const topazVector_t * coord
);

/// Given a point in world space, returns a projected point in screen space
///
topazVector_t topaz_camera_world_to_screen(
    /// The camera to use.
    topazEntity_t * camera, 

    /// The world-coordinate to use.
    const topazVector_t * coord
);

/// Sets the render resolution for the camera in pixels.
///
void topaz_camera_set_render_resolution(
    /// The camera to change.
    topazEntity_t * camera, 

    /// The width of the new resolution.
    int w, 

    /// The height of the new resolution
    int h
);


/// Returns the current render resolution height.
///
int topaz_camera_get_render_height(
    /// The camera to query.
    const topazEntity_t * camera
);

/// Returns the current render resolution width.
///
int topaz_camera_get_render_width(
    /// The camera to query.
    const topazEntity_t * camera
);

/// Sets whether the camera should filter all its visuals. This 
/// prevents graininess in the case the render resolution does not match the 
/// size of the display. The default is to filter.
void topaz_camera_set_filtered(
    /// The camera to affect.
    topazEntity_t * camera, 

    /// Whether to enable filtering. The default is to filter.
    int onOrOff
);

/// Gets the raw framebuffer for the camera.
///
topazRenderer_Framebuffer_t * topaz_camera_get_framebuffer(
    /// The camera to query.
    topazEntity_t * camera
);

/// Creates a new image that contains the visual data corresponding to the 
/// front visual. The front visual is what is actively rendered to.
///
topazAsset_t * topaz_camera_get_front_visual(
    /// The camera to query.
    topazEntity_t * camera
);

/// Creates a new image that contains the visual data corresponding to the 
/// back visual. The back visual is what was last sent to the display, so it matches 
/// exactly whats on the display accoring to the user if this camera were in use.
///
topazAsset_t * topaz_camera_get_back_visual(
    /// The camera to query.
    topazEntity_t * camera
);

/// Swaps the camera's buffers.
///
void topaz_camera_swap_buffers(
    /// The camera to affect.
    topazEntity_t * camera
);

/// Gets the buffer that the camera uses as its resultant 
/// viewing transform.
///
topazRenderer_Buffer_t * topaz_camera_get_view_transform(
    /// The camera to query.
    topazEntity_t * camera
);

/// Gets the buffer that the camera uses as its resultant 
/// projection transform.
///
topazRenderer_Buffer_t * topaz_camera_get_projection_transform(
    /// The camera to query.
    topazEntity_t * camera
);

#endif
