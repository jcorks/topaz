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

#include <topaz/components/automation.h>
#include <topaz/topaz.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
static char * MAGIC_ID__AUTOMATION = "t0p4zan1m4t0r";
#endif


typedef struct {
    // value at this keyframe 
    topazVector_t value;

    // The function to reach this 
    topazAutomation_Function func;

    // the offset in the automation when this keyframe starts
    float when;
} TAKeyframe;

typedef struct {
    // all keyframes
    topazArray_t * keyframes;

    // index of current keyframe in automation
    // the automation is always between current keyframe and next
    uint32_t currentKeyframe;

    // progress along the automation.
    // in keyframe units
    float progress;

    // amount of frames or seconds before the automation is over 
    // Used to scale the
    float limitReal;

    // whether the limit is in seconds or frames. 1 if seconds;
    int limitSeconds;

    // whether to loop. still will emit the end automation signal
    int looped;

    // whether paused
    int paused;

    // for time keeping
    uint64_t lastFrame;

    // speed multiplier!
    float speed;

    // current value
    topazVector_t currentValue;

    topazString_t * animStr;

    topaz_t * ctx;
    #ifdef TOPAZDC_DEBUG
        char * MAGIC_ID;
    #endif
} TopazAutomation;
#include <stdio.h>

static float bezier(float t, float from, float to, float mid) {
    return (1 - t)*(1 - t)*from + 2*(1-t) * t * mid + t*t*to;
}

static topazVector_t anim_interpolate(float at, topazAutomation_Function func, topazVector_t from, topazVector_t to) {
    if (at <= 0.0) return from;
    if (at >= 1.0) return to;
    topazVector_t out;
    float f;
    switch(func) {
      case topazAutomation_Function_None:
        return to;

      case topazAutomation_Function_Linear:
        f = 1-at;
        out.x = at*to.x + f*from.x;
        out.y = at*to.y + f*from.y;
        out.z = at*to.z + f*from.z;
        return out;

      case topazAutomation_Function_Slow:
        out.x = bezier(at, from.x, to.x, to.x);
        out.y = bezier(at, from.y, to.y, to.y);
        out.z = bezier(at, from.z, to.z, to.z);
        return out;

      case topazAutomation_Function_Accel:
        out.x = bezier(at, from.x, to.x, from.x);
        out.y = bezier(at, from.y, to.y, from.y);
        out.z = bezier(at, from.z, to.z, from.z);
        return out;

      case topazAutomation_Function_SoftSlow:
        out.x = bezier(at, from.x, to.x, from.x + (to.x - from.x)*0.70);
        out.y = bezier(at, from.y, to.y, from.y + (to.y - from.y)*0.70);
        out.z = bezier(at, from.z, to.z, from.z + (to.z - from.z)*0.70);
        return out;

      case topazAutomation_Function_SoftAccel:
        out.x = bezier(at, from.x, to.x, from.x + (to.x - from.x)*0.3);
        out.y = bezier(at, from.y, to.y, from.y + (to.y - from.y)*0.3);
        out.z = bezier(at, from.z, to.z, from.z + (to.z - from.z)*0.3);
        return out;


      case topazAutomation_Function_Random:
        out.x = (rand() / (1.0+rand())) * (to.x - from.x) + from.x;
        out.y = (rand() / (1.0+rand())) * (to.y - from.y) + from.y;
        out.z = (rand() / (1.0+rand())) * (to.z - from.z) + from.z;
        return out;



      default:
        return to;
    }
}

// gets the value within the automation at the given automation time
static topazVector_t animate_at(float at, TAKeyframe * frames, uint32_t len) {
    if (!len) {
        topazVector_t out = {};
        return out;
    }
    if (at < frames->when) return frames->value;
    uint32_t i;
    for(i = 0; i < len-1; ++i, frames++) {
        if (at >= frames->when &&
            at <= frames[1].when) {
            return anim_interpolate(
                (at - frames->when) / (frames[1].when - frames->when),
                frames[1].func, 
                frames->value,
                frames[1].value
            );
        }
    }
    return frames[len-1].value;
}

static TopazAutomation * automation__retrieve(topazComponent_t * c) {
    TopazAutomation * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "TopazAutomation instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__AUTOMATION);
    #endif
    return s;
}


static void automation__on_destroy(topazComponent_t * automation, TopazAutomation * a) {
    topaz_array_destroy(a->keyframes);
    topaz_string_destroy(a->animStr);
    free(a);
}

