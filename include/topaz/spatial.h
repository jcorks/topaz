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


#ifndef H_TOPAZDC__SPATIAL__INCLUDED
#define H_TOPAZDC__SPATIAL__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/matrix.h>
#include <topaz/transform.h>
///
///    A hierarchical transform object.
/// 
///    Allows for chaining transformation information including
///    position, rotation, and scaling. Because spatial objects are hierarchical,
///    each has a local transform and a global transform.
///
///
///
typedef struct topazSpatial_t topazSpatial_t;


/// Creates a spatial object.
///
topazSpatial_t * topaz_spatial_create();


/// Destroys a spatial object
///
void topaz_spatial_destroy(
    /// Spatial instance to destroy.
    topazSpatial_t * spatial
);


/// Resets the spatial back to the default state
///
void topaz_spatial_reset(
    /// Spatial instance to modify.
    topazSpatial_t * spatial
);


/// Gets the transform for this spatial object alone.
/// Modifications to it affect the global transform calculations
///
topazTransform_t * topaz_spatial_get_node(
    /// Spatial instance to query.
    topazSpatial_t * spatial
);




    
/// Gets the transform matrix that represets this Transform and its 
/// child parent transforms. If changes have been queued for children 
/// transforms or this one, those changes are applied.
///
const topazMatrix_t * topaz_spatial_get_global_transform(
    /// Spatial instance to query.
    topazSpatial_t * spatial
);

/// Swaps the built in transform for the given transform 
/// pass nullptr to return the transform back to the default.
///
void topaz_spatial_replace_transform(
    /// Spatial to update.
    topazSpatial_t * spatial, 

    /// Transform to bind to the spatial. The lifetime of the transform 
    /// should be at least as long as the spatials'
    topazTransform_t * trans
);
    
/// Sets the given spatial object as a child of this one
///
void topaz_spatial_set_as_parent(
    /// The child in the operation.
    topazSpatial_t * child, 

    /// The parent in the operation.
    topazSpatial_t * newParent
);
    
/// invalidates this transfrom manually, causing itself and children to update 
/// upon next request for its transform.
///
void topaz_spatial_invalidate(
    /// The spatial to invalidate and mark for updating.
    topazSpatial_t * spatial
);

/// Checks for an update to the spatial object.
///
void topaz_spatial_check_update(
    /// The spatial to update if applicable.
    topazSpatial_t * spatial
);


typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;

/// Returns a 16-float array that signifies the internal transform array.
/// This is intended for quick renderer upload. This will always
/// reflect the global transform
void topaz_spatial_update_model_transforms(
    /// The spatial to query.
    topazSpatial_t * spatial, 

    /// The renderer buffer to populate.
    topazRenderer_Buffer_t * id
);
	

/// Callback for when a spatial update is pushed through.
///
typedef void (*topaz_spatial_update_callback)(
    /// Spatial that received the callback
    topazSpatial_t * spatial, 

    /// User data bound to the callback
    void * userData
);


/// Function called when the spatial object 
/// changes it transform in 3D space. This can be triggered 
/// either by local changes or by hierarchy changes.
/// An ID to the added callback is returned, which can 
/// be used to remove it in the future.
///
uint32_t topaz_spatial_add_update_callback(
    /// The spatial to add a callback to.
    topazSpatial_t * spatial, 

    /// Callback to add.
    topaz_spatial_update_callback cb, 

    /// User data for the callback
    void * userData
);

/// Removes the specified callback 
///
void topaz_spatial_remove_update_callback(
    /// The spatial to modify.
    topazSpatial_t * spatial, 

    /// The callback ID to remove.
    uint32_t id
);


#endif
