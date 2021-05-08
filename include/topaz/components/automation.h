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


#ifndef H_TOPAZDC__AUTOMATION__INCLUDED
#define H_TOPAZDC__AUTOMATION__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/component.h>
#include <topaz/vector.h>





/// Enumerator for the different available automation 
/// functions. Every automation can have functions associated with it
/// that describe how values change over time. These primitive 
/// functions can be string together to make virtually any 
/// value description.
typedef enum topazAutomation_Function topazAutomation_Function;
enum topazAutomation_Function {
    /// The value instantaneously becomes the end value.
    topazAutomation_Function_None,
    /// The value linearly becomes the end value.
    topazAutomation_Function_Linear,
    /// The value follows a bezier curve, where the value starts slow then accelerates to the end value.
    topazAutomation_Function_Accel,
    /// The value follows a bezier curve, where the value starts quickly then slows.
    topazAutomation_Function_Slow,
    /// The value follows a bezier curve, where the value starts slow then accelerates to the end value, but more mildly.
    topazAutomation_Function_SoftAccel,
    /// The value follows a bezier curve, where the value starts quickly then slows, but more mildly.
    topazAutomation_Function_SoftSlow,
    /// The value becomes any value between begin and end when it is re-sampled.
    topazAutomation_Function_Random
};





/// Creates a new automation instance.
/// Recognized events:
/// "on-anim-end" - when the automation is finished
topazComponent_t * topaz_automation_create(
    /// The topaz context to create the component under.
    topaz_t * context

);

/// Adds a keyframe to the automation.
/// Every automation holds an automation, an automation 
/// is a collection of sequential keyframes that describe 
/// how a value should change over time.
void topaz_automation_add_keyframe(
    /// The automation instance to add a keyframe to.
    topazComponent_t * automation, 

    /// The value for the keyframe. Once this keyframe is 
    /// reached, the automation will take on this value.
    float value, 

    /// The traveling function that describes how 
    /// the next keyframe's value should be reached. 
    topazAutomation_Function lerp, 

    /// The offset from the end of the last keyframe.
    /// This can be querried with topaz_automation_get_length().
    float automationOffset
);



/// Same as topaz_automation_add_keyframe, except the 
/// value is a vector. 
void topaz_automation_add_vector_keyframe(
    /// The automation instance to add a keyframe to.
    topazComponent_t * automation, 

    /// The value for the keyframe. Once this keyframe is 
    /// reached, the automation will take on this value.
    const topazVector_t * value, 


    /// The traveling function that describes how 
    /// the next keyframe's value should be reached. 
    topazAutomation_Function lerp, 

    /// The offset from the end of the last keyframe.
    /// This can be querried with topaz_automation_get_length().
    float automationOffset
);



/// Removes all keyframes from an automation.
void topaz_automation_clear(
    /// The automation to clear.
    topazComponent_t * automation
);


/// Adds all the keyframes from the given automation 
/// at the end of this automation.
void topaz_automation_add_automation(
    /// The automation to add to.
    topazComponent_t * automation,

    /// The other automation.
    topazComponent_t * other
);


/// The values for each keyframe are averaged and 
/// set back into this automation.
/// This may create additional keyframes to 
/// facilitate
void topaz_automation_blend(
    /// The automation to modify.
    topazComponent_t * automation,

    /// The automation to blend with.
    topazComponent_t * other
);


/// Smooths the keyframe values across the current 
/// average.
void topaz_automation_smooth(
    /// The automation to smooth.
    topazComponent_t * automation
);

/// Adds an automation to this automation, but 
/// the automation is in string form.
void topaz_automation_add_from_string(
    /// The automation to add to.
    topazComponent_t * automation,

    /// The automation string.
    const topazString_t * other
);




/// Gets the length of the automation. This is the total accumulative
/// keyframe offsets within the automation. The automation length 
/// is meant to convey how long the automation is in a time-agnostic way.
/// The length of the automation is not necessarily how long in seconds 
/// or frames the automation is.
float topaz_automation_get_length(
    /// The automation to query.
    topazComponent_t * automation
);

/// Skip to a part of the automation. The value must be between or 
/// equal to 0 and the value of topaz_automation_get_length()
/// The value is modulo the length of the automation. Note that 
/// this is in keyframe units.
void topaz_automation_skip_to(
    /// The automation to skip within.
    topazComponent_t * automation, 
    /// The value to skip to.
    float value
);




/// Sets the real duration of the automation.
/// This is how long it takes to get through the automation
/// in seconds. If an automation is currently running,
/// the automation is reset.
void topaz_automation_set_duration_seconds(
    /// The automation to modify.
    topazComponent_t * automation, 

    /// The actual real duration.
    float seconds
);

/// Sets the real duration of the automation.
/// This is how long it takes to get through the automation in 
/// frames. If an automation is currently running, the automation 
/// is reset.
void topaz_automation_set_duration_frames(
    /// The automation to query.
    topazComponent_t * automation, 
    /// the number of frames.
    int frames
);

/// Gets the duration of the automation in either frames or seconds.
float topaz_automation_get_duration(
    /// The automation to query.
    topazComponent_t * automation
);

/// Sets whether the automation is looped.
/// When its looped, the automation will start over 
/// when the animato is done
void topaz_automation_set_looped(
    /// The automation to modify.
    topazComponent_t * automation, 

    // Whether to loop the automation.
    int loop
);


/// Gets whether the automation is looped.
int topaz_automation_get_looped(
    /// The automation to query.
    topazComponent_t * automation
);

/// Sets the speed multiplier for the automation.
/// The default is 1.0.
void topaz_automation_set_speed(
    /// The automation to modify.
    topazComponent_t * automation, 
    /// The rate of the automation automation. 
    float speed
);

/// Gets the speed of the automation.
float topaz_automation_get_speed(
    /// The automation to query.
    topazComponent_t * automation
);

/// Pauses the automation.
void topaz_automation_pause(
    /// The automation to pause.
    topazComponent_t * automation
);


/// Pauses resumes the automation.
void topaz_automation_resume(
    /// The automation to resume.
    topazComponent_t * automation
);



/// Compiles the current automation's keyframes into a string 
///
const topazString_t * topaz_automation_to_string(
    /// The automation to query.
    topazComponent_t * automation
);

/// Sets the automation from a string. This removes all 
/// the existing keyframes and replaces them with the 
/// the keyframes from the compiled string.
void topaz_automation_set_from_string(
    /// The automation the modify
    topazComponent_t * automation,

    /// The automation string to set from. 
    const topazString_t * automationString
);





/// Returns the vector animated based on the keyframes 
/// and the given progress within the automation. Progress is 
/// 0 to 1 but looped.
topazVector_t topaz_automation_vector_at(
    /// The automation to query.
    topazComponent_t * automation, 

    /// The progress along the automation.
    float progress
);

/// Returns the float animated based on the keyframes 
/// and the given progress within the automation. Progress is 
/// 0 to 1 but looped.
float topaz_automation_at(
    /// The automation to query.
    topazComponent_t * automation, 

    /// The progress along the automation.
    float progress
);



/// Returns the current vector value within the automation 
///
const topazVector_t * topaz_automation_current_vector(
    /// The automation to query.
    topazComponent_t * automation
);

/// Returns the value within the automation.
float topaz_automation_current(
    /// The automation to query.
    topazComponent_t * automation
);





#endif







