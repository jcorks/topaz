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

typedef struct topazEntity_t topazEntity_t;


/// Represents a particle attribute's range. This defines 
/// how the values start and change over time.
///
typedef struct topazParticle_Range_t topazParticle_Range_t;
struct topazParticle_Range_t {
    /// The minimum value for the attribute to start.
    ///
    float startMin;

    /// The maximum value for the attribute to start.
    ///
    float startMax;

    /// The minimum change of the value for the attribut per frame.
    ///
    float deltaMin;

    /// The maximum change of the value for the attribut per frame.
    ///
    float deltaMax;

    /// The minimum amount of random noise to be added to the attribute. 
    ///
    float noiseMin;

    /// The minimum amount of random noise to be added to the attribute. 
    ///
    float noiseMax;
};


typedef struct topazParticle_t topazParticle_t;

/// The specification of a particle. This is used as a roadmap to 
/// generate a real particle instance using topaz_particle_emitter_2d_emit()
/// 
struct topazParticle_t {
    /// Duration of the particle, in frames.
    ///
    topazParticle_Range_t duration;

    /// The x-direction scale of the particle
    ///
    topazParticle_Range_t xScale;

    /// The y-direction scale of the particle
    ///
    topazParticle_Range_t yScale;

    /// The multiplier scale of the particle, applied to both x and y
    ///
    topazParticle_Range_t scaleMultiplier;

    /// The z-rotation of the particle
    ///
    topazParticle_Range_t rotation;

    /// The direction that the particle will travel in degrees
    ///
    topazParticle_Range_t direction;

    /// The speed that the particle will travel in particles-per-frame.
    ///
    topazParticle_Range_t speed;


    /// The red value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Range_t red;


    /// The green value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Range_t green;


    /// The blue value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Range_t blue;


    /// The alpha value to color the particle. From 0 to 1, clamped
    /// 
    topazParticle_Range_t alpha;

    /// Optional image
    topazAsset_t * image;
};


/// Creates a new particle emitter entity.
///
topazEntity_t * topaz_particle_emitter_2d_create();


/// Sets which particle that this emitter should emit.
///
void topaz_particle_emitter_2d_set_particle(topazEntity_t *, const topazParticle_t *);

/// Sets whether to enable translucency when rendering the particles.
/// 
void topaz_particle_emitter_2d_enable_translucency(topazEntity_t *, int enable);

/// Sets whether to enable texture filtering when rendering the particles
/// 
void topaz_particle_emitter_2d_enable_filtering(topazEntity_t *, int enable);

/// Emits a particle with position tracking from the emitter.
/// That is, when the emitter moves, the particles will move with it
/// This method is more efficient for drawing large numbers of particles
///
void topaz_particle_emitter_2d_emit(topazEntity_t *);

/// Same as topaz_particle_emitter_2d_emit, except will be emitted count 
/// number of times
///
void topaz_particle_emitter_2d_emit_n(topazEntity_t *, int count);


/// Emits a particle with position tracking it global/root entity space.
/// That is, when the emitter moves, the particle will not move with it.
/// Only the emitters position at the time of emission will be used
/// for the particle.
///
void topaz_particle_emitter_2d_emit_independent(topazEntity_t *);






#endif
