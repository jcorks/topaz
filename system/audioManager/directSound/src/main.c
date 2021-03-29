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
#include <assert.h>
#include <windows.h>
#include <dsound.h>
#include <stdio.h>
static intptr_t api_nothing(){return 0;}

typedef struct {
    LPDIRECTSOUND8 handle;
    topaz_t * ctx;
    topazAudioManager_t * audio;
    topaz_audio_manager_stream_callback fn;
    void * userCBData;
    int ready;
    HANDLE thread;
    int enabled;
    const float * buffer;
} TopazDSound;

static void * topaz_audio_manager_dsound__create (topazAudioManager_t * man, topaz_t * ctx) {
    TopazDSound * dsound = calloc(1, sizeof(TopazDSound));
    dsound->audio = man;
    dsound->ctx = ctx;
    dsound->enabled = 1;
    return dsound;
}

static DWORD topaz_dsound_main(void * a) {
    TopazDSound * dsound = a;
    HRESULT result;
    result = DirectSoundCreate8(
        NULL, // default device
        &(dsound->handle), 
        NULL
    );
    if (result != DS_OK) return NULL;
    
    WAVEFORMATEX format = {0};
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = 48000;
    format.nBlockAlign = 4;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    format.wBitsPerSample = 16;

    DSBUFFERDESC bdesc = {0};
    bdesc.dwSize = sizeof(DSBUFFERDESC);
    bdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    bdesc.dwBufferBytes = 0;

    // TODO: USE A REAL HWND THAT WE OWN PLEASE
    // Using the desktop window handle probably is why the primary buffer 
    // doesnt loop properly and why it stutters so much.
    result = IDirectSound8_SetCooperativeLevel(dsound->handle, GetConsoleWindow(), DSSCL_WRITEPRIMARY);
    if (result != DS_OK) return 0;


    // request to interface with the primary buffer. A secondary buffer 
    // isnt necessary since we aren't using effects or additional features 
    // (all effects / features are baked into the audio already)
    LPDIRECTSOUNDBUFFER buffer;
    result = IDirectSound8_CreateSoundBuffer(dsound->handle, &bdesc, &buffer, NULL);
    if (result != DS_OK) return 0;

    result = IDirectSoundBuffer_SetFormat(buffer, &format);
    if (result != DS_OK) return 0;

    DSBCAPS caps = {0};
    caps.dwSize = sizeof(caps);
    IDirectSoundBuffer_GetCaps(buffer, &caps);

    // once the primary buffer stream starts, it will not stop.
    IDirectSoundBuffer_Play(
        buffer,
        0,
        0,
        DSBPLAY_LOOPING
    );

    // at this point, we are ready to start receiving mixer data.
    dsound->ready = 1;

    // dynamically use a buffer size based on what windows uses. 256 is the maximum since 
    // we want to maintain low latency.
    uint32_t SAMPLES = caps.dwBufferBytes/(sizeof(int16_t)*2) / 4;
    if (SAMPLES > 256) SAMPLES = 256;

    float * bufferPre = malloc(sizeof(float)*2*SAMPLES);
    int16_t * bufferReal = malloc(sizeof(int16_t)*2*SAMPLES);
    DWORD pos = 0;
    LPVOID writePtr1;
    LPVOID writePtr2;
    DWORD writePtr1SizeBytes;
    DWORD writePtr2SizeBytes;
    uint8_t * readPtr;
    uint32_t readOffset;
    while(1) {
        DWORD needsMoreData = 0;

        // Check if the buffer is close to being starved.
        // If it is, continue, else wait around.
        while(!needsMoreData) {
            DWORD play;
            DWORD write = pos;
            IDirectSoundBuffer_GetCurrentPosition(buffer, &play, NULL);
            if (play > write) {
                write += caps.dwBufferBytes;
            }

            needsMoreData = write - play < SAMPLES*2*sizeof(int16_t)*4;
            //printf("%d %d\n", write, play);
            //fflush(stdout);
            if (!needsMoreData) Sleep(1);
        }

        // the buffer needs additional data.
        // request to write to the primary buffer.
        result = IDirectSoundBuffer_Lock(
            buffer,
            pos,
            SAMPLES*2*sizeof(int16_t),
            &writePtr1, &writePtr1SizeBytes,
            &writePtr2, &writePtr2SizeBytes,
            0
        );

        if (result == DSERR_BUFFERLOST) {
            IDirectSoundBuffer_Restore(buffer);
            result = IDirectSoundBuffer_Lock(
                buffer,
                pos,
                SAMPLES*2*sizeof(int16_t),
                &writePtr1, &writePtr1SizeBytes,
                &writePtr2, &writePtr2SizeBytes,
                0
            );
        }
        uint32_t i = 0;
        if (result == DS_OK) {

            // get samples based on what windows allows us to write.
            uint32_t frameSamples = (writePtr1SizeBytes + writePtr2SizeBytes) / (sizeof(int16_t)*2);

            // get mix data
            dsound->fn(
                dsound->audio,
                //TOPAZDSOUNDBUFFERSAMPLES,
                frameSamples,
                bufferPre,
                dsound->userCBData
            );

            // choose whether to deliver empty or real samples.
            if (dsound->enabled) {
                for(i = 0; i < frameSamples*2; ++i) {
                    bufferReal[i] = bufferPre[i]*(0xffff/2);
                }
            } else {
                for(i = 0; i < frameSamples*2; ++i) {
                    bufferReal[i] = 0;
                }                
            }


            // push into either the entirety of the single buffer, or both the first and second.
            readPtr = &bufferReal[0];
            pos += writePtr1SizeBytes;
            memcpy(
                writePtr1,
                readPtr,
                writePtr1SizeBytes
            );

            // should come into here if the buffer request would loop to the start of the ring.
            // in practice this never seemed to happen, sadly. DirectSound too old?
            if (writePtr2 != NULL) {
                memcpy(
                    writePtr2,
                    readPtr+writePtr2SizeBytes,
                    writePtr2SizeBytes
                );                
                pos = writePtr2SizeBytes;
            }
            IDirectSoundBuffer_Unlock(
                buffer,
                writePtr1, writePtr1SizeBytes,
                writePtr2, writePtr2SizeBytes
            );
        } else {
            // for some reason the buffer never loops properly, so we manually reset it. 
            // if this happens when its not supposed to, we could get some unfortunate sounds.
            pos = 0;
        }

    } 
    
    return 0;
}


