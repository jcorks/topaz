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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif






// Entities are never deleted and freed. Instead, they are recycled.
static topazArray_t * dead  = NULL;
static uint64_t idPool = 1;

struct toapzEntity_t {
    int protectd;
    int priority;
    int step;
    int draw;
    uint64_t id; //<- if 0, the entity has been removed
    Entity * world;
    topazString_t * name;

    topazArray_t * children;     // normal entity list
    topazArray_t * componentsBefore;
    topazArray_t * componentsAfter;
    topazArray_t * components;

    topazArray_t * activeSet;

    topazEntity_Attributes_t api;
    topazSpatial_t * spatial;
};

int priority_comp(const topazEntity_t * a, const topazEntity_t * b) {
    return a->priority < b->priority;
}



topazEntity_t * topaz_entity_create_with_attributes(const topazEntity_Attributes_t * a) {
    topazEntity_t * out;
    if (!dead) {
        dead = topaz_array_create(sizeof(topazEntity_t *));
    } 
    uint32_t deadLen = topaz_array_get_size(dead);
    if (deadLen) {
        out = topaz_array_at(dead, topazEntity_t *, deadLen-1;
        topaz_array_set_size(dead, deadLen-1);
        topaz_string_clear(out->name);
        topaz_array_set_size(out->components, 0);
        topaz_array_set_size(out->componentsBefore, 0);
        topaz_array_set_size(out->componentsAfter, 0);
        memset(&out->api, 0, sizeof(topazEntity_Attributes_t));
        
    } else {
        out = calloc(1, sizeof(topazEntity_t));
        out->name = topaz_string_create();
        out->priorityList     = topaz_array_create(sizeof(topazEntity_t *));
        out->components       = topaz_array_create(sizeof(topazComponent_t *));
        out->componentsBefore = topaz_array_create(sizeof(topazComponent_t *));
        out->componentsAfter  = topaz_array_create(sizeof(topazComponent_t *));
        out->activeSet = topaz_array_create(sizeof(void*));
        out->spatial = topaz_spatial_create();
    }
    out->id = idPool++;
    out->step = TRUE;
    out->draw = TRUE;
    out->protectd = FALSE;
    out->api = *a;


}

topazEntity_t * topaz_entity_create() {
    topazEntity_Attributes_t attrib;
    attrib.on_attach = NULL;
    attrib.on_detach = NULL;
    attrib.on_remove = NULL;
    attrib.on_pre_step = NULL;
    attrib.on_step = NULL;
    attrib.on_pre_draw = NULL;
    attrib.on_draw = NULL;
    attrib.userData;
    return topaz_entity_create(&attrib);
}


void topaz_entity_set_attributes(topazEntity_t * e, const topasEntity_Attributes_t * a) {
    e->api = *a;
}

topazEntity_Attributes_t topaz_entity_get_attributes(const topazEntity_t * e) {
    return e->api;
}


int topaz_component_is_valid(const topazEntity_t * e) {
    return e->id != 0;
}

void topaz_entity_remove(topazEntity_t * e) {
    if (!e->id) return;

    uint32_t i;
    uint32_t len = topaz_array_get_size(e->components);
    for(i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->components, topazComponent_t *, i);
        topaz_component_emit_event(c, TOPAZ_STR_CAST("on-detach"), e, NULL);
    }
    if (e->api.on_remove) 
        e->api.on_remove(e, e->api.userData);

    uint32_t len = children.size();
    for(uint32_t i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->components, topazComponent_t *, i);
        if (topaz_entity_is_valid(c))
            topaz_entity_remove(c);
    }
    topaz_entity_detach(e);

    e->id = 0;
    topaz_array_push(dead, e);
}



const topazArray_t * topaz_entity_get_children(const topazEntity_t * e) {
    return t->children;
}

