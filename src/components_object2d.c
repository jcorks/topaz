#include <topaz/components/object2d.h>
#include <topaz/components/shape2d.h>
#include <topaz/containers/array.h>
#include <topaz/containers/table.h>
#include <topaz/containers/string.h>
#include <topaz/entity.h>
#include <topaz/component.h>
#include <topaz/math.h>
#include <topaz/matrix.h>
#include <topaz/spatial.h>
#include <topaz/topaz.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
// magic number thats checked to verify all components 
// using state control functions are actually what they claim to be
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__OBJECT_2D = "t0p4z0bj3ct2d";
#endif


typedef struct TopazObject2D_t TopazObject2D_t;

/////// bounding box 
/*
    Simple box / point operations 

*/

typedef struct {
    float x;
    float y;
    float width;
    float height;
} bbox_t;

// Returns whether the given point is within the bounding box
//static int bb_contains_point(const bbox_t *, const topazVector_t *);

// Returns whether box b is contained within box a
//static int bb_contains_box(const bbox_t * a, const bbox_t * b);

// Returns whether 2 boxes overlap
static int bb_overlaps(const bbox_t * a, const bbox_t * b);        
    





/////// tcollider_t 
/*
    Individualized collider for each component.

*/

typedef struct tcollider_t tcollider_t;

// creates a new collider
tcollider_t * collider_create();

// Destroys a collider
static void collider_destroy(tcollider_t *);

// Sets the collider geometry
static void collider_set_from_points(tcollider_t *, const topazArray_t *);

// Returns whether the given point will be contained within 
// the collider            
static int collider_will_contain_point(const tcollider_t *, const topazVector_t * other);

// Updates the transition of a collider
static void collider_update_transition(tcollider_t *, const topazVector_t * after);

// Returns whether the colliders in the current state will collide
// and if so, the general area where it will happen
static int collides_with(const tcollider_t *, const tcollider_t * otherd, topazVector_t *);

// Gets the last entity and position that was collided with
static topazEntity_t * collider_get_last_collided(const tcollider_t *, topazVector_t *);

// Sets the entity last collided with
static void collider_set_last_collided(tcollider_t *, topazEntity_t *, const topazVector_t *);

// Gets the bounding box that represents the area the collider covers
const bbox_t * collider_get_moment_bounds(const tcollider_t *);

// returns the points used to first generate the collider in question.
const topazArray_t * collider_get_source_points(const tcollider_t *);

///// SpatialMap
/*
    Contains different regions of rectangles 
    and returns which, if any, of these rectangles
    would intersect with the ones given


*/
typedef struct smap_t smap_t;

// Creates a spatial map instance
static smap_t * spatial_map_create();

// Destroys a spatial map instance 
static void spatial_map_destroy(smap_t *);


// Resets a spatial map based on the span of all objects to be contained within it.
static void spatial_map_reset(
    smap_t *,
    float spanX_, 
    float spanY_, 
    float spanW_, 
    float spanH_, 
    uint32_t estimateNumberwithinContainer,
    uint32_t MaxID
);

// Inserts a region covered in the spatial map and associates 
// that region with an ID.
static void spatial_map_insert(
    smap_t *,
    const bbox_t *,
    uint32_t ,
    uint8_t
);

typedef struct {
    // object index that collided from 
    uint32_t self;

    // object index that collided with
    uint32_t other;

} smapCollision_t;

// Returns an array of all possible collisions determined 
// from the spatial map.
static topazArray_t * spatial_map_get_collisions(smap_t * m);



///// T2DManager 
/*
    The manager controls the update cycle for all 
    object2d instances within the same topaz instance.

    The object cycle of the manager is when the positions 
    of the host of the 2d objects are updated according
    to object2d's interactions.

*/

// Gets the topaz 2d object manager for this topaz instance
static topazEntity_t * t2dm_fetch(topaz_t *);

// registers an object with the 
static void t2dm_register_object(topazEntity_t *, topazComponent_t *);

// unregisters an object. When a 2d manager unregisters its 
// last object 
static void t2dm_unregister_object(topazEntity_t *, topazComponent_t *);

// Sets the group interaction.
static void t2dm_set_interaction(topazEntity_t *, topazObject2D_Group a, topazObject2D_Group b, int);





struct TopazObject2D_t {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    float speedX;
    float speedY;
    float frictionX;
    float frictionY;
    topazVector_t last;
    topaz_t * ctx;
    topazEntity_t * manager;
    tcollider_t * collider;
    topazObject2D_Group group;
    int id;
    int16_t unitLock;
    int16_t active;
};




static TopazObject2D_t * object2d__retrieve(topazComponent_t * c) {
    TopazObject2D_t * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "Object2D instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__OBJECT_2D);
    #endif
    return s;
}

static void object2d__on_step(topazComponent_t * o, TopazObject2D_t * object) {
    topazEntity_t * host = topaz_component_get_host(o);
    topazVector_t gpos = topaz_entity_get_global_position(host);
    topazVector_t delta;
    topazVector_t next = topaz_object2d_get_next_position(o);
    delta.x = next.x - gpos.x;
    delta.y = next.y - gpos.y;
    delta.z = 0;
    if (object->active == 0) {
        t2dm_register_object(object->manager, o);
    }
    object->active = 1;
    object->last = *topaz_entity_get_position(host);

    // using the "last" model, we include manual translations as part of 
    // normal collisions.
    if (topaz_vector_get_length(&delta) > .000001) {
        topazVector_t * pos = topaz_entity_position(host);
        pos->x += delta.x;
        pos->y += delta.y;

        if (object->unitLock) {
            pos->x = (int)pos->x;
            pos->y = (int)pos->y;
        }

        // push it through        
        topaz_spatial_check_update(topaz_entity_get_spatial(host));
    }


    object->speedX *= (1.0 - object->frictionX);
    object->speedY *= (1.0 - object->frictionY);
    
}

