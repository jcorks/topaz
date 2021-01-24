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
#include <topaz/vector.h>

#ifndef H_TOPAZDC__MATH__INCLUDED
#define H_TOPAZDC__MATH__INCLUDED

/// Collection of math-related utilities.
typedef struct topazMath_t topazMath_t;

/// Standard PI value used by topaz.
///
#define topaz_math_pi 3.14159265358979323846

/// Clamping bounds the first argument, val, to be within min and max.
/// If val is less than min, it is set to min. If it is greater than max,
/// va; is set to max.
#define topaz_math_clamp(__VAL__,__MIN__,__MAX__) (__VAL__ = ((__VAL__ < __MIN__ ? __MIN__ : __VAL__) > __MAX__ ? __MAX__ : __VAL__))

/// Snapping binds a value, i, to the nearest multiple of a value,
/// subdiv.
#define topaz_math_snap(__VAL__,__SUBDIV__) (round((__VAL__) / __SUBDIV__) * __SUBDIV__)

/// Converts input radians to degrees.
///
#define topaz_math_to_degrees(__VAL__) ((__VAL__) * topaz_math_pi / 180.f)

/// Converts input degrees to radians
///
#define topaz_math_to_radians(__VAL__) ((__VAL__) * 180.f / topaz_math_pi)

/// Returns whether the given topazVector_t, pos, is within a box formed 
/// by the topazVector_t boxorigin as the topleft and w/h as the width/height
///
#define topaz_math_is_point_within_bounds(__POS__,__ORIGIN__,__W__,__H__) \
    (__POS__.x >= __ORIGIN__.x && __POS__.x <= __ORIGIN__.x + __W__ &&\
     __POS__.y >= __ORIGIN__.y && __POS__.y <= __ORIGIN__.y + __H__)


/// Returns within the distance between the topazVector_t p1 and p2
/// is less than range
///
#define topaz_math_is_point_within_range(__P1__,__P2__,__RANGE__) \
    (return topaz_vector_get_distance(&__P1__, &__P2__) < __RANGE__)

/// Returns linear interpolation of val to end with a ratio step.
///
#define topaz_math_lerp(__VAL__,__END__,__STEP__) (__VAL__+(__END__ - __VAL__)*__STEP__)

/// Returns a simple linear interpolation
///
#define topaz_math_lerp_simple(__VAL__,__END__) (topaz_math_lerp(__VAL__, __END__, .5f))

/// Returns a vector position, to emulate parallax given a maximum regression depth
/// 
topazVector_t topaz_math_parallaxize(
    /// The source point position in world space (2D) + z 
    /// coordiate.
    const topazVector_t * pos, 

    /// Camera position in world space.
    const topazVector_t * camera,

    /// View width in pixels.
    float screenWidth, 

    /// View height in pixels.
    float screenHeight, 

    /// Maximum depth.
    float screenMaxDepth
);





#endif
