#ifndef H_TOPAZGL3BUFFER_INCLUDED
#define H_TOPAZGL3BUFFER_INCLUDED


typedef struct topazGL3_Buffer_t topazGL3_Buffer_t;

#include "context.h"


// Creates a new buffer.
topazGL3_Buffer_t * topaz_gl3_buffer_create(float * data, uint32_t numElements);

// Destroys a buffer
void topaz_gl3_buffer_destroy(topazGL3_Buffer_t *);


// updates a buffer's contents
void topaz_gl3_buffer_update(
    topazGL3_Buffer_t *,
    const float * newData,
    uint32_t offset,
    uint32_t numElements
);

// Reads from a buffer
void topaz_gl3_buffer_read(
    topazGL3_Buffer_t *,
    float * outputData,
    uint32_t offset, 
    uint32_t numElements    
);


// Sets the type of buffer.
// 0 -> "offline" buffer (malloc/free)
// GL_UNIFORM_BUFFER (online)
// GL_ARRAY_BUFFER (online)
// No effect if already the type
void topaz_gl3_buffer_set_type(topazGL3_Buffer_t *, int);

// Gets the type. default is 0 (offline)
int topaz_gl3_buffer_get_type(const topazGL3_Buffer_t *);

//
GLuint topaz_gl3_buffer_get_buffer_id(topazGL3_Buffer_t *);

// pushes any data waiting to be uploaded.
void topaz_gl3_buffer_commit(topazGL3_Buffer_t *);

// sets a function to be called every time this 
// buffer is updated
typedef void (*topaz_gl3_buffer_commit_fn)(topazGL3_Buffer_t *, void *);
void topaz_gl3_buffer_set_commit_callback(
    topazGL3_Buffer_t *, 
    topaz_gl3_buffer_commit_fn,
    void *
);



// Returns a pointer to the offline buffer in memory.
const float * topaz_gl3_buffer_get_offline_ptr(const topazGL3_Buffer_t *);

// Returns the number of floats that the buffer contains
uint32_t topaz_gl3_buffer_get_float_count(const topazGL3_Buffer_t *);
#endif
