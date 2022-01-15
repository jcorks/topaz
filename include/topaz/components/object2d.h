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


#ifndef H_TOPAZDC__OBJECT2D__INCLUDED
#define H_TOPAZDC__OBJECT2D__INCLUDED

#include <topaz/component.h>
#include <topaz/vector.h>
typedef struct topazTransform_t topazTransform_t;
typedef struct topazAsset_t topazAsset_t;

///
/// Provides simple, physics-like behavior to objects, allowign for 
/// collisions and velocity control.
///
typedef struct topazObject2D_t topazObject2D_t;

/// Determines the collision group that the object is a part of.
/// Collision groups mediate which objects should collide with which.
/// Using topaz_object2d_set_group_interation(), collision groups can 
/// be set.
///
typedef enum topazObject2D_Group topazObject2D_Group;
enum topazObject2D_Group {
    /// Group A.
    topazObject2D_Group_A,

    /// Group B.
    topazObject2D_Group_B,

    /// Group C.
    topazObject2D_Group_C,

    /// Group D.
    topazObject2D_Group_D,

    /// Group E.
    topazObject2D_Group_E,

    /// Group F.
    topazObject2D_Group_F,

    /// Group G.
    topazObject2D_Group_G,

    /// Group H.
    topazObject2D_Group_H,

    /// Group I.
    topazObject2D_Group_I,

    /// Group J.
    topazObject2D_Group_J,

    /// Group K.
    topazObject2D_Group_K,

    /// Group L.
    topazObject2D_Group_L,

    /// Group M.
    topazObject2D_Group_M,

    /// Group N.
    topazObject2D_Group_N,

    /// Group O.
    topazObject2D_Group_O,

    /// Group P.
    topazObject2D_Group_P,

    /// Group Q.
    topazObject2D_Group_Q,

    /// Group R.
    topazObject2D_Group_R,

    /// Group S.
    topazObject2D_Group_S,

    /// Group T.
    topazObject2D_Group_T,

    /// Group U.
    topazObject2D_Group_U,

    /// Group V.
    topazObject2D_Group_V,

    /// Group W.
    topazObject2D_Group_W,

    /// Group X.
    topazObject2D_Group_X,

    /// Group Y.
    topazObject2D_Group_Y,

    /// Group Z.
    topazObject2D_Group_Z,
};













/// Creates a new object2d object
/// Use component_destroy to release.
///
topazComponent_t * topaz_object2d_create(
    /// The topaz context.
    topaz_t *
);


/// Compounds a velocity vector with the current velocity
/// factor is the velocity amount to add. Can be negative.
/// direction is that degree direction to apply the velocity in degrees.
///
void topaz_object2d_add_velocity(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The velocity amount to add.
    float factor, 

    /// The direction to add it in, in degrees.
    float directionDegrees
);

/// Same as topaz_object2d_add_velocity, but the direction is toward a 
/// global position.
/// factor is the amount of speed to add in that direction.
/// directionOffset is how much to offset the direction from.
///
void topaz_object2d_add_velocity_towards(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The velocity amount to add.
    float factor, 

    /// The position vector to add velocity towards, determining the direction to add velocity.
    const topazVector_t * point, 

    /// The offset from that positional direction.
    float directionOffset
);

/// Sets the current velocity
/// factor is the velocity amount to add. Can be negative.
/// direction is that degree direction to apply the velocity in degrees.
///
void topaz_object2d_set_velocity(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The amount of velocity to set as.
    float factor, 

    /// The direction in degrees to set the velocity for.
    float direction
);

/// Same as topaz_object2d_set_velocity, but the direction is toward a 
/// global position.
/// factor is the amount of speed to add in that direction.
/// directionOffset is how much to offset the direction from.
///
void topaz_object2d_set_velocity_towards(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The velocity amount to add.
    float factor, 

    /// The position vector to set velocity towards, determining the direction to set velocity.
    const topazVector_t * point, 

    /// The offset from that positional direction.
    float directionOffset
);

/// Sets the amount of resistance to speed measured as a percentile of the speed per update.
/// Every frame, only the amount percent of the host entity's velocity is retained. For example,
/// if amt is specified to be .05, every frame 5% of the Entity's speed will be lost.
/// This is applied in the horizontal direction.
///
void topaz_object2d_set_friction_x(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The mount of friction to set.
    float amt
);

