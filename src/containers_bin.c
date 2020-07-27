#include <topaz/compat.h>
#include <topaz/containers/array.h>
#include <topaz/containers/bin.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

struct topazBin_t {
    topazArray_t * alive;
    topazArray_t * dead;
};


topazBin_t * topaz_bin_create() {
    topazBin_t * out = malloc(sizeof(topazBin_t));
    out->alive = topaz_array_create(sizeof(void*));
    out->dead = topaz_array_create(sizeof(uint32_t));

    return out;
}

void topaz_bin_destroy(topazBin_t * b) {
    topaz_array_destroy(b->alive);
    topaz_array_destroy(b->dead);
    free(b);
}

uint32_t topaz_bin_add(topazBin_t * b, void * obj) {
    uint32_t deadLen = topaz_array_get_size(b->dead);
    uint32_t id;
    if (deadLen) {
        id = topaz_array_at(b->dead, uint32_t, deadLen-1);
        topaz_array_set_size(b->dead, deadLen-1);
        topaz_array_at(b->alive, void *, id) = obj;
    } else {
        id = topaz_array_get_size(b->alive);
        topaz_array_push(b->alive, b);
    }
    return id;
}

int topaz_bin_contains(const topazBin_t * b, uint32_t id) {
    return topaz_bin_fetch(b, id) != NULL;
}

void * topaz_bin_fetch(const topazBin_t * b, uint32_t id) {
    if (id >= topaz_array_get_size(b->alive)) return 0;
    return topaz_array_at(b->alive, void*, id);
}

void topaz_bin_remove(topazBin_t * b, uint32_t id) {
    // not real
    if (id >= topaz_array_get_size(b->alive)) return;
    // already removed
    if (topaz_array_at(b->alive, void*, id) == NULL) return;

    topaz_array_at(b->alive, void*, id) = NULL;
    topaz_array_push(b->dead, id);
}


topazArray_t * topaz_bin_get_all(const topazBin_t * b) {
    topazArray_t * out = topaz_array_create(sizeof(void*));
    uint32_t i;
    uint32_t len = topaz_array_get_size(b->alive);
    void ** objs = topaz_array_get_data(b->alive);

    for(i = 0; i < len; ++i) {
        if (objs[i]) {
            topaz_array_push(out, objs[i]);
        }
    }

    return out;
}


