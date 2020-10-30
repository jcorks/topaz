#include <topaz/compat.h>
#include <topaz/backends/console_display.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <topaz/containers/bin.h>
#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazConsoleDisplay_t {
    topazConsoleDisplayAPI_t api;
    topazBackend_t * backend;
    topazBin_t * cbs;
    topazArray_t * lines;
    void * userData;
};


typedef struct {
    topaz_console_display_input_callback cb;
    void * data;
} ConsoleDisplayInputCallback;




topazConsoleDisplay_t * topaz_console_display_create(topaz_t * topaz, topazBackend_t * b, topazConsoleDisplayAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.console_display_create);
        assert(api.console_display_destroy);
        assert(api.console_display_add_line);
        assert(api.console_display_clear);
        assert(api.console_display_update);
    #endif
    topazConsoleDisplay_t * out = calloc(1, sizeof(topazConsoleDisplay_t));
    out->api = api;
    out->backend = b;
    out->lines = topaz_array_create(sizeof(topazString_t*));
    out->cbs = topaz_bin_create();
    out->userData = out->api.console_display_create(out, topaz);
    return out;
}



void topaz_console_display_destroy(topazConsoleDisplay_t * d) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    d->api.console_display_destroy(d, d->userData);
    topaz_console_display_clear(d);
    topazArray_t * allCBs = topaz_bin_get_all(d->cbs);
    uint32_t i;
    uint32_t len = topaz_array_get_size(allCBs);
    for(i = 0; i < len; ++i) {
        free(topaz_array_at(allCBs, ConsoleDisplayInputCallback *, i));
    }
    topaz_array_destroy(allCBs);
    topaz_bin_destroy(d->cbs);
    topaz_array_destroy(d->lines);
    free(d);
}



topazBackend_t * topaz_console_display_get_backend(topazConsoleDisplay_t * d) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    return d->backend;
}


const topazConsoleDisplayAPI_t * topaz_console_display_get_api(topazConsoleDisplay_t * d) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif

    return &d->api;
}






void topaz_console_display_clear(topazConsoleDisplay_t * d) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    uint32_t i;
    uint32_t len = topaz_array_get_size(d->lines);
    for(i = 0; i < len; ++i) {
        topaz_string_destroy(topaz_array_at(d->lines, topazString_t *, i));
    }
    topaz_array_set_size(d->lines, 0);
    d->api.console_display_clear(d, d->userData);
}

void topaz_console_display_add_line(topazConsoleDisplay_t * d, const topazString_t * line, const topazColor_t * c) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    d->api.console_display_add_line(d, d->userData, line, c);
    topazString_t * str = topaz_string_clone(line);
    topaz_array_push(d->lines, str);
}

uint32_t topaz_console_display_get_line_count(const topazConsoleDisplay_t * d) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    return topaz_array_get_size(d->lines);
}

const topazString_t * topaz_console_display_get_line(const topazConsoleDisplay_t * d, uint32_t index) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    if (index >= topaz_array_get_size(d->lines)) return TOPAZ_STR_CAST("");
    return topaz_array_at(d->lines, topazString_t *, index);
}


void topaz_console_display_send_input(topazConsoleDisplay_t * d, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif
    topazArray_t * allCBs = topaz_bin_get_all(d->cbs);
    uint32_t i;
    uint32_t len = topaz_array_get_size(allCBs);
    ConsoleDisplayInputCallback cb;
    for(i = 0; i < len; ++i) {
        cb = *topaz_array_at(allCBs, ConsoleDisplayInputCallback *, i);
        cb.cb(d, str, cb.data);
    }
    topaz_array_destroy(allCBs);
}


void topaz_console_display_update(topazConsoleDisplay_t * d) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif

    d->api.console_display_update(d, d->userData);
}


uint32_t topaz_console_display_add_input_callback(
    topazConsoleDisplay_t * d, 
    topaz_console_display_input_callback cb,
    void * data
) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
        assert(cb && "Callback must not be NULL.");
    #endif

    ConsoleDisplayInputCallback * cbp = calloc(1, sizeof(ConsoleDisplayInputCallback));
    cbp->cb = cb;
    cbp->data = data;
    return topaz_bin_add(d->cbs, cbp);
}



void topaz_console_display_remove_input_callback(
    topazConsoleDisplay_t * d,
    uint32_t id
) {
    #ifdef TOPAZDC_DEBUG
        assert(d && "topazConsoleDisplay_t pointer cannot be NULL.");
    #endif

    ConsoleDisplayInputCallback * cb = topaz_bin_fetch(d->cbs, id);
    if (!cb) return;
    free(cb);
    topaz_bin_remove(d->cbs, id);
}





