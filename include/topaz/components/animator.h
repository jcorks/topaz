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


#ifndef H_TOPAZDC__ANIMATOR__INCLUDED
#define H_TOPAZDC__ANIMATOR__INCLUDED

#include <topaz/containers/string.h>









typedef enum topazAnimator_Function topazAnimator_Function;

enum topazAnimator_Function {
    topazAnimator_Function_None,
    topazAnimator_Function_Linear,
    topazAnimator_Function_Square,
    topazAnimator_Function_Log,
    topazAnimator_Function_Random
};




#include <topaz/component.h>
#include <topaz/vector.h>
/// Creates a new animator instance.
/// Recognized events:
/// "on-path-end" - when the path is finished
topazComponent_t * topaz_animator_create(
    /// The topaz context to create the component under.
    topaz_t * context

);

/// Adds a keyframe to the animator.
/// Every animator holds an animation, an animation 
/// is a collection of sequential keyframes that describe 
/// how a value should change over time.
void topaz_animator_add_keyframe(
    /// The animator instance to add a keyframe to.
    topazComponent_t * animator, 

    /// The value for the keyframe. Once this keyframe is 
    /// reached, the animator will take on this value.
    float value, 

    /// The traveling function that describes how 
    /// this keyframe's value should be reached. If this 
    /// keyframe is the first, this is ignored.
    topazAnimator_Function lerp, 

    /// The offset from the end of the last keyframe.
    /// This can be querried with topaz_animator_get_length().
    float animationOffset
);



/// Same as topaz_animator_add_keyframe, except the 
/// value is a vector. This is usual
void topaz_animator_add_vector_keyframe(
    /// The animator instance to add a keyframe to.
    topazComponent_t * animator, 

    /// The value for the keyframe. Once this keyframe is 
    /// reached, the animator will take on this value.
    const topazVector_t * value, 

    /// The traveling function that describes how 
    /// this keyframe's value should be reached. If this 
    /// keyframe is the first, this is ignored.
    topazAnimator_Function lerp, 

    /// The offset from the end of the last keyframe.
    /// This can be querried with topaz_animator_get_length().
    float animationOffset
);



/// Removes all keyframes from an animator.
void topaz_animator_clear(
    /// The animator to clear.
    topazComponent_t * animator
);


/// Adds all the keyframes from the given animation 
/// at the end of this animation.
void topaz_animator_add_animation(
    /// The animator to add to.
    topazComponent_t * animator,

    /// The other animator.
    topazComponent_t * other
);


/// The values for each keyframe are averaged and 
/// set back into this animator.
void topaz_animator_blend(
    /// The animator to modify.
    topazComponent_t * animator,

    /// The animator to blend with.
    topazComponent_t * other
);


/// Smooths the keyframe values across the current 
/// average.
void topaz_animator_smooth(
    /// The animator to smooth.
    topazComponent_t * animator
);

/// Adds an animation to this animation, but 
/// the animation is in string form.
void topaz_animator_add_from_string(
    /// The animator to add to.
    topazComponent_t * animator,

    /// The animation string.
    const topazString_t * other
);




/// Gets the length of the animation. This is the total accumulative
/// keyframe offsets within the animation. The animation length 
/// is meant to convey  
float topaz_animator_get_length(
    /// The animator to query.
    topazComponent_t * animator
);

/// Skip to a part of the animation. The value must be between or 
/// equal to 0 and the value of topaz_animator_get_length()
/// The value is modulated 
void topaz_animator_skip_to(
    /// The animator to skip within.
    topazComponent_t * animator, 
    /// The value to skip to.
    float value
);




/// Sets the real duration of the animation.
/// This is how long it takes to get through the animation
/// in seconds. If an animation is currently running,
/// the animation is reset.
void topaz_animator_set_duration_seconds(
    /// The animator to modify.
    topazComponent_t * animator, 

    /// The actual real duration.
    float seconds
);

/// Sets the real duration of the animation.
/// This is how long it takes to get through the animation in 
/// frames. If an animation is currently running, the animation 
/// is reset.
void topaz_animator_set_duration_frames(
    /// The animator to query.
    topazComponent_t * animator, 

);

/// Gets the duration of the animation in either frames or seconds.
float topaz_animator_get_duration(
    /// The animator to query.
    topazComponent_t * animator
)

/// Sets whether the animator is looped.
/// When its looped, the animator will start over 
/// when the animato is done
void topaz_animator_set_looped(
    /// The animator to modify.
    topazComponent_t * animator, 

    // Whether to loop the animator.
    int loop
);


/// Gets whether the animator is looped.
int topaz_animator_get_looped(
    /// The animator to query.
    topazComponent_t * animator
);

/// Sets the speed multiplier for the animation.
/// The default is 1.0.
void topaz_animator_set_speed(
    /// The animator to modify.
    topazComponent_t * animator, 
    /// The rate of the animator animation. 
    float speed
)

/// Gets the speed of the animation.
float topaz_animator_get_speed(
    /// The animator to query.
    topazComponent_t * animator
);

void topaz_animator_pause(
    topazComponent_t * animator
);

void topaz_animator_resume(
    topazComponent_t * animator
);



/// Compiles the current animator's keyframes into a string 
///
const topazString_t * topaz_animator_to_animation_string(
    /// The animator to query.
    topazComponent_t * animator
)

/// Sets the animation from a string. This removes all 
/// the existing keyframes and replaces them with the 
/// the keyframes from the compiled string.
void topaz_animator_set_from_animation_string(
    /// The animator the modify
    topazComponent_t * animator,

    /// The animator string to set from. 
    const topazString_t * animationString
);





/// Returns the vector animated based on the keyframes 
/// and the given progress within the animation. Progress is 
/// 0 to 1 but looped.
topazVector_t topaz_animator_vector_at(
    /// The animator to query.
    topazAnimator_t * animator, 

    /// The progress along the animation.
    float progress
);

/// Returns the float animated based on the keyframes 
/// and the given progress within the animation. Progress is 
/// 0 to 1 but looped.
float topaz_animator_at(
    /// The animator to query.
    topazAnimator_t * animator, 

    /// The progress along the animation.
    float progress
);



/// Returns the current vector value within the animation 
///
const topazVector_t * topaz_animator_current_vector(
    /// The animator to query.
    topazComponent_t * animator
);

/// Returns the value within the animation.
float topaz_animator_current(
    /// The animator to query.
    topazComponent_t * animator
);













