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
#include <initguid.h>
#define COBJMACROS

#include <audioclient.h>
#include <audiopolicy.h>
#include <windows.h>
#include <stdio.h>
#include <mmdeviceapi.h>

#define AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM      0x80000000
#define AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY 0x08000000


#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }






static intptr_t api_nothing(){return 0;}

typedef struct {
    topaz_t * ctx;
    topazAudioManager_t * audio;
    topaz_audio_manager_stream_callback fn;
    void * userCBData;
    int ready;
    HANDLE thread;
    int enabled;
    const float * buffer;
} TopazWASAPI;

static void * topaz_audio_manager_wasapi__create (topazAudioManager_t * man, topaz_t * ctx) {
    TopazWASAPI * wasapi = calloc(1, sizeof(TopazWASAPI));
    wasapi->audio = man;
    wasapi->ctx = ctx;
    wasapi->enabled = 1;
    return wasapi;
}


static DWORD topaz_wasapi_main(void * a) {
    TopazWASAPI * wasapi = a;
    HRESULT result;
    IMMDeviceEnumerator * deviceEnum;
    IMMDevice * device;
    IAudioClient * client;
    IAudioRenderClient * renderer;

    // com object initialization
    CoInitialize(NULL);

    // We cant use __uuidof since this is (primarily) compiled in MINGW/gcc.
    result = CoCreateInstance(
        &CLSID_MMDeviceEnumerator, NULL,
        CLSCTX_ALL,               &IID_IMMDeviceEnumerator,
        (void**)&deviceEnum    
    );
    if (FAILED(result)) return 0;

    // We just will use the default device. Maybe in the future we will be able to select devices.
    result = IMMDeviceEnumerator_GetDefaultAudioEndpoint(
        deviceEnum,
        eRender,
        eConsole,
        &device
    );
    if (FAILED(result)) return 0;


    // notify that we want to use this device.
    result = IMMDevice_Activate(
        device,
        &IID_IAudioClient, CLSCTX_ALL,
        NULL,
        (void**)&client
    );
    if (FAILED(result)) return 0;



    // next we need to specify the format of data we will be supplying to the 
    // device mixer. In most cases, the defaults (GetMixFormat()) for the 
    // device will be useable as is. We will just want to make some minor 
    // assurances so topaz's data will be useable.
    WAVEFORMATEXTENSIBLE format;
    WAVEFORMATEX * mixerFormat;
    result = IAudioClient_GetMixFormat(client, &mixerFormat);
    if (FAILED(result)) return 0;


    // The built in mixer mode is able to have additional information
    // which we care about. Namely, whether the samples are float or ints.
    int formatFloat = 0;

    // WAVEFORMATEX -> WAVEFORMATEXTENSIBLE
    if (mixerFormat->cbSize >= 22) {
        memcpy(&format, mixerFormat, sizeof(WAVEFORMATEXTENSIBLE));
        if (((WAVEFORMATEXTENSIBLE*)mixerFormat)->SubFormat.Data1 == 3) {
            formatFloat = 1;
        }
    } else {
        memcpy(&(format.Format), mixerFormat, sizeof(WAVEFORMATEX));
    }

    // first, lets modify the mix format to better suit our needs.
    // Specifically we want to guarantee that its:
    // 2 channels
    // 48000 Hz sample rate
    format.Format.nSamplesPerSec = 48000;
    format.Format.nChannels = 2;


    // initialize the audio client and hope our modifications weren't too weird.
    if (FAILED(IAudioClient_Initialize(
        client,
        AUDCLNT_SHAREMODE_SHARED,
        0,
        1000000, // 1 second i think
        AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM|
        AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
        &format.Format,
        NULL      
    ))) {
        // the mixer format is SUPER fussy, so instead of telling windows 
        // to adhere to ours, we'll adhere to whatever is on the system by default.
        // this MAY cause things to go nuts. So this should be cleaned up.
        // Namely theres no additional check yet for the system modifying the 
        // sample rate, which will be audible.
        if (mixerFormat->cbSize >= 22) {
            memcpy(&format, mixerFormat, sizeof(WAVEFORMATEXTENSIBLE));
        } else {
            memcpy(&(format.Format), mixerFormat, sizeof(WAVEFORMATEX));
        }
        result = IAudioClient_Initialize(
            client,
            AUDCLNT_SHAREMODE_SHARED,
            0,
            1000000, // 1 second i think
            AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM|
            AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
            mixerFormat,
            NULL      
        );

    }
    if (FAILED(result)) return 0;


    // Get the size of the main mixing buffer. This shouldnt change!
    uint32_t bufferFrames;
    result = IAudioClient_GetBufferSize(
        client,
        &bufferFrames
    );
    if (FAILED(result)) return 0;


    // Gets the client renderer, which holds the mix buffer.
    result = IAudioClient_GetService(
        client,
        &IID_IAudioRenderClient,
        (void**)&renderer
    );if (FAILED(result)) return 0;
    
    // raw buffer of topaz' data. Used to hold before
    // converting to a format that windows can understand.
    float * bufferPre = malloc(sizeof(float)*2*bufferFrames);


    // initial silence fill!
    BYTE * renderBuffer;
    result = IAudioRenderClient_GetBuffer(
        renderer,
        bufferFrames,
        &renderBuffer 
    ); if (FAILED(result)) return 0;
    result = IAudioRenderClient_ReleaseBuffer(
        renderer,
        bufferFrames,
        AUDCLNT_BUFFERFLAGS_SILENT // <- ignores the actual data and auto-zeroes
    ); if (FAILED(result)) return 0;


    // The client is now officially going to deliver audio to the device.
    result = IAudioClient_Start(client);
    if (FAILED(result)) return 0;
    // which means we can accept topaz's samples
    wasapi->ready = 1;

    float bufferLenSecs = bufferFrames / (float)format.Format.nSamplesPerSec;
    uint32_t paddingFrames;
    while(1) {
        // gets the padding, basically how many sampels are writable to the buffer
        result = IAudioClient_GetCurrentPadding(client, &paddingFrames);
        if (FAILED(result)) return 0;
        uint32_t frameSamples = (bufferFrames - paddingFrames)/2;

        // if we can write some, lets do so
        if (frameSamples) {

            result = IAudioRenderClient_GetBuffer(
                renderer,
                frameSamples,
                &renderBuffer 
            ); if (FAILED(result)) return 0;

            wasapi->fn(
                wasapi->audio,
                frameSamples,
                bufferPre,
                wasapi->userCBData
            );

            // convert from topaz's format to whatever the mix buffer expects.
            if (format.Format.nChannels == 2) {
                uint32_t i;
                switch(format.Format.wBitsPerSample) {
                case 16: {
                    int16_t * bufferReal = (int16_t *)renderBuffer;
                    for(i = 0; i < frameSamples*2; ++i) {
                        bufferReal[i] = bufferPre[i]*(0xffff/2);
                    }
                    break;
                }
                case 32: {
                    if (formatFloat) {
                        float * bufferReal = (float *)renderBuffer;
                        for(i = 0; i < frameSamples*2; ++i) {
                            bufferReal[i] = bufferPre[i];
                        }

                    } else {
                        int32_t * bufferReal = (int32_t *)renderBuffer;
                        for(i = 0; i < frameSamples*2; ++i) {
                            bufferReal[i] = bufferPre[i]*(0xffffffff/2);
                        }
                    }
                    break;
                }
                case 64: {
                    if (formatFloat) {
                        double * bufferReal = (double *)renderBuffer;
                        for(i = 0; i < frameSamples*2; ++i) {
                            bufferReal[i] = bufferPre[i];
                        }
                    } else {
                        int64_t * bufferReal = (int64_t *)renderBuffer;
                        for(i = 0; i < frameSamples*2; ++i) {
                            bufferReal[i] = bufferPre[i]*(0xffffffffffffffff/2);
                        }
                    }
                    break;
                }
                case 8: {
                    int8_t * bufferReal = (int8_t *)renderBuffer;
                    for(i = 0; i < frameSamples*2; ++i) {
                        bufferReal[i] = bufferPre[i]*(0xff/2);
                    }
                    break;
                }
                default:;
                    assert(!"UNSUPPORTED BUFFER BIT CONFIG");
                }
            } else {
                assert(!"UNSUPPORTED CHANNEL COUNT");
            }
            //printf("WROTE %d SAMPLES\n", frameSamples);
            //fflush(stdout);

            result = IAudioRenderClient_ReleaseBuffer(
                renderer,
                frameSamples,
                wasapi->enabled ? 0 : AUDCLNT_BUFFERFLAGS_SILENT
            ); if (FAILED(result)) return 0;
        }

        // TODO: delay will likely be large right now... still testing!
        Sleep((bufferLenSecs / 10.0) * 1000.0);
    }



    return 0;
}