void topaz_entity_step(topazEntity_t * e) {
    if (!e->step) return;
    uint64_t self = e->id;




    // PRE STEP
    if (e->api.on_pre_step) e->api->on_pre_step(e, e->api.userData);
    if (self != e->id) return;


    topazEntity_t * curEnt;
    topazComponent_t * curComp;



    // BEFORE-STEP COMPONENTS
    uint32_t numComp = topaz_array_get_size(e->componentsBefore);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsBefore),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_get_data(e->componentsBefore);
        topaz_component_step(curComp);
        if (self != e->id) return;
    }



    // CHILDREN-STEP
    uint32_t numEnts = topaz_array_get_size(e->children), n;
    topaz_array_set_size(e->activeSet, numEnts);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->children),
        sizeof(void*)*numEnts
    );

    for(int i = 0; i < numEnts; i++) {
        curEnt = topaz_array_at(e->children, topazEntity_t *, i);
        topaz_entity_step(curEnt);
        if (self != e->id) return;        
    }






    // actual running of self
    if (e->api.on_step) e->api.on_step(e, e->api.userData);






    // AFTER-STEP COMPONENTS
    numComp = topaz_array_get_size(e->componentsBefore);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsBefore),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_get_data(e->componentsBefore);
        topaz_component_step(curComp);
        if (self != e->id) return;
    }

    topaz_spatial_check_update(e->spatial);
}

/// Updates this entity, all child entities, and components
///
void topaz_entity_draw(topazEntity_t *) {
    if (!e->draw) return;
    uint64_t self = e->id;




    // PRE draw
    if (e->api.on_pre_draw) e->api->on_pre_draw(e, e->api.userData);
    if (self != e->id) return;


    topazEntity_t * curEnt;
    topazComponent_t * curComp;



    // BEFORE-DRAW COMPONENTS
    uint32_t numComp = topaz_array_get_size(e->componentsBefore);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsBefore),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_get_data(e->componentsBefore);
        topaz_component_draw(curComp);
        if (self != e->id) return;
    }



    // CHILDREN-DRAW
    uint32_t numEnts = topaz_array_get_size(e->children), n;
    topaz_array_set_size(e->activeSet, numEnts);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->children),
        sizeof(void*)*numEnts
    );

    for(int i = 0; i < numEnts; i++) {
        curEnt = topaz_array_at(e->children, topazEntity_t *, i);
        topaz_entity_draw(curEnt);
        if (self != e->id) return;        
    }






    // actual running of self
    if (e->api.on_draw) e->api.on_draw(e, e->api.userData);






    // AFTER-DRAW COMPONENTS
    numComp = topaz_array_get_size(e->componentsBefore);
    topaz_array_set_size(e->activeSet, numComp);
    memcpy(
        topaz_array_get_data(e->activeSet),
        topaz_array_get_data(e->componentsBefore),
        sizeof(void*)*numComp
    );

    for(n = 0; n < numComp; ++n) {
        curComp = topaz_array_get_data(e->componentsBefore);
        topaz_component_draw(curComp);
        if (self != e->id) return;
    }

    topaz_spatial_check_update(e->spatial);


}


void topaz_entity_attach(topazEntity_t * e, topazEntity_t * child) {
    if (child == e ||
        !child->id) {
        return;
    }

    if (child->parent) {
        topaz_entity_detach(child);
    } 

    if (!topaz_array_get_size(e->children)) {
        topaz_array_push(e->children, child);
    } else {
        uint32_t iter = topaz_array_lower_bound(e->children, child, priority_comp);
        topaz_array_insert(e->children, iter, child);
    }

    child->parent = e;
    topaz_spatial_set_as_parent(child->spatial, e->spatial);
    if (child->api.on_attach) child->api.on_attach(child, child->api.userData);
}


