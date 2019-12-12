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
        assert(api.filesys_change_directory);
        assert(api.filesys_go_to_child);
        assert(api.filesys_go_to_parent);
        assert(api.filesys_get_cwd);
        assert(api.filesys_create_dir);
        assert(api.filesys_read);
        assert(api.filesys_write);
        assert(api.filesys_query_directory);
        assert(api.filesys_is_file);
        assert(api.filesys_child_exists);
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




int topaz_filesys_change_directory(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_change_directory(&t->api, str);
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


const topazString_t * topaz_filesys_get_cwd(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_get_cwd(&t->api);
}




int topaz_filesys_create_dir(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_create_dir(&t->api, str);    
}

topazRbuffer_t * topaz_filesys_read(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_read(&t->api, str);    
}





void topaz_filesys_write(topazFilesys_t * t, const topazString_t * str, const topazWbuffer_t * b) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
        assert(str && "topazWbuffer_t pointer cannot be NULL.");
    #endif

    t->api.filesys_write(&t->api, str, b);
}

const topazArray_t * topaz_filesys_query_directory(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_query_directory(&t->api);
}

int topaz_filesys_is_file(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_is_file(&t->api, str);
}

int topaz_filesys_child_exists(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazBackend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_child_exists(&t->api, str);
}