static void automation__on_step(topazComponent_t * automation, TopazAutomation * a) {
    if (a->paused) {
        if (a->limitSeconds) {
            a->lastFrame = topaz_context_get_time(a->ctx);
        }
        return;
    }


    // no keyframes, nothing to animate
    if (!topaz_array_get_size(a->keyframes)) {
        a->currentValue.x = 0;
        a->currentValue.y = 0;
        a->currentValue.z = 0;
        if (a->limitSeconds) {
            a->lastFrame = topaz_context_get_time(a->ctx);
        }
        return;
    }

    float length = topaz_automation_get_length(automation);
    // if not looped, simply stop
    if (!a->looped) {
        if (a->progress >= length) {
            a->currentValue = topaz_array_at(a->keyframes, TAKeyframe, topaz_array_get_size(a->keyframes)-1).value;
            if (a->limitSeconds) {
                a->lastFrame = topaz_context_get_time(a->ctx);
            }
            return;
        }
    }


    // no
    float keyframeOffset;
    if (a->limitSeconds) {
        // seconds of progress
        float timeDelta = (topaz_context_get_time(a->ctx) - a->lastFrame) / 1000.0;
        keyframeOffset = (timeDelta / a->limitReal) * length;
    } else {
        keyframeOffset = (1.0 / a->limitReal)  * length;
    }
    keyframeOffset *= a->speed;
    // handle overstepping

    // apply offset
    int needsEmit = 0;
    if (a->progress + keyframeOffset > length) {
        needsEmit = 1;
        if (a->looped) {
            a->progress = fmod(a->progress + keyframeOffset, length);
            a->currentKeyframe = 0;
        } else
            a->progress = length;
    } else {
        a->progress += keyframeOffset;
    }


    // find new keyframe and value corresponding to offset.
    uint32_t len = topaz_array_get_size(a->keyframes);
    TAKeyframe * frames = topaz_array_get_data(a->keyframes);
    while(a->currentKeyframe < len-1 && a->progress >= frames[a->currentKeyframe+1].when) {
        a->currentKeyframe++;        
    }

    frames = frames+a->currentKeyframe;
    a->currentValue = anim_interpolate(
        (a->progress - frames->when) / (frames[1].when - frames->when),
        frames[1].func, 
        frames->value,
        frames[1].value
    );
    if (a->limitSeconds) {
        a->lastFrame = topaz_context_get_time(a->ctx);
    }

    // now it is safe to notify.
    if (needsEmit)
        topaz_component_emit_event(automation, TOPAZ_STR_CAST("on-anim-end"), topaz_component_get_host(automation), NULL);


}


topazComponent_t * topaz_automation_create(
    /// The topaz context to create the component under.
    topaz_t * context

) {
    TopazAutomation * data = calloc(1, sizeof(TopazAutomation));
    data->ctx = context;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__AUTOMATION;
    #endif

    data->keyframes = topaz_array_create(sizeof(TAKeyframe));
    data->currentKeyframe = 0;
    data->animStr = topaz_string_create();
    data->speed = 1.0;
    data->lastFrame = topaz_context_get_time(context);
    
    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_step     = (topaz_component_attribute_callback) automation__on_step;
    attribs.on_destroy  = (topaz_component_attribute_callback) automation__on_destroy;

    attribs.userData = data;
    topazComponent_t * out = topaz_component_create_with_attributes(TOPAZ_STR_CAST("Automation"), context, &attribs);
    topaz_component_install_event(out, TOPAZ_STR_CAST("on-anim-end"), NULL, NULL);
    return out;

}



void topaz_automation_add_keyframe(
    topazComponent_t * automation, 
    float value, 
    topazAutomation_Function lerp, 
    float automationOffset
) {
    TopazAutomation * a = automation__retrieve(automation);
    TAKeyframe frame;
    frame.value.x = value;
    frame.value.y = 0;
    frame.value.z = 0;
    frame.func = lerp;
    uint32_t kSize = topaz_array_get_size(a->keyframes);
    if (kSize) {
        frame.when = topaz_array_at(a->keyframes, TAKeyframe, kSize-1).when + automationOffset;
        // for consistency when mixing single / vector contexts
        frame.value.y = topaz_array_at(a->keyframes, TAKeyframe, kSize-1).value.y;
        frame.value.z = topaz_array_at(a->keyframes, TAKeyframe, kSize-1).value.z;
    } else {
        frame.when = automationOffset;
    }

    topaz_array_push(a->keyframes, frame);
}

void topaz_automation_add_vector_keyframe(
    topazComponent_t * automation, 
    const topazVector_t * value, 
    topazAutomation_Function lerp, 
    float automationOffset
) {
    TopazAutomation * a = automation__retrieve(automation);
    TAKeyframe frame;
    frame.value = *value;
    frame.func = lerp;
    uint32_t kSize = topaz_array_get_size(a->keyframes);
    if (kSize) {
        frame.when = topaz_array_at(a->keyframes, TAKeyframe, kSize-1).when + automationOffset;
    } else {
        frame.when = automationOffset;
    }

    topaz_array_push(a->keyframes, frame);
}

