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
#include <topaz/vector.h>
typedef struct topazComponent_t topazComponent_t;
typedef struct topazSpatial_t topazSpatial_t;
typedef struct topaz_t topaz_t;
typedef struct topazMatrix_t topazMatrix_t;


///    Basic interactive object.
/// 
///    Entities are the main classes that are extended to meet abstractions for your 
///    multimedia project. By defining attributes the you can dynamically extend 
///    entities. By attaching components you can extend the entity further.
///
///    Entities are also inherently hierarchical. By Attach()ing other Entities,
///    you can create trees of self-managed Entities.
///
///    NULL is not accepted as an argument for any entity functions.
///    Instead, use the topaz_entity_null() value to represent this 
///    no-entity state. 
///
///
///
typedef struct topazEntity_t topazEntity_t;




/// Standard function for all entity attribute callbacks
///
typedef void (*topaz_entity_attribute_callback)(
    /// The entity interacting with the callback.
    topazEntity_t * entity, 

    /// The data bound to this callback.
    void * data
);



typedef struct topazEntity_Attributes_t topazEntity_Attributes_t;
/// Attributes that define the behavior of a 
/// an entity.
///
struct topazEntity_Attributes_t {

    /// Function called when the entity is attached to a parent entity.
    ///
    topaz_entity_attribute_callback   on_attach;

    /// Function called when the entity is detached from a parent entity.
    ///
    topaz_entity_attribute_callback   on_detach;

    /// Function called just before the removal of the object.
    ///
    topaz_entity_attribute_callback   on_remove;

    /// Function called before the all other sub entities are step()'d.
    ///
    topaz_entity_attribute_callback   on_pre_step;

    /// Function called during the step() function call.
    ///
    topaz_entity_attribute_callback   on_step;

    /// Function called before all other sub entities are draw()n
    ///
    topaz_entity_attribute_callback   on_pre_draw;

    /// Function called during the draw() function call.
    ///
    topaz_entity_attribute_callback   on_draw;

    /// Custom data
    ///
    void * userData;

};



/// Creates a new entity.
///
topazEntity_t * topaz_entity_create(
    /// The topaz context.
    topaz_t * context, 

    /// The attributes to assign to this entity.
    const topazEntity_Attributes_t * attribs
);


/// Sets a type ID for this entity.
/// This can help discern between different types 
/// of type IDs in a user-defined way.
///
void topaz_entity_set_type_id(
    /// The entity to modify.
    topazEntity_t * entity,
    
    /// The typeID of the entity. This 
    /// can only be set once.
    uint64_t typeID
);

/// Gets the type ID for this entity.
/// This can help discern between different types 
/// of type IDs in a user-defined way.
///
/// If never set, is 0.
uint64_t topaz_entity_get_type_id(
    const topazEntity_t * entity
);


/// Gets the attributes of an entity.
///
/// (No script mapping)
///
const topazEntity_Attributes_t * topaz_entity_get_attributes(
    /// The entity to query.
    const topazEntity_t * entity
);

/// Returns whether this instance is valid or not.
/// If the given pointer was not returned from a topaz_entity_create*() 
/// call, behavior is undefined.
///
int topaz_entity_is_valid(
    /// The entity to query.
    const topazEntity_t * entity
);

/// Physically destroys entities that were marked for deletion prior.
///
/// (No script mapping)
///
void topaz_entity_sweep();


/// Flags an entity for destruction. This destroys all attached components 
/// and child entities as well. The entity isn't actually destroyed until
/// topaz_entity_sweep() is called. This is normally called at the end of a context 
/// iteration for you.
///
void topaz_entity_remove(
    /// The entity to remove.
    topazEntity_t * entity
);

/// Returns all child entities
///
/// Entities are ordered by priority, where the first entity
/// is guaranteed to be the one with the lowest Priority value.
/// If the entity is not valid, the contents are undefined.
const topazArray_t * topaz_entity_get_children(
    /// The entity to query.
    const topazEntity_t * entity
);

