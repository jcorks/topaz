#include <topaz/components/object2d.h>
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
static int bb_contains_point(const bbox_t *, const topazVector_t *);

// Returns whether box b is contained within box a
static int bb_contains_box(const bbox_t * a, const bbox_t * b);

// Returns whether 2 boxes overlap
static int bb_overlaps(const bbox_t * a, const bbox_t * b);        
    





/////// topazObject2D_Collider_t 
/*
    Individualized collider for each component.

*/

// creates a new collider
topazObject2D_Collider_t * collider_create();

// Destroys a collider
static void collider_destroy(topazObject2D_Collider_t *);

// Sets the collider geometry
static void collider_set_from_points(topazObject2D_Collider_t *, const topazArray_t *);

// Returns whether the given point will be contained within 
// the collider            
static int collider_will_contain_point(const topazObject2D_Collider_t *, const topazVector_t * other);

// Updates the transition of a collider
static void collider_update_transition(topazObject2D_Collider_t *, const topazVector_t * after);

// Returns whether the colliders in the current state will collide
static int collides_with(const topazObject2D_Collider_t *, const topazObject2D_Collider_t * otherd);

// Gets the last entity that was collided with
static topazEntity_t * collider_get_last_collided(const topazObject2D_Collider_t *);

// Sets the entity last collided with
static void collider_set_last_collided(topazObject2D_Collider_t *, topazEntity_t *);

// Gets the bounding box that represents the area the collider covers
const bbox_t * collider_get_moment_bounds(const topazObject2D_Collider_t *);



///// SpatialMap
/*
    Contains different regions of rectangles 
    and returns which, if any, of these rectangles
    would intersect with the ones given


*/
typedef struct smap_t smap_t;

// Creates a spatial map instance
static smap_t * spatial_map_create();

// Resets a spatial map based on the span of all objects to be contained within it.
static void spatial_map_reset(
    smap_t *,
    float spanX_, 
    float spanY_, 
    float spanW_, 
    float spanH_, 
    uint32_t numObjects
);

// Inserts a region covered in the spatial map and associates 
// that region with an ID.
static void spatial_map_insert(
    smap_t *,
    const bbox_t *,
    uint32_t 
);

// Given a region, returns all regions stored that are close to it.
// The returned set of region ID is guaranteed to be a superset of the 
// possible objects that could be encountered.
static void spatial_map_query(
    smap_t *,
    const bbox_t *,
    topazArray_t * ids
);


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
    topazObject2D_Collider_t * collider;
    topazObject2D_Group group;
};


static TopazObject2D_t * object2d__retrieve(topazComponent_t * c) {
    TopazObject2D_t * s = topaz_component_get_attributes(c)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(s && "Object2D instance is missing or instance invalid.");
        assert(s->MAGIC_ID == MAGIC_ID__OBJECT_2D);
    #endif
    return s;
}

static void object2d__on_attach(topazComponent_t * c, TopazObject2D_t * s) {
    // prevents weird collisions on frame one of stepping
    s->last = *topaz_entity_get_position(topaz_component_get_host(c));
}



static void object2d__on_destroy(topazComponent_t * c, TopazObject2D_t * s) {
    t2dm_unregister_object(s->manager, c);
}


