#include <topaz/compat.h>
#include <topaz/backends/console_display.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazConsoleDisplay_t {
    topazConsoleDisplayAPI_t api;
    topazBackend_t * backend;
    topazArray_t * lines;
    void * userData;
};





topazConsoleDisplay_t * topaz_console_display_create(topazBackend_t * b, topazConsoleDisplayAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.console_display_create);
        assert(api.console_display_destroy);
        assert(api.console_display_add_line);
        assert(api.console_display_clear);
    #endif
    topazConsoleDisplay_t * out = calloc(1, sizeof(topazConsoleDisplay_t));
    out->api = api;
    out->backend = b;
    out->api.console_display_create(&out->api);
    return out;
}