static int topaz_audio_manager_wasapi__connect(
    topazAudioManager_t * man, 
    void * a, 
    topaz_audio_manager_stream_callback audioStreamHandler, 
    void * userData
) {
    TopazWASAPI * wasapi = a;
    if (!wasapi->thread) {
        wasapi->fn = audioStreamHandler;
        wasapi->userCBData = userData;
    
        DWORD id;
        wasapi->thread = CreateThread(
            NULL,
            0,
            topaz_wasapi_main,
            wasapi,
            0,
            &id
        );
    }
    return wasapi->ready;
}


//void      (*audio_manager_set_sample_rate)(topazAudioManager_t *, void *, uint32_t);
static uint32_t topaz_audio_manager_wasapi__get_sample_rate(
    topazAudioManager_t * man, 
    void * a
) {
    return 48000;
}
//int       (*audio_manager_is_underrun)    (topazAudioManager_t *, void *);
static void topaz_audio_manager_wasapi__enable_output(
    topazAudioManager_t * man, 
    void * a, 
    int doIt
) {
    TopazWASAPI * wasapi = a;
    wasapi->enabled = doIt;
}


float topaz_audio_manager_wasapi__get_current_output_sample(
    topazAudioManager_t * man, 
    void * a
) {
    TopazWASAPI * wasapi = a;
    return wasapi->buffer ? *wasapi->buffer : 0;
}






void topaz_system_audioManager_wasapi__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazAudioManagerAPI_t * api
) {
    topaz_system_backend_bind(
        backend, 
        // name
        TOPAZ_STR_CAST("WASAPI"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2021"),

        // desc 
        TOPAZ_STR_CAST("Backend bridge to deliver audio to WASAPI"),




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


    api->audio_manager_create          = topaz_audio_manager_wasapi__create;
    api->audio_manager_destroy         = (void (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_connect         = topaz_audio_manager_wasapi__connect;
    api->audio_manager_set_sample_rate = (void (*)(topazAudioManager_t *, void *, uint32_t)) api_nothing;
    api->audio_manager_get_sample_rate = topaz_audio_manager_wasapi__get_sample_rate;
    api->audio_manager_is_underrun     = (int (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_enable_output   = topaz_audio_manager_wasapi__enable_output;
    api->audio_manager_set_volume_multiplier = (void (*)(topazAudioManager_t *, void *, float)) api_nothing;
    api->audio_manager_get_volume_multiplier = (float (*)(topazAudioManager_t *, void *)) api_nothing;
    api->audio_manager_get_current_output_sample = topaz_audio_manager_wasapi__get_current_output_sample;

}







