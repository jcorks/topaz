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


#ifndef H_TOPAZDC__SHAPE2D__INCLUDED
#define H_TOPAZDC__SHAPE2D__INCLUDED

#include <topaz/component.h>
#include <topaz/color.h>
#include <topaz/vector.h>
#include <topaz/render2d.h>
typedef struct topazTransform_t topazTransform_t;
typedef struct topazAsset_t topazAsset_t;

/// Can express basic 2D objects, such as images and shapes.
typedef struct topazShape2D_t topazShape2D_t;


/// Creates a new shape2d object
/// Use component_destroy to release.
///
topazComponent_t * topaz_shape2d_create(
    /// The topaz context.
    topaz_t * context
);

/// Gets the color of the object.
/// The default color is #000000 (black)
///
topazColor_t topaz_shape2d_get_color(
    /// The shape2d to query.
    topazComponent_t * shape2d
);

/// Sets the color of the object.
///
void topaz_shape2d_set_color(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The color to use.
    topazColor_t color
);

/// Gets the trasform for the object
///
/// This function has no script mapping; however,
/// the position, scale, and rotation for this 
/// node are settable and gettable via 
/// setPosition, setScale, setRotation, 
/// getPosition, getScale, and getRotation as part 
/// of the shape2d instance.
///
topazTransform_t * topaz_shape2d_get_node(
    /// The shape2d to query.
    topazComponent_t * shape2d
);

/// Gets the animation speed of the image. This is how quickly 
/// the object will flip through image frames. The time is returned in seconds.
///
float topaz_shape2d_get_anim_speed(
    /// The shape2d to query.
    topazComponent_t * shape2d
);

/// Sets the animation speed of the image. The time is returned in seconds.
/// The default is 1 / 60th of a second.
///
void topaz_shape2d_set_anim_speed(
    /// The shape2d to modify
    topazComponent_t * shape2d, 

    /// The new speed to set.
    float speed
);

/// Returns the center point of the shape2d from which local 
/// transforms are applied. The default is 0, 0
///
const topazVector_t * topaz_shape2d_get_center(
    /// The shape2d to query.
    topazComponent_t * shape2d
);

/// Sets the center point of the shape2d.
///
void topaz_shape2d_set_center(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The center position.
    const topazVector_t * center
);

/// Forms the object into a triangle. The Shape2D origin is the top-left of the
/// rectangle. Width and height are in pixels.
///
void topaz_shape2d_form_rectangle(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The width of the rectangle.
    float width, 

    /// The height of the rectangle.
    float height
);


/// Forms the object into the shape and visual of an image.
/// If the image has multiple frames, the image is drawn as an animation
/// accoring to topaz_shape2d_get_anim_speed
///
void topaz_shape2d_form_image(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The image asset to use as the source.
    topazAsset_t * asset
);

/// Same as topaz_shape2d_form_image, except a specific width/height 
/// can be given. If so, the image will be rescaled to match this width/height.
/// If the asset is omitted, the same asset is used as last call.
///
void topaz_shape2d_form_image_scaled(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The image asset to use as the source.
    topazAsset_t * asset, 

    /// The custom width of the shape2d.
    float width, 

    /// The custom height of the shape2d.
    float height
);


/// Forms the object into a specific frame of the given image asset.
/// The given frame will always be valid. If its beyond the number of frames 
/// in the image asset, the the frame index will be equal to frame modulo
/// the total frame count. If the asset is omitted, the same asset is used as last call.
///
void topaz_shape2d_form_image_frame(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The image asset to use as source.
    topazAsset_t * asset, 

    /// The frame of the image asset to use.
    uint32_t frame
);

/// Same as topaz_shape2d_form_image_frame, except with a forced width and 
/// height, in pixels.
void topaz_shape2d_form_image_frame_scaled(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The image asset to use as source.
    topazAsset_t * asset, 

    /// The frame of the image asset to use.
    uint32_t frame, 

    /// The custom width of the shape2d.
    float width, 

    /// The custom height of the shape2d.
    float height
);


/// Forms the object into an estimated circle in triangles. numIterations
/// refers to how many triangles to form it from. The higher this number, the 
/// more "smooth" the circle will appear. This also can be used to make 
/// simple geometric objects of equal-length sides. For example, using numIterations 
/// of 3 will yield an equalateral triangle.
/// 
void topaz_shape2d_form_radial(
    /// The component to modify.
    topazComponent_t * shape2d, 

    /// The radius of the radial object.
    float radius, 

    /// The number of sides to shape.
    uint32_t numIterations
);


/// Forms the object into a generic set of triangles. The given array 
/// should be populated with topazVector_t objects.
/// The points are interpreted as positions and are expected to be in groups of 3.
/// If a non-multiple-of-three number of points is given,
/// the remainder points are ignored.
///
/// In the script mapping of this, the array given is of plain numbers rather than vectors.
///
void topaz_shape2d_form_triangles(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The source vertices of the shape2d
    const topazArray_t * vertices
);

/// Forms a collection of line segments.
/// If an odd number of points are given, the last point is ignored.
/// The array should be the a collection of topazVector_t objects.
///
/// In the script mapping of this, the array given is of plain numbers rather than vectors.
///
void topaz_shape2d_form_lines(
    /// The shape2d to modify.
    topazComponent_t * shape2d, 

    /// The vertices for the lines.
    const topazArray_t * vertices
);


/// Sets the rendering attributes.
///
void topaz_shape2d_set_attribute(
    /// The shape2d to modify
    topazComponent_t * shape2d, 

    /// The attribute to modify
    topazRenderer_Attribute attribute,

    /// The new attribute value
    int value
);

/// Gets the rendering attributes.
///
int topaz_shape2d_get_attribute (
    /// The shape2d to query.
    topazComponent_t * shape2d,
    
    /// The attribute to query
    topazRenderer_Attribute attribute
);



#endif
