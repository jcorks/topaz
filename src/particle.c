#include <topaz/particle.h>
#include <topaz/containers/string.h>
#include <topaz/topaz.h>
#include <topaz/component.h>
#include <topaz/components/automation.h>
#include <topaz/modules/graphics.h>
#include <topaz/modules/resources.h>
#include <topaz/spatial.h>
#include <topaz/rng.h>
#include <topaz/entity.h>
#include <topaz/asset.h>
#include <topaz/assets/image.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PROP_COUNT (topazParticle_Property__Alpha+1)
#define PARAM_COUNT (topazRender2D_Parameter_TextureFilterHint+1)

typedef struct {
    // automation string function
    topazString_t * anim;
    // random min per frame.
    float noiseMin;
    // random max
    float noiseMax;
} ParticleProperty;

struct topazParticle_t {
    // asset name for image.
    topazString_t * image;
    // all parameters for render2d within emitter
    topazRender2D_Parameter params[PARAM_COUNT];

    // properties for the particle
    ParticleProperty props[PROP_COUNT];

    // the state string from to_string() cached.
    topazString_t * state;
};



/// Creates a new particle specification.
///
topazParticle_t * topaz_particle_create() {
    topazParticle_t * out = calloc(1, sizeof(topazParticle_t));
    out->image = topaz_string_create_from_c_str("topaz.square");
    out->state = topaz_string_create();

    uint32_t i;
    for(i = 0; i < PROP_COUNT; ++i) {
        out->props[i].anim = topaz_string_create();
    }
    return out;
}


/// Destroys the particle.
void topaz_particle_destroy(
    /// The particle to destroy.
    topazParticle_t * part
) {
    topaz_string_destroy(part->image);
    topaz_string_destroy(part->state);
    uint32_t i;
    for(i = 0; i < PROP_COUNT; ++i) {
        topaz_string_destroy(part->props[i].anim);
    }

    free(part);
}

#define READ_INT (atoi(topaz_string_get_c_str(topaz_string_chain_current(src)))); topaz_string_chain_proceed(src);
#define READ_STR (                           (topaz_string_chain_current(src))) ; topaz_string_chain_proceed(src);
#define READ_NUM (atof(topaz_string_get_c_str(topaz_string_chain_current(src)))); topaz_string_chain_proceed(src);

/// Sets the particle specification from a string.
/// This string is assumed to have come from a
/// topaz_particle_to_string() call.
void topaz_particle_set_from_string(
    /// The particle to modify.
    topazParticle_t * part, 

    /// The string to read from.
    const topazString_t * str
) {
    topazString_t * src = topaz_string_clone(str);
    topaz_string_chain_start(src, TOPAZ_STR_CAST("|"));

    topazString_t * next;

    int vernum = READ_INT;
          next = READ_STR;
    int count = 0;
    topaz_string_set(part->image, next);


    int i;
    for(i = 0; i < PARAM_COUNT; ++i) {
        part->params[i] = READ_INT;
    }


    if (vernum == 1) {
        while(!topaz_string_chain_is_end(src) && count < PROP_COUNT) {
            next =  READ_STR;
            topaz_string_set(part->props[count].anim, next);
            part->props[count].noiseMin = READ_NUM;
            part->props[count].noiseMax = READ_NUM;
            count++;
        }
    }

    // clear invalid ones; in the case the 
    // string was unreadable (either partly or entirely)
    // count will be less than prop_count, so 
    // empty them out to reduce negative effects.
    for(i = count; i < PROP_COUNT; ++i) {
        topaz_string_set(part->props[count].anim, next);
        part->props[i].noiseMin = 0;
        part->props[i].noiseMax = 0;
        topaz_string_clear(part->props[i].anim);
    }
    topaz_string_clear(part->state);
}

/// Returns a read-only string representation of the particle's
/// state, allowing for saving / loading the state of the particle.
const topazString_t * topaz_particle_to_string(
    /// The particle to query.
    topazParticle_t * part
) {
    topaz_string_clear(part->state);
    topaz_string_concat_printf(part->state, "1|%s|", topaz_string_get_c_str(part->image));
    int i;
    for(i = 0; i < PARAM_COUNT; ++i) {
        topaz_string_concat_printf(part->state, "%d|", part->params[i]);
    }
    for(i = 0; i < PROP_COUNT; ++i) {
        topaz_string_concat(part->state, part->props[i].anim);
        topaz_string_concat(part->state, TOPAZ_STR_CAST("|"));
        topaz_string_concat_printf(
            part->state, 
            "%f|%f|", 
            part->props[i].noiseMin,
            part->props[i].noiseMax        
        );
    }
    return part->state;
}




