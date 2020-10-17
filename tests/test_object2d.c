#include <topaz/entity.h>
#include <topaz/components/object2d.h>
#include <topaz/compat.h>
#include <topaz/topaz.h>

#include <math.h>
#include <string.h>
#include <stdint.h>


int is_close(float a, float b) {
    return (fabs(fabs(a) - fabs(b))) < 0.0001;
}


int test__object2d_simple() {
    topaz_t * ctx = topaz_context_create_empty();
    topazEntity_t * ent = topaz_entity_create(ctx);

    topazComponent_t * o2d  = topaz_object2d_create(ctx);
    topaz_entity_add_component(ent, o2d);

    // currently at 0,0
    topaz_context_step(ctx);
    if (!(
        is_close(topaz_entity_get_position(ent)->x, 0) &&
        is_close(topaz_entity_get_position(ent)->y, 0) &&
        is_close(topaz_entity_get_position(ent)->z, 0))) {
        // Why: No attributes: the default 0 position of the entity should be intact.
        // This could be indicative of an entity error as well.
        return 1;
    }

    // Speed is in "units per frame" so, after one update,
    // it should move.
    topaz_object2d_add_velocity(o2d, 1, 0);

    // apply 
    topaz_entity_step(ent);    

    if (!(
        is_close(topaz_entity_get_position(ent)->x, 1) &&
        is_close(topaz_entity_get_position(ent)->y, 0)
    )) {
        // why: the entity should have traveled one unit in the 0-degree direction, which 
        // is to the right.
        return 2;
    }

    if (!is_close(topaz_object2d_get_direction(o2d), 0)) {
        // why: We set the direction to 0 degrees
        return 3;        
    }


    // since position y points DOWN, we 
    topaz_object2d_set_velocity(o2d, 1, 90);
    
    if (!is_close(topaz_object2d_get_direction(o2d), 90)) {
        // why: We set the direction to 90 degrees
        return 4;        
    }

    // apply
    topaz_entity_step(ent);
   
    if (!(
        is_close(topaz_entity_get_position(ent)->x, 1) &&
        is_close(topaz_entity_get_position(ent)->y, 1)
    )) {
        // the new direction should have made sure that we were heading in the pos y direction
        return 5;
    }

    if (!(
        is_close(topaz_object2d_get_velocity_x(o2d), 0) &&
        is_close(topaz_object2d_get_velocity_y(o2d), 1)
    )) {
        // Why: should both be one at this point
        return 6;
    }


    // try to negate the velocity by 
    // going back to the origin
    topazVector_t origin = {1, 0, 0};
    topaz_object2d_add_velocity_towards(o2d, 1, &origin, 0);

    topaz_entity_step(ent);

        
    if (!(
        is_close(topaz_object2d_get_velocity_x(o2d), 0) &&
        is_close(topaz_object2d_get_velocity_y(o2d), 0)
    )) {
        // Why: topaz_object2d_add_velocity_towards should have made the 
        // entity go in the direction of the point (1, 0) with a speed of 1,
        // canceling out the speed of (0, 1)
        return 7;
    }
    
    // go back to 2, 1
    topaz_object2d_set_velocity(o2d, sqrt(2), 45);

    // but put friction 
    topaz_object2d_set_friction_x(o2d, .5);
    topaz_object2d_set_friction_y(o2d, .5);

    // update
    topaz_entity_step(ent);

        
    if (!(
        is_close(topaz_object2d_get_velocity_x(o2d), .5) &&
        is_close(topaz_object2d_get_velocity_y(o2d), .5)
    )) {
        // Why: friction should have reduced x and y vel
        return 8;
    }

    if (!(
        is_close(topaz_object2d_get_direction(o2d), 45)
    )) {
        // Why, velocit is .5, .5, which would put the direction in 
        // 45 degrees (pos x, y of equal scale)
        return 9;
    }
    

    if (!(
        is_close(topaz_entity_get_position(ent)->x, 1.5) &&
        is_close(topaz_entity_get_position(ent)->y, 1.5)
    )) {
        // Why: friction reduces the velocity per update. This velocity 
        // reduction is applied BEFORE the position update.
        return 10;
    }

    // reset friction
    topaz_object2d_set_friction_x(o2d, 0);
    topaz_object2d_set_friction_y(o2d, 0);

    topaz_entity_remove(ent);
    topaz_context_destroy(ctx);

    return 0;
}


int test__object2d_advanced() {
    

    return 0;
}
