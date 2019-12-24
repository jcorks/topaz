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
#include <topaz/component.h>
#include <topaz/entity.h>
#include <topaz/spatial.h>
#include <topaz/containers/table.h>
#include <topaz/containers/array.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



static topazArray_t * dead = NULL;

typedef struct {
    topaz_event_handler fn;
    void * data;
} EventHandler;

typedef struct {
    topazArray_t * hooks;    // of EventHandler;
    topazArray_t * handlers; // of EventHandler;
} EventSet;



struct topazComponent_t {      
    topazString_t * tag;
    topazEntity_t * host;
    
    topazTable_t * events;
    topazArray_t * eventStore;
    topazArray_t * eventStoreDead;

    topazComponent_Attributes_t api;

    int step;
    int draw;
    int valid;
};




topazComponent_t * topaz_component_create() {
    topazComponent_Attributes_t api;
    api.on_attach = NULL;
    api.on_detach = NULL;
    api.on_step = NULL;
    api.on_draw = NULL;
    api.on_destroy = NULL;
    api.userData = NULL;
    return topaz_component_create_with_attributes(&api);
}

topazComponent_t * topaz_component_create_with_attributes(const topazComponent_Attributes_t * api) {
    if (!dead) {
        dead = topaz_array_create(sizeof(topazComponent_t *));
    }

    topazComponent_t * out;
    uint32_t deadLen = topaz_array_get_size(dead);
    if (deadLen) {
        out = topaz_array_at(dead, topazComponent_t *, deadLen-1);
        topaz_array_set_size(dead, deadLen-1);  
    } else {
        out = calloc(1, sizeof(topazComponent_t));
        out->host = TOPAZ_ENULL;
        out->events = topaz_table_create_hash_topaz_string();
        out->eventStore = topaz_array_create(sizeof(EventSet));
        out->eventStoreDead = topaz_array_create(sizeof(int));
        out->tag = topaz_string_create();
    }
    topaz_array_set_size(out->eventStore, 1);    
    out->step = TRUE;
    out->draw = TRUE;
    out->api = *api;
    out->valid = TRUE;

    topaz_component_install_event(out, TOPAZ_STR_CAST("on-attach"), NULL, NULL);
    topaz_component_install_event(out, TOPAZ_STR_CAST("on-detach"), NULL, NULL);
    return out;

}


const topazComponent_Attributes_t * topaz_component_get_attributes(const topazComponent_t * c) {
    return &c->api;
}


void topaz_component_set_attributes(topazComponent_t * c, const topazComponent_Attributes_t * a) {
    if (!c->valid) return;
    c->api = *a;
}


topazComponent_t * topaz_component_null() {
    static topazComponent_t * c = NULL;
    if (!c) {
        c = calloc(1, sizeof(topazComponent_t));
        c->events = topaz_table_create_hash_topaz_string();
        c->host = TOPAZ_ENULL;
    }
    return c;
}



void topaz_component_destroy(topazComponent_t * c) {
    if (!c->valid) return;
    if (topaz_entity_is_valid(c->host)) {
        topaz_component_detach(c);
    }    

    topaz_array_set_size(c->eventStore, 0);
    topaz_array_set_size(c->eventStoreDead, 0);
    topaz_table_clear(c->events);
    topaz_string_clear(c->tag);

    c->step = FALSE;
    c->draw = FALSE;
    c->valid = FALSE;
    topaz_array_push(dead, c);
}


void topaz_component_attach(topazComponent_t * c, topazEntity_t * e) {
    if (!c->valid) return;
    if (topaz_entity_is_valid(c->host)) {
        topaz_component_detach(c);
    }
    c->host = e;
    topaz_component_emit_event(c, TOPAZ_STR_CAST("on-attach"), e, NULL);    
}

void topaz_component_detach(topazComponent_t * t) {
    if (t->valid!=TRUE) return;
    if (!topaz_entity_is_valid(t->host)) return;

    t->valid = 2; // prevent self-recursion
    topaz_entity_remove_component(t->host, t);
    t->valid = TRUE;


    topaz_component_emit_event(t, TOPAZ_STR_CAST("on-detach"), TOPAZ_ENULL, NULL);
    t->host = TOPAZ_ENULL;
            
}



void topaz_component_step(topazComponent_t * c) {
    if (c->step && c->api.on_step)
        c->api.on_step(c, c->api.userData);
}

void topaz_component_draw(topazComponent_t * c) {
    if (c->draw && c->api.on_draw)
        c->api.on_draw(c, c->api.userData);
}



