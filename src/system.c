#include <topaz/system.h>
#include <topaz/containers/table.h>
#include <topaz/containers/string.h>
#include <stdlib.h>
static topazTable_t * renderers = NULL;
static topazTable_t * inputManagers = NULL;
static topazTable_t * audioManagers = NULL;
static topazTable_t * filesystems = NULL;
static topazTable_t * times = NULL;
static topazTable_t * displays = NULL;

// external
void topaz_system_configure_base();

typedef struct {
    topazBackend_t * (*backend_callback)();
    void (*api_callback)(void *);
} BackendHandler;

static topazTable_t * backend_type_to_table(const topazString_t * name) {
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("renderer"))) return renderers;
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("inputManagers"))) return inputManagers;
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("audioManagers"))) return audioManagers;
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("filesystems"))) return filesystems;
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("times"))) return times;
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("displays"))) return displays;
    return NULL;
}

static BackendHandler * backend_retrieve(const topazString_t * type, const topazString_t * name, topazTable_t ** table) {
    *table = backend_type_to_table(type);
    if (!table) {

        return NULL;
    }

    BackendHandler * handler = (topaz_table_find(*table, name));

    if (!handler) {
        return NULL;
    }

    return handler;
}


void topaz_system_configure() {
    if (renderers) {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: topaz_system_configure was already called!");
        #endif

        renderers     = topaz_table_create_hash_topaz_string();
        inputManagers = topaz_table_create_hash_topaz_string();
        audioManagers = topaz_table_create_hash_topaz_string();
        filesystems   = topaz_table_create_hash_topaz_string();
        times         = topaz_table_create_hash_topaz_string();
        displays      = topaz_table_create_hash_topaz_string();

        topaz_system_configure_base();

    }
}



int topaz_system_config_add_handler(
    const topazString_t * backendType,
    const topazString_t * backendName,
    topazBackend_t * (*backend_callback)(),
    void (*api_callback)(void *)
) {
    topazTable_t * table = NULL;
    BackendHandler * handler = backend_retrieve(
        backendType,
        backendName,
        &table
    );

    if (!table) {
        #ifdef TOPAZDC_DEBUG
            printf("Cannot add handler %s (backend %s doesnt exist?)", topaz_string_get_data(backendType), topaz_string_get_data(backendName));
        #endif
        return 0;
    }

    if (handler) {
        #ifdef TOPAZDC_DEBUG
            printf("Cannot add handler %s (already exists)", topaz_string_get_data(backendName));
        #endif
        return 0;
    }

    handler = malloc(sizeof(BackendHandler));
    handler->backend_callback = backend_callback;
    handler->api_callback = api_callback;

    topaz_table_insert(table, backendName, handler);
    return 1;
}