static void object2d__on_attach(topazComponent_t * c, TopazObject2D_t * s) {
    // prevents weird collisions on frame one of stepping
    s->last = *topaz_entity_get_position(topaz_component_get_host(c));
}



static void object2d__on_detach(topazComponent_t * c, TopazObject2D_t * s) {
    if (s->active > 0)
        t2dm_unregister_object(s->manager, c);
}

static void object2d__on_destroy(topazComponent_t * c, TopazObject2D_t * s) {
    collider_destroy(s->collider);
    free(s);
}



topazComponent_t * topaz_object2d_create(topaz_t * t) {
    TopazObject2D_t * data = calloc(1, sizeof(TopazObject2D_t));
    data->ctx = t;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__OBJECT_2D;
    #endif
    data->manager = t2dm_fetch(t);
    data->collider = collider_create();
    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));
    attribs.on_destroy  = (topaz_component_attribute_callback) object2d__on_destroy;    
    attribs.on_detach   = (topaz_component_attribute_callback) object2d__on_detach;    
    attribs.on_attach   = (topaz_component_attribute_callback) object2d__on_attach;
    attribs.on_step     = (topaz_component_attribute_callback) object2d__on_step;
    attribs.userData = data;
    topazComponent_t * out = topaz_component_create_with_attributes(TOPAZ_STR_CAST("Object2D"), t, &attribs);

    topaz_component_install_event(out, TOPAZ_STR_CAST("on-collide"), NULL, NULL);
    static int idp = 1;
    data->id = idp++;    
    return out;
}


void topaz_object2d_add_velocity(
    topazComponent_t * c, 
    float factor, 
    float direction
) {
    TopazObject2D_t * s = object2d__retrieve(c);

    s->speedX += factor * (cos((topaz_math_pi / 180.f) * direction));
    s->speedY += factor * (sin((topaz_math_pi / 180.f) * direction));
}



void topaz_object2d_add_velocity_towards(
    topazComponent_t * c, 
    float factor, 
    const topazVector_t * p, 
    float directionOffset
) {
    TopazObject2D_t * s = object2d__retrieve(c);

    topazVector_t src = {0};

    src = topaz_matrix_transform(
        topaz_spatial_get_global_transform(
            topaz_entity_get_spatial(
                topaz_component_get_host(c)
            )
        ),
        &src
    );
    

    float direction = topaz_vector_point_at_2d(&src, p) + directionOffset;

    s->speedX += factor * (cos((topaz_math_pi / 180.f) * direction));
    s->speedY += factor * (sin((topaz_math_pi / 180.f) * direction));
}



void topaz_object2d_set_velocity(
    topazComponent_t * c, 
    float factor, 
    float direction
) {
    TopazObject2D_t * s = object2d__retrieve(c);

    s->speedX = factor * (cos((topaz_math_pi / 180.f) * direction));
    s->speedY = factor * (sin((topaz_math_pi / 180.f) * direction));

}

void topaz_object2d_set_velocity_towards(
    topazComponent_t * c, 
    float factor, 
    const topazVector_t * p, 
    float directionOffset
) {
    TopazObject2D_t * s = object2d__retrieve(c);

    topazVector_t src = topaz_entity_get_global_position(topaz_component_get_host(c));
    
    float direction = topaz_vector_point_at_2d(&src, p) + directionOffset;

    s->speedX = factor * (cos((topaz_math_pi / 180.f) * direction));
    s->speedY = factor * (sin((topaz_math_pi / 180.f) * direction));
}


/// Sets whether to bind the calculated positions 
/// to whole units only. The default is false.
void topaz_object2d_set_unit_lock(
    topazComponent_t * c,
    int doIt
) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->unitLock = doIt;    
}



void topaz_object2d_set_friction_x(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->frictionX = amt;
}

void topaz_object2d_set_friction_y(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->frictionY = amt;
}

float topaz_object2d_get_friction_x(const topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve((topazComponent_t *)c);    
    return s->frictionX;
}

float topaz_object2d_get_friction_y(const topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve((topazComponent_t *)c);    
    return s->frictionY;
}


float topaz_object2d_get_direction(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    topazVector_t speed = {s->speedX, s->speedY, 0};
    topazVector_t origin = {};
    return topaz_vector_point_at_2d(&origin, &speed);
}

void topaz_object2d_halt(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->speedX = 0;
    s->speedY = 0;
}

void topaz_object2d_reset_motion(topazComponent_t * c) {
    topazEntity_t * e = topaz_component_get_host(c);
    if (!e) return;
    TopazObject2D_t * object = object2d__retrieve(c);    
    // essentially removes the smear
    topazVector_t v = *topaz_entity_get_position(e);
    collider_update_transition(
        object->collider,
        &v
    );        

}

float topaz_object2d_get_velocity_x(const topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve((topazComponent_t *)c);    
    return s->speedX;
}

float topaz_object2d_get_velocity_y(const topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve((topazComponent_t *)c);    
    return s->speedY;
}

void topaz_object2d_set_velocity_x(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->speedX = amt;
}

void topaz_object2d_set_velocity_y(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->speedY = amt;
}

float topaz_object2d_get_speed(const topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve((topazComponent_t *)c);    
    topazVector_t v = {s->speedX, s->speedY, 0};
    return topaz_vector_get_length(&v);
}

void topaz_object2d_set_speed(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    topazVector_t v = {s->speedX, s->speedY, 0};
    topaz_vector_normalize(&v);
    v.x *= amt;
    v.y *= amt;
    s->speedX = v.x;
    s->speedY = v.y;
}