void topaz_automation_clear(
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);
    topaz_array_set_size(a->keyframes, 0);
}

void topaz_automation_add_automation(
    topazComponent_t * automation,
    topazComponent_t * other
) {
    TopazAutomation * a = automation__retrieve(automation);
    TopazAutomation * aOther = automation__retrieve(other);

    uint32_t i;
    uint32_t len = topaz_array_get_size(aOther->keyframes);
    for(i = 0; i < len; ++i) {
        topaz_array_push(
            a->keyframes,
            topaz_array_at(
                aOther->keyframes,
                TAKeyframe,
                i
            )
        );
    }
}

void topaz_automation_blend(
    topazComponent_t * automation,
    topazComponent_t * other
) {
    TopazAutomation * a = automation__retrieve(automation);
    TopazAutomation * aOther = automation__retrieve(other);


    uint32_t i = 0, n = 0;
    uint32_t aLen = topaz_array_get_size(a->keyframes);
    uint32_t otherLen = topaz_array_get_size(aOther->keyframes);

    TAKeyframe * otherFrames = topaz_array_get_data(aOther->keyframes);
    TAKeyframe * thisFrames = malloc(sizeof(TAKeyframe)*aLen);
    memcpy(thisFrames, topaz_array_get_data(a->keyframes), sizeof(TAKeyframe)*aLen);
    topaz_array_set_size(a->keyframes, 0);

    float thisStart  = thisFrames[0].when;
    float thisEnd    = thisFrames[aLen-1].when;
    float otherStart = otherFrames[0].when;
    float otherEnd   = otherFrames[otherLen-1].when;



    TAKeyframe next;

    while(i < aLen && 
          n < otherLen) {

        if (otherFrames[n].when < thisFrames[i].when) {
            // other precedent
            next = otherFrames[n];
            if (next.when >= thisStart &&
                next.when <= thisEnd) {
                topazVector_t v = animate_at(next.when, thisFrames, aLen);
                next.value.x = (next.value.x + v.x)/2;
                next.value.y = (next.value.y + v.y)/2;
                next.value.z = (next.value.z + v.z)/2;
            }
            topaz_array_push(a->keyframes, next);
            n++;
        } else {
            // this precedent
            next = thisFrames[i];
            if (next.when >= otherStart &&
                next.when <= otherEnd) {
                topazVector_t v = animate_at(next.when, otherFrames, otherLen);
                next.value.x = (next.value.x + v.x)/2;
                next.value.y = (next.value.y + v.y)/2;
                next.value.z = (next.value.z + v.z)/2;
            }
            topaz_array_push(a->keyframes, next);
            i++;
        }
    }

    free(thisFrames);
}


void topaz_automation_smooth(
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);
    uint32_t len = topaz_array_get_size(a->keyframes);
    TAKeyframe * frames = topaz_array_get_data(a->keyframes);

    uint32_t i;
    topazVector_t avg = {};
    for(i = 0; i < len; ++i) {
        avg.x += frames[i].value.x;
        avg.y += frames[i].value.y;
        avg.z += frames[i].value.z;
    }
    avg.x /= len;
    avg.y /= len;
    avg.z /= len;

    for(i = 0; i < len; ++i) {
        frames[i].value.x = (frames[i].value.x + avg.x)/2;
        frames[i].value.y = (frames[i].value.y + avg.y)/2;
        frames[i].value.z = (frames[i].value.z + avg.z)/2;
    }
}

void topaz_automation_add_from_string(
    topazComponent_t * automation,
    const topazString_t * other
) {
    TopazAutomation * a = automation__retrieve(automation);
    topazComponent_t * temp = topaz_automation_create(a->ctx);
    topaz_automation_set_from_string(temp, other);
    topaz_automation_add_automation(automation, temp);
    topaz_component_destroy(temp);
}


float topaz_automation_get_length(
    /// The automation to query.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);
    uint32_t len = topaz_array_get_size(a->keyframes);
    if (!len) return 0;
    return topaz_array_at(a->keyframes, TAKeyframe, topaz_array_get_size(a->keyframes)-1).when;
}

void topaz_automation_skip_to(
    /// The automation to skip within.
    topazComponent_t * automation, 
    /// The value to skip to.
    float value
) {
    TopazAutomation * a = automation__retrieve(automation);
    float length = topaz_automation_get_length(automation);
    uint32_t len = topaz_array_get_size(a->keyframes);
    if (!len) return;
    TAKeyframe * frames = topaz_array_get_data(a->keyframes);
    value = fmod(value, length);
    a->lastFrame = topaz_context_get_time(a->ctx);

    uint32_t i;
    for(i = 0; i < len-1; ++i, frames++) {
        if (value >= frames->when) {
            a->currentKeyframe = i;
            a->progress = value;
            a->currentValue = topaz_automation_vector_at(automation, value);
            return;
        }
    }

}

