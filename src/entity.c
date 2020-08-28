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
#include <topaz/entity.h>
#include <topaz/component.h>
#include <topaz/spatial.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif






// Entities are never deleted and freed. Instead, they are recycled.
static topazArray_t * dead  = NULL;

struct topazEntity_t {
    topazEntity_t * parent;
    topazString_t * name;

    topazArray_t * children;     // normal entity list
    topazArray_t * componentsBefore;
    topazArray_t * componentsAfter;
    topazArray_t * components;

    topazArray_t * activeSet;

    topazEntity_Attributes_t api;
    topazSpatial_t * spatial;
    topaz_t * context;

    int protectd;
    int priority;
    int step;
    int draw;
    int valid;
};

int priority_comp(const topazEntity_t ** a, const topazEntity_t ** b) {
    return (*a)->priority < (*b)->priority;
}

topazEntity_t * topaz_entity_null() {
    static topazEntity_t * e = NULL;
    if (!e) {
        e = calloc(1, sizeof(topazEntity_t));
        e->name = topaz_string_create();
        e->children         = topaz_array_create(sizeof(topazEntity_t *));
        e->components       = topaz_array_create(sizeof(topazComponent_t *));
        e->componentsBefore = topaz_array_create(sizeof(topazComponent_t *));
        e->componentsAfter  = topaz_array_create(sizeof(topazComponent_t *));
        e->activeSet = topaz_array_create(sizeof(void*));
        e->spatial = topaz_spatial_create();
        e->context = NULL;
    }
    return e;
}

topazEntity_t * topaz_entity_create_with_attributes(topaz_t * ctx, const topazEntity_Attributes_t * a) {
    topazEntity_t * out;
    if (!dead) {
        dead = topaz_array_create(sizeof(topazEntity_t *));
    } 
    uint32_t deadLen = topaz_array_get_size(dead);
    if (deadLen) {
        out = topaz_array_at(dead, topazEntity_t *, deadLen-1);
        topaz_array_set_size(dead, deadLen-1);
        
    } else {
        out = calloc(1, sizeof(topazEntity_t));
        out->name = topaz_string_create();
        out->children         = topaz_array_create(sizeof(topazEntity_t *));
        out->components       = topaz_array_create(sizeof(topazComponent_t *));
        out->componentsBefore = topaz_array_create(sizeof(topazComponent_t *));
        out->componentsAfter  = topaz_array_create(sizeof(topazComponent_t *));
        out->activeSet = topaz_array_create(sizeof(void*));
        out->spatial = topaz_spatial_create();
        out->parent = topaz_entity_null();
    }
    out->context = ctx;
    out->valid = TRUE;
    out->step = TRUE;
    out->draw = TRUE;
    out->protectd = FALSE;
    out->api = *a;

    return out;
}




topazEntity_t * topaz_entity_create(topaz_t * ctx) {
    topazEntity_Attributes_t attrib;
    attrib.on_attach = NULL;
    attrib.on_detach = NULL;
    attrib.on_remove = NULL;
    attrib.on_pre_step = NULL;
    attrib.on_step = NULL;
    attrib.on_pre_draw = NULL;
    attrib.on_draw = NULL;
    attrib.userData = NULL;
    return topaz_entity_create_with_attributes(ctx, &attrib);
}


void topaz_entity_set_attributes(topazEntity_t * e, const topazEntity_Attributes_t * a) {
    e->api = *a;
}

const topazEntity_Attributes_t * topaz_entity_get_attributes(const topazEntity_t * e) {
    // for null entities, its already empty. No need for Id check
    return &e->api;
}


int topaz_entity_is_valid(const topazEntity_t * e) {
    return e->valid;
}