/// Updates this entity, all child entities, and components.
/// Normally this is called for you under the topaz context.
///
void topaz_entity_step(
    /// The entity to step.
    topazEntity_t * entity
);

/// Updates this entity, all child entities, and components
/// Normally this is called for you under the topaz context.
///
void topaz_entity_draw(
    /// The entity to draw.
    topazEntity_t * entity
);

/// Attaches a child entity to this entity.
///
void topaz_entity_attach(
    /// The entity to act as parent.
    topazEntity_t * entity, 

    /// The child entity.
    topazEntity_t * child
);

/// Detaches the entity from its parent.
///
void topaz_entity_detach(
    /// The child entity to detach from the parent.
    topazEntity_t * child
);

/// Gets the parent entity. If none returns NULL.
///
topazEntity_t * topaz_entity_get_parent(
    /// The entity to query.
    const topazEntity_t * entity
);

/// Returns the first child entity with the given name.
/// Only the immediate children are checked.
///
topazEntity_t * topaz_entity_query(
    /// The parent entity to search.
    const topazEntity_t * parent, 
    /// The name to search.
    const topazString_t * name
);


/// Returns the first child entity with the given name, but 
/// searches recursively.
///
topazEntity_t * topaz_entity_search(
    /// The top-most entity to search from.
    const topazEntity_t * searchRoot, 

    /// The name to search.
    const topazString_t * name
);


/// Alters the priority of this entity.
///
/// Priorty determines the order in which this
/// entity is updated. A lower priority means it will be drawn and updated earlier.
/// It is undefined which entity is updated first if both have the same priority.
void topaz_entity_set_priority(
    /// The entity to modify.
    topazEntity_t * entity, 

    /// The new priority to set.
    int priority
);

/// Sends the entity to be the last eneity to be updated.
///
void topaz_entity_set_priority_last(
    /// The entity to modify.
    topazEntity_t * entity
);

/// Sends the entity to be the first entity to be updated.
///
void topaz_entity_set_priority_first(
    /// The entity to modify.
    topazEntity_t * entity
);

/// Gets the priority of this entity. This determines its order within its parent entity.
///
int topaz_entity_get_priority(
    /// The entity to query.
    const topazEntity_t * entity
);




/// Convenience function for getting a read-only reference to 
/// the rotation.
const topazVector_t * topaz_entity_get_rotation(
    /// The entity to query.
    const topazEntity_t * entity
);

/// Convenience function for getting a read-only reference to 
/// the position.
const topazVector_t * topaz_entity_get_position(
    /// The entity to query.
    const topazEntity_t * entity
);


/// Convenience function for getting a read-only reference to 
/// the scale.
const topazVector_t * topaz_entity_get_scale(
    /// The entity to query.
    const topazEntity_t * entity
);




/// Convenience function for getting a writeable reference to 
/// the rotation.
/// Scripting equivalent is setRotation.
///
topazVector_t * topaz_entity_rotation(
    /// The entity to modify.
    topazEntity_t * entity
);


/// Convenience function for getting a writeable reference to 
/// the position.
/// Scripting equivalent is setPosition.
///
topazVector_t * topaz_entity_position(
    /// The entity to modify.
    topazEntity_t * entity
);


/// Convenience function for getting a writeable reference to 
/// the scale.
/// Scripting equivalent is setScale.
///
topazVector_t * topaz_entity_scale(
    /// The entity to modify.
    topazEntity_t * entity
);


/// When called, the entity will orient itself so that it "looks"
/// at the given point in 3D space. This modifies the local 
/// rotation of the entity.
///
void topaz_entity_look_at(
    /// The camera to modify
    topazEntity_t * entity, 

    /// The 3D position of the focal target that the camera should look at
    const topazVector_t * target,

    /// What vector direction constitutes the "Up" position of the camera.
    /// This helps determine rotations of the camera.
    const topazVector_t * up
);



/// Gets the position of the entity taking into account parent entity transforms
///
topazVector_t topaz_entity_get_global_position(
    /// The entity to query.
    const topazEntity_t * entity
);





