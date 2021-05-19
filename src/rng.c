#include <topaz/compat.h>
#include <topaz/rng.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




/*
    topaz RNG is going to implement TT800
    as published by Makoto Matsumoto in 1996

*/

#define TT800_SEED_COUNT 25
#define TT800_END 7

struct topazRNG_t {
    int index;
    uint32_t state[TT800_SEED_COUNT];
    
};


const uint32_t tt800_a[2] = {
    0x0,
    0x8ebfd028
};


// Twists the RNG by using itself as a seed 
// and reorienting its state.
static void tt800_twist(topazRNG_t * rng) {
    int i;
    uint32_t * state = rng->state;


    for(i = 0; i < (TT800_SEED_COUNT - TT800_END); ++i) {
        state[i] = (state[i+TT800_END]) ^  
                   (state[i] >> 1) ^
                   (tt800_a[state[i]%2]);

        
    }

    for(; i < TT800_SEED_COUNT; ++i) {
        state[i] = (state[i+(TT800_END - TT800_SEED_COUNT)]) ^
                   (state[i] >> 1) ^
                   (tt800_a[state[i]%2]);
    }
    rng->index = 0;
}



// Returns a new RNG across uint32_t
static uint32_t tt800_next(topazRNG_t * rng) {
    if (rng->index == TT800_SEED_COUNT) {
        tt800_twist(rng);
        rng->index = 0;
    }
    uint32_t * state = rng->state;


    int index = rng->index++;

    
    uint32_t out = state[index];
    out ^= (out << 7)  & 0x2b5b2500;
    out ^= (out << 15) & 0xdb8b0000;
    out ^= (out >> 16);
    return out;
}


// initializes the state with a default seed
static void tt800_init_seed(topazRNG_t * rng) {
    uint32_t defaultSeeds[] = {
        0x30b72d97,
        0x2027d4ad,
        0x3631a263,
        0xf8a3dcaa,
        0x402417b9,

        0xd437b359,
        0x0b33c134,
        0xdd185f38,
        0x0c991509,
        0x00a04c8d,

        0x773ff5fb,
        0xba19cf6b,
        0x1f17cc9e,
        0xebaddf26,
        0xe69ecd9a,

        0x92c4a6dc,
        0xb8150e72,
        0xf0e1e969,
        0x9dfc6d1e,
        0xbce24d44,

        0x233375ae,
        0xdc159ae4,
        0x9757ea4c,
        0xe2282e2b,
        0xe9585859,
    };
    memcpy(rng->state, defaultSeeds, sizeof(uint32_t)*TT800_SEED_COUNT);
}






topazRNG_t * topaz_rng_create() {
    topazRNG_t * out = calloc(1, sizeof(topazRNG_t));
    tt800_init_seed(out);
    out->index = 0;
    return out;
}

void topaz_rng_destroy(topazRNG_t * t) {
    free(t);
}



void topaz_rng_set_seed(topazRNG_t * t, uint64_t d) {
    tt800_init_seed(t);

    uint8_t * src = (uint8_t*)&d;
    int i;
    for(i = 0; i < sizeof(uint64_t); ++i) {
        t->state[i%TT800_SEED_COUNT] += src[i%TT800_SEED_COUNT];
    }
}


int topaz_rng_next_int(topazRNG_t * t) {
    return tt800_next(t);
}

float topaz_rng_next_value(topazRNG_t * t) {
    return ((float)tt800_next(t)) / (0xffffffff); 
}



