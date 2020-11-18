#include <topaz/compat.h>
#include <topaz/spatial.h>
#include <topaz/spatial.h>
#include <topaz/backends/renderer.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




static void send_update_signal(topazSpatial_t *);

struct topazSpatial_t {
    topazSpatial_t * parent;
    topazArray_t * children;
    topazTransform_t * transformOwned;
    topazMatrix_t global;
    int needsUpdate;

	topazTransform_t * node;

    topazArray_t * localStack;
    topazArray_t * callbacks;
};


typedef struct {
    void (*fn)(topazSpatial_t *, void * data);
    void * data;
    uint32_t id;
} UpdateCallback;


static void transform_update(topazTransform_t * t, void * s) {
    topaz_spatial_invalidate(s);
}

topazTransform_t * topaz_spatial_get_node(topazSpatial_t * t) {
    // TODO: should we have immediate updates? previous version we didnt...
    return t->node;
}

topazSpatial_t * topaz_spatial_create() {
    topazSpatial_t * o = calloc(1, sizeof(topazSpatial_t));
    o->node = topaz_transform_create();
    o->transformOwned = o->node;
    o->children = topaz_array_create(sizeof(topazSpatial_t*));
    o->callbacks = topaz_array_create(sizeof(UpdateCallback));
    

    topaz_transform_add_callback(o->node, transform_update, o);
    send_update_signal(o);
    return o;
}


void topaz_spatial_reset(topazSpatial_t * s) {
    topaz_spatial_set_as_parent(s, NULL);

    topaz_transform_reset(s->transformOwned);

    s->node = s->transformOwned;
    topaz_array_set_size(s->children, 0);
    topaz_array_set_size(s->callbacks, 0);
    
    topaz_transform_add_callback(s->node, transform_update, s);
    send_update_signal(s);
}



void topaz_spatial_destroy(topazSpatial_t * s) {
    topaz_spatial_set_as_parent(s, NULL);
    topaz_transform_destroy(s->transformOwned);
    topaz_array_destroy(s->children);
    topaz_array_destroy(s->callbacks);
    free(s);
}


void topaz_spatial_invalidate(topazSpatial_t * s) {
    send_update_signal(s); 
}

const topazMatrix_t * topaz_spatial_get_global_transform(topazSpatial_t *s) {
    topaz_spatial_check_update(s);
    return &s->global;
}

void topaz_spatial_replace_transform(topazSpatial_t * s, topazTransform_t * t) {
    topaz_transform_remove_callback(s->node, transform_update, s);

    if (!t) {
        s->node = s->transformOwned;
    } else {
        s->node = t;
    }

    topaz_transform_add_callback(s->node, transform_update, s);
}

void topaz_spatial_set_as_parent(topazSpatial_t * s, topazSpatial_t * newParent) {
    if (s->parent) {
        topazSpatial_t * parent = s->parent;
        uint64_t ct = topaz_array_get_size(parent->children);
        uint64_t i;
        for(i = 0; i < ct; ++i) {
            if (topaz_array_at(parent->children, topazSpatial_t*, i) == s) {
                topaz_array_remove(parent->children, i);
                break;
            }
        }
        s->parent = NULL;
    }


    if (!newParent) return;
    
    s->parent = newParent;
    #ifdef TOPAZDC_DEBUG
        assert(newParent != s);
    #endif


    topaz_array_push(newParent->children, s);
    send_update_signal(s);
}



void topaz_spatial_update_model_transform(
    topazSpatial_t * s, 
    const topazMatrix_t * camera, 
    topazRenderer_Buffer_t * t) 
{
    topaz_spatial_check_update(s);
    topazMatrix_t global = s->global;
    topazMatrix_t normalTransform = topaz_matrix_multiply(camera, &global);
    topaz_matrix_invert(&normalTransform);
    topaz_matrix_reverse_majority(&global);
    

    topaz_renderer_buffer_update(t, &global.data[0], 0, 16);
    topaz_renderer_buffer_update(t, &normalTransform.data[0], 16, 16);
}


void send_update_signal(topazSpatial_t * s) {
    if (!s->localStack) {
        s->localStack = topaz_array_create(sizeof(topazSpatial_t *));
    }

    topaz_array_set_size(s->localStack, 0);
    topaz_array_push(s->localStack, s);

    uint32_t count = 1;
    while(count) {
        topazSpatial_t * current = topaz_array_at(
            s->localStack, 
            topazSpatial_t *, 
            count-1
        );
        count-=1;
        topaz_array_set_size(s->localStack, count);

        current->needsUpdate = TRUE;

        uint32_t ct = topaz_array_get_size(current->children);
        uint32_t i;
        for(i = 0; i < ct; ++i) {
            topaz_array_push(s->localStack, topaz_array_at(current->children, topazSpatial_t *, i));
            count++;
        }
    }
}

void topaz_spatial_check_update(topazSpatial_t * s) {
    if (s->parent) {
        topaz_spatial_check_update(s->parent);
    }

    if (topaz_transform_needs_update(s->node)) s->needsUpdate = TRUE;
    if (!s->needsUpdate) return;

    if (s->parent) {
        s->global = topaz_matrix_multiply(
            topaz_spatial_get_global_transform(s->parent), 
            topaz_transform_get_matrix(s->node)
        );
    } else {
        s->global = *topaz_transform_get_matrix(s->node);
    }

    send_update_signal(s);
    s->needsUpdate = FALSE;

    uint32_t i;
    uint32_t len = topaz_array_get_size(s->callbacks);
    for(i = 0; i < len; ++i) {
        topaz_array_at(s->callbacks, UpdateCallback, i).fn(
            s,
            topaz_array_at(s->callbacks, UpdateCallback, i).data    
        );
    }
    
}


uint32_t topaz_spatial_add_update_callback(
    topazSpatial_t * s, 
    void(*fn)(topazSpatial_t *, void *), 
    void * userData
) {
    static uint32_t idPool = 1;
    UpdateCallback cb;
    cb.fn = fn;
    cb.data = userData;
    cb.id = idPool++;
    topaz_array_push(s->callbacks, cb);
    return cb.id;
}


void topaz_spatial_remove_update_callback(
    topazSpatial_t * s,
    uint32_t id
) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->callbacks);
    for(i = 0; i < len; ++i) {
        UpdateCallback * cb = &topaz_array_at(s->callbacks, UpdateCallback, i);
        if (cb->id == id) {
            topaz_array_remove(s->callbacks, i);
            return;
        }
    }

}