/// Gets a reference to the spatial reference.
///
/// (No script mapping)
///
topazSpatial_t * topaz_entity_get_spatial(
    /// The entity to query.
    const topazEntity_t * entity
);










/// Returns wether or not the Engine is handling calling Step() automatically,
/// taking into account the Entity's hierarchy.
///
int topaz_entity_is_stepping(
    /// The entity to query.
    const topazEntity_t * entity
);


/// Returns wether or not the Engine is handling calling Draw() automatically,
/// taking into account the Entity's hierarchy.
///
int topaz_entity_is_drawing(
    /// The entity to query.
    const topazEntity_t * entity
);


/// Whether the engine should call Step() automatically for this entity.
/// Note that topaz_entity_step() calls also manage components and child entities.
/// The default is true.
///
void topaz_entity_set_stepping(
    /// The entity to modify.
    topazEntity_t *, 

    /// Whether to enable stepping.
    int enable
);

/// Whether the engine should call Draw() automatically for this entity.
/// Note that topaz_entity_draw() calls also manage components and child entities.
/// The default is true.
///
void topaz_entity_set_drawing(
    /// The entity to modify.
    topazEntity_t * entity, 

    /// Whether to enable drawing.
    int enable
);


/// Returns whether this entity is currently stepping.
///
int topaz_entity_get_stepping(
    /// The entity to query.
    const topazEntity_t * entity
);


/// Returns whether this entity is currently drawing.
///
int topaz_entity_get_drawing(
    /// The entity to query.
    const topazEntity_t * entity
);


/// Returns the local transform matrix of 
/// the entity.
const topazMatrix_t * topaz_entity_get_local_matrix(
    /// The entity to query. May internally trigger updates.
    ///
    topazEntity_t * entity
);


/// Returns the global transform matrix of 
/// the entity.
const topazMatrix_t * topaz_entity_get_global_matrix(
    /// The entity to query. May internally trigger updates.
    ///
    topazEntity_t * entity
);






/// Attaches a component to this entity.
///
/// Once attached, the component's
/// Step and Draw functions will be called before this entity's Step and Draw.
/// 
void topaz_entity_add_component(
    /// The entity to add a component to.
    topazEntity_t * entity, 

    /// The component to add to the entity.
    topazComponent_t * component
);

/// Same as topaz_entity_add_component(), but the component is updated 
/// after this entity's Step and Draw functions.
void topaz_entity_add_component_after(
    /// The entity to add a component to.
    topazEntity_t * entity, 

    /// The component to add to the entity.
    topazComponent_t * component
);

/// Returns all components
///
const topazArray_t * topaz_entity_get_components(
    /// The entity to get components from.
    topazEntity_t * entity
);

/// Returns the first component with the given tag.
///
topazComponent_t * topaz_entity_query_component(
    /// The entity to query.
    topazEntity_t * entity, 

    /// The tag to search within the entity.
    const topazString_t * tag
);

/// Removes the given component from the entity.
///
void topaz_entity_remove_component(
    /// The entity to remove a component from.
    topazEntity_t * entity, 

    /// The component to remove.
    topazComponent_t * component
);

/// Sets the name of the entity. Once set, it will not change.
///
void topaz_entity_set_name(
    /// The entity to modify.
    topazEntity_t * entity, 

    /// The new name for the entity.
    const topazString_t * newName
);

/// Gets the name of the entity.
///
const topazString_t * topaz_entity_get_name(
    /// The entity to query.
    const topazEntity_t * entity
);







/// Returns the "NULL" entity, which represents an entity that doesn't 
/// exist. NULL is not accepted as an argument for any entity functions, 
/// nor should any entity functions return 0x0 NULL. Instead, 
/// topaz_entity_null() is used. topaz_entity_null() is the same for the 
/// duration of the program.
///
/// (No script mapping)
///
topazEntity_t * topaz_entity_null();

/// A define which provides an expression equivalent to topaz_entity_null()
///
#define TOPAZ_ENULL (topaz_entity_null())


#endif