topazVector_t topaz_object2d_get_next_position(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    topazVector_t newPos;

    topazVector_t p = topaz_entity_get_global_position(topaz_component_get_host(c));


    newPos.x = p.x + s->speedX*(1.0 - s->frictionX);
    newPos.y = p.y + s->speedY*(1.0 - s->frictionY);

    return newPos;
}

const topazVector_t * topaz_object2d_get_last_position(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    return &s->last;
}

void topaz_object2d_set_group(topazComponent_t * c, topazObject2D_Group g) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->group = g;    
}

topazObject2D_Group topaz_object2d_get_group(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    return s->group; 
}


void topaz_object2d_set_group_interaction(topaz_t * t, topazObject2D_Group a, topazObject2D_Group b, int doIt) {
    topazEntity_t * m = t2dm_fetch(t);
    t2dm_set_interaction(m, a, b, doIt); 
}

void topaz_object2d_set_collider(
    topazComponent_t * c,
    const topazArray_t * points
) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    collider_set_from_points(s->collider, points);    
}


void topaz_object2d_set_collider_radial(
    topazComponent_t * c,
    float radius,
    uint32_t numPts
) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    topazVector_t * inpts = calloc(numPts, sizeof(topazVector_t));
     
    float RADIAN_MAX = 3.14159265359*2;
    for(uint32_t i = 0; i < numPts; ++i) {
        inpts[i].x = cos((i / (float)numPts)*RADIAN_MAX) * radius;
        inpts[i].y = sin((i / (float)numPts)*RADIAN_MAX) * radius;
    }
    collider_set_from_points(s->collider, TOPAZ_ARRAY_CAST(inpts, topazVector_t, numPts));
    free(inpts);
}

const topazArray_t * topaz_object2d_get_collider(
    const topazComponent_t * c
) {
    TopazObject2D_t * s = object2d__retrieve((topazComponent_t*)c);    
    return collider_get_source_points(s->collider);
}



topazEntity_t * topaz_object2d_get_last_collided(
    topazComponent_t * c,
    topazVector_t * p
) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    return collider_get_last_collided(s->collider, p);
}










/////////////////////////////////
///// collider 

typedef struct {
    topazVector_t a;
    topazVector_t b;
} cline_t;