void topaz_entity_remove(topazEntity_t * e) {
    if (!e->valid) return;

    if (e->api.on_remove) 
        e->api.on_remove(e, e->api.userData);


    uint32_t i;
    uint32_t len = topaz_array_get_size(e->components);
    for(i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->components, topazComponent_t *, i);
        topaz_component_destroy(c);

    }



    len = topaz_array_get_size(e->children);
    for(uint32_t i = 0; i < len; ++i) {
        topazEntity_t * c = topaz_array_at(e->children, topazEntity_t *, i);
        if (topaz_entity_is_valid(c))
            topaz_entity_remove(c);
    }
    topaz_entity_detach(e);

    topaz_string_clear(e->name);
    topaz_array_set_size(e->children, 0);
    topaz_array_set_size(e->components, 0);
    topaz_array_set_size(e->componentsBefore, 0);
    topaz_array_set_size(e->componentsAfter, 0);
    topaz_spatial_reset(e->spatial);
    e->valid = 0;

    topaz_array_push(dead, e);
}



const topazArray_t * topaz_entity_get_children(const topazEntity_t * e) {
    // for null entities, its already empty. No need for Id check
    return e->children;
}

void topaz_entity_step(topazEntity_t * e) {
    if (!e->step) return;
    uint64_t self = e->valid;




    // PRE STEP
    if (e->api.on_pre_step) e->api.on_pre_step(e, e->api.userData);
    if (self != e->valid) return;


    topazEntity_t * curEnt;
    topazComponent_t * curComp;
    uint32_t n;


    // BEFORE-STEP COMPONENTS
    uint32_t numComp = topaz_array_get_size(e->componentsBefore);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsBefore),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_at(e->activeSet, topazComponent_t *, n);
        topaz_component_step(curComp);
        if (self != e->valid) return;
    }



    // CHILDREN-STEP
    uint32_t numEnts = topaz_array_get_size(e->children);
    topaz_array_set_size(e->activeSet, numEnts);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->children),
        sizeof(void*)*numEnts
    );

    for(n = 0; n < numEnts; n++) {
        curEnt = topaz_array_at(e->children, topazEntity_t *, n);
        topaz_entity_step(curEnt);
        if (self != e->valid) return;        
    }






    // actual running of self
    if (e->api.on_step) e->api.on_step(e, e->api.userData);






    // AFTER-STEP COMPONENTS
    numComp = topaz_array_get_size(e->componentsAfter);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsAfter),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_at(e->activeSet, topazComponent_t *, n);
        topaz_component_step(curComp);
        if (self != e->valid) return;
    }

    topaz_spatial_check_update(e->spatial);
}

/// Updates this entity, all child entities, and components
///
void topaz_entity_draw(topazEntity_t * e) {
    if (!e->draw) return;
    uint64_t self = e->valid;




    // PRE draw
    if (e->api.on_pre_draw) e->api.on_pre_draw(e, e->api.userData);
    if (self != e->valid) return;


    topazEntity_t * curEnt;
    topazComponent_t * curComp;
    uint32_t n;


    // BEFORE-DRAW COMPONENTS
    uint32_t numComp = topaz_array_get_size(e->componentsBefore);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsBefore),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_at(e->activeSet, topazComponent_t *, n);
        topaz_component_draw(curComp);
        if (self != e->valid) return;
    }



    // CHILDREN-DRAW
    uint32_t numEnts = topaz_array_get_size(e->children);
    topaz_array_set_size(e->activeSet, numEnts);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->children),
        sizeof(void*)*numEnts
    );

    for(int i = 0; i < numEnts; i++) {
        curEnt = topaz_array_at(e->children, topazEntity_t *, i);
        topaz_entity_draw(curEnt);
        if (self != e->valid) return;        
    }






    // actual running of self
    if (e->api.on_draw) e->api.on_draw(e, e->api.userData);






    // AFTER-DRAW COMPONENTS
    numComp = topaz_array_get_size(e->componentsAfter);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsAfter),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_at(e->activeSet, topazComponent_t *, n);
        topaz_component_draw(curComp);
        if (self != e->valid) return;
    }

    topaz_spatial_check_update(e->spatial);


}