void topaz_particle_set_param(
    /// The particle to modify
    topazParticle_t * part,
    /// The parameter to modify
    topazRender2D_Parameter p,
    /// The value of the parameter.
    int value
) {
    if (p < 0 || p >= PARAM_COUNT) return;
    part->params[p] = value;
}


/// Sets the image
void topaz_particle_set_image(
    topazParticle_t * part,
    const topazString_t * p
) {
    topaz_string_set(part->image, p);
}


/// The minimum value for the attribute to start.
///
void topaz_particle_set_noise_min(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    float value
) {
    if (prop < 0 || prop >= PROP_COUNT) return;
    part->props[prop].noiseMin = value;
}
/// The maximum value for the attribute to start.
///
void topaz_particle_set_noise_max(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    float value
) {
    if (prop < 0 || prop >= PROP_COUNT) return;
    part->props[prop].noiseMax = value;

}

/// Sets how the particle property modifies its value over 
/// its duration. animString is expected to be output from
/// topaz_automation_to_string() which will describe 
/// how the value changes from the start of its life to the end.
void topaz_particle_set_function(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    /// The automation state string.
    const topazString_t * value
) {
    if (prop < 0 || prop >= PROP_COUNT) return;
    topaz_string_set(part->props[prop].anim, value);

}



typedef struct ActiveParticle ActiveParticle;

struct ActiveParticle {
    topazVector_t position;
    float life; // frames left
    float duration;
    float at;

    // next particle in chain
    ActiveParticle * next;
    // previous in chain.
    ActiveParticle * prev;
};

typedef struct {
    // automation instances that act as transform 
    // functions
    topazComponent_t * propsActive[PROP_COUNT];
    float propsMinNoise[PROP_COUNT];    
    float propsMaxNoise[PROP_COUNT];    

    // object for rendering all particles. 
    topazRender2D_t  * renderBrush;

    // all active particles, linked list
    // If NULL, no active particles
    ActiveParticle * active;

    // for noise
    topazRNG_t * rng;

    // context;
    topaz_t * ctx;
} TopazEmitter;


static float get_prop_value(TopazEmitter * e, topazParticle_Property p, float at) {
    float noise = e->propsMinNoise[p] + topaz_rng_next_value(e->rng)*(e->propsMaxNoise[p] - e->propsMinNoise[p]);
    return noise + topaz_automation_at(e->propsActive[p], at);
}


// update positions, duration and stats
static void update_active_particle(TopazEmitter * e, ActiveParticle * p) {
    if (p->life <= 0) {
        if (p->prev) p->prev->next = p->next;
        else         e->active = p->next;
        if (p->next) p->next->prev = p->prev;
        free(p);
        return;
    }
    // 0 - 1 of where we are in the animation
    p->at = p->life / p->duration; 


    float direction =  get_prop_value(e, topazParticle_Property__Direction,p->at);
    float speedX =    get_prop_value(e, topazParticle_Property__SpeedX,p->at);
    float speedY =    get_prop_value(e, topazParticle_Property__SpeedY,p->at);


    p->position.x += speedX * cos(direction * M_PI * (1/180.0));
    p->position.y += speedY * sin(direction * M_PI * (1/180.0));

    p->life-=1;
}


static void render_active_particle(TopazEmitter * e, ActiveParticle * p) {
    float scaleX =    get_prop_value(e, topazParticle_Property__ScaleX,p->at);
    float scaleY =    get_prop_value(e, topazParticle_Property__ScaleY,p->at);
    float scaleMult = get_prop_value(e, topazParticle_Property__ScaleMultiplier,p->at);

    float red =    get_prop_value(e, topazParticle_Property__Red,p->at);
    float blue =    get_prop_value(e, topazParticle_Property__Blue,p->at);
    float green =    get_prop_value(e, topazParticle_Property__Green,p->at);
    float alpha =    get_prop_value(e, topazParticle_Property__Alpha,p->at);

    float rotation =  get_prop_value(e, topazParticle_Property__Rotation,p->at);

    topazTransform_t * transform = topaz_spatial_get_node(topaz_render2d_get_spatial(e->renderBrush));

    topazVector_t scale = {
        scaleX * scaleMult,
        scaleY * scaleMult,
        1
    };
    topazVector_t rotationV = {
        1,  
        1,
        rotation
    };

    *topaz_transform_position(transform) = p->position;
    *topaz_transform_scale(transform) = scale;
    *topaz_transform_rotation(transform) = rotationV;

    topazRenderer_2D_Vertex_t v[6] = {
        {-0.5, -0.5,   red, green, blue, alpha,  0, 0},
        {-0.5,  0.5,   red, green, blue, alpha,  0, 1},
        { 0.5,  0.5,   red, green, blue, alpha,  1, 1},

        { 0.5,  0.5,   red, green, blue, alpha,  0, 0},
        {-0.5, -0.5,   red, green, blue, alpha,  1, 1},
        { 0.5, -0.5,   red, green, blue, alpha,  1, 0}
    };

    topaz_render2d_set_vertices(
        e->renderBrush,
        TOPAZ_ARRAY_CAST(v, topazRenderer_2D_Vertex_t, 6)
    );

    topaz_graphics_request_draw_2d(
        topaz_context_get_graphics(e->ctx),
        e->renderBrush
    );

}

