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


#ifndef H_TOPAZDC__ENTITY__INCLUDED
#define H_TOPAZDC__ENTITY__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/containers/array.h>

/*

    Entity
    -----
    
    Basic interactive object.
 
    Entities are the main classes that are extended to meet abstractions for your 
    multimedia project. By defining attributes the you can dynamically extend 
    entities. By attaching components you can extend the entity further.

    Entities are also inherently hierarchical. By Attach()ing other Entities,
    you can create trees of self-managed Entities.


*/
typedef struct topazEntity_t topazEntity_t;



typedef struct topazEntity_Attributes_t topazEntity_Logic_t;

struct topazEntity_Attributes_t {

    /// Function called when the entity is attached to a parent entity.
    ///
    void (*on_attach)(topazEntity_t *, void *);

    /// Function called when the entity is detached from a parent entity.
    ///
    void (*on_detach)(topazEntity_t *, void *);

    /// Function called just before the removal of the object.
    ///
    void (*on_remove)(topazEntity_t *, void *);

    /// Function called before the all other sub entities are step()'d.
    ///
    void (*on_pre_step)(topazEntity_t *, void *);

    /// Function called during the step() function call.
    ///
    void (*on_step)(topazEntity_t *, void *);

    /// Function called before all other sub entities are draw()n
    ///
    void (*on_pre_draw)(topazEntity_t *, void *);

    /// Function called during the draw() function call.
    ///
    void (*on_draw)(topazEntity_t *, void *);

    /// Custom data
    ///
    void * userData;

};



/// Creates a new entity.
///
topazEntity_t * topaz_entity_create();

/// Creates a new entity with attributes
///
topazEntity_t * topaz_entity_create_with_attributes(const topazEntity_Attributes_t *);


/// Sets the attributes for the entity. Attributes determine how the 
/// entity changes over time. 
void topaz_entity_set_attributes(topazEntity_t *, const topasEntity_Attributes_t *);

/// Gets the attributes of an entity.
///
topazEntity_Attributes_t topaz_entity_get_attributes(const topazEntity_t *);

/// Returns whether this instance is valid or not.
/// If the given pointer was not returned from a topaz_entity_create*() 
/// call, behavior is undefined.
///
int topaz_component_is_valid(const topazEntity_t *);



/// Flags an entity for destruction
///
void topaz_entity_remove(topazEntity_t *);

/// Returns all child entities
///
/// Entities are ordered by priority, where the first entity
/// is guaranteed to be the one with the lowest Priority value.
/// If the entity is not valid, the contents are undefined.
const topazArray_t * topaz_entity_get_children(const topazEntity_t *);

/// Updates this entity, all child entities, and components
///
void topaz_entity_step(topazEntity_t *);

/// Updates this entity, all child entities, and components
///
void topaz_entity_draw(topazEntity_t *);

/// Attaches a child entity to this entity.
///
void topaz_entity_attach(topazEntity_t *, topazEntity_t * child);

/// Detaches the entity from its parent.
///
void topaz_entity_detach(topazEntity_t *);

/// Gets the parent entity. If none returns NULL.
///
topazEntity_t * topaz_entity_get_parent(const topazEntity_t *);




/// Alters the priority of this entity.
///
/// Priorty determines the order in which this
/// entity is updated. A lower priority means it will be drawn and updated earlier.
/// It is undefined which entity is updated first if both have the same priority.
void topaz_entity_set_priority(topazEntity_t *, int);

/// Sends the entity to be the last eneity to be updated.
///
void topaz_entity_set_priority_last(topazEntity_t *);

/// Sends the entity to be the first entity to be updated.
///
void topaz_entity_set_priority_first(topazEntity_t *);

/// Gets the priority of this entity. This determines its order within its parent entity.
///
int topaz_entity_get_priority(const topazEntity_t *);




/// Returns wether or not the Engine is handling calling Step() automatically,
/// taking into account the Entity's hierarchy.
///
int topaz_entity_is_stepping(const topazEntity_t *);

/// Returns wether or not the Engine is handling calling Draw() automatically,
/// taking into account the Entity's hierarchy.
///
int topaz_entity_is_drawing(const topazEntity_t *);

/// Whether the engine should call Step() automatically for this entity.
/// Note that topaz_entity_step() calls also manage components and child entities.
/// The default is true.
///
void topaz_entity_set_stepping(topazEntity_t *, int);

/// Whether the engine should call Draw() automatically for this entity.
/// Note that topaz_entity_draw() calls also manage components and child entities.
/// The default is true.
///
void topaz_entity_set_drawing(topazEntity_t *, int);


/// Returns whether this entity is currently stepping.
///
int topaz_entity_get_stepping(const topazEntity_t *);

/// Returns whether this entity is currently drawing.
///
int topaz_entity_get_drawing(const topazEntity_t *);







/// \brief Attaches a component to this entity.
///
/// Once attached, the component's
/// Step and Draw functions will be called before this entity's Step and Draw.
/// 
void topaz_entity_add_component(topazEntity *, topazComponent_t *);

/// Same as topaz_entity_add_component(), but the component is updated 
/// after this entity's Step and Draw functions.
void topaz_entity_add_component_after(topazEntity *, topazComponent_t *);

/// Returns all components
///
const topazArray_t * topaz_entity_get_components(topazEntity_t *);

/// Returns the first component with the given tag.
///
topazComponent_t * topaz_component_query(topazEntity_t *, const topazString_t *);

/// Removes the given component from the entity.
///
void topaz_entity_remove_component(topazEntity_t *, topazComponent_t *);

/// Sets the name of the entity. Once set, it will not change.
///
void topaz_entity_set_name(topazEntity_t *, const topazString_t *);

/// Gets the name of the entity.
///
const topazString_t * topaz_entity_get_name(const topazEntity_t *);






#endif
