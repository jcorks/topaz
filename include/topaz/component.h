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


#ifndef H_TOPAZDC__COMPONENT__INCLUDED
#define H_TOPAZDC__COMPONENT__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
typedef struct topaz_t topaz_t;
typedef struct topazEntity_t topazEntity_t;

/// Extends the functionality of Entities.
///
/// Each component can hold its own update cycle (step / draw)
/// as well as implement a generic event / callback system, 
/// allowing for extensions of all kinds.
///
typedef struct topazComponent_t topazComponent_t;


/// Standard callback for all component attributes.
///
typedef void (*topaz_component_attribute_callback)(
    /// The component revieing the callback
    topazComponent_t * component, 

    /// Data bound to the callback.
    void * cbData
);




typedef struct topazComponent_Attributes_t topazComponent_Attributes_t;
/// Attributes of the component. Attributes define a baseline behavior 
/// in response to common events. 
///
struct topazComponent_Attributes_t {

    /// Function called when stepping
    ///
    topaz_component_attribute_callback on_step;

    /// Function called when drawing
    ///
    topaz_component_attribute_callback on_draw;

    /// Function called when attaching the component to the entity.
    ///
    topaz_component_attribute_callback on_attach;

    /// Function called when attaching the component to the entity.
    ///
    topaz_component_attribute_callback on_detach;

    /// Function called before destroying the component.
    ///
    topaz_component_attribute_callback on_destroy;





    /// User data passed as argument to all attribute functions.
    ///
    void * userData;
};



/// Creates a new component
///
topazComponent_t * topaz_component_create(
    /// Helps uniquely identify the component
    const topazString_t * tagName, 
    /// The topaz context.
    topaz_t * context,
    /// The attributes to define the component with.     
    const topazComponent_Attributes_t * attribs
);



/// Returns the attributes of the component.
///
const topazComponent_Attributes_t * topaz_component_get_attributes(
    /// The component to query.
    const topazComponent_t * component    
);







/// Destroys a component. This detaches the component from the entity 
/// which emits the detach event.
///
void topaz_component_destroy(
    /// the component to remove.
    topazComponent_t * component
);


/// Attaches the component to an entity. This triggers the attach event.
///
void topaz_component_attach(
    /// The component to attach to an entity.
    topazComponent_t * component,

    /// The entity to attach to.
    topazEntity_t * entity
);

/// Detaches the component from an entity. This triggers the detach event.
///
void topaz_component_detach(
    /// The component to detach from its host.
    topazComponent_t * component
);



/// Steps the component. Normally called for you if attached to a host.
///
void topaz_component_step(
    /// The component to step.
    topazComponent_t * component
);

/// Draws the component. Normally called for you if attached to a host.
///
void topaz_component_draw(
    /// The component to draw.
    topazComponent_t * component
);



/// Gets whether this component will update when a step 
/// is requested.
///
int topaz_component_get_stepping(
    /// The component to query.
    const topazComponent_t * component
);

/// Gets whether this component will draw when a draw
/// is requested.
///
int topaz_component_get_drawing(
    /// The component to query.
    const topazComponent_t * component
);



/// Sets whether this component will step.
///
void topaz_component_set_stepping(
    /// The component to modify.
    topazComponent_t * component, 

    /// Whether to enable.
    int trueOrFalse
);

/// Sets whether this component will draw.
///
void topaz_component_set_drawing(
    /// The component to modify.
    topazComponent_t *, 

    /// Whether to enable.
    int trueOrFalse
);

/// Returns the tag for the component.
///
const topazString_t * topaz_component_get_tag(
    /// The component to query.
    const topazComponent_t * component
);

/// Sets the tag for the component.
///
void topaz_component_set_tag(
    /// The component to modify.
    topazComponent_t * component, 
    
    /// The tag to set.
    const topazString_t * newTag
);


/// Returns the set host of the component. If no host is set,
/// nullptr is returned.
///
topazEntity_t * topaz_component_get_host(
    /// The component to query.
    const topazComponent_t * component
);

