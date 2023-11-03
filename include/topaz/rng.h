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


#ifndef H_TOPAZDC__RNG__INCLUDED
#define H_TOPAZDC__RNG__INCLUDED


#include <stdint.h>

///
/// Instanced random number generator.
/// With the same seed, the number sequence is 
/// guaranteed to match. This RNG is insufficient for 
/// security purposes.
///
typedef struct topazRNG_t topazRNG_t;

/// Creates a new random number generator.
/// This chooses a default seed for you.
///
topazRNG_t * topaz_rng_create();


/// Destroys a random number generator
///
/// (No script mapping)
///
void topaz_rng_destroy(
    /// The RNG to destroy.
    topazRNG_t * rng
);



/// Sets the seed for the random number generator.
/// 
void topaz_rng_set_seed(
    /// The RNG to modify.
    topazRNG_t * rng, 

    /// The seed to set.
    uint64_t seed
);


/// Gets the next random integer from the RNG
/// The range is [0, 0xffffffff].
///
int topaz_rng_next_int(
    /// The RNG to pull from.
    topazRNG_t * rng    
);

/// Gets the next random value from the RNG.
/// The range is [0, 1].
///
float topaz_rng_next_value(
    /// The RNG to pull from.
    topazRNG_t * rng
);






#endif
