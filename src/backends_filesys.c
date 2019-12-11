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
    topazFilesys_t * out = calloc(1, sizeof(topazFilesys_t));
    out->api = api;
    out->backend = b;
    out->api.filesys_create(&out->api);
    return out;
}


void topaz_filesys_destroy(topazFilesys_t * t) {
    t->api.filesys_destroy(&t->api);
}





topazBackend_t * topaz_filesys_get_backend(topazFilesys_t * t) {
    return t->backend;
}

topazFilesysAPI_t topaz_filesys_get_api(topazFilesys_t * t) {
    return t->api;
}




int topaz_filesys_change_directory(topazFilesys_t * t, const topazString_t * str) {
    return t->api.filesys_change_directory(&t->api, str);
}


int topaz_filesys_go_to_child(topazFilesys_t * t, const topazString_t * str) {
    return t->api.filesys_go_to_child(&t->api, str);
}


int topaz_filesys_go_to_parent(topazFilesys_t * t) {
    return t->api.filesys_go_to_parent(&t->api);
}


const topazString_t * topaz_filesys_get_cwd(topazFilesys_t * t) {
    return t->api.filesys_get_cwd(&t->api);
}




int topaz_filesys_create_dir(topazFilesys_t * t, const topazString_t * str) {
    return t->api.filesys_create_dir(&t->api, str);    
}

topazRbuffer_t * topaz_filesys_read(topazFilesys_t * t, const topazString_t * str) {
    return t->api.filesys_read(&t->api, str);    
}





void topaz_filesys_write(topazFilesys_t * t, const topazString_t * str, const topazWbuffer_t * b) {
    t->api.filesys_write(&t->api, str, b);
}

const topazArray_t * topaz_filesys_query_directory(topazFilesys_t * t) {
    return t->api.filesys_query_directory(&t->api);
}

int topaz_filesys_is_file(topazFilesys_t * t, const topazString_t * str) {
    return t->api.filesys_is_file(&t->api, str);
}

int topaz_filesys_child_exists(topazFilesys_t * t, const topazString_t * str) {
    return t->api.filesys_child_exists(&t->api, str);
}