void topaz_automation_set_duration_seconds(
    /// The automation to modify.
    topazComponent_t * automation, 

    /// The actual real duration.
    float seconds
) {
    TopazAutomation * a = automation__retrieve(automation);
    a->limitReal = seconds;
    a->limitSeconds = 1;
}
void topaz_automation_set_duration_frames(
    /// The automation to query.
    topazComponent_t * automation, 
    /// the number of frames
    int frames
) {
    TopazAutomation * a = automation__retrieve(automation);
    a->limitReal = frames;
    a->limitSeconds = 0;
}

float topaz_automation_get_duration(
    /// The automation to query.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);
    return a->limitReal;
}

void topaz_automation_set_looped(
    /// The automation to modify.
    topazComponent_t * automation, 

    // Whether to loop the automation.
    int loop
) {
    TopazAutomation * a = automation__retrieve(automation);
    a->looped = loop;
}

int topaz_automation_get_looped(
    /// The automation to query.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);
    return a->looped;
}


void topaz_automation_set_speed(
    /// The automation to modify.
    topazComponent_t * automation, 
    /// The rate of the automation automation. 
    float speed
) {
    TopazAutomation * a = automation__retrieve(automation);
    a->speed = speed;

}

float topaz_automation_get_speed(
    /// The automation to query.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);   
    return a->speed;
}


void topaz_automation_pause(
    /// The automation to pause.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);   
    a->paused = 1;
}


void topaz_automation_resume(
    /// The automation to resume.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);   
    a->paused = 1;
}

topazVector_t topaz_automation_vector_at(
    /// The automation to query.
    topazComponent_t * automation, 

    /// The progress along the automation.
    float progress
) {
    TopazAutomation * a = automation__retrieve(automation);   
    return animate_at(
        fmod(progress, 1)*topaz_automation_get_length(automation),
        topaz_array_get_data(a->keyframes),
        topaz_array_get_size(a->keyframes)
    );
}

float topaz_automation_at(
    /// The automation to query.
    topazComponent_t * automation, 

    /// The progress along the automation.
    float progress
) {
    TopazAutomation * a = automation__retrieve(automation);   
    return animate_at(
        fmod(progress, 1)*topaz_automation_get_length(automation),
        topaz_array_get_data(a->keyframes),
        topaz_array_get_size(a->keyframes)
    ).x;
}


const topazVector_t * topaz_automation_current_vector(
    /// The automation to query.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);   
    return &a->currentValue;
}

float topaz_automation_current(
    /// The automation to query.
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);   
    return a->currentValue.x;
}



const topazString_t * topaz_automation_to_string(
    topazComponent_t * automation
) {
    TopazAutomation * a = automation__retrieve(automation);
    topaz_string_clear(a->animStr);

    uint32_t len = topaz_array_get_size(a->keyframes);
    TAKeyframe * frames = topaz_array_get_data(a->keyframes);
    uint32_t i;
    for(i = 0; i < len; ++i) {
        topaz_string_concat_printf(
            a->animStr, 
            "%f,%f,%f,%d,%f,",
            frames[i].value.x,
            frames[i].value.y,
            frames[i].value.z,
            frames[i].func,
            frames[i].when
        );
    }
    return a->animStr;
}



void topaz_automation_set_from_string(
    topazComponent_t * automation,
    const topazString_t * automationString
) {
    TopazAutomation * a = automation__retrieve(automation);
    topaz_string_clear(a->animStr);
    topaz_array_set_size(a->keyframes, 0);

    topazString_t * src = topaz_string_clone(automationString);
    topaz_string_chain_start(src, TOPAZ_STR_CAST(","));
    while(!topaz_string_chain_is_end(src)) {
        TAKeyframe keyframe;
        const topazString_t * iter = topaz_string_chain_current(src);
        keyframe.value.x = atof(topaz_string_get_c_str(iter)); topaz_string_chain_proceed(src);
        keyframe.value.y = atof(topaz_string_get_c_str(iter)); topaz_string_chain_proceed(src);
        keyframe.value.z = atof(topaz_string_get_c_str(iter)); topaz_string_chain_proceed(src);
        keyframe.func    = atoi(topaz_string_get_c_str(iter)); topaz_string_chain_proceed(src);
        keyframe.when    = atof(topaz_string_get_c_str(iter)); topaz_string_chain_proceed(src);

        topaz_array_push(a->keyframes, keyframe);
    }
    topaz_string_destroy(src);
}




