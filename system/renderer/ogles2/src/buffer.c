#include "buffer.h"
#include <stdlib.h>
#include <string.h>

struct topazES2_Buffer_t {
    float * data;
    uint32_t numFloats;
    GLuint object;
    GLenum type;
    int dirty;

    topaz_es2_buffer_commit_fn onCommit;
    void * fnData;
};



topazES2_Buffer_t * topaz_es2_buffer_create(float * data, uint32_t numElements) {
    TOPAZ_GLES_FN_IN;
    topazES2_Buffer_t * out = calloc(1, sizeof(topazES2_Buffer_t));
    if (data) {
        out->data = malloc(numElements * sizeof(float));
        memcpy(out->data, data, numElements * sizeof(float));
    } else {
        out->data = calloc(numElements, sizeof(float));
    }

    glGenBuffers(1, &out->object); TOPAZ_GLES_CALL_CHECK;    
    return out;
}

void topaz_es2_buffer_destroy(topazES2_Buffer_t * b) {
    glDeleteBuffers(1, &b->object);
    free(b->data);
    free(b);
}


// updates a buffer's contents
void topaz_es2_buffer_update(
    topazES2_Buffer_t * b,
    const float * newData,
    uint32_t offset,
    uint32_t numElements
) {
    uint32_t copyAmt = b->numFloats-offset < numElements ? (b->numFloats-offset) : numElements;
    memcpy(b->data+offset, newData, copyAmt*sizeof(float)); 
    b->dirty = 1;
}

// Reads from a buffer
void topaz_es2_buffer_read(
    topazES2_Buffer_t * b,
    float * outputData,
    uint32_t offset, 
    uint32_t numElements    
) {

    uint32_t copyAmt = b->numFloats-offset < numElements ? (b->numFloats-offset) : numElements;
    memcpy(outputData, b->data+offset, copyAmt*sizeof(float)); 
}


// Sets the type of buffer.
// 0 -> "offline" buffer (malloc/free)
// GL_UNIFORM_BUFFER (online)
// GL_ARRAY_BUFFER (online)
// No effect if already the type
void topaz_es2_buffer_set_type(topazES2_Buffer_t * b, int type) {
    if (b->type == type) return;
    b->type = type;
    b->dirty = 1;
    topaz_es2_buffer_commit(b);
}

int topaz_es2_buffer_get_type(const topazES2_Buffer_t * b) {
    return b->type;
}

GLuint topaz_es2_buffer_get_buffer_id(topazES2_Buffer_t * b) {
    return b->object;
}

void topaz_es2_buffer_commit(topazES2_Buffer_t * b) {
    if (!b->dirty) return;

    TOPAZ_GLES_FN_IN;

    glBindBuffer(b->type, b->object); TOPAZ_GLES_CALL_CHECK;    
    glBufferData(b->type, b->numFloats*sizeof(float), b->data, GL_STATIC_DRAW);
    glBindBuffer(b->type, 0); TOPAZ_GLES_CALL_CHECK;        
    b->dirty = 0;    
    if (b->onCommit)
        b->onCommit(b, b->fnData);
}


void topaz_es2_buffer_set_commit_callback(
    topazES2_Buffer_t * b, 
    topaz_es2_buffer_commit_fn fn,
    void * fnData) {
    b->onCommit = fn;
    b->fnData = fnData;
}

const float * topaz_es2_buffer_get_offline_ptr(const topazES2_Buffer_t * b) {
    return b->data;
}

uint32_t topaz_es2_buffer_get_float_count(const topazES2_Buffer_t * b) {
    return b->numFloats;
}


