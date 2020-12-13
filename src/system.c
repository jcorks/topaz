#include <topaz/system.h>
#include <topaz/containers/table.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/backends/api/iox_api.h>
#include <stdlib.h>
#include <stdio.h>
#include <topaz/compat.h>

static topazTable_t * renderers = NULL;
static topazTable_t * inputManagers = NULL;
static topazTable_t * audioManagers = NULL;
static topazTable_t * filesystems = NULL;
static topazTable_t * times = NULL;
static topazTable_t * fontRenderers = NULL;
static topazTable_t * displays = NULL;
static topazTable_t * scripts = NULL;
static topazTable_t * consoleDisplays = NULL;
static topazTable_t * ioxs = NULL;

// external
void topaz_system_configure_base();

typedef struct {
    void (*backend_callback)(topazSystem_t *, topazSystem_Backend_t *, void * api);
} BackendHandler;

struct topazSystem_t {
    BackendHandler renderer;
    BackendHandler inputManager;
    BackendHandler audioManager;
    BackendHandler filesystem;
    BackendHandler time;
    BackendHandler fontRenderer;
    BackendHandler display;
    BackendHandler script;
    BackendHandler consoleDisplay;
    BackendHandler iox;

    topazArray_t * backends;
};


struct topazSystem_Backend_t {
    topazString_t * name;
    topazString_t * version;
    topazString_t * author;
    topazString_t * description;

    void (*on_pre_step) (topazSystem_Backend_t *, void *);
    void (*on_post_step)(topazSystem_Backend_t *, void *);
    void (*on_pre_draw) (topazSystem_Backend_t *, void *);
    void (*on_post_draw)(topazSystem_Backend_t *, void *);

    void * userData;

    int topazMajorVersion;
    int topazMinorVersion;
    int topazMicroVersion;
};




static BackendHandler default_renderer;
static BackendHandler default_inputManager;
static BackendHandler default_audioManager;
static BackendHandler default_filesystem;
static BackendHandler default_time;
static BackendHandler default_fontRenderer;
static BackendHandler default_display;
static BackendHandler default_script;
static BackendHandler default_consoleDisplay;
static BackendHandler default_iox;

BackendHandler * system_get_backend(topazSystem_t * s, const topazString_t * name) {
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("renderer"))) return &s->renderer;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("inputManager"))) return &s->inputManager;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("audioManager"))) return &s->audioManager;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("filesys")))return &s->filesystem;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("time"))) return &s->time;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("fontRenderer"))) return &s->fontRenderer;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("display"))) return &s->display;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("script"))) return &s->script;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("consoleDisplay"))) return &s->consoleDisplay;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("iox"))) return &s->iox;
    return NULL;
}

static topazTable_t * backend_type_to_table(const topazString_t * name) {
    if (topaz_string_test_eq(name, TOPAZ_STR_CAST("renderer"))) return renderers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("inputManager"))) return inputManagers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("audioManager"))) return audioManagers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("filesys"))) return filesystems;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("time"))) return times;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("fontRenderer"))) return fontRenderers;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("display"))) return displays;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("script"))) return scripts;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("consoleDisplay"))) return consoleDisplays;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("iox"))) return ioxs;
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
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("fontRenderer"))) return &default_fontRenderer;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("display"))) return &default_display;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("script"))) return &default_script;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("consoleDisplay"))) return &default_consoleDisplay;
    else if (topaz_string_test_eq(name, TOPAZ_STR_CAST("iox"))) return &default_iox; 
   return NULL;
}


void topaz_system_configure() {
    if (renderers) {
        #ifdef TOPAZDC_DEBUG
            printf("WARNING: topaz_system_configure was already called!");
        #endif
        return;
    }
    renderers       = topaz_table_create_hash_topaz_string();
    inputManagers   = topaz_table_create_hash_topaz_string();
    audioManagers   = topaz_table_create_hash_topaz_string();
    filesystems     = topaz_table_create_hash_topaz_string();
    times           = topaz_table_create_hash_topaz_string();
    fontRenderers   = topaz_table_create_hash_topaz_string();
    displays        = topaz_table_create_hash_topaz_string();
    scripts         = topaz_table_create_hash_topaz_string();
    consoleDisplays = topaz_table_create_hash_topaz_string();
    ioxs            = topaz_table_create_hash_topaz_string();

    topaz_system_configure_base();
}



int topaz_system_config_add_handler(
    const topazString_t * backendType,
    const topazString_t * backendName,
    void (*backend_callback)(topazSystem_t *, topazSystem_Backend_t *, void * api)
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
    *get_backend_default(backendType) = *handler;

    topaz_table_insert(table, backendName, handler);
    return 1;
}



topazSystem_t * topaz_system_create_default() {
    topazSystem_t * s = calloc(1, sizeof(topazSystem_t));
    s->renderer       = default_renderer;
    s->inputManager   = default_inputManager;
    s->audioManager   = default_audioManager;
    s->filesystem     = default_filesystem;
    s->time           = default_time;
    s->fontRenderer   = default_fontRenderer;
    s->display        = default_display;
    s->script         = default_script;
    s->consoleDisplay = default_consoleDisplay;
    s->iox            = default_iox;
    s->backends = topaz_array_create(sizeof(topazSystem_Backend_t *));
    return s; 
}


