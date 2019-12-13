#include <topaz/compat.h>
#include <topaz/backends/filesys.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazFilesys_t {
    topazFilesysAPI_t api;
    topazBackend_t * backend;
};





topazFilesys_t * topaz_filesys_create(topazBackend_t * b, topazFilesysAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazBackend_t pointer cannot be NULL.");
        assert(api.filesys_create);
        assert(api.filesys_destroy);
        assert(api.filesys_set_path);
        assert(api.filesys_go_to_child);
        assert(api.filesys_go_to_parent);
        assert(api.filesys_get_path);
        assert(api.filesys_create_node);
        assert(api.filesys_read);
        assert(api.filesys_write);
        assert(api.filesys_query);
        assert(api.filesys_is_node);
        assert(api.filesys_is_child);
    #endif
    topazFilesys_t * out = calloc(1, sizeof(topazFilesys_t));
    out->api = api;
    out->backend = b;
    out->api.filesys_create(&out->api);
    return out;
}


void topaz_filesys_destroy(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif
    t->api.filesys_destroy(&t->api);
}





topazBackend_t * topaz_filesys_get_backend(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazFilesysAPI_t topaz_filesys_get_api(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api;
}




int topaz_filesys_set_path(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_set_path(&t->api, str);
}


int topaz_filesys_go_to_child(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_go_to_child(&t->api, str);
}


int topaz_filesys_go_to_parent(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_go_to_parent(&t->api);
}


const topazString_t * topaz_filesys_get_path(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_get_path(&t->api);
}




int topaz_filesys_create_node(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_create_node(&t->api, str);    
}

topazRbuffer_t * topaz_filesys_read(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_read(&t->api, str);    
}





int topaz_filesys_write(topazFilesys_t * t, const topazString_t * str, const topazWbuffer_t * b) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
        assert(str && "topazWbuffer_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_write(&t->api, str, b);
}

const topazArray_t * topaz_filesys_query(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_query(&t->api);
}

int topaz_filesys_is_node(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_is_node(&t->api, str);
}

int topaz_filesys_is_child(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_is_child(&t->api, str);
}



