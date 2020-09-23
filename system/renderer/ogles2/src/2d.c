#include "2d.h"
#include <topaz/containers/array.h>
#include <topaz/containers/bin.h>
#include <topaz/matrix.h>
#include "program2d.h"

#include <stdlib.h>
#include <string.h>




typedef struct {
    // all vertices packed together with correct tex coords
    // actual sampler (texture id)
    // local matrix to be uploaded to uniform
    topazES2_Program2D_Renderable_t pdata;

    // texture needs to be maintained through queries to texman to get 
    // real coordinates
    topazES2_Texture_t * texture;

    // holds vertex data (offline)
    topazES2_Buffer_t * linkedBuffer; 

} ES2Object;

struct topazES2_2D_t {
    topazBin_t * objects;
    topazArray_t * queued; // of type topazES2_Program2D_Renderable_t *
    topazES2_TexMan_t * textureManager;
    topazES2_Program2D_t * program;
};



// creates a new object;
static ES2Object * object_create();

// destroys an object. if a buffer is linked, its callback is removed
static void object_destroy(ES2Object *);

// response callback for changes to a linked buffer
static void object_on_linked_buffer_commit(topazES2_Buffer_t * buffer, void * object);

// forces reforming of the vbo and other tasks
static void object_prepare_renderable(ES2Object *);









static GLuint * textureManager_staticIDs;

topazES2_2D_t * topaz_es2_2d_create(topazES2_TexMan_t * t) {
    topazES2_2D_t * out = calloc(1, sizeof(topazES2_2D_t));
    out->objects = topaz_bin_create();
    out->queued = topaz_array_create(sizeof(topazES2_Program2D_Renderable_t*));
    out->program = topaz_es2_p2d_create();
    out->textureManager = t;
    textureManager_staticIDs = (GLuint*)topaz_es2_texman_gl_textures(t);
    return out;
}

void topaz_es2_2d_destroy(topazES2_2D_t * t) {
    topaz_bin_destroy(t->objects);
    topaz_array_destroy(t->queued);
    free(t);
}

int topaz_es2_2d_add_objects(topazES2_2D_t * t, uint32_t * output, uint32_t count) {
    uint32_t i;
    for(i = 0; i < count; ++i) {
        output[i] = topaz_bin_add(t->objects, object_create());
    }
    return count;
}

void topaz_es2_2d_remove_objects(topazES2_2D_t * t, uint32_t * input, uint32_t count) {
    uint32_t i, n;
    for(i = 0; i < count; ++i) {
        ES2Object * obj = topaz_bin_fetch(t->objects, input[i]);
        uint32_t len = topaz_array_get_size(t->queued);

        // remove if queued
        for(n = 0; n < len; ++n) {
            if (topaz_array_at(t->queued, topazES2_Program2D_Renderable_t  *, n) == &(obj->pdata)) {
                topaz_array_remove(t->queued, n);
                n--;
            }
        }
        object_destroy(obj);
    }
}



void topaz_es2_2d_queue_objects(topazES2_2D_t * t, const uint32_t * objects, uint32_t count) {
    uint32_t i;
    for(i = 0; i < count; ++i) {
        ES2Object * obj = topaz_bin_fetch(t->objects, objects[i]);
        if (obj->linkedBuffer)
            topaz_es2_buffer_commit(obj->linkedBuffer);
        topazES2_Program2D_Renderable_t * r = &(obj->pdata);
        topaz_array_push(t->queued, r);
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
    ES2Object * obj = topaz_bin_fetch(t->objects, object);
    if (obj->linkedBuffer == buf) return;
    topaz_es2_buffer_set_type(buf, 0); // ensures its offline.
    topaz_es2_buffer_set_commit_callback(obj->linkedBuffer, 0, 0);
    topaz_es2_buffer_set_commit_callback(buf, object_on_linked_buffer_commit, obj);
    obj->linkedBuffer = buf;
    object_prepare_renderable(obj);
}


void topaz_es2_2d_set_object_texture(
    topazES2_2D_t * t,
    uint32_t object,
    topazES2_Texture_t * tex
) {
    ES2Object * obj = topaz_bin_fetch(t->objects, object);
    obj->texture = tex;

    // gotta re-pull tex coords
    object_prepare_renderable(obj);
}


// renders all queued objects
void topaz_es2_2d_render(
    topazES2_2D_t * t,
    const topazRenderer_2D_Context_t * ctx,
    const topazRenderer_ProcessAttribs_t * attribs
) {
    topaz_es2_p2d_render(
        t->program,
        ctx,
        attribs,
        topaz_array_get_data(t->queued),
        topaz_array_get_size(t->queued)
    );
}


















////// statics ////////

static ES2Object * object_create() {
    TOPAZ_GLES_FN_IN;
    ES2Object * out = calloc(1, sizeof(ES2Object));
    topaz_matrix_set_identity(&out->pdata.localMatrix);
    glGenBuffers(1, &out->pdata.vbo);TOPAZ_GLES_CALL_CHECK;
    return out;
}

// destroys an object. if a buffer is linked, its callback is removed
static void object_destroy(ES2Object * o) {
    TOPAZ_GLES_FN_IN;
    glDeleteBuffers(1, &o->pdata.vbo);TOPAZ_GLES_CALL_CHECK;
    free(o);
}

// response callback for changes to a linked buffer
static void object_on_linked_buffer_commit(topazES2_Buffer_t * buffer, void * object) {
    object_prepare_renderable(object);
}

// forces reforming of the vbo and other tasks
static void object_prepare_renderable(ES2Object * o) {
    TOPAZ_GLES_FN_IN;

    // has to populate a vbo
    if (o->linkedBuffer) {
        o->pdata.vboVertexCount = topaz_es2_buffer_get_float_count(o->linkedBuffer) / 8; // 2d vertex is always 8 floats
        topazRenderer_2D_Vertex_t * vtxSrc = (topazRenderer_2D_Vertex_t *)topaz_es2_buffer_get_offline_ptr(o->linkedBuffer);
        topazRenderer_2D_Vertex_t * vtx    = malloc(sizeof(topazRenderer_2D_Vertex_t)*o->pdata.vboVertexCount);


        if (o->texture) {
            // need to convert local coordinates to ones used in the 
            // actual openGL texture.
            int atlasIndex;
            uint32_t i;
            for(i = 0; i < o->pdata.vboVertexCount; ++i) {
                vtx[i] = vtxSrc[i];
                topaz_es2_texture_local_coords_to_atlas(
                    o->texture,
                    vtx[i].texX,
                    vtx[i].texY,
                    &vtx[i].texX,
                    &vtx[i].texY,
                    &atlasIndex
                );
            }
            o->pdata.texture = textureManager_staticIDs[atlasIndex];
        } else {
            // no texture? blind copy! easy!
            memcpy(vtx, vtxSrc, sizeof(topazRenderer_2D_Vertex_t)*o->pdata.vboVertexCount);
        }
            
        glBindBuffer(GL_ARRAY_BUFFER, o->pdata.vbo); TOPAZ_GLES_CALL_CHECK;
        glBufferData(GL_ARRAY_BUFFER, sizeof(topazRenderer_2D_Vertex_t)*o->pdata.vboVertexCount, vtx, GL_DYNAMIC_DRAW);TOPAZ_GLES_CALL_CHECK;
        glBindBuffer(GL_ARRAY_BUFFER, 0);TOPAZ_GLES_CALL_CHECK;
        free(vtx);
        
    } else {
        // deactivate / dont use.
        o->pdata.vboVertexCount = 0;
    }   

}