void topaz_entity_detach(topazEntity_t * e) {
    if (!e->id) return;
    if (!e->parent) return;

    uint32_t iter = topaz_array_lower_bound(
        e->parent->children, 
        e,
        priority_comp
    );

    uint32_t i;
    uint32_t len = topaz_array_get_size(e->parent->children);
    for(i = iter; i < len; ++i) {
        if (topaz_array_at(e->parent->children, topazEntity_t *, i) == e) break;
    }

    #ifdef TOPAZDC_DEBUG
        assert(i < topaz_array_get_size(e->parent->children));
    #endif
    topaz_array_remove(e->parent->children, i);

    e->world = nullptr;
    topaz_spatial_set_as_parent(e->spatial, NULL);


    // Try to find the index of the entity
    e->api.on_detach(e, e->api.userData);
}

topazEntity_t * topaz_entity_get_parent(const topazEntity_t * e) {
    return r->parent;
}




void topaz_entity_set_priority(topazEntity_t * e, int priority) {
    if (!e->id) return;

    if (e->parent) {
        void(*tempAttach)(topazEntity_t *, void *) = e->api.on_attach;
        void(*tempDetach)(topazEntity_t *, void *) = e->api.on_detach;
        topazEntity_t * parent = e->parent;
        topaz_entity_detach(e);
        e->priority = priority;
        topaz_entity_attach(e->parent, e);

        e->api.on_attach = tempAttach;
        e->api.on_detach = tempDetach;
    } else {
        priority = p;
    }

}

void topaz_entity_set_priority_last(topazEntity_t * e) {
    if (!e->parent) return;

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
    return e->priority;
}




int topaz_entity_is_stepping(const topazEntity_t * e) {
    if (!e->step) return FALSE;
    if (!e->parent) return true;
    do {
        e = e->parent;
        if (!e->step) return FALSE;
    } while(e->parent);
    return TRUE;
}

int topaz_entity_is_drawing(const topazEntity_t * e) {
    if (!e->draw) return FALSE;
    if (!e->parent) return TRUE;
    do {
        e = e->parent;
        if (!e->draw) return FALSE;
    } while(e->parent;
    return TRUE;

}

void topaz_entity_set_stepping(topazEntity_t * e, int d) {
    e->step = d;
}

void topaz_entity_set_drawing(topazEntity_t * e, int d) {
    e->draw = d;
}


int topaz_entity_get_stepping(const topazEntity_t * e) {
    return e->step;
}

int topaz_entity_get_drawing(const topazEntity_t * e) {
    return e->draw;
}







void topaz_entity_add_component(topazEntity * e, topazComponent_t * t) {
    if (topaz_component_get_host(t) == e) return;
    topaz_array_push(e->components, t);
    topaz_array_push(e->componentsBefore, t);
    topaz_component_attach(t, e);
}

void topaz_entity_add_component_after(topazEntity *, topazComponent_t *) {
    if (topaz_component_get_host(t) == e) return;
    topaz_array_push(e->components, t);
    topaz_array_push(e->componentsAfter, t);
    topaz_component_attach(t, e);
}


const topazArray_t * topaz_entity_get_components(topazEntity_t * t) {
    return t->components;
}

topazComponent_t * topaz_component_query(topazEntity_t * e, const topazString_t * str) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(e->components);
    for(i = 0; i < len; ++i) {
        if (topaz_string_test_eq(
                str, 
                topaz_array_at(
                    e->components,
                    topazComponent_t *,
                    i
                )
            )
        ) {

        }
    }
}

/// Removes the given component from the entity.
///
void topaz_entity_remove_component(topazEntity_t * e, topazComponent_t * t) {
    uint32_t i;
    uint32_t len; 

    len = topaz_array_get_size(t->components);
    for(i = 0; i < len; ++i) {
        topazComponent_t * c = topaz_array_at(e->components, topazComponent_t *, i);
        if (c == t) {
            topaz_array
        }
    }
}

/// Sets the name of the entity. Once set, it will not change.
///
void topaz_entity_set_name(topazEntity_t *, const topazString_t *);

/// Gets the name of the entity.
///
const topazString_t * topaz_entity_get_name(const topazEntity_t *);


