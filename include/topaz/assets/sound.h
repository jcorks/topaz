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


#ifndef H_TOPAZDC__SOUND__INCLUDED
#define H_TOPAZDC__SOUND__INCLUDED

#include <topaz/asset.h>
typedef struct topazArray_t topazArray_t;
typedef struct topazSound_Sample_t topazSound_Sample_t;


/// Asset to hold sound data.
typedef struct topazSound_t topazSound_t;


/// A single sample of audio. All audio data is expressed in 
/// samples. Each sample consists of 2 pieces of data: one for the 
/// left channel and one for the right channel for stereo output.
struct topazSound_Sample_t {
    /// The left part of the sample.
    uint16_t leftSample;
    /// The right part of the sample.
    uint16_t rightSample;
};

/// Creates a new sound asset object
/// Use asset_destroy to release.
///
topazAsset_t * topaz_sound_create_empty(
    /// The topaz context.
    topaz_t * context
);



/// Creates a sound asset with the given name.
topazAsset_t * topaz_sound_create(
    /// The topaz context.
    topaz_t * context,

    /// Name of the asset.    
    const topazString_t * name
);


/// Sets the raw samples for the asset. 
/// This replaces all current samples.
/// If the sound is "in use", then this function does nothing.
void topaz_sound_set_samples(
    /// The topaz sound asset.
    topazAsset_t * sound, 
    /// An array of samples.
    const topazSound_Sample_t * samples, 
    /// The number of samples to upload.
    uint32_t sampleCount
);

/// Returns a read-only array of raw sample data 
/// within the asset. The array is of topazSound_Sample_t
///
const topazArray_t * topaz_sound_get_samples(
    /// The topaz sound asset.
    topazAsset_t * sound
);


/// Gets the number of samples within this audio asset.
uint32_t topaz_sound_get_sample_count(
    /// The topaz sound asset.
    topazAsset_t * sound
);

/// Tags the asset as currently in use. Accumulates like a counter.
void topaz_sound_tag_in_use(
    /// The topaz sound asset.
    topazAsset_t * sound
);

/// Untags the asset's usage status.
void topaz_sound_untag_in_use(
    /// The topaz sound asset.
    topazAsset_t * sound
);

/// Returns wheter the sound is in use.
int topaz_sound_is_in_use(
    /// The topaz sound asset.
    topazAsset_t * sound
);



#endif