int topaz_system_set_backend_handler(
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


topazArray_t * topaz_system_get_available_backends(
    const topazString_t * backendType
) {
    topazArray_t * out = topaz_array_create(sizeof(topazString_t *));
    topazTable_t * table;
    table = backend_type_to_table(backendType);
    if (!table) {
        return out;
    }

    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, table);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        topazString_t * key = topaz_table_iter_get_key(iter);
        topaz_array_push(out, key);
    }
    return out;
}





/// Creates a backend instance 
topazSystem_Backend_t * topaz_system_create_backend(
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

    topazSystem_Backend_t * out = calloc(1, sizeof(topazSystem_Backend_t));
    topaz_array_push(s->backends, out);

    // populates the api object. up to the user-side programmer and 
    // the backend developers to be correct on their ends.
    t->backend_callback((topazSystem_t *)s, out, APImapping);
    return out;
}






#include <topaz/compat.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif





void topaz_system_backend_bind(
    topazSystem_Backend_t * out,
    const topazString_t * name,
    const topazString_t * version,
    const topazString_t * author,
    const topazString_t * description,



    void (*on_pre_step) (topazSystem_Backend_t *, void *),
    void (*on_post_step)(topazSystem_Backend_t *, void *),
    void (*on_pre_draw) (topazSystem_Backend_t *, void *),
    void (*on_post_draw)(topazSystem_Backend_t *, void *),
    void * userData,

    int topazMajorVersion,
    int topazMinorVersion,
    int topazMicroVersion

) {
    out->name        = topaz_string_clone(name);
    out->version     = topaz_string_clone(version);
    out->author      = topaz_string_clone(author);
    out->description = topaz_string_clone(description);

    out->on_pre_step = on_pre_step;
    out->on_pre_draw = on_pre_draw;
    out->on_post_step = on_post_step;
    out->on_post_draw = on_post_draw;

    out->userData = userData;

    out->topazMajorVersion = topazMajorVersion;
    out->topazMinorVersion = topazMinorVersion;
    out->topazMicroVersion = topazMicroVersion;

}






int topaz_system_backend_get_topaz_major_version(const topazSystem_Backend_t * t) {
    return t->topazMajorVersion;
}

int topaz_system_backend_get_topaz_minor_version(const topazSystem_Backend_t * t) {
    return t->topazMinorVersion;
}

int topaz_system_backend_get_topaz_micro_version(const topazSystem_Backend_t * t) {
    return t->topazMicroVersion;
}





const topazString_t * topaz_system_backend_get_name   (const topazSystem_Backend_t * t) {
    return t->name;
}

const topazString_t * topaz_system_backend_get_version(const topazSystem_Backend_t * t) {
    return t->version;
}

const topazString_t * topaz_system_backend_get_author (const topazSystem_Backend_t * t) {
    return t->author;
}

const topazString_t * topaz_system_backend_get_description(const topazSystem_Backend_t * t) {
    return t->description;
}






void topaz_system_backend_pre_step(topazSystem_Backend_t * t) {
    if (t->on_pre_step)
        t->on_pre_step(t, t->userData);
}

void topaz_system_backend_post_step(topazSystem_Backend_t * t) {
    if (t->on_post_step)
        t->on_post_step(t, t->userData);
}


void topaz_system_backend_pre_draw(topazSystem_Backend_t * t) {
    if (t->on_pre_step)
        t->on_pre_step(t, t->userData);
}

void topaz_system_backend_post_draw(topazSystem_Backend_t * t) {
    if (t->on_post_draw)
        t->on_post_draw(t, t->userData);
}

void * topaz_system_backend_get_user_data(topazSystem_Backend_t * t) {
    return t->userData;
}


void topaz_system_pre_step(topazSystem_t * s) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->backends); ++i) {
        topaz_system_backend_pre_step(topaz_array_at(s->backends, topazSystem_Backend_t *, i));
    }
}

void topaz_system_post_step(topazSystem_t * s) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->backends); ++i) {
        topaz_system_backend_post_step(topaz_array_at(s->backends, topazSystem_Backend_t *, i));
    }
}


void topaz_system_pre_draw(topazSystem_t * s) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->backends); ++i) {
        topaz_system_backend_pre_draw(topaz_array_at(s->backends, topazSystem_Backend_t *, i));
    }
}

void topaz_system_post_draw(topazSystem_t * s) {
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(s->backends); ++i) {
        topaz_system_backend_post_draw(topaz_array_at(s->backends, topazSystem_Backend_t *, i));
    }
}







// Brings in external implementations using the build system,
// namely the implementation of "topaz_system_configure_base()",
// which is designed to /ONLY/ contain calls to
// topaz_system_config_add_handler()
#include <topaz_config__system_backends>
