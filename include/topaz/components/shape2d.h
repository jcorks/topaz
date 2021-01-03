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
/*

    Shape2D
    -----

    Can express basic 2D objects, such as images and shapes.

*/



/// Creates a new shape2d object
/// Use component_destroy to release.
///
topazComponent_t * topaz_shape2d_create(topaz_t *);

/// Gets the color of the object.
/// The default color is #000000 (black)
///
topazColor_t topaz_shape2d_get_color(topazComponent_t *);

/// Sets the color of the object.
///
void topaz_shape2d_set_color(topazComponent_t *, topazColor_t);

/// Gets the trasform for the object
///
topazTransform_t * topaz_shape2d_get_node(topazComponent_t *);

/// Gets the animation speed of the image. This is how quickly 
/// the object will flip through image frames. The time is returned in seconds.
///
float topaz_shape2d_get_anim_speed(topazComponent_t *);

/// Sets the animation speed of the image. The time is returned in seconds.
/// The default is 1 / 60th of a second.
///
void topaz_shape2d_set_anim_speed(topazComponent_t *, float);

/// Returns the center point of the shape2d from which local 
/// transforms are applied. The default is 0, 0
///
const topazVector_t * topaz_shape2d_get_center(topazComponent_t *);

/// Sets the center point of the shape2d.
///
void topaz_shape2d_set_center(topazComponent_t *, const topazVector_t *);

/// Forms the object into a triangle. The Shape2D origin is the top-left of the
/// rectangle. Width and height are in pixels.
///
void topaz_shape2d_form_rectangle(topazComponent_t *, float width, float height);


/// Forms the object into the shape and visual of an image.
/// If the image has multiple frames, the image is drawn as an animation
/// accoring to topaz_shape2d_get_anim_speed
///
void topaz_shape2d_form_image(topazComponent_t *, topazAsset_t *);

/// Same as topaz_shape2d_form_image, except a specific width/height 
/// can be given. If so, the image will be rescaled to match this width/height.
///
void topaz_shape2d_form_image_scaled(topazComponent_t *, topazAsset_t *, float width, float height);


/// Forms the object into a specific frame of the given image asset.
/// The given frame will always be valid. If its beyond the number of frames 
/// in the image asset, the the frame index will be equal to frame modulo
/// the total frame count.
///
void topaz_shape2d_form_image_frame(topazComponent_t *, topazAsset_t *, uint32_t frame);

/// Same as topaz_shape2d_form_image_frame, except with a forced width and 
/// height, in pixels.
void topaz_shape2d_form_image_frame_scaled(topazComponent_t *, topazAsset_t *, uint32_t frame, float forcedWidth, float forcedHeight);


/// Forms the object into an estimated circle in triangles. numIterations
/// refers to how many triangles to form it from. The higher this number, the 
/// more "smooth" the circle will appear. This also can be used to make 
/// simple geometric objects of equal-length sides. For example, using numIterations 
/// of 3 will yield an equalateral triangle.
/// 
void topaz_shape2d_form_radial(topazComponent_t *, float radius, uint32_t numIterations);


/// Forms the object into a generic set of triangles. The given array 
/// should be populated with topazVector_t objects.
/// The points are interpreted as positions and are expected to be in groups of 3.
/// If a non-multiple-of-three number of points is given,
/// the remainder points are ignored.
///
void topaz_shape2d_form_triangles(topazComponent_t *, const topazArray_t *);

/// Forms a collection of line segments.
/// If an odd number of points are given, the last point is ignored.
/// The array should be the a collection of topazVector_t objects.
///
void topaz_shape2d_form_lines(topazComponent_t *, const topazArray_t *);


/// Sets the parameter value. The value accepted is one of the appropriate 
/// enum values within renderer.h
///
void topaz_shape2d_set_parameter(topazComponent_t *, topazRender2D_Parameter, int);

/// Gets the parameter value.
///
int topaz_shape2d_get_parameter(topazComponent_t *, topazRender2D_Parameter);



#endif
