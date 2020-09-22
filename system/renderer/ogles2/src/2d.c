#include "2d.h"
#include <topaz/containers/array.h>
#include <topaz/containers/bin.h>
#include <topaz/matrix.h>

typedef struct {
    // all vertices packed together
    GLuint vbo; 

    // set for locationUniformMatrix
    topazMatrix_t localTransform;

    // texture needs to be maintained through queries to texman to get 
    // real coordinates
    topazES2_Texture_t * texture;

    // holds vertex data (offline)
    topazES2_Buffer_t * linkedBuffer; 

} GL2DObjectES2;

struct topazES2_2D_t {
    GLint locationVBO;
    GLint locationUniformMatrix;
    GLint locationUniform;

    topazBin_t * objects;
    topazArray_t * queued;
}



// creates a new object;
static GL2ObjectES2 * object_create();

// destroys an object. if a buffer is linked, its callback is removed
static void object_destroy(GL2ObjectES2 *);

// response callback for changes to a linked buffer
static void object_on_linked_buffer_commit(topazES2_Buffer_t * buffer, GL2ObjectES2 * object);

// forces reforming of the vbo
static void object_prepare_vbo(GL2ObjectES2 *);











topazES2_2D_t * topaz_es2_2d_create(topazES2_t *);

void topaz_es2_2d_destroy(topazES2_2D_t *);

int topaz_es2_2d_add_objects(topazES2_2D_t * t, uint32_t * output, uint32_t count) {
    uint32_t i;
    for(i = 0; i < count; ++i) {
        GL2ObjectES2 * obj = object_create();
        output[i] = topaz_bin_add(t->objects);
    }
    return count;
}

void topaz_es2_2d_remove_objects(topazES2_2D_t *, uint32_t * input, uint32_t count) {
    uint32_t i, n;
    for(i = 0; i < count; ++i) {
        GL2ObjectES2 * obj = topaz_bin_fetch(t->objects, input[i]);
        uint32_t len = topaz_array_get_size(t->queued);

        // remove if queued
        for(n = 0; n < count; ++n) {
            if (topaz_array_at(t->queued, GL2ObjectES2 *, n) == obj) {
                topaz_array_remove(t->queued, n);
                n--;
            }
        }
        object_destroy(obj);
    }
    return count;    
}



void topaz_es2_2d_queue_objects(topazES2_2D_t * t, const uint32_t * objects, uint32_t count) {
    uint32_t i;
    for(i = 0; i < count; ++i) {
        GL2DObjectES * obj = topaz_bin_fetch(objects[i]);
        topaz_es2_buffer_commit(obj);
        topaz_array_push(t->queued, obj);
    }
}

void topaz_es2_2d_clear_queue(topazES2_2D_t * t) {
    topaz_array_set_size(t->queued, 0);
}


// sets the vertices
void topaz_es2_2d_set_object_vertices(
    topazES2_2D_t * t,
    uint32_t object,
    topazES2_Buffer_t * buf
) {
    GL2ObjectES2 * obj = topaz_bin_fetch(t->objects);
    if (obj->linkedBuffer == buf) return;

    topaz_es2_buffer_set_commit_callback(obj->linkedBuffer, 0, 0);
    topaz_es2_buffer_set_commit_callback(buf, object_on_linked_buffer_commit, obj);
    obj->linkedBuffer = buf;
    object_prepare_vbo(obj);
}


void topaz_es2_2d_set_object_texture(
    topazES2_2D_t * t,
    uint32_t object,
    topazES2_Texture_t * tex
) {
    GL2ObjectES2 * obj = topaz_bin_fetch(t->objects);
    obj->texture = tex;

    // gotta re-pull tex coords
    object_prepare_vbo(obj);
}


// renders all queued objects
void topaz_es2_2d_render(
    topazES2_2D_t *
) {
    
}

