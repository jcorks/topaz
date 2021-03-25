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


#ifndef H_TOPAZDC__AUDIO_MANAGER__INCLUDED
#define H_TOPAZDC__AUDIO_MANAGER__INCLUDED

#include <topaz/backends/api/audio_manager_api.h>
#include <topaz/system.h>



/// Handles the nitty-gritty of audio.
/// The AudioManager must continuously process any pushed data.
/// Under most implementations, a threaded approach would be most appropriate.
typedef struct topazAudioManager_t topazAudioManager_t;



/// Creates a new audio manager object. 
///
topazAudioManager_t * topaz_audio_manager_create(
    /// The topaz context.
    topaz_t * context, 

    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazAudioManagerAPI_t api
);


/// Destroys and cleans up an audio manager object
///
void topaz_audio_manager_destroy(
    /// The audio manager to destroy.
    topazAudioManager_t * audioMan
);





/// Retrieves the backend for this audio manager object.
///
topazSystem_Backend_t * topaz_audio_manager_get_backend(
    /// The audio manager to query.
    topazAudioManager_t * audioMan
);


/// Returns the API for this audio manager.
///
topazAudioManagerAPI_t topaz_audio_manager_get_api(
    /// The audio manager to query.
    topazAudioManager_t * audioMan
);






/// Callback that delivers process data. The format is in
/// 2 channel, interleaved form, meaning that the samples should be organized
/// in an interleaved channel format. In most device setups, the 
/// odd samples are for the left speaker, and the
/// even samples are for the right speaker
/// each sample is a floating point number
/// and expected to be normalized between -1 and 1.
///
typedef void (*topaz_audio_manager_stream_callback)(
    /// The audio manager to process data.
    topazAudioManager_t * audioMan, 

    /// The number of floating point values
    uint32_t sampleCount, 

    /// The raw, normalized audio data.
    /// This buffer should be written to.
    float * audioSamples,

    /// The data bound with with callback 
    void * callbackData
);


/// Makes the connection to hardware to attempt to
/// bring the backend to a usable state. Returns whether or 
/// not the connection was successfully made. Else,
/// it is recommended to try again.
/// NOTE: the callback may be run on another thread.
int topaz_audio_manager_connect(
    /// The audio manager to modify.
    topazAudioManager_t * audioMan,

    /// The callback to deliver the audiostream to the backend.
    topaz_audio_manager_stream_callback callback, 

    /// user data.
    void * callbackData
);

/// Sets the number of samples second.
/// The default is 44100.
///
void topaz_audio_manager_set_sample_rate(
    /// The audio manager to modify.
    topazAudioManager_t * audioMan, 

    /// The sample rate.
    uint32_t sampleRate
);

/// returns the current sample rate in kHz
///
uint32_t topaz_audio_manager_get_sample_rate(
    /// The audio manager to query.
    topazAudioManager_t * audioMan
);




/// Returns whether or not the AudioManager is in an underrun state
///
int topaz_audio_manager_is_underrun(
    /// The audio manager to query.
    topazAudioManager_t * audioMan
);

/// Enables or disables output
///
void topaz_audio_manager_enable_output(
    /// The audio manager to modify.
    topazAudioManager_t * audioMan, 

    /// Whether to enable.
    int enable
);


/// Sets the volume multipler. This value
/// is clamped to 0 and 1 and represents a scale
/// applied to all samples before interpretation.
/// the default is .5;
void topaz_audio_manager_set_volume_multiplier(
    /// The audio manager to modify
    topazAudioManager_t * audioMan, 

    /// The volume amount.
    float amt
);

/// Returns the current column multiplier.
float topaz_audio_manager_get_volume_multiplier(
    /// The audio manager to query.
    topazAudioManager_t * audioMan
);

/// gets an approximate output sample at the time of calling
/// from one of the channels.
float topaz_audio_manager_get_current_output_sample(
    /// The audio manager to query.
    topazAudioManager_t * audioMan
);








#endif

