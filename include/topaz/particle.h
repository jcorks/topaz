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

#ifndef H_TOPAZDC__PARTICLE__INCLUDED
#define H_TOPAZDC__PARTICLE__INCLUDED

#include <topaz/render2d.h>

typedef struct topazEntity_t topazEntity_t;
typedef struct topazString_t topazString_t;
typedef struct topazRenderer_Attributes_t topazRenderer_Attributes_t;
typedef struct topaz_t topaz_t;
typedef struct topazAsset_t topazAsset_t;

/// The specification of a particle. This is used as a roadmap to 
/// generate a real particle instance using topaz_particle_emitter_2d_emit()
///
typedef struct topazParticle_t topazParticle_t;

typedef enum topazParticle_Property topazParticle_Property;
enum topazParticle_Property {
    /// Duration of the particle, in frames.
    /// The transform function is not used; noise min/max is treated as a minimum / maximum spread.
    ///
    topazParticle_Property__Duration,
    /// The x-direction scale of the particle
    ///
    topazParticle_Property__ScaleX,
    /// The y-direction scale of the particle
    ///
    topazParticle_Property__ScaleY,
    /// The multiplier scale of the particle, applied to both x and y
    ///
    topazParticle_Property__ScaleMultiplier,
    /// The z-rotation of the particle
    ///
    topazParticle_Property__Rotation,
    /// The direction that the particle will travel in degrees
    ///
    topazParticle_Property__Direction,
    /// The x speed that the particle will travel in pixels-per-frame.
    ///
    topazParticle_Property__SpeedX,
    /// The y speed that the particle will travel in pixels-per-frame.
    ///
    topazParticle_Property__SpeedY,
    /// The red value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Property__Red,
    /// The green value to color the particle. From 0 to 1, clamped
    ///
    topazParticle_Property__Green,
    /// The blue value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Property__Blue,
    /// The alpha value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Property__Alpha
};



/// Creates a new particle specification.
///
topazParticle_t * topaz_particle_create(
    /// the topaz context
    topaz_t * context
);


/// Destroys the particle.
///
/// (No script mapping) 
///
void topaz_particle_destroy(
    /// The particle to destroy.
    topazParticle_t * part
);

/// Sets the particle specification from a string.
/// This string is assumed to have come from a
/// topaz_particle_to_string() call.
void topaz_particle_set_from_string(
    /// The particle to modify.
    topazParticle_t * part, 

    /// The string to read from.
    const topazString_t * string
);

/// Returns a read-only string representation of the particle's
/// state, allowing for saving / loading the state of the particle.
const topazString_t * topaz_particle_to_string(
    /// The particle to query.
    topazParticle_t * part
);

/// Modifies a the rendering attributes.
///
void topaz_particle_set_attribute(
    /// The particle to modify
    topazParticle_t * part,

    /// The attribute to modify
    topazRenderer_Attribute attribute,

    /// The new attribute value
    int value
);

/// Gets the current attributes for particle rendering.
int topaz_particle_get_attribute(
    /// The particle to query
    topazParticle_t * part,
    
    /// The attribute to query
    topazRenderer_Attribute attribute    
);



/// Sets the image
void topaz_particle_set_image(
    /// The particle to modify.
    topazParticle_t * part,

    /// The asset image to set.
    topazAsset_t * image
);


/// The minimum value for the attribute to be applied every query
///
void topaz_particle_set_noise_min(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    /// The value to set.
    float value
);
/// The maximum value for the attribute to start.
///
void topaz_particle_set_noise_max(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    /// The value to set.
    float value
);

/// The minimum value for the attribute to be applied every query
///
void topaz_particle_set_offset_min(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    /// The value to set.
    float value
);


/// The maximum value for an offset of an attribute
///
void topaz_particle_set_offset_max(
    /// The particle to modify.
    topazParticle_t * part,
    /// The property to modify.
    topazParticle_Property prop,
    /// The value to set.
    float value
);

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
    const topazString_t * animString
);










/// Creates a new particle emitter entity.
///
topazEntity_t * topaz_particle_emitter_2d_create(
    /// the topaz context.
    topaz_t * context
);


/// Sets which particle that this emitter should emit.
///
void topaz_particle_emitter_2d_set_particle(
    /// The emitter to modify.
    topazEntity_t * emitter, 

    /// The particle that acts as a specification 
    /// for future particles produced by the emitter.
    /// Its information is copied into the emitter.
    const topazParticle_t * particle
);




/// Emits a number of particles at the location of the emitter.
///
void topaz_particle_emitter_2d_emit(
    /// The emitter to emit from.
    topazEntity_t * emitter, 

    /// The number of particles to emit.
    int count
);






#endif
