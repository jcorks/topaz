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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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

    } else {
        out = calloc(1, sizeof(topazComponent_t));
        out->host = TOPAZ_ENULL;
        out->events = topaz_table_create_hash_string();
        out->eventStore = topaz_array_new(sizeof(EventSet));
    }
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


topazComponent_t topaz_component_null() {
    static topazComponent_t * c = NULL;
    if (!c) {
        c = calloc(1, sizeof(topazComponent_t));
        c->events = topaz_table_create_hash_string();
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
    topaz_table_clear(c->events);
    c->step = FALSE;
    c->draw = FALSE;
    c->valid = FALSE;
    topaz_array_push(dead, c);
}


void topaz_component_attach(topazComponent_t * c, topazEntity_t * e) {
    if (!c->valid) return;
    
}

/// Detaches the component from an entity. This triggers the detach event.
///
void topaz_component_detach(topazComponent_t *) {
    topaz_component_emit_event(c, TOPAZ_STR_CAST("on-detach"), e, NULL);
    
}



/// Steps the component.
///
void topaz_component_step(topazComponent_t *);

/// Draws the component.
///
void topaz_component_draw(topazComponent_t *);



/// Gets whether this component will update when a step 
/// is requested.
///
int topaz_component_get_stepping(const topazComponent_t *);

/// Gets whether this component will draw when a draw
/// is requested.
///
int topaz_component_get_drawing(const topazComponent_t *);



/// Sets whether this component will step.
///
void topaz_component_set_stepping(topazComponent_t *, int);

/// Sets whether this component will draw.
///
void topaz_component_set_drawing(topazComponent_t *, int);

/// Returns the tag for the component.
///
const topazString_t * topaz_component_get_tag();

/// Sets the tag for the component.
///
void topaz_component_set_tag(topazComponent_t *, const topazString_t *);

/// Returns the set host of the component. If no host is set,
/// nullptr is returned.
///
topazEntity_t * topaz_component_get_host(const topazComponent_t *);





/// Function type for all event signals.
///
typedef int (*topaz_event_handler)(
    /// The component instance handling this event.
    ///
    topazComponent_t * component, 

    /// The user data specifified when setting this handler.
    ///
    void * dataEvent,

    /// The entity that triggered this event.
    /// 
    topazEntity_t * source, 

    /// Data sent when the emitted event.
    ///
    void * sourceEvent
);



/// Triggers the specified event for this EventSystem
///
/// Returns whether the event was allowed to propogate all the
/// way to the main handler (in other words, lets you know whether
/// all the handlers allowed the event to happen)
/// Source is passed to the handler and is usually the source of the event (but does not have to be)
/// @param eventName Name of the event. This should match the name that was installed.
/// @param source Optional ID that indicates the source of the event. For example, in a collision, this may be the object collided with.
/// @param args Optional string vector with additional information to be used by the event.
int topaz_component_emit_event(topazComponent_t *, const topazString_t * eventName, topazEntity_t * source, void * sourceData);

/// Same as topaz_component_emit_event() but the source entity and source data are NULL
///
int topaz_component_emit_event_anonymous(topazComponent_t *, const topazString_t * eventName);

/// Returns whether there exists at least one handler for the given event
///
int topaz_component_can_handle_event(topazComponent_t *, const topazString_t * eventName);


/// Adds a hook to the event. 
///
/// A hook happens at the end of a given event after all the
/// handlers have been run. Hooks occur regardless of event handler propogation.
/// (the return value is ignored for all hooks)
void topaz_component_install_hook(topazComponent_t *, const topazString_t * eventName, topaz_event_handler, void * eventData);
    
/// Removes a hook added with InstallHook()
///
void topaz_component_uninstall_hook(topazComponent_t *, const topazString_t * eventName, topaz_event_handler, void *);

/// Adds a handler to an event. 
///
/// Handlers that are added are run in LIFO order
/// and their return values dictate whether the event should propogate.
/// the last handler run for an event is always the main handler of the event.
void topaz_component_install_handler(topazComponent_t *, const topazString_t * eventName, topaz_event_handler, void *);

/// Removes a handler added with InstallHandler()
///
void topaz_component_uninstall_handler(topazComponent_t *, const topazString_t * eventName, topaz_event_handler, void *);


/// Returns a list of event names that this eventsystem is able to process
///
topazArray_t * topaz_component_get_known_events(topazComponent_t *);




/// Adds a new event to be recognized by the EventSystem.
///
/// if mainHandler is nullptr, the event is still added, but has no default
/// handler is set. The default handler is always guaranteed to run first for the event.
void topaz_component_install_event(topazComponent_t *, const topazString_t * eventName, topaz_event_handler, void *);

/// removes a handler of an event
///
void topaz_component_uninstall_event(topazComponent_t *, const topazString_t * eventName);