// returns whether 2 lines intersect. Used extensively for colliders
static int line_intersect(const cline_t * l0, const cline_t * l1, topazVector_t * intersect) {
    // "LeMothe's Method"
    float s1_x = l0->b.x - l0->a.x;
    float s1_y = l0->b.y - l0->a.y;
    
    
    float s2_x = l1->b.x - l1->a.x;
    float s2_y = l1->b.y - l1->a.y;

    float s = (- s1_y * (l0->a.x - l1->a.x) + s1_x * (l0->a.y - l1->a.y)) / (-s2_x * s1_y + s1_x * s2_y);
    float t = (  s2_x * (l0->a.y - l1->a.y) - s2_y * (l0->a.x - l1->a.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        intersect->x = l0->a.x + (t * s1_x);
        intersect->y = l0->a.y + (t * s1_y);
        return 1;
    }
    return 0;
}


struct tcollider_t {
    topazArray_t * staticPoints;
    topazArray_t * smear;
    topazArray_t * srcPoints;

    bbox_t smearBounds;
    topazVector_t oldPosition;
    int stationary;
    int isSet;
    topazEntity_t * lastCollided;
    topazVector_t lastCollidedPosition;
};

tcollider_t * collider_create() {
    tcollider_t * o = calloc(1, sizeof(tcollider_t));
    o->staticPoints = topaz_array_create(sizeof(cline_t));
    o->smear        = topaz_array_create(sizeof(cline_t));
    o->lastCollided = topaz_entity_null();
    o->srcPoints    = topaz_array_create(sizeof(topazVector_t));
    return o;
}

// Destroys a collider
void collider_destroy(tcollider_t * c) {
    topaz_array_destroy(c->staticPoints);
    topaz_array_destroy(c->smear);
    topaz_array_destroy(c->srcPoints);
    free(c);
}

void collider_set_from_points(
    tcollider_t * c, 
    const topazArray_t * pts
) { 
    if (c->srcPoints)
        topaz_array_destroy(c->srcPoints);
    c->srcPoints = topaz_array_clone(pts);
    // transfer raw points into lines
    topaz_array_set_size(c->staticPoints, 0);
    uint32_t len = topaz_array_get_size(pts);
    if (!len || len == 1) return;


    topazVector_t * ptsData = topaz_array_get_data(pts);
    cline_t line;
    for(uint32_t i = 0; i < len-1; ++i) {
        line.a = ptsData[i];
        line.b = ptsData[i+1];
        topaz_array_push(c->staticPoints, line);
    }
    line.a = ptsData[len-1];
    line.b = ptsData[0];
    topaz_array_push(c->staticPoints, line);

    
    c->stationary = 0;
    c->isSet = 0;
}


int collider_will_contain_point(
    const tcollider_t * c, 
    const topazVector_t * other
) {
    uint32_t realStart = c->stationary ? 0 : topaz_array_get_size(c->smear)/3;
    uint32_t realCount = c->stationary ? topaz_array_get_size(c->smear) : realStart*2;
    uint32_t i, n;

    
    int thisP_x;
    int thisP_y;

    int nextP_x;
    int nextP_y;

    int x = round(other->x);
    int y = round(other->y);

    int in = 0;
    // Jordan curve theorem, based on PNPOLY (WRF)
    for(i = realStart, n = realCount - 1; i < realCount; n = i++) { // flow control for wrapping behavior. i is current poly, n is next
        thisP_x = round(topaz_array_at(c->smear, cline_t, i).a.x);
        thisP_y = round(topaz_array_at(c->smear, cline_t, i).a.y);

        nextP_x = round(topaz_array_at(c->smear, cline_t, n).a.x);
        nextP_y = round(topaz_array_at(c->smear, cline_t, n).a.y);

        if (
                ((thisP_y > y) != 
                 (nextP_y > y))            
            &&
            
                (x < (nextP_x - thisP_x) * (y - thisP_y) / (nextP_y - thisP_y) + thisP_x)
            
            ) {
            in = !in;
        }
    }
    return in;
}


void collider_update_transition(
    tcollider_t * c, 
    const topazVector_t * after
) {
    if (!topaz_array_get_size(c->staticPoints)) return;
    
    if (!c->isSet) {
        c->oldPosition = *after;
        c->isSet = 1;
    }
    uint32_t i;
    uint32_t len;
    cline_t  line;
    topazVector_t before = c->oldPosition;
    c->oldPosition = *after;
    
    if (topaz_vector_get_distance(after, &before) < .000001) {
        // if we were already stationary, neither the smear bounds nor the static points have changed!
        if (c->stationary) {
            return;
        }

        topaz_array_set_size(c->smear, 0);
        // procs shorthand smear and bounds
        // only the "after" smears are taken
        // (the smears are also 1/3 the size when stationary since it doesnt include the
        // union of the movement smear and old polygon smear)
        len = topaz_array_get_size(c->staticPoints);
        for(i = 0; i < len; ++i) {
            line.a.x = topaz_array_at(c->staticPoints, cline_t, i).a.x + after->x;
            line.a.y = topaz_array_at(c->staticPoints, cline_t, i).a.y + after->y;

            line.b.x = topaz_array_at(c->staticPoints, cline_t, i).b.x + after->x;
            line.b.y = topaz_array_at(c->staticPoints, cline_t, i).b.y + after->y;

            topaz_array_push(c->smear, line);
        }
        
        c->smearBounds.x = topaz_array_at(c->smear, cline_t, 0).a.x;
        c->smearBounds.y = topaz_array_at(c->smear, cline_t, 0).a.y;
        float smearX2 = c->smearBounds.x;
        float smearY2 = c->smearBounds.y;
        topazVector_t afterWorld;
    
        len = topaz_array_get_size(c->smear);
        for(i = 0; i < len; ++i) {
            afterWorld  = topaz_array_at(c->smear, cline_t, i).a;
            if (afterWorld.x < c->smearBounds.x) c->smearBounds.x = afterWorld.x;
            if (afterWorld.y < c->smearBounds.y) c->smearBounds.y = afterWorld.y;
            if (afterWorld.x > smearX2) smearX2 = afterWorld.x;
            if (afterWorld.y > smearY2) smearY2 = afterWorld.y;    
        }
        
        c->smearBounds.width = smearX2 - c->smearBounds.x;
        c->smearBounds.height = smearY2 - c->smearBounds.y;
        
        
        
        c->stationary = 1;        
        return;
    } else {
        c->stationary = 0;
        topaz_array_set_size(c->smear, 0);
    }
    
    len = topaz_array_get_size(c->staticPoints);
    for(i = 0; i < len; ++i) {
        line.a.x = topaz_array_at(c->staticPoints, cline_t, i).a.x + before.x;
        line.a.y = topaz_array_at(c->staticPoints, cline_t, i).a.y + before.y;

        line.b.x = topaz_array_at(c->staticPoints, cline_t, i).b.x + before.x;
        line.b.y = topaz_array_at(c->staticPoints, cline_t, i).b.y + before.y;

        topaz_array_push(c->smear, line);
    }
    


    for(i = 0; i < len; ++i) {
        line.a.x = topaz_array_at(c->staticPoints, cline_t, i).a.x + before.x;
        line.a.y = topaz_array_at(c->staticPoints, cline_t, i).a.y + before.y;

        line.b.x = topaz_array_at(c->staticPoints, cline_t, i).b.x + before.x;
        line.b.y = topaz_array_at(c->staticPoints, cline_t, i).b.y + before.y;

        topaz_array_push(c->smear, line);
    }

    c->smearBounds.x = topaz_array_at(c->smear, cline_t, 0).a.x;
    c->smearBounds.y = topaz_array_at(c->smear, cline_t, 0).a.y;

    float smearX2 = c->smearBounds.x;
    float smearY2 = c->smearBounds.y;
    topazVector_t beforeWorld;
    topazVector_t afterWorld;

    for(i = 0; i < len; ++i) {
        beforeWorld.x = topaz_array_at(c->staticPoints, cline_t, i).a.x + before.x;
        beforeWorld.y = topaz_array_at(c->staticPoints, cline_t, i).a.y + before.y;

        afterWorld.x = topaz_array_at(c->staticPoints, cline_t, i).a.x + after->x;
        afterWorld.y = topaz_array_at(c->staticPoints, cline_t, i).a.y + after->y;


        line.a = beforeWorld;
        line.b = afterWorld;
        topaz_array_push(c->smear, line);

        if (beforeWorld.x < c->smearBounds.x) c->smearBounds.x = beforeWorld.x;
        if (beforeWorld.y < c->smearBounds.y) c->smearBounds.y = beforeWorld.y;
        if (afterWorld.x < c->smearBounds.x) c->smearBounds.x = afterWorld.x;
        if (afterWorld.y < c->smearBounds.y) c->smearBounds.y = afterWorld.y;

        if (beforeWorld.x > smearX2) smearX2 = beforeWorld.x;
        if (beforeWorld.y > smearY2) smearY2 = beforeWorld.y;
        if (afterWorld.x > smearX2) smearX2 = afterWorld.x;
        if (afterWorld.y > smearY2) smearY2 = afterWorld.y;

    }
    
    c->smearBounds.width = smearX2 - c->smearBounds.x;
    c->smearBounds.height = smearY2 - c->smearBounds.y;
    
    
}


int collides_with(
    const tcollider_t * c, 
    const tcollider_t * other,
    topazVector_t * intersect
) {
    
    uint32_t smearSize_self  = topaz_array_get_size(c->smear);
    uint32_t smearSize_other = topaz_array_get_size(other->smear);
    uint32_t n, i;
    for(i = 0; i < smearSize_self; ++i) {
        for(n = 0; n < smearSize_other; ++n) {
            if (line_intersect(
                &topaz_array_at(    c->smear, cline_t, i),
                &topaz_array_at(other->smear, cline_t, n),
                intersect
            )) {
                return 1;
            }
        }
    }

    uint32_t size = c->stationary ? smearSize_self : (smearSize_self/3)*2;
    for(i = c->stationary ? 0 : smearSize_self/3; i < size; ++i) {
        if (collider_will_contain_point(
            other,
            &topaz_array_at(c->smear, cline_t, i).a
        )) {
            *intersect = topaz_array_at(c->smear, cline_t, i).a;
            return 1;
        }
    }
    
    
    size = other->stationary ? smearSize_other : (smearSize_other/3)*2;
    for(i = other->stationary ? 0 : smearSize_other/3; i < size; ++i) {
        if (collider_will_contain_point(
            c,
            &topaz_array_at(other->smear, cline_t, i).a
        )) {
            *intersect = topaz_array_at(other->smear, cline_t, i).a;
            return 1;
        }
    }

    
    return 0;
}


topazEntity_t * collider_get_last_collided(const tcollider_t * c, topazVector_t * p) {
    if (p) *p = c->lastCollidedPosition;
    return c->lastCollided;
}

void collider_set_last_collided(tcollider_t * c, topazEntity_t * e, const topazVector_t * col) {
    c->lastCollided = e;
    c->lastCollidedPosition = *col;
}

const bbox_t * collider_get_moment_bounds(const tcollider_t * c) {
    return &c->smearBounds;
}

const topazArray_t * collider_get_source_points(const tcollider_t * c) {
    return c->srcPoints;
}




////////////////////////////////
///// bbox 

/*        
int bb_contains_point(const bbox_t * b, const topazVector_t * p) {
    return (p->x >= b->x &&
            p->x <= b->x+b->width &&
            p->y >= b->y &&
            p->y <= b->y+b->width);
}

int bb_contains_box(const bbox_t * a, const bbox_t * b) {
    return (b->x >= a->x &&
            b->x+b->width <= a->x+a->width &&
            b->y >= a->y &&
            b->y+b->height <= a->y+a->height);        
}
*/
int bb_overlaps(const bbox_t * a, const bbox_t * b) {
    return 
        a->x < (b->x+b->width)  && (a->x+a->width) > b->x &&
        (a->y+a->height) > b->y && a->y < (b->y+b->height);
}






///////////////////////////////
////// SpatialMap 

struct smap_t {
    float spanX;
    float spanY;
    float spanW;
    float spanH;
      
    int span;
    // every slot holds uint32_t
    topazArray_t *** fieldRed;
    topazArray_t *** fieldBlu;
    topazArray_t *   slotHeap;
    topazArray_t *   fieldAllocRed; // holds topazArray_t *, size span*span
    topazArray_t *   fieldAllocBlu; // holds topazArray_t *, size span*span
    topazTable_t * collisions;
    uint32_t heapIter;
    uint32_t heapAlloc;
    uint32_t numObjects;
};


typedef struct {
    int minX;
    int maxX;
    int minY;
    int maxY;
} XYRange;

static int spatial_map_x_position_to_x_index(const smap_t * m, float x) {
    float fracX = (x - m->spanX) / m->spanW;
    if (fracX < 0.f) fracX = 0.f;
    if (fracX > 1.f) fracX = 1.f;

    return (int)(fracX * (m->span-1));
}

static int spatial_map_y_position_to_y_index(const smap_t * m, float y) {
    float fracY = (y - m->spanY) / m->spanH;
    if (fracY < 0.f) fracY = 0.f;
    if (fracY > 1.f) fracY = 1.f;

    return (int)(fracY * (m->span-1));
}
  
int spatial_map_get_closest_enclosing_span(uint32_t count) {
    return ceil(sqrt(count))*4;
}


XYRange spatial_map_get_covered_regions(const smap_t * m, const bbox_t * cover) {
    XYRange range;
    range.minX   = spatial_map_x_position_to_x_index(m, cover->x);
    range.maxX   = spatial_map_x_position_to_x_index(m, cover->x+cover->width);
    range.minY   = spatial_map_y_position_to_y_index(m, cover->y);
    range.maxY   = spatial_map_y_position_to_y_index(m, cover->y+cover->height);
    return range;
}

#define SMAP_SIZE_CHUNK 64

static void spatial_map_heap_resize(smap_t * m) {
    uint32_t i;
    for(i = 0; i < SMAP_SIZE_CHUNK; ++i) {
        topazArray_t * slot = topaz_array_create(sizeof(uint32_t));
        topaz_array_push(m->slotHeap, slot);
        m->heapAlloc++;
    }

}

// Creates a spatial map instance
static smap_t * spatial_map_create() {
    smap_t * out = calloc(1, sizeof(smap_t));
    out->slotHeap      = topaz_array_create(sizeof(topazArray_t*));
    out->fieldAllocRed = topaz_array_create(sizeof(topazArray_t*));
    out->fieldAllocBlu = topaz_array_create(sizeof(topazArray_t*));
    out->collisions    = topaz_table_create_hash_pointer();

    spatial_map_heap_resize(out);
    return out;
}

static void spatial_map_destroy(smap_t * m) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(m->slotHeap);
    for(i = 0; i < len; ++i) {
        topaz_array_destroy(topaz_array_at(m->slotHeap, topazArray_t *, i));
    }
    topaz_array_destroy(m->slotHeap);
    topaz_table_destroy(m->collisions);
    free(m->fieldRed);
    topaz_array_destroy(m->fieldAllocRed);
    free(m->fieldBlu);
    topaz_array_destroy(m->fieldAllocBlu);
    
}

