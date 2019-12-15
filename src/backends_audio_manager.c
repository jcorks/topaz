#include <topaz/compat.h>
#include <topaz/backends/audio_manager.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazAudioManager_t {
    topazAudioManagerAPI_t api;
    topazBackend_t * backend;
};





topazAudioManager_t * topaz_audio_manager_create(topazBackend_t * b, topazAudioManagerAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.audio_manager_create);
        assert(api.audio_manager_destroy);
        assert(api.audio_manager_connect);
        assert(api.audio_manager_set_sample_rate);
        assert(api.audio_manager_get_sample_rate);
        assert(api.audio_manager_is_underrun);
        assert(api.audio_manager_enable_output);
        assert(api.audio_manager_set_volume_multiplier);
        assert(api.audio_manager_get_volume_multiplier);
        assert(api.audio_manager_get_current_output_sample);
    #endif
    topazAudioManager_t * out = calloc(1, sizeof(topazAudioManager_t));
    out->api = api;
    out->backend = b;
    out->api.audio_manager_create(&out->api);
    return out;
}


void topaz_audio_manager_destroy(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif
    t->api.audio_manager_destroy(&t->api);
}





topazBackend_t * topaz_audio_manager_get_backend(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazAudioManagerAPI_t topaz_audio_manager_get_api(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api;
}





int topaz_audio_manager_connect(topazAudioManager_t * t,
    void (*audioStreamHandler)(topazAudioManager_t *, uint32_t, float *, void *), 
    void * data

) {
    #ifdef TOPAZDC_DEBUG
        assert(audioStreamHandler && "Audio stream handler cannot be NULL.");
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    return t->api.audio_manager_connect(&t->api, audioStreamHandler, data);
}

void topaz_audio_manager_set_sample_rate(topazAudioManager_t * t, uint32_t a) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    t->api.audio_manager_set_sample_rate(&t->api, a);
}

uint32_t topaz_audio_manager_get_sample_rate(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    return t->api.audio_manager_get_sample_rate(&t->api);
}




int topaz_audio_manager_is_underrun(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    return t->api.audio_manager_is_underrun(&t->api);
}

void topaz_audio_manager_enable_output(topazAudioManager_t * t, int a) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    t->api.audio_manager_enable_output(&t->api, a);
}


void topaz_audio_manager_set_volume_multiplier(topazAudioManager_t * t, float a) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    t->api.audio_manager_set_volume_multiplier(&t->api, a);
}


float topaz_audio_manager_get_volume_multiplier(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif
    return t->api.audio_manager_get_volume_multiplier(&t->api);
}

float topaz_audio_manager_get_current_output_sample(topazAudioManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazAudioManager_t pointer cannot be NULL.");
    #endif

    return t->api.audio_manager_get_current_output_sample(&t->api);
}





