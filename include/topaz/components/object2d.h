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
/*

    Object2D
    -----

    Contains simple 2D movement and collision detection.

*/



/// Determines the collision group that the object is a part of.
/// Collision groups mediate which objects should collide with which.
/// Using topaz_object2d_set_group_interation(), collision groups can 
/// be set.
///
typedef enum {
    topazObject2D_Group_A,
    topazObject2D_Group_B,
    topazObject2D_Group_C,
    topazObject2D_Group_D,
    topazObject2D_Group_E,
    topazObject2D_Group_F,
    topazObject2D_Group_G,
    topazObject2D_Group_H,
    topazObject2D_Group_I,
    topazObject2D_Group_J,
    topazObject2D_Group_K,
    topazObject2D_Group_L,
    topazObject2D_Group_M,
    topazObject2D_Group_N,
    topazObject2D_Group_O,
    topazObject2D_Group_P,
    topazObject2D_Group_Q,
    topazObject2D_Group_R,
    topazObject2D_Group_S,
    topazObject2D_Group_T,
    topazObject2D_Group_U,
    topazObject2D_Group_V,
    topazObject2D_Group_W,
    topazObject2D_Group_X,
    topazObject2D_Group_Y,
    topazObject2D_Group_Z,
} topazObject2D_Group;













/// Creates a new object2d object
/// Use component_destroy to release.
///
topazComponent_t * topaz_object2d_create(topaz_t *);


/// Compounds a velocity vector with the current velocity
/// factor is the velocity amount to add. Can be negative.
/// direction is that degree direction to apply the velocity in degrees.
///
void topaz_object2d_add_velocity(topazComponent_t *, float factor, float directionDegrees);

/// Same as topaz_object2d_add_velocity, but the direction is toward a 
/// global position.
/// factor is the amount of speed to add in that direction.
/// directionOffset is how much to offset the direction from.
///
void topaz_object2d_add_velocity_towards(topazComponent_t *, float factor, const topazVector_t * point, float directionOffset);

/// Sets the current velocity
/// factor is the velocity amount to add. Can be negative.
/// direction is that degree direction to apply the velocity in degrees.
///
void topaz_object2d_set_velocity(topazComponent_t *, float vector, float direction);

/// Same as topaz_object2d_set_velocity, but the direction is toward a 
/// global position.
/// factor is the amount of speed to add in that direction.
/// directionOffset is how much to offset the direction from.
///
void topaz_object2d_set_velocity_towards(topazComponent_t *, float factor, const topazVector_t * point, float directionOffset);

/// Sets the amount of resistance to speed measured as a percentile of the speed per update.
/// Every frame, only the amount percent of the host entity's velocity is retained. For example,
/// if amt is specified to be .05, every frame 5% of the Entity's speed will be lost.
/// This is applied in the horizontal direction.
///
void topaz_object2d_set_friction_x(topazComponent_t *, float amt);

/// Same as topaz_object2d_set_friction_x(), except applied in the vertical direction.
///
void topaz_object2d_set_friction_y(topazComponent_t *, float amt);

/// Returns the current direction that the entity is traveling in degrees.
///
float topaz_object2d_get_direction(topazComponent_t *);

/// Reduces the speed of the Entity to 0.
///
void topaz_object2d_halt(topazComponent_t *);

/// Resets the motion path of the Object2D collider.
/// Normally, all collisions take into account the motion from previous position
/// to current. topaz_object_reset_motion() eliminates this motion, 
/// allowing for discontinuous motion.
///
void topaz_object2d_reset_motion(topazComponent_t *);

/// Returns the horizontal component of the current velocity.
///
float topaz_object2d_get_velocity_x(const topazComponent_t *);

/// Returns the vertical component of the current velocity.
///
float topaz_object2d_get_velocity_y(const topazComponent_t *);

/// Sets the vertical velocity for the component.
///
void topaz_object2d_set_velocity_x(topazComponent_t *, float);

/// Sets the vertical velocity for the component.
///
void topaz_object2d_set_velocity_y(topazComponent_t *, float);

/// Returns the speed of the object.
///
float topaz_object2d_get_speed(const topazComponent_t *);

/// Sets the speed of the object in the current direction
///
void topaz_object2d_set_speed(topazComponent_t *, float);

/// Returns the position of the Entity after another frame of movement.
///
topazVector_t topaz_object2d_get_next_position(topazComponent_t *);

/// Returns the previous position of the entity
///
const topazVector_t * topaz_object2d_get_last_position(topazComponent_t *);


/// Sets the object's collision group. The default is topazObject2D_Group_A.
///
void topaz_object2d_set_group(topazComponent_t *, topazObject2D_Group);

/// Sets whether groups should interact. By default, only Group_A interacts with Group_A
///
void topaz_object2d_set_group_interaction(topaz_t *, topazObject2D_Group, topazObject2D_Group, int shouldInteract);

/// Creates a collider from a collection of topazVector_t 2D point coordinates
/// It is assumed that the polygon is formed by making lines between these points 
/// in the order presented within the array. It is assumed that the center of the object 
/// is at 0, 0.
///
void topaz_object2d_set_collider(
    topazComponent_t *,
    const topazArray_t * points
);


/// Creates a radial collider about 0, 0 with the given radius and 
/// number of sides.
///
void topaz_object2d_set_collider_radial(
    topazComponent_t *,
    float radius,
    uint32_t numSub
);

/// Returns the points that consitute the collider.
///
const topazArray_t * topaz_object2d_get_collider(
    const topazComponent_t *
);


/// Returns the last entity that collided with this one.
/// If none, return topaz_entity_null();
topazEntity_t * topaz_object2d_get_last_collided(
    topazComponent_t *
);



#endif


