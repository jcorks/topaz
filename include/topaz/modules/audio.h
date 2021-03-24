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


#ifndef H_TOPAZDC__AUDIO__INCLUDED
#define H_TOPAZDC__AUDIO__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
typedef struct topaz_t topaz_t;
typedef struct topazAudioManager_t topazAudioManager_t;


/// Module that handles all audio related functionality.
typedef struct topazAudio_t topazAudio_t;




/// Allows for interaction with a playing sound.
///
/// An Active instance represents a soundclip of audio that is currently
/// undergoing playback. As such, you can use it to modify how the
/// corresponding audio is playing. Because typical playback systems are done on
/// separate threads, the reflection of changes caused by the calling of
/// these functions is not immediately guaranteed.
typedef struct topazAudio_Active_t topazAudio_Active_t;




/// Creates a new auto instance. Normally not necessary; the topaz 
/// context has an audio instance of its own.
topazAudio_t * topaz_audio_create(
    /// The context to use.
    topaz_t * context
);

/// Destroys and frees a topaz audio instance.
///
void topaz_audio_destroy(
    /// The audio instance to destroy.
    topazAudio_t * audio
);


/// Updates the audio engine. This is normally done for you 
///
void topaz_audio_update(
    /// The audio instance to update.
    topazAudio_t * audio
);




/// Queues sound for immediate playback
void topaz_audio_play_sound(
    /// The audio module to play.
    topazAudio_t * audio, 
    /// The sound to play.
    topazAsset_t * asset,
    /// The channel to send the audio block to play on.
    uint8_t channel,
    /// The volume the sound should play at.
    float volume,
    /// The panning for the sound. 0.f is all the way to the left, 1.f is all the way to the right.
    float panning
);


/// Queues sound for immediate playback, but returns 
/// an object that can be used to interact with the 
/// active clip.
topazAudio_Active_t topaz_audio_play_sound_interactive(
    /// The audio module to play.
    topazAudio_t * audio, 
    /// The sound to play.
    topazAsset_t * asset,
    /// The channel to send the audio block to play on.
    uint8_t channel,
    /// The volume the sound should play at.
    float volume,
    /// The panning for the sound. 0.f is all the way to the left, 1.f is all the way to the right.
    float panning
);



/// Removes all effects from a channel.
void topaz_audio_channel_reset(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel,
);

/// Keeps the stream active for the given channel.
/// By default, channels will only be active if there are samples playing through it 
/// This is normally fine, execpt this means effects will stop once no samples are 
/// being processed through a channel. For something like reverberation, this 
/// would cut off meaningful information from being output. Keeping the channel 
/// awake sacrifices performance for making more quality.
void topaz_audio_channel_keep_awake(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel,
);


/// Sets the volume for the given channel
/// 0.f denotes minimum volume and 1.f maximum.
/// The values are clipped if they are beyond these bounds.
///
void topaz_audio_channel_set_volume(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel,
    /// The amount, from 0 to 1. Will be clamped.
    float amt
);


/// Sets the panning for the entire channel.
/// 0.f denotes all the way to the left and 1.f all the way to the right.
/// The values are clipped if they are beyond these bounds.
///
void topaz_audio_channel_set_volume(
    /// The audio module to modify.
    topazAudio_t * audio,
    /// The channel to to modify.
    uint8_t channel,
    /// The amount, from 0 to 1. Will be clamped.
    float amt
);


/// Gets the audio manager instance for this audio module.
topazAudioManager_t * topaz_audio_get_manager(
    /// The audio module to query.
    topazAudio_t * audio
);

   
   
   
   
   
   
/// Signals to the audio module that this active 
/// sound is no longer going to be used by user code.
/// This must be called when the active sound is no longer 
/// needed. After this call, the
void topaz_audio_active_destroy(
    topazAudio_Active_t * aSound
);
   
/// Sets the volume of the ActiveSound.
void topaz_audio_active_set_volume(
    /// The active sound to modify.
    topazAudio_Active_t * aSound, 
    /// The new volume to use. The value is clamped to the range 0.f and 1.f.
    float v
);

/// Sets the panning.
///
void topaz_audio_active_set_panning(
    /// The active sound to modify.
    topazAudio_Active_t * aSound, 
    /// The new panning. The value is clamped to the range 0.f and 1.f.
    float v
);

/// Sets whether the sound should be replayed once it finishes.
///
/// The default is not to repeat.
void topaz_audio_active_set_repeat(
    /// The active sound to modify.
    topazAudio_Active_t * aSound, 
    /// If true, the sound will repeat. If false, the sound will not repeat.
    int b
);

/// Sets the progress in the sound to play from.
///
/// f is interpreted as a fraction of the progress of the sound,
/// where 0 is the beginning of the sound and 1 is the end of the sound.
/// The value is clamped to this range.
void topaz_audio_active_seek(
    /// The active sound to modify.
    topazAudio_Active_t * aSound, 
    /// The place in the sound to continue playback from.
    float f
);

///  Halts the sound.
///
void topaz_audio_active_stop(
    /// The active sound to modify.
    topazAudio_Active_t * aSound
);

/// Pauses the sound if it was playing.
///
void topaz_audio_active_pause(
    /// The active sound to modify.
    topazAudio_Active_t * aSound
);

/// Resumes the sound's playback if it was Pause()ed.
///
void topaz_audio_active_resume(
    /// The active sound to modify.
    topazAudio_Active_t * aSound
);












#endif