// Resets a spatial map based on the span of all objects to be contained within it.
static void spatial_map_reset(
    smap_t * m,
    float spanX_, 
    float spanY_, 
    float spanW_, 
    float spanH_, 
    uint32_t numObjectsContainer,
    uint32_t maxID
) {
    m->spanX = spanX_;
    m->spanY = spanY_;
    m->spanW = spanW_;
    m->spanH = spanH_;

    m->span = spatial_map_get_closest_enclosing_span(numObjectsContainer);


    topaz_array_set_size(m->fieldAllocRed, m->span*m->span);
    topaz_array_set_size(m->fieldAllocBlu, m->span*m->span);
    
    m->heapIter = 0;
    m->numObjects = maxID;
    topaz_table_clear(m->collisions);


    // first prep
    topazArray_t ** fieldAIter = topaz_array_get_data(m->fieldAllocRed);
    uint32_t i;
    uint32_t len = m->span*m->span;
    for(i = 0; i < len; ++i) {
        fieldAIter[i] = NULL;
    }

    // then populate
    free(m->fieldRed);
    m->fieldRed = malloc(sizeof(topazArray_t**)*m->span);
    for(i = 0; i < m->span; ++i) {
        m->fieldRed[i] = fieldAIter+i*m->span;
    }


    fieldAIter = topaz_array_get_data(m->fieldAllocBlu);
    for(i = 0; i < len; ++i) {
        fieldAIter[i] = NULL;
    }

    // then populate
    free(m->fieldBlu);
    m->fieldBlu = malloc(sizeof(topazArray_t**)*m->span);
    for(i = 0; i < m->span; ++i) {
        m->fieldBlu[i] = fieldAIter+i*m->span;
    }



}


