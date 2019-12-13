#include <topaz/compat.h>
#include <topaz/backends/time.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazTime_t {
    topazTimeAPI_t api;
    topazBackend_t * backend;
};





topazTime_t * topaz_time_create(topazBackend_t * b, topazTimeAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.time_create);
        assert(api.time_destroy);
        assert(api.time_sleep_ms);
        assert(api.time_ms_since_startup);

    #endif
    topazTime_t * out = calloc(1, sizeof(topazTime_t));
    out->api = api;
    out->backend = b;
    out->api.time_create(&out->api);
    return out;
}


void topaz_time_destroy(topazTime_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif
    t->api.time_destroy(&t->api);
}





topazBackend_t * topaz_time_get_backend(topazTime_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazTimeAPI_t topaz_time_get_api(topazTime_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api;
}




void topaz_time_sleep_ms(topazTime_t * t, uint64_t ms) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    t->api.time_sleep_ms(&t->api, ms);
}


uint64_t topaz_time_ms_since_startup(topazTime_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api.time_ms_since_startup(&t->api);
}


