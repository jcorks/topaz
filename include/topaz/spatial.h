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
/*

    Spatial
    -----
    A hierarchical transform object.
 
    Allows for chaining transformation information including
    position, rotation, and scaling. Because Nodes are hierarchical,
    each has a local transform and a global transform.




*/
typedef struct topazSpatial_t topazSpatial_t;


/// Creates a spatial object.
///
topazSpatial_t * topaz_spatial_create();


/// Destroys a spatial object
///
void topaz_spatial_destroy(topazSpatial_t *);



/// Gets the transform for this spatial object alone.
/// Modifications to it affect the global transform calculations
///
topazTransform_t * topaz_spatial_get_node(topazSpatial_t *);




    
/// Gets the transform matrix that represets this Transform and its 
/// child parent transforms. If changes have been queued for children 
/// transforms or this one, those changes are applied.
///
const topazMatrix_t * topaz_spatial_get_global_transform(topazSpatial_t *);

/// Swaps the built in transform for the given transform 
/// pass nullptr to return the transform back to the default.
///
void topaz_spatial_replace_transform(topazSpatial_t *, topazTransform_t *);
    
/// Sets the given spatial object as a child of this one
///
void topaz_spatial_set_as_parent(topazSpatial_t *, topazSpatial_t *);
    
/// invalidates this transfrom manually, causing itself and children to update 
/// upon next request for its transform.
///
void topaz_spatial_invalidate(topazSpatial_t *);

/// Checks for an update to the spatial object.
///
void topaz_spatial_check_update(topazSpatial_t *);

//Returns a 16-float array that signifies the internal transform array.
//This is intended for quick renderer upload. This will always
//reflect the global transform
typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;
void topaz_spatial_update_model_transforms(topazSpatial_t *, topazRenderer_Buffer_t * id);
	

/// Function called when the spatial object 
/// changes it transform in 3D space. This can be triggered 
/// either by local changes or by hierarchy changes.
///
void topaz_spatial_add_update_callback(
    topazSpatial_t *, 

    /// Callback
    /// 
    void(*)(topazSpatial_t *, void *), 


    /// User data for the callback
    ///
    void * userData
);

/// Removes the specified callback 
///
void topaz_spatial_remove_update_callback(
    topazSpatial_t *, 

    /// Callback
    /// 
    void(*)(topazSpatial_t *, void *), 


    /// User data for the callback
    ///
    void * userData

);


#endif
