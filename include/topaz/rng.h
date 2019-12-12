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

/*

    RNG
    -----

    Instanced random number generator


*/
typedef struct topazRNG_t topazRNG_t;

/// Creates a new random number generator.
/// This chooses a default seed for you.
///
topazRNG_t * topaz_rng_create();


/// Destroys a random number generator
///
void topaz_rng_destroy(topazRNG_t * t);



/// Sets the seed for the random number generator.
/// 
void topaz_rng_set_seed(topazRNG_t * t, uint64_t);


/// Gets the next random integer from the RNG
///
int topaz_rng_next_int(topazRNG_t *);

/// Gets the next random value from the RNG
///
float topaz_rng_next_value(topazRNG_t *);






#endif