// Inserts a region covered in the spatial map and associates 
// that region with an ID and adds the ID collisions with it to 
// a hash table.
// Every spat
static void spatial_map_insert(
    smap_t * m,
    const bbox_t * region,
    uint32_t id,
    uint8_t isRed
) {
    XYRange range = spatial_map_get_covered_regions(m, region);
    int x, y, i;
    topazArray_t *** fieldWrite;
    topazArray_t *** fieldTest;

    if (isRed) {
        fieldWrite = m->fieldRed;
        fieldTest  = m->fieldBlu;
    } else {
        fieldWrite = m->fieldBlu;
        fieldTest  = m->fieldRed;
    }

    for(y = range.minY; y <= range.maxY; ++y) {
        for(x = range.minX; x <= range.maxX; ++x) {
            if (!fieldWrite[x][y]) {
                if (m->heapIter >= m->heapAlloc) {
                    spatial_map_heap_resize(m);
                }
                fieldWrite[x][y] = topaz_array_at(m->slotHeap, topazArray_t *, m->heapIter);
                topaz_array_set_size(fieldWrite[x][y], 0); // reset
                m->heapIter++;
            }
            topaz_array_push(fieldWrite[x][y], id);



            if (fieldTest[x][y]) {
                topazArray_t * a = fieldTest[x][y];
                uint32_t len = topaz_array_get_size(a);
                for(i = 0; i < len; ++i) {
                    uint32_t other = topaz_array_at(a, uint32_t, i);

                    void * collidedKey;
                    if (id < other) {
                        collidedKey = ((void*)0x0 + id + ((other)*m->numObjects));
                    } else {
                        collidedKey = ((void*)0x0 + id*m->numObjects + (other));
                    }            
                    topaz_table_insert(m->collisions, collidedKey, (void*)1);
                }
            }
        }
    }
}


// gets all unique collisions as an array of spamCollision_t 
static topazArray_t * spatial_map_get_collisions(smap_t * m) {
    topazTableIter_t * iter = topaz_table_iter_create();
    topazArray_t * out = topaz_array_create(sizeof(smapCollision_t));
    if (sizeof(void*) == sizeof(uint64_t)) {
        for(topaz_table_iter_start(iter, m->collisions);
            !topaz_table_iter_is_end(iter);
            topaz_table_iter_proceed(iter)) {
            
            const void * collisionKey = topaz_table_iter_get_key(iter);
        
            smapCollision_t c = {
                ((uint64_t)collisionKey)%m->numObjects,
                ((uint64_t)collisionKey)/m->numObjects
            };

            topaz_array_push(out, c);
        }
    } else if (sizeof(void*) == sizeof(uint32_t)) {
        for(topaz_table_iter_start(iter, m->collisions);
            !topaz_table_iter_is_end(iter);
            topaz_table_iter_proceed(iter)) {
            
            const void * collisionKey = topaz_table_iter_get_key(iter);
        
            smapCollision_t c = {
                ((uintptr_t)collisionKey)%m->numObjects,
                ((uintptr_t)collisionKey)/m->numObjects
            };

            topaz_array_push(out, c);
        }
    } // 16 bit worth it?
    topaz_table_iter_destroy(iter);
    return out;
}