static int topaz_audio_manager_dsound__connect(
    topazAudioManager_t * man, 
    void * a, 
    topaz_audio_manager_stream_callback audioStreamHandler, 
    void * userData
) {
    TopazDSound * dsound = a;
    if (!dsound->thread) {
        dsound->fn = audioStreamHandler;
        dsound->userCBData = userData;
    
        DWORD id;
        dsound->thread = CreateThread(
            NULL,
            0,
            topaz_dsound_main,
            dsound,
            0,
            &id
        );
    }
    return dsound->ready;
}


//void      (*audio_manager_set_sample_rate)(topazAudioManager_t *, void *, uint32_t);
static uint32_t topaz_audio_manager_dsound__get_sample_rate(
    topazAudioManager_t * man, 
    void * a
) {
    return 48000;
}
//int       (*audio_manager_is_underrun)    (topazAudioManager_t *, void *);
static void topaz_audio_manager_dsound__enable_output(
    topazAudioManager_t * man, 
    void * a, 
    int doIt
) {
    TopazDSound * dsound = a;
    dsound->enabled = doIt;
}


float topaz_audio_manager_dsound__get_current_output_sample(
    topazAudioManager_t * man, 
    void * a
) {
    TopazDSound * dsound = a;
    return dsound->buffer ? *dsound->buffer : 0;
}






void topaz_system_audioManager_directSound__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazAudioManagerAPI_t * api
) {
    topaz_system_backend_bind(
        backend, 
        // name
        TOPAZ_STR_CAST("DirectSound"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2021"),

        // desc 
        TOPAZ_STR_CAST("Backend bridge to deliver audio to DirectSound. This should not be used; WASAPI is more modern and has fewer issues."),




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


    api->audio_manager_create          = topaz_audio_manager_dsound__create;
    api->audio_manager_destroy         = (void (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_connect         = topaz_audio_manager_dsound__connect;
    api->audio_manager_set_sample_rate = (void (*)(topazAudioManager_t *, void *, uint32_t)) api_nothing;
    api->audio_manager_get_sample_rate = topaz_audio_manager_dsound__get_sample_rate;
    api->audio_manager_is_underrun     = (int (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_enable_output   = topaz_audio_manager_dsound__enable_output;
    api->audio_manager_set_volume_multiplier = (void (*)(topazAudioManager_t *, void *, float)) api_nothing;
    api->audio_manager_get_volume_multiplier = (float (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_get_current_output_sample = topaz_audio_manager_dsound__get_current_output_sample;

}