topazComponent_t * topaz_object2d_create(topaz_t * t) {
    TopazObject2D_t * data = calloc(1, sizeof(TopazObject2D_t));
    data->ctx = t;
    #ifdef TOPAZDC_DEBUG
    data->MAGIC_ID = MAGIC_ID__OBJECT_2D;
    #endif
    data->manager = t2dm_fetch(t);

    // create base component and assign attribs
    topazComponent_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazComponent_Attributes_t));

    attribs.on_destroy  = (topaz_component_attribute_callback) object2d__on_destroy;    
    attribs.on_attach   = (topaz_component_attribute_callback) object2d__on_attach;
    attribs.userData = data;
    topazComponent_t * out = topaz_component_create_with_attributes(TOPAZ_STR_CAST("Object2D"), t, &attribs);
    t2dm_register_object(data->manager, out);

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

    topazVector_t delta;    
    topazVector_t src = {0};

    src = topaz_matrix_transform(
        topaz_spatial_get_global_transform(
            topaz_entity_get_spatial(
                topaz_component_get_host(c)
            )
        ),
        &src
    );
    
    delta.x = p->x - src.x;
    delta.y = p->y - src.y;

    float direction = topaz_vector_rotation_z(&delta) + directionOffset;

    s->speedX += factor * (cos((topaz_math_pi / 180.f) * direction));
    s->speedY += factor * (sin((topaz_math_pi / 180.f) * direction));
}



void topaz_object_set_velocity(
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

    topazVector_t delta;    
    topazVector_t src = topaz_entity_get_global_position(topaz_component_get_host(c));
    
    delta.x = p->x - src.x;
    delta.y = p->y - src.y;

    float direction = topaz_vector_rotation_z(&delta) + directionOffset;

    s->speedX = factor * (cos((topaz_math_pi / 180.f) * direction));
    s->speedY = factor * (sin((topaz_math_pi / 180.f) * direction));
}

void topaz_object2d_set_friction_x(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->frictionX = amt;
}

void topaz_object2d_set_friction_y(topazComponent_t * c, float amt) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->frictionY = amt;
}

float topaz_object2d_get_direction(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    topazVector_t speed = {s->speedX, s->speedY, 0};
    return topaz_vector_rotation_z(&speed);
}

void topaz_object2d_halt(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->speedX = 0;
    s->speedY = 0;
}

void topaz_object2d_reset_motion(topazComponent_t * c) {
    // TODO
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

topazObject2D_Collider_t * topaz_object2d_get_collider(topazComponent_t * c) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    return s->collider;    
}

void topaz_object2d_set_group(topazComponent_t * c, topazObject2D_Group g) {
    TopazObject2D_t * s = object2d__retrieve(c);    
    s->group = g;    
}

void topaz_object2d_set_group_interaction(topaz_t * t, topazObject2D_Group a, topazObject2D_Group b, int doIt) {
    topazEntity_t * m = t2dm_fetch(t);
    t2dm_set_interaction(m, a, b, doIt); 
}

static void object_update(topazComponent_t * o) {
    topazEntity_t * host = topaz_component_get_host(o);
    topazVector_t gpos = topaz_entity_get_global_position(host);
    topazVector_t delta;
    topazVector_t next = topaz_object2d_get_next_position(o);
    delta.x = next.x - gpos.x;
    delta.y = next.y - gpos.y;
    delta.z = 0;

    // using the "last" model, we include manual translations as part of 
    // normal collisions.
    if (topaz_vector_get_length(&delta) > .000001 && topaz_component_emit_event_anonymous(o, TOPAZ_STR_CAST("on-move"))) {
        topazVector_t * pos = topaz_entity_position(host);
        pos->x += delta.x;
        pos->y += delta.y;

        // push it through        
        topaz_spatial_check_update(topaz_entity_get_spatial(host));
    }

    TopazObject2D_t * object = topaz_component_get_attributes(o)->userData;
    object->last = *topaz_entity_get_position(host);

    object->speedX *= (1.0 - object->frictionX);
    object->speedY *= (1.0 - object->frictionY);
    
}




/////////////////////////////////
///// collider 

typedef struct {
    topazVector_t a;
    topazVector_t b;
} cline_t;