int topaz_component_get_stepping(const topazComponent_t * c) {
    return c->step;
}

int topaz_component_get_drawing(const topazComponent_t * c) {
    return c->draw;
}



void topaz_component_set_stepping(topazComponent_t * c, int d) {
    if (!c->valid) return;
    c->step = d;
}

void topaz_component_set_drawing(topazComponent_t * c, int d) {
    if (!c->valid) return;
    c->draw = d;
}

const topazString_t * topaz_component_get_tag(const topazComponent_t * c) {
    return c->tag;
}

void topaz_component_set_tag(topazComponent_t * c, const topazString_t * s) {
    if (!c->valid) return;
    topaz_string_set(c->tag, s);
}

topazEntity_t * topaz_component_get_host(const topazComponent_t * c) {
    return c->host;
}



int topaz_component_emit_event(
    topazComponent_t * c, 
    const topazString_t * eventName, 
    topazEntity_t * source, 
    void * sourceData) 
{
    int index = (int)(intptr_t)topaz_table_find(c->events, eventName);
    if (!index) {
        #ifdef TOPAZDC_DEBUG
            printf(
                "topaz_component_emit_event: (component %s) unknown signal %s\n", 
                topaz_string_get_c_str(c->tag),
                topaz_string_get_c_str(eventName)
            );
        #endif
        return FALSE;
    }

    #ifdef TOPAZDC_DEBUG
        assert(source && "Entity source of an even emission must not be NULL.");
    #endif


    #ifdef TOPAZDC_DEBUG
        assert(index > 0 && index < topaz_array_get_size(c->eventStore));
    #endif


    EventSet * set = &topaz_array_at(c->eventStore, EventSet, index);


    int retval = TRUE;

    uint32_t handlersLen = topaz_array_get_size(set->handlers);
    EventHandler * handler;

    uint32_t i;
    if (handlersLen) {
        // reverse order
        for(i = handlersLen - 1; i >= 0; --i) {
            handler = &topaz_array_at(set->handlers, EventHandler, i);            
            if (!handler->fn(
                c,
                handler->data,
                source,
                sourceData
            )) {
                // request cancellation
                retval = FALSE;
                break;
            }
        }
    }

    uint32_t hooksLen = topaz_array_get_size(set->hooks);
    for(i = 0; i < hooksLen; ++i) {
        handler = &topaz_array_at(set->hooks, EventHandler, i);     
        handler->fn(
            c,
            handler->data,
            source,
            sourceData
        );
    }

    return retval;    
}

int topaz_component_emit_event_anonymous(topazComponent_t * c, const topazString_t * eventName) {
    return topaz_component_emit_event(
        c,
        eventName,
        TOPAZ_ENULL,
        NULL
    );
}

int topaz_component_can_handle_event(topazComponent_t * c, const topazString_t * eventName) {
    if (!c->valid) return FALSE;
    if (topaz_table_entry_exists(c->events, eventName)) {
        return TRUE;
    }
    return FALSE;
}


void topaz_component_install_hook(topazComponent_t * c, const topazString_t * eventName, topaz_event_handler fn, void * eventData) {
    if (!fn) return;
    int index = (int)(intptr_t)topaz_table_find(c->events, eventName);
    if (!index) {
        #ifdef TOPAZDC_DEBUG
            printf(
                "topaz_component_emit_event: (component %s) unknown signal %s\n", 
                topaz_string_get_c_str(c->tag),
                topaz_string_get_c_str(eventName)
            );
        #endif
        return;
    }


    #ifdef TOPAZDC_DEBUG
        assert(index > 0 && index < topaz_array_get_size(c->eventStore));
    #endif


    EventSet * set = &topaz_array_at(c->eventStore, EventSet, index);
    EventHandler handler;
    handler.fn = fn;
    handler.data = eventData;
    topaz_array_push(set->hooks, handler);
}
    

void topaz_component_uninstall_hook(topazComponent_t * c, const topazString_t * eventName, topaz_event_handler fn, void * eventData) {
    if (!fn) return;
    int index = (int)(intptr_t)topaz_table_find(c->events, eventName);
    if (!index) {
        #ifdef TOPAZDC_DEBUG
            printf(
                "topaz_component_emit_event: (component %s) unknown signal %s\n", 
                topaz_string_get_c_str(c->tag),
                topaz_string_get_c_str(eventName)
            );
        #endif
        return;
    }


    #ifdef TOPAZDC_DEBUG
        assert(index > 0 && index < topaz_array_get_size(c->eventStore));
    #endif


    EventSet * set = &topaz_array_at(c->eventStore, EventSet, index);
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(set->hooks); ++i) {
        if (topaz_array_at(set->hooks, EventHandler, i).fn   == fn &&
            topaz_array_at(set->hooks, EventHandler, i).data == eventData) {
            topaz_array_remove(set->hooks, i);
        }
    }
}