void topaz_entity_attach(topazEntity_t * e, topazEntity_t * child) {
    if (child == e ||
        !child->valid) {
        return;
    }

    if (child->parent->valid) {
        topaz_entity_detach(child);
    } 

    if (!topaz_array_get_size(e->children)) {
        topaz_array_push(e->children, child);
    } else {
        uint32_t iter = topaz_array_lower_bound(
            e->children, 
            &child, 
            (int(*)(const void *, const void *))priority_comp
        );
        topaz_array_insert(e->children, iter, child);
    }

    child->parent = e;
    topaz_spatial_set_as_parent(child->spatial, e->spatial);
    if (child->api.on_attach) child->api.on_attach(child, child->api.userData);
}


void topaz_entity_detach(topazEntity_t * e) {
    if (!e->valid) return;
    if (!e->parent->valid) return;
    uint32_t len = topaz_array_get_size(e->parent->children);
    if (len > 1) {
        uint32_t iter = topaz_array_lower_bound(
            e->parent->children, 
            e,
            (int(*)(const void *, const void *))priority_comp
        );

        uint32_t i;
        for(i = iter; i < len; ++i) {
            if (topaz_array_at(e->parent->children, topazEntity_t *, i) == e) break;
        }

        #ifdef TOPAZDC_DEBUG
            assert(i < topaz_array_get_size(e->parent->children));
        #endif
        topaz_array_remove(e->parent->children, i);
    } else if (len == 1) {
        topaz_array_set_size(e->parent->children, 0);
    }
    e->parent = topaz_entity_null();
    topaz_spatial_set_as_parent(e->spatial, NULL);


    // Try to find the index of the entity
    if (e->api.on_detach)
        e->api.on_detach(e, e->api.userData);
}

topazEntity_t * topaz_entity_get_parent(const topazEntity_t * e) {
    return e->parent;
}




void topaz_entity_set_priority(topazEntity_t * e, int priority) {
    if (!e->valid) return;

    if (e->parent->valid) {
        void(*tempAttach)(topazEntity_t *, void *) = e->api.on_attach;
        void(*tempDetach)(topazEntity_t *, void *) = e->api.on_detach;

        topazEntity_t * parent = e->parent;
        topaz_entity_detach(e);
        e->priority = priority;
        topaz_entity_attach(parent, e);

        e->api.on_attach = tempAttach;
        e->api.on_detach = tempDetach;
    } else {
        e->priority = priority;
    }

}

void topaz_entity_set_priority_last(topazEntity_t * e) {
    if (!e->parent) return;
    if (!e->valid) return;

    topaz_entity_set_priority(
        e,
        topaz_array_at(
            e->parent->children,
            topazEntity_t *,
            topaz_array_get_size(e->parent->children)-1
        )->priority+1
    );
}


void topaz_entity_set_priority_first(topazEntity_t * e) {
    if (!e->parent) return;
    if (!e->valid) return;

    topaz_entity_set_priority(
        e,
        topaz_array_at(
            e->parent->children,
            topazEntity_t *,
            0
        )->priority-1
    );

}


int topaz_entity_get_priority(const topazEntity_t * e) {
    // for null entities, its already 0. No need for Id check
    return e->priority;
}




int topaz_entity_is_stepping(const topazEntity_t * e) {
    if (!e->step) return FALSE;
    if (!e->parent->valid) return TRUE;
    do {
        e = e->parent;
        if (!e->step) return FALSE;
    } while(e->parent->valid);
    return TRUE;
}

int topaz_entity_is_drawing(const topazEntity_t * e) {
    if (!e->draw) return FALSE;
    if (!e->parent->valid) return TRUE;
    do {
        e = e->parent;
        if (!e->draw) return FALSE;
    } while(e->parent->valid);
    return TRUE;

}

void topaz_entity_set_stepping(topazEntity_t * e, int d) {
    if (!e->valid) return;
    e->step = d;
}

void topaz_entity_set_drawing(topazEntity_t * e, int d) {
    if (!e->valid) return;
    e->draw = d;
}


