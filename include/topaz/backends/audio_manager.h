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



/*

    AudioManager
    -----
    handles the nitty-gritty of audio.
    The AudioManager must continuously process any pushed data.
    Under most implementations, a threaded approach would be most appropriate.

*/

typedef struct topazAudioManager_t topazAudioManager_t;



/// Creates a new audio manager object. 
///
topazAudioManager_t * topaz_audio_manager_create(topaz_t *, topazSystem_Backend_t *, topazAudioManagerAPI_t);


/// Destroys and cleans up an audio manager object
///
void topaz_audio_manager_destroy(topazAudioManager_t *);





/// Retrieves the backend for this audio manager object.
///
topazSystem_Backend_t * topaz_audio_manager_get_backend(topazAudioManager_t *);


/// Returns the API for this audio manager.
///
topazAudioManagerAPI_t topaz_audio_manager_get_api(topazAudioManager_t *);


// Makes the connection to hardware to attempt to
// bring the backend to a usable state. Returns whether or 
// not the connection was successfully made. Else,
// it is recommended to try again.
int topaz_audio_manager_connect(topazAudioManager_t *,

    // Callback that delivers process data. The format is in
    // 2 channel, interleaved form, meaning that the samples should be organized
    // in an interleaved channel format. In most device setups, the 
    // odd samples are for the left speaker, and the
    // even samples are for the right speaker
    // each sample is a floating point number
    // and expected to be normalized between -1 and 1.
    //
    // THe first argument is the number of floating point values,
    // and the second argument is the raw normalized data in 
    // interleaved format.
    //
    // Last argument is user data.
    void (*audioStreamHandler)(topazAudioManager_t *, uint32_t, float *, void  *), 

    // user data.
    void *

);

/// Sets the number of samples second.
/// The default is 44100.
///
void topaz_audio_manager_set_sample_rate(topazAudioManager_t *, uint32_t);

/// returns the current sample rate in kHz
///
uint32_t topaz_audio_manager_get_sample_rate(topazAudioManager_t *);




/// Returns whether or not the AudioManager is in an underrun state
///
int topaz_audio_manager_is_underrun(topazAudioManager_t *);

/// Enables or disables output
///
void topaz_audio_manager_enable_output(topazAudioManager_t *, int);


/// Set and Get the volume multipler. This value
/// is clamped to 0 and 1 and represents a scale
/// applied to all samples before interpretation.
/// the default is .5;
void topaz_audio_manager_set_volume_multiplier(topazAudioManager_t *, float);
float topaz_audio_manager_get_volume_multiplier(topazAudioManager_t *);

/// gets an approximate output sample at the time of calling
/// from one of the channels.
float topaz_audio_manager_get_current_output_sample(topazAudioManager_t *);








#endif

