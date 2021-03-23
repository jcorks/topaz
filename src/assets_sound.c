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

#include <topaz/assets/sound.h>
#include <topaz/compat.h>
#include <topaz/containers/array.h>
#include <topaz/topaz.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all assets
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__SOUND = "t0p4zs0und";
#endif



typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif

    topazArray_t * data;
    int inUse;
} TopazSoundData;


static void data__destroy(topazAsset_t * a, void * userData) {
    TopazSoundData * d = userData;
    topaz_array_destroy(d->data);
    free(d);
}

static TopazSoundData * data__retrieve(const topazAsset_t * a) {
    TopazSoundData * ret = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(ret->MAGIC_ID == MAGIC_ID__SOUND);
    #endif
    return ret;
}



topazAsset_t * topaz_sound_create(topaz_t * t, const topazString_t * name) {

    TopazSoundData * img = calloc(1, sizeof(TopazSoundData));
    #ifdef TOPAZDC_DEBUG
        img->MAGIC_ID = MAGIC_ID__SOUND;
    #endif
    img->data = topaz_array_create(sizeof(topazSound_Sample_t));


    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = data__destroy;
    attribs.userData = img;

    return topaz_asset_create(
        t,
        topazAsset_Type_Data,
        name, 
        &attribs
    );
}


topazAsset_t * topaz_sound_create_empty(topaz_t * t) {
    return topaz_sound_create(
        t,
        TOPAZ_STR_CAST("")
    );
}



void topaz_sound_set_samples(topazAsset_t * a, const topazSound_Sample_t * s, uint32_t sCount) {
    TopazSoundData * d = data__retrieve(a);
    if (d->inUse) return;
    topaz_array_set_size(d->data, sCount);
    memcpy(
        topaz_array_get_data(d->data),
        s,
        sCount*sizeof(topazSound_Sample_t)
    );
}




const topazArray_t * topaz_sound_get_samples(topazAsset_t * a) {
    TopazSoundData * d = data__retrieve(a);
    return d->data;
}

uint32_t topaz_sound_get_sample_count(topazAsset_t * a) {
    TopazSoundData * d = data__retrieve(a);
    return topaz_array_get_size(d->data);
}


void topaz_sound_tag_in_use(
    /// The topaz sound asset.
    topazAsset_t * a
) {
    TopazSoundData * d = data__retrieve(a);
    d->inUse++;
}

/// Untags the asset's usage status.
void topaz_sound_untag_in_use(
    /// The topaz sound asset.
    topazAsset_t * a
) {
    TopazSoundData * d = data__retrieve(a);
    if (d->inUse > 0)
        d->inUse--;
}

int topaz_sound_is_in_use(
    /// The topaz sound asset.
    topazAsset_t * a
) {
    TopazSoundData * d = data__retrieve(a);
    return d->inUse != 0;
}






