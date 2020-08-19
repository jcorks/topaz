#include <topaz/system.h>
#include <topaz/containers/table.h>
#include <topaz/containers/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <topaz/compat.h>

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

struct topazSystem_t {
    BackendHandler renderer;
    BackendHandler inputManager;
    BackendHandler audioManager;
    BackendHandler filesystem;
    BackendHandler time;
    BackendHandler display;
};



static BackendHandler default_renderer;
static BackendHandler default_inputManager;
static BackendHandler default_audioManager;
static BackendHandler default_filesystem;
static BackendHandler default_time;
static BackendHandler default_display;

BackendHandler * system_get_backend(topazSystem_t * s, const topazString_t * name) {
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("renderer"))) return &s->renderer;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("inputManager"))) return &s->inputManager;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("audioManager"))) return &s->audioManager;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("filesys")))return &s->filesystem;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("time"))) return &s->time;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("display"))) return &s->display;
    return NULL;
}

static topazTable_t * backend_type_to_table(const topazString_t * name) {
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("renderer"))) return renderers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("inputManager"))) return inputManagers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("audioManager"))) return audioManagers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("filesys"))) return filesystems;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("time"))) return times;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("display"))) return displays;
    return NULL;
}


static BackendHandler * backend_retrieve(const topazString_t * type, const topazString_t * name, topazTable_t ** tableSrc) {
    topazTable_t * table;
    table = backend_type_to_table(type);
    if (!table) {
        return NULL;
    }

    if (tableSrc) {
        *tableSrc = table;
    }

    BackendHandler * handler = (topaz_table_find(table, name));

    if (!handler) {
        return NULL;
    }

    return handler;
}

static BackendHandler * get_backend_default(const topazString_t * name) {
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("renderer"))) return &default_renderer;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("inputManager"))) return &default_inputManager;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("audioManager"))) return &default_audioManager;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("filesys"))) return &default_filesystem;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("time"))) return &default_time;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("display"))) return &default_display;
    return NULL;
}


void topaz_system_configure() {
    if (renderers) {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: topaz_system_configure was already called!");
        #endif
        return;
    }
    renderers     = topaz_table_create_hash_topaz_string();
    inputManagers = topaz_table_create_hash_topaz_string();
    audioManagers = topaz_table_create_hash_topaz_string();
    filesystems   = topaz_table_create_hash_topaz_string();
    times         = topaz_table_create_hash_topaz_string();
    displays      = topaz_table_create_hash_topaz_string();

    topaz_system_configure_base();
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
            printf("Cannot add handler %s (backend %s doesnt exist?)", topaz_string_get_c_str(backendType), topaz_string_get_c_str(backendName));
        #endif
        return 0;
    }

    if (handler) {
        #ifdef TOPAZDC_DEBUG
            printf("Cannot add handler %s (already exists)", topaz_string_get_c_str(backendName));
        #endif
        return 0;
    }

    handler = malloc(sizeof(BackendHandler));
    handler->backend_callback = backend_callback;
    handler->api_callback = api_callback;

    *get_backend_default(backendType) = *handler;

    topaz_table_insert(table, backendName, handler);
    return 1;
}



topazSystem_t * topaz_system_create_default() {
    topazSystem_t * s = calloc(1, sizeof(topazSystem_t));
    s->renderer     = default_renderer;
    s->inputManager = default_inputManager;
    s->audioManager = default_audioManager;
    s->filesystem   = default_filesystem;
    s->time         = default_time;
    s->display      = default_display;
    return s; 
}


int topaz_system_set_backend(
    topazSystem_t * s, 
    const topazString_t * backendType,
    const topazString_t * backendName
) {
    BackendHandler * handler = backend_retrieve(
        backendType,
        backendName,
        NULL
    );
    if (!handler) {
        #ifdef TOPAZDC_DEBUG 
            printf(
                "Unknown backend requested (name: %s, type %s)\n",
                topaz_string_get_c_str(backendName),
                topaz_string_get_c_str(backendType)
            );
        #endif
        return FALSE;
    }
    BackendHandler * t = system_get_backend(s, backendType);

    if (!t) {
        #ifdef TOPAZDC_DEBUG 
            printf(
                "Unknown backend requested (type %s)\n",
                topaz_string_get_c_str(backendType)
            );
        #endif
        return FALSE;
    }
    *t = *handler;
    return TRUE;
}


int topaz_system_is_backend_available(
    const topazString_t * backendType,
    const topazString_t * backendName
) {
    return backend_retrieve(backendType, backendName, NULL) != NULL;
}


/// Creates a backend instance 
topazBackend_t * topaz_system_create_backend(
    const topazSystem_t * s, 
    const topazString_t * backendType, 
    void * APImapping
) {
    BackendHandler * t = system_get_backend((topazSystem_t *)s, backendType);
    if (!t) {
        #ifdef TOPAZDC_DEBUG 
            printf(
                "Unknown backend requested (type %s)\n",
                topaz_string_get_c_str(backendType)
            );
        #endif
        return NULL;
    }

    // populates the api object. up to the user-side programmer and 
    // the backend developers to be correct on their ends.
    t->api_callback(APImapping);

    return t->backend_callback();
}


// Brings in external implementations using the build system,
// namely the implementation of "topaz_system_configure_base()",
// which is designed to /ONLY/ contain calls to
// topaz_system_config_add_handler()
#include <topaz_config__system_backends>