static void emitter__on_step(topazEntity_t * e, void * data) {
    TopazEmitter * emitter = data;
    ActiveParticle * iter = emitter->active;
    ActiveParticle * next;
    while(iter) {
        next = iter->next;
        update_active_particle(emitter, iter);
        iter = next;
    }
}

static void emitter__on_draw(topazEntity_t * e, void * data) {
    TopazEmitter * emitter = data;
    ActiveParticle * iter = emitter->active;
    ActiveParticle * next;
    while(iter) {
        next = iter->next;
        render_active_particle(emitter, iter);
        iter = next;
    }
}


topazEntity_t * topaz_particle_emitter_2d_create(
    /// the topaz context.
    topaz_t * context
) {
    TopazEmitter * data = calloc(1, sizeof(TopazEmitter)); 



    data->ctx = context;
    int i;
    for(i = 0; i < PROP_COUNT; ++i) {
        data->propsActive[i] = topaz_automation_create(context);
    }
    data->rng = topaz_rng_create();
    data->renderBrush = topaz_render2d_create(topaz_graphics_get_renderer_2d(topaz_context_get_graphics(context)), NULL);

    // create base component and assign attribs
    topazEntity_Attributes_t attribs;
    memset(&attribs, 0, sizeof(topazEntity_Attributes_t));

    attribs.on_step     = (topaz_entity_attribute_callback) emitter__on_step;
    attribs.on_draw     = (topaz_entity_attribute_callback) emitter__on_draw;


    attribs.userData = data;
    topazEntity_t * out = topaz_entity_create_with_attributes(context, &attribs);
    return out;
}




void topaz_particle_emitter_2d_set_particle(
    topazEntity_t * e, 
    const topazParticle_t * p
) {
    TopazEmitter * emitter = topaz_entity_get_attributes(e)->userData;

    // first, kill all current particles if applicable
    ActiveParticle * iter = emitter->active;
    while(iter) {
        iter->life = 0;
        iter = iter->next;
    }


    // transfer texture to render brush
    topazAsset_t * image = topaz_resources_fetch_asset(
        topaz_context_get_resources(emitter->ctx),
        topazAsset_Type_Image,
        p->image
    );

    topazImage_Frame_t * f = topaz_image_get_frame(image, 0);
    if (!f) {
        topaz_render2d_set_texture(
            emitter->renderBrush,
            topaz_image_frame_get_texture(f)
        );
    }


    // copy params
    int i;
    for(i = 0; i < PARAM_COUNT; ++i) {
        topaz_render2d_set_parameter(
            emitter->renderBrush,
            i,
            p->params[i]
        );
    }

    for(i = 0; i < PROP_COUNT; ++i) {
        topaz_automation_set_from_string(emitter->propsActive[i], p->props[i].anim);
        emitter->propsMaxNoise[i] = p->props[i].noiseMax;
        emitter->propsMinNoise[i] = p->props[i].noiseMin;
    }
}


static void particle_emit(
    topazEntity_t * e,
    TopazEmitter * emitter
) {
    
    ActiveParticle * p = calloc(1, sizeof(ActiveParticle));
    p->duration = emitter->propsMinNoise[topazParticle_Property__Duration] + topaz_rng_next_value(emitter->rng)*(emitter->propsMaxNoise[topazParticle_Property__Duration] - emitter->propsMinNoise[topazParticle_Property__Duration]);    
    p->position = *topaz_entity_get_position(e);
    if (!emitter->active) {
        emitter->active = p;
    } else {
        emitter->active->next = p;
        p->prev = emitter->active->next;
    }
}


void topaz_particle_emitter_2d_emit(
    /// The emitter to emit from.
    topazEntity_t * e, 

    /// The number of particles to emit.
    int count
) {
    TopazEmitter * emitter = topaz_entity_get_attributes(e)->userData;
    int i;
    for(i = 0; i < count; ++i) {
        particle_emit(e, emitter);        
    }
}

void topaz_particle_emitter_2d_set_independent(
    topazEntity_t * emitter,
    int i
) {

}
