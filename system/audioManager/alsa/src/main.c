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




#include "backend.h"
#include <topaz/version.h>
#include <topaz/system.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <pthread.h>

static intptr_t api_nothing(){return 0;}

typedef struct {
    snd_pcm_t * handle;
    topaz_t * ctx;
    topazAudioManager_t * audio;
    topaz_audio_manager_stream_callback fn;
    void * userCBData;
    int ready;
    pthread_t thread;
    int enabled;
    const float * buffer;
} TopazALSA;

static void * topaz_audio_manager_alsa__create (topazAudioManager_t * man, topaz_t * ctx) {
    TopazALSA * alsa = calloc(1, sizeof(TopazALSA));
    alsa->audio = man;
    alsa->ctx = ctx;
    alsa->enabled = 1;
    return alsa;
}

#define TOPAZALSABUFFERSAMPLES 256

static void * topaz_alsa_main(void * a) {
    TopazALSA * alsa = a;
    if (!alsa->handle) {
        int result = snd_pcm_open(
            &(alsa->handle), 
            "default",
            SND_PCM_STREAM_PLAYBACK,
            0
        );
        if (result < 0) return NULL;
    }
    
    
    if (snd_pcm_set_params(
        alsa->handle,
        SND_PCM_FORMAT_FLOAT_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        2,
        48000,
        1,
        20000
    ) < 0) {
        return NULL;
    }
    
    
    
    alsa->ready = 1;
    
    // only push when the processor
    snd_pcm_nonblock(alsa->handle, 0);


    float buffer[TOPAZALSABUFFERSAMPLES*2];
    float bufferEmpty[TOPAZALSABUFFERSAMPLES*2] = {0};
    alsa->buffer = &buffer[0];
    while(1) {
        alsa->fn(
            alsa->audio,
            TOPAZALSABUFFERSAMPLES,
            buffer,
            alsa->userCBData
        );
        
        if (alsa->enabled) {
            snd_pcm_writei(
                alsa->handle,
                buffer,
                TOPAZALSABUFFERSAMPLES
            );
        } else {
            snd_pcm_writei(
                alsa->handle,
                bufferEmpty,
                TOPAZALSABUFFERSAMPLES
            );        
        }
    }    
    
    return NULL;
}


static int topaz_audio_manager_alsa__connect(
    topazAudioManager_t * man, 
    void * a, 
    topaz_audio_manager_stream_callback audioStreamHandler, 
    void * userData
) {
    return 1;
    TopazALSA * alsa = a;
    if (!alsa->thread) {
        alsa->fn = audioStreamHandler;
        alsa->userCBData = userData;
    
        pthread_create(
            &(alsa->thread),
            NULL,
            topaz_alsa_main,
            alsa
        );
    }
    return alsa->ready;
}


//void      (*audio_manager_set_sample_rate)(topazAudioManager_t *, void *, uint32_t);
static uint32_t topaz_audio_manager_alsa__get_sample_rate(
    topazAudioManager_t * man, 
    void * a
) {
    return 48000;
}
//int       (*audio_manager_is_underrun)    (topazAudioManager_t *, void *);
static void topaz_audio_manager_alsa__enable_output(
    topazAudioManager_t * man, 
    void * a, 
    int doIt
) {
    TopazALSA * alsa = a;
    alsa->enabled = doIt;
}


float topaz_audio_manager_alsa__get_current_output_sample(
    topazAudioManager_t * man, 
    void * a
) {
    TopazALSA * alsa = a;
    return alsa->buffer ? *alsa->buffer : 0;
}






void topaz_system_audioManager_alsa__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazAudioManagerAPI_t * api
) {
    topaz_system_backend_bind(
        backend, 
        // name
        TOPAZ_STR_CAST("ALSA"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2021"),

        // desc 
        TOPAZ_STR_CAST("Backend bridge to deliver audio to ALSA"),




        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );


    api->audio_manager_create          = topaz_audio_manager_alsa__create;
    api->audio_manager_destroy         = (void (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_connect         = topaz_audio_manager_alsa__connect;
    api->audio_manager_set_sample_rate = (void (*)(topazAudioManager_t *, void *, uint32_t)) api_nothing;
    api->audio_manager_get_sample_rate = topaz_audio_manager_alsa__get_sample_rate;
    api->audio_manager_is_underrun     = (int (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_enable_output   = topaz_audio_manager_alsa__enable_output;
    api->audio_manager_set_volume_multiplier = (void (*)(topazAudioManager_t *, void *, float)) api_nothing;
    api->audio_manager_get_volume_multiplier = (float (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_get_current_output_sample = topaz_audio_manager_alsa__get_current_output_sample;

}