void topaz_component_install_handler(topazComponent_t * c, const topazString_t * eventName, topaz_event_handler fn, void * eventData) {
    if (!fn) return;
    int index = (int)(intptr_t)topaz_table_find(c->events, eventName);
    if (!index) {
        #ifdef TOPAZDC_DEBUG
            printf(
                "topaz_component_emit_event: (component %s) unknown signal %s\n", 
                topaz_string_get_c_str(c->tag),
                topaz_string_get_c_str(eventName)
            );
        #endif
        return;
    }


    #ifdef TOPAZDC_DEBUG
        assert(index > 0 && index < topaz_array_get_size(c->eventStore));
    #endif


    EventSet * set = &topaz_array_at(c->eventStore, EventSet, index);
    EventHandler handler;
    handler.fn = fn;
    handler.data = eventData;
    topaz_array_push(set->handlers, handler);
}

void topaz_component_uninstall_handler(topazComponent_t * c, const topazString_t * eventName, topaz_event_handler fn, void * eventData) {

    if (!fn) return;
    int index = (int)(intptr_t)topaz_table_find(c->events, eventName);
    if (!index) {
        #ifdef TOPAZDC_DEBUG
            printf(
                "topaz_component_emit_event: (component %s) unknown signal %s\n", 
                topaz_string_get_c_str(c->tag),
                topaz_string_get_c_str(eventName)
            );
        #endif
        return;
    }


    #ifdef TOPAZDC_DEBUG
        assert(index > 0 && index < topaz_array_get_size(c->eventStore));
    #endif


    EventSet * set = &topaz_array_at(c->eventStore, EventSet, index);
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(set->handlers); ++i) {
        if (topaz_array_at(set->handlers, EventHandler, i).fn   == fn &&
            topaz_array_at(set->handlers, EventHandler, i).data == eventData) {

            topaz_array_remove(set->handlers, i);
        }
    }
}


topazArray_t * topaz_component_get_known_events(topazComponent_t * c) {
    topazArray_t * out = topaz_array_create(sizeof(topazString_t*));
    if (!c->valid) return out;

    topazTableIter_t * iter = topaz_table_iter_create();
    for( topaz_table_iter_start(iter, c->events);
        !topaz_table_iter_is_end(iter);
         topaz_table_iter_proceed(iter)) {

        topazString_t * str = topaz_string_clone(
            topaz_table_iter_get_key(iter)
        );

        topaz_array_push(out, str);
    }
    topaz_table_iter_destroy(iter);
    return out;
}




void topaz_component_install_event(
    topazComponent_t * c, 
    const topazString_t * eventName, 
    topaz_event_handler handler, 
    void * data) 
{
    if (!c->valid) return;
    if (topaz_table_entry_exists(c->events, eventName)) {
        return;
    }

    EventSet * set;
    uint32_t deadLen = topaz_array_get_size(c->eventStoreDead);
    int index;
    if (deadLen) {
        index = topaz_array_at(c->eventStoreDead, int, deadLen-1);
        topaz_array_set_size(c->eventStoreDead, deadLen-1);

        set = &topaz_array_at(c->eventStore, EventSet, index);
        topaz_array_set_size(set->hooks,    0);
        topaz_array_set_size(set->handlers, 0);
    } else {
        index = topaz_array_get_size(c->eventStore);
        topaz_array_set_size(c->eventStore, index+1);

        set = &topaz_array_at(c->eventStore, EventSet, index);
        set->hooks    = topaz_array_create(sizeof(EventHandler));
        set->handlers = topaz_array_create(sizeof(EventHandler));
    }
    topaz_table_insert(c->events, eventName, (void*)(intptr_t)index);

    if (handler) {
        EventHandler h;
        h.fn = handler;
        h.data = data;

        topaz_array_push(set->handlers, h);
    }
    

}

void topaz_component_uninstall_event(topazComponent_t * c, const topazString_t * eventName) {
    if (!c->valid) return;
    int deadIndex = (int)(intptr_t)topaz_table_find(c->events, eventName);
    if (deadIndex) {
        topaz_array_push(c->eventStoreDead, deadIndex);
        topaz_table_remove(c->events, eventName);   
    }
}