int topaz_entity_get_stepping(const topazEntity_t * e) {
    return e->step;
}

int topaz_entity_get_drawing(const topazEntity_t * e) {
    return e->draw;
}







void topaz_entity_add_component(topazEntity_t * e, topazComponent_t * t) {
    if (!e->valid) return;
    if (topaz_component_get_host(t) == e) return;
    topaz_component_attach(t, e);
    topaz_array_push(e->components, t);
    topaz_array_push(e->componentsBefore, t);
}

void topaz_entity_add_component_after(topazEntity_t * e, topazComponent_t * t) {
    if (!e->valid) return;
    if (topaz_component_get_host(t) == e) return;
    topaz_component_attach(t, e);
    topaz_array_push(e->components, t);
    topaz_array_push(e->componentsAfter, t);
}


const topazArray_t * topaz_entity_get_components(topazEntity_t * t) {
    // For null entities, its already empty.
    return t->components;
}

topazComponent_t * topaz_component_query(topazEntity_t * e, const topazString_t * str) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(e->components);
    for(i = 0; i < len; ++i) {
        if (topaz_string_test_eq(
                str, 
                topaz_component_get_tag(
                    topaz_array_at(
                        e->components,
                        topazComponent_t *,
                        i
                    )
                )
            )
        ) {
            return topaz_array_at(
                e->components,
                topazComponent_t *,
                i
            );
        }
    }
    return topaz_component_null();
}

/// Removes the given component from the entity.
///
void topaz_entity_remove_component(topazEntity_t * e, topazComponent_t * t) {
    if (topaz_component_get_host(t) != e) return;
    topaz_component_detach(t);    

    uint32_t i;
    uint32_t len; 

    len = topaz_array_get_size(e->components);
    for(i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->components, topazComponent_t *, i);
        if (c == t) {
            topaz_array_remove(e->components, i);
            break;
        }
    }

    len = topaz_array_get_size(e->componentsBefore);
    for(i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->componentsBefore, topazComponent_t *, i);
        if (c == t) {
            topaz_array_remove(e->componentsBefore, i);
            break;
        }
    }

    len = topaz_array_get_size(e->componentsAfter);
    for(i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->componentsAfter, topazComponent_t *, i);
        if (c == t) {
            topaz_array_remove(e->componentsAfter, i);
            break;
        }
    }

}

void topaz_entity_set_name(topazEntity_t * e, const topazString_t * s) {
    if (!e->valid) return;
    topaz_string_set(e->name, s);
}

const topazString_t * topaz_entity_get_name(const topazEntity_t * e) {
    return e->name;
}




const topazVector_t * topaz_entity_get_rotation(const topazEntity_t * e) {
    return topaz_transform_get_rotation(topaz_spatial_get_node(e->spatial));
}

const topazVector_t * topaz_entity_get_position(const topazEntity_t * e) {
    return topaz_transform_get_position(topaz_spatial_get_node(e->spatial));
}

const topazVector_t * topaz_entity_get_scale(const topazEntity_t * e) {
    return topaz_transform_get_scale(topaz_spatial_get_node(e->spatial));
}





topazVector_t * topaz_entity_rotation(topazEntity_t * e) {
    return topaz_transform_rotation(topaz_spatial_get_node(e->spatial));
}

topazVector_t * topaz_entity_position(topazEntity_t * e) {
    return topaz_transform_position(topaz_spatial_get_node(e->spatial));
}

topazVector_t * topaz_entity_scale(topazEntity_t * e) {
    return topaz_transform_scale(topaz_spatial_get_node(e->spatial));
}




topazVector_t topaz_entity_get_global_position(const topazEntity_t * s) {
    topazVector_t e;
    e.x = e.y = e.z = 0.f;
    return topaz_matrix_transform(topaz_spatial_get_global_transform(s->spatial), &e);
}



topazSpatial_t * topaz_entity_get_spatial(const topazEntity_t * e) {
    return e->spatial;
}   

