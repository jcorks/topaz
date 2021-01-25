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


#ifndef H_TOPAZDC__TRANSFORM__INCLUDED
#define H_TOPAZDC__TRANSFORM__INCLUDED


#include <topaz/vector.h>
#include <topaz/matrix.h>

///
///    A hierarchical transform object.
///  
///    Allows for chaining transformation information including
///    position, rotation, and scaling. Because Transforms are hierarchical,
///    each has a local transform and a global transform.
///
typedef struct topazTransform_t topazTransform_t;


/// Creates a new transform object 
///
topazTransform_t * topaz_transform_create();


/// Destroys a transform object 
///
void topaz_transform_destroy(
    /// Transform to destroy.
    topazTransform_t * transform
);


/// Rests the transform to the initial transform state.
///
void topaz_transform_reset(
    /// Transform to modify.
    topazTransform_t * transform
);


    
/// Gets a read-only, quick reference to the rotation component.
///
const topazVector_t * topaz_transform_get_rotation(
    /// The transform to query.
    const topazTransform_t * transform
);

/// Gets a read-only, quick reference to the position component.
///
const topazVector_t * topaz_transform_get_position(
    /// The transform to query.
    const topazTransform_t * transform
);

/// Gets a read-only, quick reference to the scale component.
///
const topazVector_t * topaz_transform_get_scale(
    /// The transform to query.
    const topazTransform_t * transform
);



/// Returns a writable reference to the rotation component. 
/// Once called, flags the node to recalculate its transforms
/// on the following frame.
topazVector_t * topaz_transform_rotation(
    /// The transform to query. It will be marked for updating after this call in case 
    /// the transform property is changed.
    topazTransform_t * transform
);

/// Returns a writable reference to the position component. 
/// Once called, flags the node to recalculate its transforms
/// on the following frame.
topazVector_t * topaz_transform_position(
    /// The transform to query. It will be marked for updating after this call in case 
    /// the transform property is changed.
    topazTransform_t * transform
);

/// Returns a writable reference to the scale component. 
/// Once called, flags the node to recalculate its transforms
/// on the following frame.
topazVector_t * topaz_transform_scale(
    /// The transform to query. It will be marked for updating after this call in case 
    /// the transform property is changed.
    topazTransform_t * transform
);


/// Sets whether to reverse the node's interpretation of 
/// of translation for transform calculation. Rarely useful.
///
void topaz_transform_set_reverse_translation(
    /// The transform to modify.
    topazTransform_t * transform, 

    /// Whether to reverse.
    int reverse
);






/// Gets the transform matrix representative
/// of this Transform. At this time, any requests to modify 
/// the transform are processed. If any such request was made, 
/// the transform is updated and any OnTransformUpdate functors 
/// are called.
const topazMatrix_t * topaz_transform_get_matrix(
    /// The transform to query. It might internally trigger to update
    /// if the transform is invalidated.
    topazTransform_t * transform
);

/// Returns whether an update to this transform is 
/// pending.
int topaz_transform_needs_update(
    /// Returns whether the transform needs to process updates.
    const topazTransform_t * transform
);




/// A callback for when a transform commits queued changes. 
/// Individual properties of a transform (position, rotation, etc) 
/// are editable parts of the transform, but will need to be 
/// compiled into a matrix. 
typedef void (*topaz_transform_callback)(
    /// The transform that was updated.
    topazTransform_t * transform, 

    /// The data bound to the callback.
    void * callbackData
);



/// Adds a transform update.
///
void topaz_transform_add_callback(
    /// The transform to modify.
    topazTransform_t * transform,

    /// Function called when the transform.
    /// The first argument is the transform in question.
    /// The last argument is the userdata.
    topaz_transform_callback callback,

    /// Data to bind to the callback.
    void * userData
);

/// Removes the given callback
///
void topaz_transform_remove_callback(topazTransform_t *,
    /// The callback to remove.
    topaz_transform_callback callback,

    /// The callback data bound to the callback.
    void * userData
);


#endif
