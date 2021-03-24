#include <topaz/compat.h>
#include <topaz/backends/filesys.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif 


struct topazFilesys_t {
    topazFilesysAPI_t api;
    topazSystem_Backend_t * backend;
    void * userData;

    topazString_t * childPath;
};





topazFilesys_t * topaz_filesys_create(topaz_t * ctx, topazSystem_Backend_t * b, topazFilesysAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(api.filesys_create);
        assert(api.filesys_destroy);
        assert(api.filesys_set_path);
        assert(api.filesys_go_to_child);
        assert(api.filesys_go_to_parent);
        assert(api.filesys_get_path);
        assert(api.filesys_get_child_path);
        assert(api.filesys_create_node);
        assert(api.filesys_read);
        assert(api.filesys_write);
        assert(api.filesys_query);
        assert(api.filesys_is_node);
        assert(api.filesys_is_child);
        assert(api.filesys_split_path);
        assert(api.filesys_join_path);
    #endif
    topazFilesys_t * out = calloc(1, sizeof(topazFilesys_t));
    out->api = api;
    out->backend = b;
    out->userData = out->api.filesys_create(out, ctx);
    return out;
}


void topaz_filesys_destroy(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif
    t->api.filesys_destroy(t, t->userData);
    if (t->childPath) topaz_string_destroy(t->childPath);
}





topazSystem_Backend_t * topaz_filesys_get_backend(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazFilesysAPI_t topaz_filesys_get_api(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api;
}




int topaz_filesys_set_path(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_set_path(t, t->userData, str);
}


int topaz_filesys_go_to_child(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_go_to_child(t, t->userData, str);
}


int topaz_filesys_go_to_parent(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_go_to_parent(t, t->userData);
}


const topazString_t * topaz_filesys_get_path(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_get_path(t, t->userData);
}

const topazString_t * topaz_filesys_get_child_path(topazFilesys_t * t, const topazString_t * name) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif
    if (t->childPath) topaz_string_destroy(t->childPath);
    t->childPath = t->api.filesys_get_child_path(t, t->userData, name);
    return t->childPath;
}



const topazArray_t * topaz_filesys_split_path(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_split_path(t, t->userData, str);
    
}

const topazString_t * topaz_filesys_join_path(topazFilesys_t * t, const topazArray_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_join_path(t, t->userData, str);
    
}

int topaz_filesys_create_node(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_create_node(t, t->userData, str);    
}

topazRbuffer_t * topaz_filesys_read(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_read(t, t->userData, str);    
}





int topaz_filesys_write(topazFilesys_t * t, const topazString_t * str, const topazWbuffer_t * b) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
        assert(str && "topazWbuffer_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_write(t, t->userData, str, b);
}

const topazArray_t * topaz_filesys_query(topazFilesys_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_query(t, t->userData);
}

int topaz_filesys_is_node(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_is_node(t, t->userData, str);
}

int topaz_filesys_is_child(topazFilesys_t * t, const topazString_t * str) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(str && "topazString_t pointer cannot be NULL.");
    #endif

    return t->api.filesys_is_child(t, t->userData, str);
}



