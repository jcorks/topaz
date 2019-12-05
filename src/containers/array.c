#include <topaz/compat.h>
#include <topaz/containers/array.h>

#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


#define array_resize_amt 1.4
#define array_presize_amt 16

struct topazArray_t {
    uint32_t allocSize;
    uint32_t size;
    uint32_t sizeofType;
    uint8_t * data;
};


topazArray_t * topaz_array_create(uint32_t typesize) {
    topazArray_t * a = malloc(sizeof(topazArray_t));
    a->sizeofType = typesize;
    a->size = 0;
    a->allocSize = array_presize_amt;
    a->data = malloc(typesize*array_presize_amt);
}

void topaz_array_destroy(topazArray_t * t) {
    free(t->data);
    free(t);
}



topazArray_t * topaz_array_clone(const topazArray_t * src) {
    topazArray_t * a = malloc(sizeof(topazArray_t));

    // do not clone pre-alloc size
    a->allocSize = src->size;
    a->size = src->size;
    a->sizeofType = src->sizeofType;
    a->data = malloc(src->size*a->sizeofType);
    return a;
}


uint32_t topaz_array_get_size(const topazArray_t * t) {
    return t->size;
}

void topaz_array_push_n(topazArray_t * t, void * elements, uint32_t count) {
    if (t->size + count > t->allocSize) {
        t->allocSize += t->allocSize*array_presize_amt;
        t->data = realloc(t->allocSize*t->sizeofType);
    }
    memcpy(
        (t->data)+(t->size*t->sizeofType), 
        elements, 
        count*t->sizeofType
    );
}


void topaz_array_remove(topazArray_t * t, uint32_t index) {
    #ifdef TOPAZDC_DEBUG
        assert(index < t->size);
    #endif

    uint32_t indexByte = index*t->sizeofType
    memmove(
        t->data+(indexByte),
        t->data+(indexByte + t->sizeofType),
        (t->size-1)*t->sizeofType
    );
}