/*
// dumps the spatial map into some visuals. Just for debugging
static void spatial_map_dump_visual(smap_t * m, topazComponent_t * bgShape, topazComponent_t * fgShape) {
    topazArray_t * bg = topaz_array_create(sizeof(topazVector_t));
    topazArray_t * fg = topaz_array_create(sizeof(topazVector_t));
    

    int x, y;

    for(x = 0; x <= m->span; ++x) {
        topazVector_t v;
        v.z = 0;
        v.x = m->spanX + ((m->spanW) / (float)m->span)*x; 
        v.y = m->spanY;
        topaz_array_push(bg, v); 

        v.y = m->spanY+m->spanH; 
        topaz_array_push(bg, v); 

    }
    for(y = 0; y <= m->span; ++y) {
        topazVector_t v;
        v.z = 0;
        v.x = m->spanX; 
        v.y = m->spanY + ((m->spanH) / (float)m->span)*y; 
        topaz_array_push(bg, v); 
        
        v.x = m->spanX + m->spanW; 
        topaz_array_push(bg, v); 
    }

    topaz_shape2d_form_lines(bgShape, bg);
    topaz_shape2d_set_color(bgShape, topaz_color_from_amt(.3, .2, .2, 1));

    for(y = 0; y < m->span; ++y) {
        for(x = 0; x < m->span; ++x) {
            topazArray_t * obj = m->field[x][y];
            if (obj && topaz_array_get_size(obj)) {
                topazVector_t v;
                v.y = m->spanY + ((m->spanH) / (float)m->span)*y; 
                v.x = m->spanX + ((m->spanW) / (float)m->span)*x; 
                v.z = 0;
                topaz_array_push(fg, v); 

                v.x += (m->spanW) / (float)m->span;
                v.y += (m->spanH) / (float)m->span;
                topaz_array_push(fg, v); 

                v.y -= (m->spanH) / (float)m->span;
                topaz_array_push(fg, v); 




                v.y = m->spanY + ((m->spanH) / (float)m->span)*y; 
                v.x = m->spanX + ((m->spanW) / (float)m->span)*x; 
                v.z = 0;
                topaz_array_push(fg, v); 

                v.y += (m->spanH) / (float)m->span;
                topaz_array_push(fg, v); 

                v.x += (m->spanW) / (float)m->span;
                topaz_array_push(fg, v); 



            }
        }
    }

    topaz_shape2d_form_triangles(fgShape, fg);
    topaz_shape2d_set_color(fgShape, topaz_color_from_int(255, 255, 0, 255));


    topaz_array_destroy(fg);
    topaz_array_destroy(bg);
}
*/

///////////////////////////////
////// T2DM 


typedef struct {
    topazArray_t * objects;
    topazArray_t * setObjs;
    topazComponent_t * debugVisual;
    topazComponent_t * debugVisualActive;

    topazTable_t * smapTable;
    //smap_t * sMap[topazObject2D_Group_Z+1][topazObject2D_Group_Z+1];



} T2DMData;
#include <stdio.h>

static void t2dm_on_step(topazEntity_t * e, T2DMData * m) {
    uint32_t numObj = topaz_array_get_size(m->objects);
    uint32_t i;
    topazEntity_t * host;
    topazComponent_t * c;
    TopazObject2D_t * object;

    uint32_t counts[topazObject2D_Group_Z+1] = {};    

    // first, update colliders
    for(i = 0; i < numObj; ++i) {
        c = topaz_array_at(m->objects, topazComponent_t *, i);
        host = topaz_component_get_host(c);
        if (!host) {
            topaz_array_remove(m->objects, i);
            numObj--;
            i--;
            continue;
        }
        
        object = topaz_component_get_attributes(c)->userData;
        if (object->active == 2) {
            // 2 means "DID NOT UPDATE IN STEP()" 
            // so remove from main objects.
            object->active = 0;
            topaz_array_remove(m->objects, i);
            numObj--;
            i--;
            continue;
        }
        object->active = 2;
        topazVector_t next = topaz_object2d_get_next_position(c);
        collider_update_transition(
            object->collider,
            &next
        );        
        counts[object->group]++;
    }
    // collision events may produce new objects, which could disrupt the data.
    topazArray_t * objectsNow = topaz_array_clone(m->objects);
    topazComponent_t ** objects = topaz_array_get_data(objectsNow);




    ////////////////////////////////
    // uniform spatial indexing method
    ////////////////////////////////
    


    
    
    float spaceX  =  FLT_MAX, spaceX2  = -FLT_MAX,
          spaceY  =  FLT_MAX, spaceY2  = -FLT_MAX;
    
    for(i = 0; i < numObj; ++i) {
        object = topaz_component_get_attributes(objects[i])->userData;
        const bbox_t * b = collider_get_moment_bounds(object->collider);

        if (b->x < spaceX) spaceX = b->x;
        if (b->y < spaceY) spaceY = b->y;
        if (b->x+b->width > spaceX2) spaceX2 = b->x+b->width;
        if (b->y+b->height > spaceY2) spaceY2 = b->y+b->height;
    }
    float spaceW = spaceX2 - spaceX;
    float spaceH = spaceY2 - spaceY;


    // no viable collision detection can occur, so just update the objects and drop out.
    if (spaceW == 0.f || spaceH == 0.f) {
        topaz_array_destroy(objectsNow);
        return;
    }


    topazArray_t * activeLo[topazObject2D_Group_Z+1] = {};
    topazArray_t * activeHi[topazObject2D_Group_Z+1] = {};

    topazArray_t * allintersect = topaz_array_create(sizeof(smap_t*));    


    // gather valid tables for faster lookup
    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, m->smapTable);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        uint32_t val = (uintptr_t)topaz_table_iter_get_key(iter);
        
        uint16_t lo_x = val%(topazObject2D_Group_Z+1);
        uint16_t hi_y = val/(topazObject2D_Group_Z+1);

        smap_t * smap = topaz_table_iter_get_value(iter);
        spatial_map_reset(smap,
            spaceX,
            spaceY,
            spaceW,
            spaceH,
            counts[lo_x] + counts[hi_y],
            numObj
        );        

        // ACTIVE HAS REPEATS
        if (!activeLo[lo_x]) { 
            activeLo[lo_x] = topaz_array_create(sizeof(smap_t *));
        } 

        if (!activeHi[hi_y]) { 
            activeHi[hi_y] = topaz_array_create(sizeof(smap_t *));
        } 

        topaz_array_push(activeLo[lo_x], smap);        
        topaz_array_push(activeHi[hi_y], smap);        

        topaz_array_push(allintersect, smap);        

            
    }
    


    uint32_t n;
    for(i = 0; i < numObj; ++i) {
        object = topaz_component_get_attributes(objects[i])->userData;        
        topazArray_t * r = activeLo[object->group];

        // for all active smaps that refer to this group, add the bounds
        // this is where interaction group filtering occurs
        if (r) {
            for(n = 0; n < topaz_array_get_size(r); ++n) {
                spatial_map_insert(
                    topaz_array_at(r, smap_t *, n), 
                    collider_get_moment_bounds(object->collider), 
                    i,
                    1
                );
            }
        }

        
        r = activeHi[object->group];
        if (r) {
            for(n = 0; n < topaz_array_get_size(r); ++n) {
                spatial_map_insert(
                    topaz_array_at(r, smap_t *, n), 
                    collider_get_moment_bounds(object->collider), 
                    i,
                    0
                );
            }
        }
    }
    

    
    TopazObject2D_t * current;
    TopazObject2D_t * other;
    uint32_t mapN;
    topazVector_t intersect;

    uint32_t mapCount = topaz_array_get_size(allintersect);
    for(mapN = 0; mapN < mapCount; ++mapN) {
        smap_t * smap = topaz_array_at(allintersect, smap_t *, mapN);

        topazArray_t * collisions = spatial_map_get_collisions(smap);
        uint32_t nCol = topaz_array_get_size(collisions);
        for(i = 0; i < nCol; ++i) {
            smapCollision_t possible = topaz_array_at(collisions, smapCollision_t, i);
            current = topaz_component_get_attributes(objects[possible.self])->userData;
            other   = topaz_component_get_attributes(objects[possible.other])->userData;
            
            if (current == other) continue;


            // too far, would not result in a collision
            if (!bb_overlaps(
                    collider_get_moment_bounds(current->collider),
                    collider_get_moment_bounds(  other->collider)
            )) {
                continue;
            }
            


            // do full collision detection
            if (collides_with(
                    current->collider,
                      other->collider,
                    &intersect               
            )) {
                topazEntity_t * currentHost = topaz_component_get_host(objects[possible.self]);
                topazEntity_t * otherHost   = topaz_component_get_host(objects[possible.other]);
                collider_set_last_collided(current->collider, otherHost, &intersect);
                collider_set_last_collided(other->collider,   currentHost, &intersect);
                topaz_component_emit_event(objects[possible.self],  TOPAZ_STR_CAST("on-collide"), otherHost,   NULL);
                topaz_component_emit_event(objects[possible.other], TOPAZ_STR_CAST("on-collide"), currentHost, NULL);
            }

        }
    }
    
    topaz_array_destroy(allintersect);
    topaz_array_destroy(objectsNow);
    for(i = 0; i <= topazObject2D_Group_Z; ++i) {
        if (activeLo[i])topaz_array_destroy(activeLo[i]);
        if (activeHi[i])topaz_array_destroy(activeHi[i]);

    }
}


