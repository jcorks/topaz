/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/

#include <topaz/compat.h>
#include <topaz/transform.h>
#include <topaz/matrix.h>
#include <topaz/vector.h>
#include <topaz/containers/array.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




struct topazTransform_t {

    topazMatrix_t localTransform;

    int reverse;
    int needsUpdate;
    topazVector_t position;
    topazVector_t rotation;
    topazVector_t scale;
    

    topazArray_t * callbacks;
};

typedef struct {
    void (*fn)(topazTransform_t *, void *);
    void * data;
} TransformCallback;


static void compute_local(topazTransform_t * t) {
    topaz_matrix_set_identity(&t->localTransform);
    

    if (t->position.x != 0.f ||
        t->position.y != 0.f ||
        t->position.z != 0.f) {
        if (t->reverse) {
            topaz_matrix_translate(
                &t->localTransform,
                t->position.x *-1,
                t->position.y *-1,
                t->position.z *-1
            );

        } else {
            topaz_matrix_translate(
                &t->localTransform,
                t->position.x,
                t->position.y,
                t->position.z
            );
        }
    }

    if (t->rotation.x != 0.f ||
        t->rotation.y != 0.f ||
        t->rotation.z != 0.f) {

        topaz_matrix_rotate_by_angles(
            &t->localTransform,
            t->rotation.x,
            t->rotation.y,
            t->rotation.z
        );
    }


    if (t->scale.x != 1.f ||
        t->scale.y != 1.f ||
        t->scale.z != 1.f) {

        topaz_matrix_scale(
            &t->localTransform,
            t->scale.x,
            t->scale.y,
            t->scale.z
        ); 


    }
    t->needsUpdate = FALSE;


    uint32_t i;
    uint32_t len = topaz_array_get_size(t->callbacks);
    for(i = 0; i < len; ++i) {
        TransformCallback * cb = &topaz_array_at(t->callbacks, TransformCallback, i);
        cb->fn(t, cb->data);
    }
}
    
topazTransform_t * topaz_transform_create() {
    topazTransform_t * out = calloc(1, sizeof(topazTransform_t));
    out->scale.x = 0;
    out->scale.y = 0;
    out->scale.z = 0;
    out->needsUpdate = FALSE;

    out->callbacks = topaz_array_create(sizeof(TransformCallback));
    return out;
}


void topaz_transform_destroy(topazTransform_t * t) {
    topaz_array_destroy(t->callbacks);
    free(t);
}




/// Gets a read-only, quick reference to the rotation component.
///
const topazVector_t * topaz_transform_get_rotation(const topazTransform_t * t) {
    return &t->rotation;
}

/// Gets a read-only, quick reference to the position component.
///
const topazVector_t * topaz_transform_get_position(const topazTransform_t * t) {
    return &t->position;
}

/// Gets a read-only, quick reference to the scale component.
///
const topazVector_t * topaz_transform_get_scale(const topazTransform_t * t) {
    return &t->scale;
}

topazVector_t * topaz_transform_rotation(topazTransform_t * t) {
    t->needsUpdate = TRUE;
    return &t->rotation;
}

topazVector_t * topaz_transform_position(topazTransform_t * t) {
    t->needsUpdate = TRUE;
    return &t->position;
}

topazVector_t * topaz_transform_scale(topazTransform_t * t) {
    t->needsUpdate = TRUE;
    return &t->scale;
}


void topaz_transform_set_reverse_translation(topazTransform_t * t, int rev) {
    if (rev != t->reverse) {
        t->reverse = rev;
        t->needsUpdate = TRUE;
    }
}






const topazMatrix_t * topaz_transform_get_matrix(topazTransform_t * t) {
    if (t->needsUpdate) {
        compute_local(t);
    }
    return &t->localTransform;
}

int topaz_transform_needs_update(const topazTransform_t * t) {
    return t->needsUpdate;
}











void AddTransformCallback(
    topazTransform_t * t,
    void (*topazTransform)(topazTransform_t *, void *),
    void * userData
) {
    TransformCallback cb;
    cb.fn = topazTransform;
    cb.data = userData;

    topaz_array_push(t->callbacks, cb);
}

void RemoveTransformCallback(
    topazTransform_t * t,
    void (*topazTransform)(topazTransform_t *, void *)
) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->callbacks);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(t->callbacks, TransformCallback, i).fn == topazTransform) {
            topaz_array_remove(t->callbacks, i);            
            return;
        }
    }
}