/// Returns the context that this component belongs in.
///
topaz_t * topaz_component_get_context(
    /// The component to query.
    const topazComponent_t * component
);

/// Returns the emtpy component. This value is consistent across all 
/// invocations.
///
topazComponent_t * topaz_component_null();

/// Macro that can be used as an expression instead of the call 
/// topaz_component_null() 
///
#define TOPAZ_CNULL (topaz_component_null())


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



/// Triggers the specified event for this Component
///
/// Returns whether the event was allowed to propogate all the
/// way to the main handler (in other words, lets you know whether
/// all the handlers allowed the event to happen)
/// Source is passed to the handler and is usually the source of the event (but does not have to be)
int topaz_component_emit_event(
    /// The component to emit the event from
    topazComponent_t * component, 

    /// Name of the event. This should match the name that was installed.
    const topazString_t * eventName, 

    /// Entity that indicates the source of the event. For example, in a collision, this may be the object collided with.
    topazEntity_t * source, 

    /// Optional string vector with additional information to be used by the event.
    void * sourceData
);

/// Same as topaz_component_emit_event() but the source entity and source data are NULL
///
int topaz_component_emit_event_anonymous(
    /// The component to emit the event from.
    topazComponent_t * component, 

    /// The name of the event to emit.
    const topazString_t * eventName
);

/// Returns whether there exists at least one handler for the given event
///
int topaz_component_can_handle_event(
    /// The component to query.
    topazComponent_t * component, 

    /// The event name to query.
    const topazString_t * eventName
);


/// Adds a hook to the event. 
///
/// A hook happens at the end of a given event after all the
/// handlers have been run. Hooks occur regardless of event handler propogation.
/// (the return value is ignored for all hooks)
uint32_t topaz_component_install_hook(
    /// The component to install a hook within.
    topazComponent_t * component, 

    /// The event to add the hook to.
    const topazString_t * eventName, 

    /// The handler that will be called when the event is triggered.
    topaz_event_handler hook, 

    /// The data to bind with the hook when calling it.
    void * eventData
);
    
/// Removes a hook added with InstallHook()
///
void topaz_component_uninstall_hook(
    /// The component to remove the hook from.
    topazComponent_t * component, 

    /// The name of the event 
    const topazString_t * eventName, 

    /// The index of the hook to remove.
    uint32_t index
);

/// Adds a handler to an event. 
///
/// Handlers that are added are run in LIFO order
/// and their return values dictate whether the event should propogate.
/// the last handler run for an event is always the main handler of the event.
uint32_t topaz_component_install_handler(

    /// The component to install a handler into.
    topazComponent_t * component, 

    /// The event name to install to.
    const topazString_t * eventName, 

    /// The handler called when handling the event.
    topaz_event_handler handler, 

    /// The data to bind with the handler.
    void * handlerData
);

/// Removes a handler added with InstallHandler()
///
void topaz_component_uninstall_handler(
    /// The component to remove the handler from.
    topazComponent_t * component, 

    /// The event name to uninstall the handler from.
    const topazString_t * eventName, 

    /// The index of the handler.
    uint32_t index
);


/// Returns a list of event names that this component is able to process
/// The array needs to be freed by the caller.
///
topazArray_t * topaz_component_get_known_events(
    /// The component to query.
    topazComponent_t * component
);




/// Adds a new event to be recognized by the Component.
///
/// if mainHandler is nullptr, the event is still added, but has no default
/// handler is set. The default handler is always guaranteed to run first for the event.
void topaz_component_install_event(
    /// The component to install an event to.
    topazComponent_t * component, 

    /// The name of the event.
    const topazString_t * eventName, 

    /// The default handler for the new event.
    topaz_event_handler defaultHandler, 

    /// The data to bind with the default handler.
    void * handlerData
);

/// removes a handler of an event
///
void topaz_component_uninstall_event(
    /// The component to remove an event from.
    topazComponent_t * component, 

    /// The event to remove from the handler.
    const topazString_t * eventName
);






#endif