topazEntity_t * t2dm_fetch(topaz_t * t) {
    static topazTable_t * allManagers = NULL;
    if (!allManagers) {
        allManagers = topaz_table_create_hash_pointer();
    }
    topazEntity_t * mE = topaz_table_find(allManagers, t);
    if (mE) return mE;

    T2DMData * m = calloc(1, sizeof(T2DMData));
    m->objects = topaz_array_create(sizeof(topazComponent_t *));
    m->setObjs = topaz_array_create(sizeof(uint32_t));
    m->smapTable = topaz_table_create_hash_pointer();


    topazEntity_Attributes_t attribs = {0};
    attribs.on_step = (topaz_entity_attribute_callback)t2dm_on_step;
    attribs.userData = m;

    mE = topaz_entity_create_with_attributes(t, &attribs);
    topaz_table_insert(allManagers, t, mE);
    topaz_context_attach_post_manager(t, mE);


    // spatial map visual debugging, if needed.
        //m->debugVisual = topaz_shape2d_create(t);
        //m->debugVisualActive = topaz_shape2d_create(t);
        //topaz_entity_add_component(mE, m->debugVisual);
        //topaz_entity_add_component(mE, m->debugVisualActive);

    return mE;
}

void t2dm_register_object(topazEntity_t * e, topazComponent_t * o) {
    T2DMData * inst = topaz_entity_get_attributes(e)->userData;
    topaz_array_push(inst->objects, o);
}

void t2dm_unregister_object(topazEntity_t * e, topazComponent_t * o) {
    T2DMData * inst = topaz_entity_get_attributes(e)->userData;
    uint32_t i;
    uint32_t len = topaz_array_get_size(inst->objects); 
    topazComponent_t ** iter = topaz_array_get_data(inst->objects);
    for(i = 0; i < len; ++i) {
        if (iter[i] == o) {
            topaz_array_remove(inst->objects, i);
            return;
        }
    }

}

// Sets the group interaction.
void t2dm_set_interaction(topazEntity_t * e, topazObject2D_Group a, topazObject2D_Group b, int doIt) {
    T2DMData * inst = topaz_entity_get_attributes(e)->userData;
    
    // a is always first, a is 'X' and b is 'Y', x is always less.
    if (a > b) {
        topazObject2D_Group temp = a;
        a = b;
        b = temp;
    }

    uint32_t key = a + b*(topazObject2D_Group_Z+1);
    smap_t * smap = topaz_table_find_by_uint(inst->smapTable, key);
    
    if (smap && !doIt) {
        spatial_map_destroy(smap);
        topaz_table_remove_by_uint(inst->smapTable, key);
    } else if (!smap && doIt) {
        smap_t * smap = spatial_map_create();
        topaz_table_insert_by_uint(inst->smapTable, key, smap);
    }
}








