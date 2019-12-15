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


#ifndef H_TOPAZDC__AUDIO_MANAGER_API__INCLUDED
#define H_TOPAZDC__AUDIO_MANAGER_API__INCLUDED

#include <stdint.h>


typedef struct topazAudioManager_t topazAudioManager_t;

/*

    AudioManagerAPI
    -----
    
    The set of functions that define how the time abstraction should 
    behave. Creating filsystem abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/time.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazAudioManagerAPI_t topazAudioManagerAPI_t;


/// Each function is an implementation-facing copy of 
/// the user-side API for topazAudioManager_t. See <topaz/backends/AudioManager.h>
///
struct topazAudioManagerAPI_t {
    void      (*audio_manager_create)           (topazAudioManagerAPI_t *);
    void      (*audio_manager_destroy)          (topazAudioManagerAPI_t *);


    int       (*audio_manager_connect)        (topazAudioManagerAPI_t *, void (*audioStreamHandler)(topazAudioManager_t *, uint32_t, float *, void *), void *);
    void      (*audio_manager_set_sample_rate)(topazAudioManagerAPI_t *, uint32_t);
    uint32_t  (*audio_manager_get_sample_rate)(topazAudioManagerAPI_t *);
    int       (*audio_manager_is_underrun)    (topazAudioManagerAPI_t *);
    void      (*audio_manager_enable_output)  (topazAudioManagerAPI_t *, int);
    void      (*audio_manager_set_volume_multiplier)(topazAudioManagerAPI_t *, float);
    float     (*audio_manager_get_volume_multiplier)(topazAudioManagerAPI_t *);
    float     (*audio_manager_get_current_output_sample)(topazAudioManagerAPI_t *);

    /// User-given data. This is expected to data needed to persist
    /// throughout the lifetime of the AudioManager
    ///
    void * implementationData;


};



#endif