// returns whether 2 lines intersect. Used extensively for colliders
static int line_intersect(const cline_t * l0, const cline_t * l1) {
    // "LeMothe's Method"
    float s1_x = l0->b.x - l0->a.x;
    float s1_y = l0->b.y - l0->a.y;
    
    
    float s2_x = l1->b.x - l1->a.x;
    float s2_y = l1->b.y - l1->a.y;

    float s = (- s1_y * (l0->a.x - l1->a.x) + s1_x * (l0->a.y - l1->a.y)) / (-s2_x * s1_y + s1_x * s2_y);
    float t = (  s2_x * (l0->a.y - l1->a.y) - s2_y * (l0->a.x - l1->a.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t>= 0 && t <= 1) {
        return 1;
    }
    return 0;
}


struct topazObject2D_Collider_t {
    topazArray_t * staticPoints;
    topazArray_t * smear;

    bbox_t smearBounds;
    topazVector_t oldPosition;
    int stationary;
    int isSet;
    topazEntity_t * lastCollided;
};

topazObject2D_Collider_t * collider_create() {
    topazObject2D_Collider_t * o = calloc(1, sizeof(topazObject2D_Collider_t));
    o->staticPoints = topaz_array_create(sizeof(cline_t));
    o->smear        = topaz_array_create(sizeof(cline_t));
    o->lastCollided = topaz_entity_null();
    return o;
}

// Destroys a collider
void collider_destroy(topazObject2D_Collider_t * c) {
    topaz_array_destroy(c->staticPoints);
    topaz_array_destroy(c->smear);
    free(c);
}

void collider_set_from_points(
    topazObject2D_Collider_t * c, 
    const topazArray_t * pts
) { 
    uint32_t len = topaz_array_get_size(pts);
    if (!len || len == 1) return;

    // transfer raw points into lines
    topaz_array_set_size(c->staticPoints, 0);
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
    const topazObject2D_Collider_t * c, 
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
    topazObject2D_Collider_t * c, 
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
    const topazObject2D_Collider_t * c, 
    const topazObject2D_Collider_t * other
) {
    
    uint32_t smearSize_self  = topaz_array_get_size(c->smear);
    uint32_t smearSize_other = topaz_array_get_size(other->smear);
    uint32_t n, i;
    for(i = 0; i < smearSize_self; ++i) {
        for(n = 0; n < smearSize_other; ++n) {
            if (line_intersect(
                &topaz_array_at(    c->smear, cline_t, i),
                &topaz_array_at(other->smear, cline_t, n)
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
            return 1;
        }
    }
    
    
    size = other->stationary ? smearSize_other : (smearSize_other/3)*2;
    for(i = other->stationary ? 0 : smearSize_other/3; i < size; ++i) {
        if (collider_will_contain_point(
            c,
            &topaz_array_at(other->smear, cline_t, i).a
        )) {
            return 1;
        }
    }

    
    return 0;
}


topazEntity_t * collider_get_last_collided(const topazObject2D_Collider_t * c) {
    return c->lastCollided;
}

void collider_set_last_collided(topazObject2D_Collider_t * c, topazEntity_t * e) {
    c->lastCollided = e;
}

const bbox_t * collider_get_moment_bounds(const topazObject2D_Collider_t * c) {
    return &c->smearBounds;
}




////////////////////////////////
///// bbox 

        
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

int bb_overlaps(const bbox_t * a, const bbox_t * b) {
    return 
        a->x < b->x+b->width  && a->x+a->width > b->x &&
        a->y+b->height > b->y && a->y < b->y+b->height;
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
    topazArray_t * visitedQuery; // size numObjects, holds uint8_t
    topazArray_t *** field;
    topazArray_t *   slotHeap;
    topazArray_t *   fieldAlloc; // holds topazArray_t *, size span*span
    uint32_t heapSize;

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
    }
    m->heapSize += SMAP_SIZE_CHUNK;
    
}

// Creates a spatial map instance
static smap_t * spatial_map_create() {
    smap_t * out = calloc(1, sizeof(smap_t));
    out->visitedQuery = topaz_array_create(sizeof(uint8_t));
    out->slotHeap     = topaz_array_create(sizeof(topazArray_t*));
    out->fieldAlloc   = topaz_array_create(sizeof(topazArray_t*));

    spatial_map_heap_resize(out);
    return out;
}

// Resets a spatial map based on the span of all objects to be contained within it.
static void spatial_map_reset(
    smap_t * m,
    float spanX_, 
    float spanY_, 
    float spanW_, 
    float spanH_, 
    uint32_t numObjects
) {
    m->spanX = spanX_;
    m->spanY = spanY_;
    m->spanW = spanW_;
    m->spanH = spanH_;

    m->span = spatial_map_get_closest_enclosing_span(numObjects);

    // reset visited 
    if (topaz_array_get_size(m->visitedQuery) < numObjects) {
        topaz_array_set_size(m->visitedQuery, numObjects);
        memset(topaz_array_get_data(m->visitedQuery), 0, numObjects);
    }

    topaz_array_set_size(m->fieldAlloc, m->span*m->span);
    

    // first prep
    topazArray_t ** fieldAIter = topaz_array_get_data(m->fieldAlloc);
    uint32_t i;
    uint32_t len = numObjects*numObjects;
    for(i = 0; i < len; ++i) {
        fieldAIter[i] = NULL;
    }



    // then populate
    free(m->field);
    m->field = malloc(sizeof(topazArray_t**)*m->span);
    for(i = 0; i < m->span; ++i) {
        m->field[i] = fieldAIter+i*m->span;
    }
}

// Inserts a region covered in the spatial map and associates 
// that region with an ID.
static void spatial_map_insert(
    smap_t * m,
    const bbox_t * region,
    uint32_t id
) {
    XYRange range = spatial_map_get_covered_regions(m, region);
    int x, y;
    
    for(y = range.minY; y <= range.maxY; ++y) {
        for(x = range.minX; x <= range.maxX; ++x) {
            if (!m->field[x][y]) {
                if (!m->heapSize) {
                    spatial_map_heap_resize(m);
                }
                m->field[x][y] = topaz_array_at(m->slotHeap, topazArray_t *, m->heapSize-1);
                topaz_array_set_size(m->field[x][y], 0); // reset
                m->heapSize--;
            }
               
            topaz_array_push(m->field[x][y], index);
        }
    }
}

// Given a region, returns all regions stored that are close to it.
// The returned set of region ID is guaranteed to be a superset of the 
// possible objects that could be encountered.
static void spatial_map_query(
    smap_t * m,
    const bbox_t * region,
    topazArray_t * ids
) {
    XYRange range = spatial_map_get_covered_regions(m, region);
    uint32_t len, i, n;
    int x, y;
    for(y = range.minY; y <= range.maxY; ++y) {
        for(x = range.minX; x <= range.maxX; ++x) {
            topazArray_t * obj = m->field[x][y];
            len = topaz_array_get_size(obj);
            if (len) {
                
                for(n = 0; n < len; ++n) {
                    uint32_t id = topaz_array_at(obj, uint32_t, n);
                    if (topaz_array_at(m->visitedQuery, uint8_t, id)) continue;
                    topaz_array_push(ids, id);
                    topaz_array_at(m->visitedQuery, uint8_t, id) = 1;
                }
            }
        }
    }

    // cleanup visited
    len = topaz_array_get_size(ids);    
    uint32_t * idsData = topaz_array_get_data(ids);
    for(i = 0; i < len; ++i) {
        topaz_array_at(m->visitedQuery, uint8_t, idsData[i]) = 0;
    }
}





///////////////////////////////
////// T2DM 


typedef struct {
    topazArray_t * objects;
    uint8_t interactionMatrix[topazObject2D_Group_Z+1][topazObject2D_Group_Z+1];
    smap_t * sMap;
    topazTable_t * collided;
    topazArray_t * setObjs;
} T2DMData;


static void t2dm_on_step(topazEntity_t * e, T2DMData * m) {
    uint32_t numObj = topaz_array_get_size(m->objects);
    uint32_t i;
    topazEntity_t * host;
    topazComponent_t * c;
    TopazObject2D_t * object;
    

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
        topazVector_t next = topaz_object2d_get_next_position(c);
        collider_update_transition(
            object->collider,
            &next
        );        
    }
    topazComponent_t ** objects = topaz_array_get_data(m->objects);




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
        object_update(objects[i]);
        return;
    }


    // setup tracking of who collided with who this iteration.
    // tracked with just a hashtable. The table id is always:
    // (void*)(0x0 + indexLower + indexHigher*numObj) 
    void * collidedKey;
    topaz_table_clear(m->collided);
    

    spatial_map_reset(m->sMap,
        spaceX,
        spaceY,
        spaceW,
        spaceH,
        numObj
    );
    
    
    for(uint32_t i = 0; i < numObj; ++i) {
        object = topaz_component_get_attributes(objects[i])->userData;
        spatial_map_insert(m->sMap, collider_get_moment_bounds(object->collider), i);
    }
    
    
    
    TopazObject2D_t * current;
    TopazObject2D_t * other;
    uint32_t len;


    topaz_array_set_size(m->setObjs, 0);
    int hadCol = 0;
    for(i = 0; i < numObj; ++i) {
        if (hadCol) {
            topaz_array_set_size(m->setObjs, 0);
            hadCol = 0;
        }
        
        // get all collisions
        current = topaz_component_get_attributes(objects[i])->userData;
        spatial_map_query(m->sMap, collider_get_moment_bounds(object->collider), m->setObjs);
        

        // process each detected collision
        len = topaz_array_get_size(m->setObjs);
        uint32_t * setObjs = topaz_array_get_data(m->setObjs);
        for(uint32_t n = 0; n != len; ++n) {                
            other = topaz_component_get_attributes(objects[setObjs[n]])->userData;
            
            // is it just the same entity?
            if (other == current) {
                continue;
            }

            // filter based on collision groups
            if (!m->interactionMatrix[current->group][other->group]) {
                continue;                
            }


            // no repeats! If we're already collided, skip very overlap check
            if (i < setObjs[n]) {
                collidedKey = (void*)(0x0 + i + (setObjs[n]*numObj));
            } else {
                collidedKey = (void*)(0x0 + i*numObj + (setObjs[n]));
            }

            if (topaz_table_find(m->collided, collidedKey)) continue;
            topaz_table_insert(m->collided, collidedKey, (void*)0x1);



            // too far, would not result in a collidion
            if (!bb_overlaps(
                    collider_get_moment_bounds(current->collider),
                    collider_get_moment_bounds(  other->collider)
            )) {
                continue;
            }

    
            // do full collision detection
            if (collides_with(
                    current->collider,
                      other->collider               
            )) {
                topazEntity_t * currentHost = topaz_component_get_host(objects[i]);
                topazEntity_t * otherHost   = topaz_component_get_host(objects[setObjs[n]]);
                topaz_component_emit_event(objects[i],          TOPAZ_STR_CAST("on-collide"), otherHost,   NULL);
                topaz_component_emit_event(objects[setObjs[n]], TOPAZ_STR_CAST("on-collide"), currentHost, NULL);
                collider_set_last_collided(current->collider, otherHost);
                collider_set_last_collided(other->collider,   currentHost);
            }
            hadCol = 1;
        }
    }

    
    // apply new positions
    for(i = 0; i < numObj; ++i) {
        object_update(objects[i]);
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
    m->collided = topaz_table_create_hash_pointer();
    m->setObjs = topaz_array_create(sizeof(uint32_t));
    m->sMap = spatial_map_create();

    topazEntity_Attributes_t attribs = {0};
    attribs.on_step = (topaz_entity_attribute_callback)t2dm_on_step;
    attribs.userData = m;

    mE = topaz_entity_create_with_attributes(t, &attribs);
    topaz_table_insert(allManagers, t, mE);
    topaz_context_attach_manager(t, mE);
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
    inst->interactionMatrix[a][b] = doIt;
    inst->interactionMatrix[b][a] = doIt;
}









