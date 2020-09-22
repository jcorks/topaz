#ifndef H_TOPAZES2BUFFER_INCLUDED
#define H_TOPAZES2BUFFER_INCLUDED


#include "context.h"


typedef struct topazES2_Buffer_t topazES2_Buffer_t;

// Creates a new buffer.
topazES2_Buffer_t * topaz_es2_buffer_create(float * data, uint32_t numElements);

// Destroys a buffer
void topaz_es2_buffer_destroy(topazES2_Buffer_t *);


// updates a buffer's contents
void topaz_es2_buffer_update(
    topazES2_Buffer_t *,
    const float * newData,
    uint32_t offset,
    uint32_t numElements
);

// Reads from a buffer
void topaz_es2_buffer_read(
    topazES2_Buffer_t *,
    float * outputData,
    uint32_t offset, 
    uint32_t numElements    
);


// Sets the type of buffer.
// 0 -> "offline" buffer (malloc/free)
// GL_UNIFORM_BUFFER (online)
// GL_ARRAY_BUFFER (online)
// No effect if already the type
void topaz_es2_buffer_set_type(topazES2_Buffer_t *, int);

// Gets the type. default is 0 (offline)
int topaz_es2_buffer_get_type(const topazES2_Buffer_t *);

//
GLuint topaz_es2_buffer_get_buffer_id(topazES2_Buffer_t *);

// pushes any data waiting to be uploaded.
void topaz_es2_buffer_commit(topazES2_Buffer_t *);

// sets a function to be called every time this 
// buffer is updated
typedef void (*topaz_es2_buffer_commit_fn)(topazES2_Buffer_t *, void *)
void topaz_es2_buffer_set_commit_callback(topazES2_Buffer_t *, void *);

#endif