/// Same as topaz_object2d_set_friction_x(), except applied in the vertical direction.
///
void topaz_object2d_set_friction_y(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The mount of friction to set.
    float amt
);


/// Gets the x direction friction. Starts at 0.
///
float topaz_object2d_get_friction_x(
    /// The object2d To query.
    const topazComponent_t * o2d
);

/// Gets the y direction friction. Starts at 0.
///
float topaz_object2d_get_friction_y(
    /// The object2d To query.
    const topazComponent_t * o2d
);


/// Returns the current direction that the entity is traveling in degrees.
///
float topaz_object2d_get_direction(
    /// The object2d to query.
    topazComponent_t * o2d
);

/// Reduces the speed of the Entity to 0.
///
void topaz_object2d_halt(
    /// The object2d to modify.
    topazComponent_t * o2d
);

/// Resets the motion path of the Object2D collider.
/// Normally, all collisions take into account the motion from previous position
/// to current. topaz_object_reset_motion() eliminates this motion, 
/// allowing for discontinuous motion.
///
void topaz_object2d_reset_motion(
    /// The object2d to reset.
    topazComponent_t * o2d
);

/// Returns the horizontal component of the current velocity.
///
float topaz_object2d_get_velocity_x(
    /// The object2d to query.
    const topazComponent_t * o2d
);

/// Returns the vertical component of the current velocity.
///
float topaz_object2d_get_velocity_y(
    /// The object2d to query.
    const topazComponent_t * o2d
);


/// Sets the vertical velocity for the component.
///
void topaz_object2d_set_velocity_x(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The new velocity.
    float amt
);

/// Sets the vertical velocity for the component.
///
void topaz_object2d_set_velocity_y(
    /// The object2d to modify.
    topazComponent_t * o2d,

    /// The new velocity amount. 
    float amt
);

/// Returns the speed of the object.
///
float topaz_object2d_get_speed(
    /// The object2d to query.
    const topazComponent_t * o2d
);


/// Sets the speed of the object in the current direction
///
void topaz_object2d_set_speed(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The new speed.
    float speed
);

/// Returns the position of the Entity after another frame of movement.
///
topazVector_t topaz_object2d_get_next_position(
    /// The object2d to query.
    topazComponent_t * o2d
);

/// Returns the previous position of the entity
///
const topazVector_t * topaz_object2d_get_last_position(
    /// The object2d to query.
    topazComponent_t * o2d
);


/// Sets the object's collision group. The default is topazObject2D_Group_A.
///
void topaz_object2d_set_group(
    /// The object2d to modify.
    topazComponent_t * o2d, 

    /// The new collision group.
    topazObject2D_Group group
);

/// Gets the object's collision group. The default is topazObject2D_Group_A.
///
topazObject2D_Group topaz_object2d_get_group(
    /// The object2d to query.
    topazComponent_t * o2d
);


/// Sets whether groups should interact. By default, only Group_A interacts with Group_A
///
void topaz_object2d_set_group_interaction(
    /// The topaz context
    topaz_t * context, 

    /// The first group in the interaction
    topazObject2D_Group a, 

    /// The second group in the interaction
    topazObject2D_Group b, 

    /// Whether to interact or not.
    int shouldInteract
);

/// Creates a collider from a collection of topazVector_t 2D point coordinates
/// It is assumed that the polygon is formed by making lines between these points 
/// in the order presented within the array. It is assumed that the center of the object 
/// is at 0, 0.
///
void topaz_object2d_set_collider(
    /// The object2d to modify.
    topazComponent_t * o2d,

    /// The new points for the collider.
    const topazArray_t * points
);


/// Creates a radial collider about 0, 0 with the given radius and 
/// number of sides.
///
void topaz_object2d_set_collider_radial(
    /// The object2d to modify.
    topazComponent_t * o2d,

    /// The radius of the new collider.
    float radius,

    /// The number of sides of the collider.
    uint32_t numSub
);

/// Returns the points that consitute the collider.
///
const topazArray_t * topaz_object2d_get_collider(
    /// The object2d to query.
    const topazComponent_t * o2d
);


/// Returns the last entity that collided with this one.
/// If none, return topaz_entity_null();
topazEntity_t * topaz_object2d_get_last_collided(
    /// The object2d to query.
    topazComponent_t * o2d
);


/// Sets whether to bind the calculated positions 
/// to whole units only. The default is false.
void topaz_object2d_set_unit_lock(
    topazComponent_t * o2d,
    int doIt
);


#endif


